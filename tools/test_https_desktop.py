"""Real loopback HTTPS/browser probe; no device access or trusted-store changes."""
import argparse
from datetime import datetime,timedelta,timezone
import hashlib
import ipaddress
import json
from pathlib import Path
import subprocess
import threading
import urllib.request
import ssl
from cryptography import x509
from cryptography.hazmat.primitives import hashes,serialization
from cryptography.hazmat.primitives.asymmetric import rsa
from cryptography.x509.oid import NameOID
from serve_https_desktop import server

def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',required=True,type=Path);p.add_argument('--node-modules',required=True,type=Path);a=p.parse_args()
    root=Path(__file__).resolve().parents[1];a.output.mkdir(parents=True,exist_ok=False)
    private=a.output/'tls-local';private.mkdir()
    key=rsa.generate_private_key(public_exponent=65537,key_size=2048)
    now=datetime.now(timezone.utc)
    name=x509.Name([x509.NameAttribute(NameOID.COMMON_NAME,'localhost')])
    cert=(x509.CertificateBuilder().subject_name(name).issuer_name(name).public_key(key.public_key()).serial_number(x509.random_serial_number())
          .not_valid_before(now-timedelta(minutes=1)).not_valid_after(now+timedelta(hours=1))
          .add_extension(x509.SubjectAlternativeName([x509.DNSName('localhost'),x509.IPAddress(ipaddress.ip_address('127.0.0.1'))]),critical=False)
          .add_extension(x509.BasicConstraints(ca=True,path_length=0),critical=True).sign(key,hashes.SHA256()))
    keypath=private/'key.pem';certpath=private/'cert.pem'
    keypath.write_bytes(key.private_bytes(serialization.Encoding.PEM,serialization.PrivateFormat.PKCS8,serialization.NoEncryption()))
    certpath.write_bytes(cert.public_bytes(serialization.Encoding.PEM))
    s=server(root/'desktop',certpath,keypath,port=0);port=s.server_address[1]
    t=threading.Thread(target=s.serve_forever,daemon=True);t.start()
    result={'hardware_accessed':False,'router_installed':False,'http_listener_created':False,'browser_errors':[]}
    try:
        context=ssl.create_default_context(cafile=str(certpath))
        asset_results={}
        for asset in ('index.html','app.js','style.css'):
            with urllib.request.urlopen(f'https://127.0.0.1:{port}/{asset}',context=context,timeout=5) as r:
                raw=r.read();expected=(root/'desktop'/asset).read_bytes();assert raw==expected
                assert r.headers['Strict-Transport-Security']=='max-age=31536000'
                asset_results[asset]={'bytes':len(raw),'sha256':hashlib.sha256(raw).hexdigest(),'tls_verified':True}
        try:
            urllib.request.urlopen(f'http://127.0.0.1:{port}/index.html',timeout=3)
            raise AssertionError('plain HTTP unexpectedly accepted')
        except (OSError,urllib.error.URLError):result['plaintext_http_rejected']=True
        script=a.output/'browser-probe.cjs'
        script.write_text('''const { chromium }=require(process.argv[2]+"/playwright");
const launchOptions={headless:true,channel:'msedge'};
(async()=>{const browser=await chromium.launch(launchOptions);const context=await browser.newContext({ignoreHTTPSErrors:true});const page=await context.newPage();let errors=[],requests=[];page.on('pageerror',e=>errors.push(String(e)));page.on('request',r=>requests.push(r.url()));await page.goto(process.argv[3]);await page.locator('[data-open=notes]').click();await page.getByRole('textbox',{name:'Notas'}).fill('Nota de prueba HTTPS');await page.getByRole('button',{name:'Cerrar Notas en este navegador'}).click();await page.locator('[data-open=notes]').click();if(await page.getByRole('textbox',{name:'Notas'}).inputValue()!=='Nota de prueba HTTPS')throw Error('notes persistence');await page.locator('[data-open=snake]').click();await page.locator('canvas').focus();await page.keyboard.press('ArrowUp');await page.waitForTimeout(170);await page.getByRole('button',{name:'Reiniciar',exact:true}).click();await page.locator('[data-open=about]').click();if(errors.length)throw Error(errors.join('\\n'));if(requests.some(u=>!u.startsWith(process.argv[3].replace('/index.html','/'))))throw Error('external request');await page.screenshot({path:process.argv[4],fullPage:true});console.log(JSON.stringify({notes_persisted:true,snake_canvas:true,windows_opened:await page.locator('.window').count(),errors,requests}));await page.close();const blocked=await context.newPage();await blocked.route('http://127.0.0.1:'+process.argv[5]+'/**',async route=>{const path=require('path'),fs=require('fs');let file=new URL(route.request().url()).pathname.split('/').pop()||'index.html';if(!['index.html','style.css','app.js'].includes(file))return route.abort();await route.fulfill({body:fs.readFileSync(path.join(process.argv[6],file)),contentType:file.endsWith('.js')?'text/javascript':file.endsWith('.css')?'text/css':'text/html'});});await blocked.goto('http://127.0.0.1:'+process.argv[5]+'/index.html');if(!await blocked.locator('#blocked').isVisible())throw Error('HTTP client guard');if(await blocked.locator('nav button:not(:disabled)').count())throw Error('HTTP interactive apps');console.log(JSON.stringify({http_client_guard:true}));await browser.close();})().catch(e=>{console.error(e);process.exit(1)});''',encoding='utf-8')
        run=subprocess.run(['node',str(script),str(a.node_modules),f'https://127.0.0.1:{port}/index.html',str(a.output/'desktop.png'),str(port+1),str(root/'desktop')],capture_output=True,text=True,encoding='utf-8',errors='replace',timeout=60)
        (a.output/'browser.stdout').write_text(run.stdout,encoding='utf-8');(a.output/'browser.stderr').write_text(run.stderr,encoding='utf-8')
        assert run.returncode==0,run.stderr
        result.update(status='PASS_LOOPBACK_HTTPS_AND_BROWSER',assets=asset_results,browser=[json.loads(line) for line in run.stdout.splitlines() if line.startswith('{')],shell_bytes=sum(x['bytes'] for x in asset_results.values()))
    except Exception as exc:result.update(status='FAIL_LOOPBACK_HTTPS_AND_BROWSER',error=str(exc))
    finally:
        s.shutdown();s.server_close();t.join(timeout=5)
        keypath.unlink();certpath.unlink();private.rmdir()
        result['temporary_test_key_removed']=True
        (a.output/'result.json').write_text(json.dumps(result,indent=2)+'\n',encoding='utf-8')
        print(json.dumps(result,indent=2))
    return int(result['status']!='PASS_LOOPBACK_HTTPS_AND_BROWSER')

if __name__=='__main__':raise SystemExit(main())
