#!/usr/bin/env python3
"""Verify complete prepared kernel source then call immutable AELITA builder.

New wrapper preserves the already-running predecessor. Existing real symvers
is a hash-bound reference only; no copying or fabrication. No downloads/Git/HW.
"""
import argparse,hashlib,json,subprocess,sys
from pathlib import Path
PREDECESSOR_SHA='6eb8199d80b6ccb1aa4486194d302b5646b2ddf5c43bfdc658c55c3a5a2d56a4'
def sha(p):
 with Path(p).open('rb')as f:return hashlib.file_digest(f,'sha256').hexdigest()
def main():
 p=argparse.ArgumentParser(description=__doc__,add_help=False,allow_abbrev=False)
 p.add_argument('--kernel-source-pins',type=Path,required=True)
 p.add_argument('--expected-kernel-source-pins-sha256',required=True)
 p.add_argument('--expected-reference-symvers-sha256',required=True)
 a,forward=p.parse_known_args()
 try:
  def arg(name):
   if name not in forward:raise ValueError('Required forwardedargument absent:'+name)
   return Path(forward[forward.index(name)+1]).resolve()
  source=arg('--kernel-source');context=arg('--kernel-context');reference=arg('--reference-symvers')if'--reference-symvers'in forward else context/'Module.symvers'
  predecessor=Path(__file__).with_name('linux73_build_aelita.py');assert sha(predecessor)==PREDECESSOR_SHA,'Predecessor changed'
  assert sha(a.kernel_source_pins)==a.expected_kernel_source_pins_sha256,'Prepared source manifest SHA mismatch'
  pins=json.loads(a.kernel_source_pins.read_text());assert pins,'Empty prepared source manifest'
  actual={p.relative_to(source).as_posix()for p in source.rglob('*')if p.is_file()and not p.is_symlink()and'.git'not in p.parts};assert actual==set(pins),'Prepared source manifest incomplete or foreign regularfiles'
  for rel,h in pins.items():
   member=Path(rel);assert not member.is_absolute()and'..'not in member.parts,'Unsafe source member';assert sha(source/member)==h,'Prepared source hash mismatch:'+rel
  assert sha(reference)==a.expected_reference_symvers_sha256,'Reference symvers SHA mismatch'
  rc=subprocess.run([sys.executable,str(predecessor),*forward]).returncode
  for rel,h in pins.items():assert sha(source/rel)==h,'Input source mutated during builder:'+rel
  assert sha(reference)==a.expected_reference_symvers_sha256,'Reference symvers changed during builder';assert sha(predecessor)==PREDECESSOR_SHA
  return rc
 except Exception as e:print(json.dumps({'status':'NO_GO_AELITA_COMPLETE_SOURCE_WRAPPER','error':str(e),'hardware_accessed':False}),file=sys.stderr);return 1
if __name__=='__main__':raise SystemExit(main())
