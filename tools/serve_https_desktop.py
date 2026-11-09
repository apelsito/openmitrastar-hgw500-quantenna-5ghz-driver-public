"""Explicit standalone HTTPS static server. Never provides an HTTP listener."""
import argparse
from functools import partial
from http.server import SimpleHTTPRequestHandler, ThreadingHTTPServer
from pathlib import Path
import ssl

class Handler(SimpleHTTPRequestHandler):
    def end_headers(self):
        self.send_header('Strict-Transport-Security','max-age=31536000')
        self.send_header('X-Content-Type-Options','nosniff')
        self.send_header('Referrer-Policy','no-referrer')
        self.send_header('Content-Security-Policy',"default-src 'self'; script-src 'self'; style-src 'self'; connect-src 'self'; frame-src 'self'; object-src 'none'; base-uri 'none'")
        super().end_headers()
    def list_directory(self,path):
        self.send_error(403,'Directory listing disabled')
        return None

def server(root,cert,key,bind='127.0.0.1',port=8443):
    root=Path(root).resolve()
    if not root.is_dir() or not Path(cert).is_file() or not Path(key).is_file():
        raise ValueError('Existing root, certificate and key required')
    context=ssl.SSLContext(ssl.PROTOCOL_TLS_SERVER)
    context.minimum_version=ssl.TLSVersion.TLSv1_2
    context.load_cert_chain(certfile=str(cert),keyfile=str(key))
    httpd=ThreadingHTTPServer((bind,port),partial(Handler,directory=str(root)))
    httpd.socket=context.wrap_socket(httpd.socket,server_side=True)
    return httpd

if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--root',required=True,type=Path);p.add_argument('--cert',required=True,type=Path);p.add_argument('--key',required=True,type=Path)
    p.add_argument('--bind',default='127.0.0.1');p.add_argument('--port',default=8443,type=int)
    a=p.parse_args();s=server(a.root,a.cert,a.key,a.bind,a.port)
    print('HTTPS listener ready; no HTTP listener created',flush=True)
    try:s.serve_forever()
    except KeyboardInterrupt:pass
    finally:s.server_close()
