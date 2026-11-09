#!/usr/bin/env python3
"""Build current external EMAC only against a freshly rebuilt Linux7.3 ABI.

No downloads, Git mutation, packing, hardware or DT access. --plan-only writes
nothing. Source headers and complete peer modules must already share the ABI.
"""
import argparse,hashlib,json,os,re,shutil,subprocess,sys
from pathlib import Path
HERE=Path(__file__).resolve().parent
GCC='3918a0da3c9f2cfde359f6fed1d0df19e737438dfd2cc60edae63f2dcbf14263'
def sha(path):
 with Path(path).open('rb')as stream:return hashlib.file_digest(stream,'sha256').hexdigest()
def require(ok,msg):
 if not ok:raise ValueError(msg)
def main():
 p=argparse.ArgumentParser(description=__doc__)
 for name in('kernel-source','kernel-context','output'):p.add_argument('--'+name,type=Path,required=True)
 p.add_argument('--toolchain-prefix',required=True)
 p.add_argument('--expected-config-sha256',required=True)
 p.add_argument('--expected-symvers-sha256',required=True)
 p.add_argument('--jobs',type=int,default=2)
 mode=p.add_mutually_exclusive_group(required=True);mode.add_argument('--plan-only',action='store_true');mode.add_argument('--build',action='store_true');a=p.parse_args()
 try:
  k,c,o=[x.resolve()for x in(a.kernel_source,a.kernel_context,a.output)]
  require(not o.exists(),'Use a new output directory');require(1<=a.jobs<=32,'jobs must be1..32')
  require(all(not any(ch.isspace()for ch in str(x))for x in(k,c,o,HERE)),'Kbuild paths must not contain whitespace')
  require(k.is_dir()and c.is_dir(),'Kernel/context missing');require(k not in o.parents and c not in o.parents and HERE not in o.parents,'Output must be outside input trees')
  mk=(k/'Makefile').read_text();require(re.search(r'^VERSION\s*=\s*7\s*$',mk,re.M)and re.search(r'^PATCHLEVEL\s*=\s*3\s*$',mk,re.M),'Kernel must be Linux7.3')
  require(sha(c/'.config')==a.expected_config_sha256,'New config hash mismatch');require(sha(c/'Module.symvers')==a.expected_symvers_sha256,'New symvers hash mismatch')
  cfg=(c/'.config').read_text()
  for key in('MODULES','ARC_CPU_750D','ARC_MMU_V2','ARC_PAGE_SIZE_8K','ARC_PLAT_TOPAZ_BASIC','QTN_TOPAZ_ETH73'):
   require('CONFIG_'+key+'=y'in cfg,'Required new context option:'+key)
  require('CONFIG_SMP=y'not in cfg and'CONFIG_MODULE_FORCE_UNLOAD=y'not in cfg,'UP and no forced unload required')
  for rel in('include/generated/autoconf.h','include/generated/utsrelease.h','scripts/mod/modpost','scripts/module.lds'):
   require((c/rel).is_file()or(k/rel).is_file(),'Prepared generated/build context missing:'+rel)
  # Structural overlay check prevents building this driver on the old packet
  # layout. Full peer rebuild identity remains a separate manifest gate.
  skb=(k/'include/linux/skbuff.h').read_text();ndev=(k/'include/linux/netdevice.h').read_text()
  for marker in('qtn_cb','orig_dev','recycle_list'):require(marker in skb,'Quantenna packet ABI absent:'+marker)
  require('qtn_flags'in ndev,'Quantenna net_device ABI absent')
  compiler=Path(a.toolchain_prefix+'gcc');require(sha(compiler)==GCC,'Approved GCC hash mismatch')
  for suffix in('ld','nm','objdump','readelf'):require(Path(a.toolchain_prefix+suffix).is_file(),'Toolchain missing:'+suffix)
  manifest=json.loads((HERE/'SOURCE_MANIFEST.json').read_text());members=manifest['files'];pins={}
  for rel,digest in members.items():
   file=HERE/rel;require(file.is_file()and sha(file)==digest,'Source manifest mismatch:'+rel);pins[str(file)]=digest
  require({x.relative_to(HERE).as_posix()for x in(HERE/'driver').rglob('*')if x.is_file()}=={x for x in members if x.startswith('driver/')},'Driver manifest coverage mismatch')
  for file in(HERE/'SOURCE_MANIFEST.json',Path(__file__),k/'Makefile',c/'.config',c/'Module.symvers',compiler,k/'include/linux/skbuff.h',k/'include/linux/netdevice.h'):pins[str(file)]=sha(file)
  cmds=[['make','-C',str(k),'O='+str(c),'ARCH=arc','CROSS_COMPILE='+a.toolchain_prefix,'M='+str(o/'driver'),'-j'+str(a.jobs),'modules']]
  report={'schema':'openmitrastar.emac73-new-abi-build/v1','status':'PASS_PLAN_EXTERNAL_EMAC_NEW_KERNEL_ABI_INPUTS','hardware_accessed':False,'firmware_eligible':False,'old_objects_reused':False,'DT_included':False,'commands':cmds,'input_pins':pins,'limits':['All kernel/base/QDRV modules must already be freshly rebuilt against this exact config/symvers.','This only compiles external EMAC; it does not prepare board source or install hardware.']}
  if a.plan_only:print(json.dumps(report,indent=2));return 0
  require(os.name=='posix','Build requires Linux/WSL');o.mkdir();shutil.copytree(HERE/'driver',o/'driver');(o/'input-pins.json').write_text(json.dumps(pins,indent=2)+'\n')
  env=os.environ|{'SOURCE_DATE_EPOCH':'1792365298','KBUILD_BUILD_TIMESTAMP':'@1792365298','KBUILD_BUILD_USER':'openmitrastar','KBUILD_BUILD_HOST':'aelita73','KBUILD_BUILD_VERSION':'1'}
  with(o/'build.log').open('xb')as log:subprocess.run(cmds[0],stdout=log,stderr=subprocess.STDOUT,check=True,env=env)
  module=o/'driver/qtn-topaz-emac-eth73.ko';require(module.is_file(),'External module missing')
  asm=subprocess.check_output([a.toolchain_prefix+'objdump','-d',str(module)],text=True);require(not re.search(r'\t(?:swape|llock|scond)(?:\t| )',asm),'Forbidden ARC atomic instruction')
  exports={x.split()[1]for x in(c/'Module.symvers').read_text().splitlines()if x.split()};imports={x.split()[-1]for x in subprocess.check_output([a.toolchain_prefix+'nm','-u',str(module)],text=True).splitlines()if x.split()};require(not imports-exports,'Unresolved current kernel imports:'+str(sorted(imports-exports)))
  require(all(sha(file)==digest for file,digest in pins.items()),'Input changed during build')
  report.update(status='PASS_SINGLE_EXTERNAL_EMAC73_MODULE_BUILD_PENDING_PAIR_ABI_PHYSICAL',module={'path':'driver/qtn-topaz-emac-eth73.ko','bytes':module.stat().st_size,'sha256':sha(module)},imports_resolved=len(imports));(o/'result.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report,indent=2));return 0
 except Exception as e:print(json.dumps({'status':'NO_GO_EMAC73_NEW_ABI_BUILD','error':str(e),'hardware_accessed':False}),file=sys.stderr);return 1
if __name__=='__main__':raise SystemExit(main())
