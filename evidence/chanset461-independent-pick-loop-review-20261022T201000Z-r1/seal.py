"""Seal analysis-only independently decoded OEM selection loop evidence."""
import hashlib,json,struct,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[3];out=Path(__file__).parent
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
target=ROOT/'outputs/rea/oem-wlan-missingops-20261022T134300Z-linked/wlan-ANALYSIS_ONLY-NOT_FIRMWARE.elf'
assert sha(target)=='79fbcbd4634a9fc32fdf8ac0c997f3bd0329131162a1ea4948794e4986c7e40a'
decode=ROOT/'outputs/rea/chanset461-independent-pick-loop-decode-20261022T200000Z-r1'
log=(decode/'ghidra.log').read_text();assert 'NO_DECODE' not in log
for at,dest in [('00014e10','00014ec8'),('00014e54','00014ece'),('00014e8c','00014ed6'),('00014ec0','00014ede')]:assert f'ram:{at}: brlt r3,r2,0x{dest}' in log
assert 'ram:00014ee2: add r4,r4,0x50' in log
raw=target.read_bytes();off=struct.unpack_from('<I',raw,32)[0];width,count,names=struct.unpack_from('<HHH',raw,46);sections=[]
for i in range(count):
 h=struct.unpack_from('<10I',raw,off+i*width)
 if h[3]<=0x14dd2 and 0x14ee6<=h[3]+h[5]:sections.append(h)
assert len(sections)==1
h=sections[0];fragment=raw[h[4]+0x14dd2-h[3]:h[4]+0x14ee6-h[3]]
assert len(fragment)==0x114
result={'status':'PASS_INDEPENDENT_ANALYSIS_ONLY_OEM_SIGNED_PICK_LOOP_DECODE','hardware_accessed':False,'firmware_eligible':False,'range_start':'0x14dd2','range_end_exclusive':'0x14ee6','fragment_bytes':len(fragment),'fragment_sha256':hashlib.sha256(fragment).hexdigest(),'confirmed':['80-byte candidate stride.','Candidate cost at offset0x4c loads into r3; prior width-class winner cost loads into r2.','Signed brlt replaces width-class winner only when candidate cost is strictly less.','Equal costs retain first winner.'],'limits':['Synthetic linked analysis ELF contains synthetic external addresses and must never enter firmware.','Fragment confirms per-width-class ranking; final cross-width benefit ranking is separate evidence.','No operational461 capability, device/channel state or RF acceptance.'],'evidence_pins':[{'path':str(p),'bytes':p.stat().st_size,'sha256':sha(p)}for p in[target,decode/'DumpPick.java',decode/'ghidra.log']]}
assert not(out/'result.json').exists();(out/'result.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps({'status':result['status'],'receipt_sha256':sha(out/'result.json')},indent=2))
