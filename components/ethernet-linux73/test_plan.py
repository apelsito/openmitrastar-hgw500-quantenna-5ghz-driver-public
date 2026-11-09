"""Mock-inputplancontrol fixture; noARCcompile/kernel/HWclaim."""
import argparse,contextlib,hashlib,importlib.util,io,json,sys
from pathlib import Path
HERE=Path(__file__).resolve().parent
def main():
 p=argparse.ArgumentParser();p.add_argument('--output',type=Path,required=True);a=p.parse_args();assert not a.output.exists();a.output.mkdir(parents=True)
 spec=importlib.util.spec_from_file_location('emac_portable_build',HERE/'build.py');m=importlib.util.module_from_spec(spec);spec.loader.exec_module(m)
 kernel=a.output/'fake-kernel';context=a.output/'fake-context';tc=a.output/'fake-toolchain';kernel.mkdir();context.mkdir();tc.mkdir()
 def write(p,s):p.parent.mkdir(parents=True,exist_ok=True);p.write_text(s)
 write(kernel/'Makefile','VERSION = 7\nPATCHLEVEL = 3\n');write(kernel/'include/linux/skbuff.h','qtn_cb orig_dev recycle_list\n');write(kernel/'include/linux/netdevice.h','qtn_flags\n')
 cfg=''.join('CONFIG_'+x+'=y\n'for x in('MODULES','ARC_CPU_750D','ARC_MMU_V2','ARC_PAGE_SIZE_8K','ARC_PLAT_TOPAZ_BASIC','QTN_TOPAZ_ETH73'));write(context/'.config',cfg);write(context/'Module.symvers','0x00000000\ttest_kernel_export\tvmlinux\tEXPORT_SYMBOL\n')
 for rel in('include/generated/autoconf.h','include/generated/utsrelease.h','scripts/mod/modpost','scripts/module.lds'):write(context/rel,'fake plan fixture only\n')
 for suffix in('gcc','ld','nm','objdump','readelf'):write(tc/('arc-snps-linux-uclibc-'+suffix),'fake '+suffix+' plan fixture, never executable\n')
 m.GCC=m.sha(tc/'arc-snps-linux-uclibc-gcc');base=['build.py','--kernel-source',str(kernel),'--kernel-context',str(context),'--toolchain-prefix',str(tc/'arc-snps-linux-uclibc-'),'--expected-config-sha256',m.sha(context/'.config'),'--expected-symvers-sha256',m.sha(context/'Module.symvers'),'--output',str(a.output/'must-not-exist'),'--plan-only'];old=sys.argv;rows=[]
 for mode in('valid','wrong-config','wrong-symvers','wrong-abi'):
  args=list(base)
  if mode=='wrong-config':args[args.index('--expected-config-sha256')+1]='0'*64
  elif mode=='wrong-symvers':args[args.index('--expected-symvers-sha256')+1]='0'*64
  elif mode=='wrong-abi':write(kernel/'include/linux/skbuff.h','qtn_cb orig_dev\n')
  sys.argv=args;capture=io.StringIO()
  with contextlib.redirect_stdout(capture),contextlib.redirect_stderr(capture):rc=m.main()
  require=(rc==0)if mode=='valid'else(rc==1);assert require and not(a.output/'must-not-exist').exists();rows.append({'case':mode,'expected_result':True,'output_not_created':True})
 sys.argv=old;r={'status':'PASS_EXTERNAL_EMAC_PLAN_AND_THREE_NEGATIVE_CONTROLS_FAKE_INPUTS','hardware_accessed':False,'ARC_compilation':False,'cases':rows,'tool_sha256':m.sha(HERE/'build.py'),'limits':'Inputs/toolchain are mocks; onlyargument/hash/ABIrefusal andplan-nomutation controlflow tested.'};(a.output/'result.json').write_text(json.dumps(r,indent=2)+'\n');print(json.dumps(r))
if __name__=='__main__':main()
