#!/usr/bin/env python3
"""Prepare a NEW Linux7.3 source/config from pinned repositories, without build.

No network, Git mutation, DT/private asset copying or hardware. Source-only
composition reproduces the audited v6 non-private source changes; config omits
private initramfs/DT/keys. Generated headers/symvers require a subsequent build.
"""
import argparse,hashlib,json,os,re,shutil,subprocess,sys,tarfile
from pathlib import Path
HERE=Path(__file__).resolve().parent
UPSTREAM='c03c1a14507dfaf8b856c2608f9ac216021fc222'
REALTEK='a924ed874ba925c1b9fb2e42cbb2ef2e59d74e5d'
def sha(p):
 with Path(p).open('rb')as f:return hashlib.file_digest(f,'sha256').hexdigest()
def require(ok,msg):
 if not ok:raise ValueError(msg)
def safe_member(name):
 p=Path(name.replace('\\','/'));require(not p.is_absolute()and'..'not in p.parts,'Unsafe source member');return p
def forbidden(p):return p.suffix in('.dts','.dtb','.ko','.o','.elf','.bin','.pem','.key','.crt')or any(x in p.parts for x in('initramfs','local-only','secrets'))
def main():
 p=argparse.ArgumentParser(description=__doc__)
 for name in('realtek','driver-source','output'):p.add_argument('--'+name,type=Path,required=True)
 upstream=p.add_mutually_exclusive_group(required=True);upstream.add_argument('--upstream',type=Path);upstream.add_argument('--upstream-archive',type=Path,help='Pinned pristine linux-7.3.tar.xz, alternative to Git checkout')
 p.add_argument('--current-topaz-header',action='store_true',help='Use current MACinit/TXdiag register header instead of exact audited v6 header')
 p.add_argument('--toolchain-prefix',help='Required for --build-kernel-only, approved ARC prefix');p.add_argument('--jobs',type=int,default=2)
 modes=p.add_mutually_exclusive_group(required=True);modes.add_argument('--plan-only',action='store_true');modes.add_argument('--prepare-only',action='store_true');modes.add_argument('--build-kernel-only',action='store_true');a=p.parse_args()
 try:
  u=(a.upstream or a.upstream_archive).resolve();rt,d,o=[x.resolve()for x in(a.realtek,a.driver_source,a.output)];require(not o.exists(),'Choose a new output directory')
  for root in(rt,d,HERE):require(root.is_dir()and root not in o.parents,'Missing input or output inside input tree')
  require(u.exists()and u not in o.parents,'Upstream input absent or output inside it')
  require(all(not any(c.isspace()for c in str(root))for root in(u,rt,d,o,HERE)),'Linux source paths must not contain whitespace')
  roots=[(rt,REALTEK)]
  if a.upstream:require(u.is_dir(),'Upstream Git checkout required');roots.append((u,UPSTREAM))
  else:require(sha(u)=='87363c24f10e06321eb362765521094e94471f6bff8bce2d6b3e40f39f004953','Pinned Linux7.3 archive SHA256 mismatch')
  for root,commit in roots:
   require(subprocess.check_output(['git','-C',str(root),'rev-parse','HEAD'],text=True).strip()==commit,'Pinned Git HEAD mismatch: '+str(root))
   # Input data below is verified against pinned manifests. The upstream tree
   # is exported from the exact Git commit, so unrelated worktree edits cannot
   # enter it. Avoid a broad scan of the complete legacy/Realtek checkout.
  audit=HERE/'kernel-preparation-v1/AUDIT.json';q=json.loads(audit.read_text());board=HERE/'kernel-preparation-v1/board-overlay';ro=rt/'kernel/radio2g-linux-7.3-v1';rp=json.loads((ro/'SOURCE_MANIFEST.json').read_text());dr=json.loads((d/'result.json').read_text());layers=[];pins={str(audit):sha(audit),str(d/'result.json'):sha(d/'result.json')}
  # Basic/MMU/IRQ/PCIe deltas captured exactly from the completed v6 source.
  for rel,h in q['copied_public_board_files'].items():
   rel=safe_member(rel);src=board/rel;require(not forbidden(rel)and sha(src)==h,'Audited board file mismatch');layers.append((src,rel,'audited-v6-board'));pins[str(src)]=h
  for row in rp['files']:
   member=safe_member(row['path'])
   if not member.as_posix().startswith('overlay/'):continue
   rel=Path(*member.parts[1:])
   if forbidden(rel):continue
   src=ro/member;require(sha(src)==row['sha256'],'Pinned Realtek overlay mismatch');layers.append((src,rel,'pinned-Realtek'));pins[str(src)]=row['sha256']
  for rel,h in dr['files'].items():
   member=safe_member(rel)
   if not member.as_posix().startswith('kernel-overlay/'):continue
   rel=Path(*member.parts[1:]);require(not forbidden(rel),'Private/DT artifact in driver overlay');src=d/member;require(sha(src)==h,'Quantenna overlay mismatch');layers.append((src,rel,'selected-Quantenna'));pins[str(src)]=h
  if a.current_topaz_header:
   src=HERE/'board-overlay/include/linux/soc/qtn/topaz-ethernet.h';layers.append((src,Path('include/linux/soc/qtn/topaz-ethernet.h'),'current-MACinit-header'));pins[str(src)]=sha(src)
  cfg=HERE/'kernel-preparation-v2/config.sanitized';require(cfg.is_file(),'Sanitized config template absent');text=cfg.read_text();require('CONFIG_QTN_TOPAZ_ETH73=y'in text,'ETH73 config omitted');require('CONFIG_INITRAMFS_SOURCE=""'in text and'CONFIG_BUILTIN_DTB_NAME=""'in text,'Private boot source or DT selected');require(not re.search(r'(?:/home/|/mnt/|C:\\|192\.168\.|BEGIN .*PRIVATE KEY)',text),'Private path/identity in config');pins[str(cfg)]=sha(cfg);pins[str(Path(__file__))]=sha(Path(__file__))
  if a.build_kernel_only:
   require(a.toolchain_prefix and 1<=a.jobs<=32,'Kernel build requires approved prefix and jobs1..32');gcc=Path(a.toolchain_prefix+'gcc');require(sha(gcc)=='3918a0da3c9f2cfde359f6fed1d0df19e737438dfd2cc60edae63f2dcbf14263','Approved GCC hash mismatch');pins[str(gcc)]=sha(gcc)
   for suffix in('ld','ar','nm','objdump','readelf'):require(Path(a.toolchain_prefix+suffix).is_file(),'Toolchain missing:'+suffix)
  expected={rel.as_posix():sha(src)for src,rel,layer in layers};report={'schema':'openmitrastar.aelita-prepared-kernel-source/v1','status':'PASS_PLAN_PINNED_KERNEL_BOARD_CONFIG_NO_PRIVATE_ASSETS','hardware_accessed':False,'upstream_commit':UPSTREAM,'realtek_commit':REALTEK,'driver_source':str(d),'layer_files':len(layers),'final_overlay_files':len(expected),'overlay_files':expected,'input_pins':pins,'config_template_sha256':sha(cfg),'private_DT_initramfs_keys_copied':False,'new_generated_headers':False,'new_Module_symvers':False,'compiled_kernel':False,'reproduces':'Audited v6 regular board/MMU/PCIe source and selected Quantenna overlay; excludes private DT/initramfs and sanitizes config','current_topaz_header_selected':a.current_topaz_header,'limits':['No hardware boot possible from this source-only result until a separate sanitized board DT is supplied.','Prepare source/config, then olddefconfig/modules_prepare/full kernel/base-module build generates the actual new context.','Reset.c remains upstream in audited v6; no Topaz SoC restart implementation is claimed.']}
  if a.plan_only:print(json.dumps(report,indent=2));return 0
  require(os.name=='posix','Preparation requires Linux/WSL');o.mkdir();kernel=o/'kernel-source';kernel.mkdir();context=o/'kernel-context';context.mkdir()
  if a.upstream:
   proc=subprocess.Popen(['git','-C',str(u),'archive','--format=tar',UPSTREAM],stdout=subprocess.PIPE)
   with tarfile.open(fileobj=proc.stdout,mode='r|')as tar:tar.extractall(kernel,filter='data')
   require(proc.wait()==0,'git archive failed')
  else:
   with tarfile.open(u)as tar:
    for member in tar:
     name=member.name.split('/',1)[1]if '/'in member.name else''
     if not name:continue
     safe_member(name);member.name=name;tar.extract(member,kernel,filter='data')
  for src,rel,layer in layers:
   dst=kernel/rel;dst.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(src,dst)
  for rel,h in expected.items():require(sha(kernel/rel)==h,'Composed source mismatch:'+rel)
  require('config QTN_TOPAZ_ETH73'in(kernel/'arch/arc/plat-topaz-basic/Kconfig').read_text(),'ETH73 definition missing');require('hwirq == 28'in(kernel/'arch/arc/kernel/intc-compact.c').read_text(),'Combined PCIe interrupt gate missing')
  (context/'.config').write_bytes(cfg.read_bytes());require(all(sha(src)==h for src,h in pins.items()),'Input drift')
  source_pins={p.relative_to(kernel).as_posix():sha(p)for p in kernel.rglob('*')if p.is_file()and not p.is_symlink()};(o/'kernel-source-pins.json').write_text(json.dumps(source_pins,indent=2)+'\n')
  report.update(status='PASS_PREPARED_NEW_KERNEL73_PUBLIC_SOURCE_SANITIZED_CONFIG_BUILD_UNRUN',kernel_source=str(kernel),kernel_context=str(context),config_sha256=sha(context/'.config'),kernel_makefile_sha256=sha(kernel/'Makefile'),complete_regular_source_files_pinned=len(source_pins),kernel_source_pins_sha256=sha(o/'kernel-source-pins.json'))
  if a.build_kernel_only:
   make=['make','-C',str(kernel),'O='+str(context),'ARCH=arc','CROSS_COMPILE='+a.toolchain_prefix];env=os.environ|{'SOURCE_DATE_EPOCH':'1792365298','KBUILD_BUILD_TIMESTAMP':'@1792365298','KBUILD_BUILD_USER':'openmitrastar','KBUILD_BUILD_HOST':'aelita73','KBUILD_BUILD_VERSION':'1'}
   with(o/'kernel-00.log').open('xb')as log:subprocess.run(make+['olddefconfig'],stdout=log,stderr=subprocess.STDOUT,env=env,check=True)
   final_cfg=(context/'.config').read_text();require('CONFIG_BUILTIN_DTB_NAME=""'in final_cfg and'CONFIG_INITRAMFS_SOURCE=""'in final_cfg,'olddefconfig selected a forbidden builtin DT/initramfs');require('CONFIG_QTN_TOPAZ_ETH73=y'in final_cfg,'olddefconfig removed ETH73 board contract')
   # ARC always selects GENERIC_BUILTIN_DTB; that framework flag is not a DT
   # selection. Its dts/Makefile uses only the explicitly empty name above.
   require('dtb-y\t:= $(addsuffix .dtb, $(CONFIG_BUILTIN_DTB_NAME))'in(kernel/'arch/arc/boot/dts/Makefile').read_text(),'Unexpected builtin DT selection rule')
   for i,cmd in enumerate((make+['modules_prepare'],make+['-j'+str(a.jobs),'vmlinux','modules']),1):
    with(o/f'kernel-{i:02d}.log').open('xb')as log:subprocess.run(cmd,stdout=log,stderr=subprocess.STDOUT,env=env,check=True)
   require((context/'Module.symvers').is_file(),'New generated Module.symvers absent');report.update(status='PASS_SINGLE_KERNEL73_BASE_MODULE_BUILD_PENDING_DRIVER_PAIR_ABI_PHYSICAL',new_Module_symvers=True,new_generated_headers=True,compiled_kernel=True,config_sha256=sha(context/'.config'),symvers_sha256=sha(context/'Module.symvers'),vmlinux_sha256=sha(context/'vmlinux'))
  (o/'result.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report,indent=2));return 0
 except Exception as e:print(json.dumps({'status':'NO_GO_KERNEL73_SOURCE_PREPARATION','error':str(e),'hardware_accessed':False}),file=sys.stderr);return 1
if __name__=='__main__':raise SystemExit(main())
