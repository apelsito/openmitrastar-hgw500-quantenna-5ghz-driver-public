"""Seal isolated old/new scan lock ordering source and ARC unit evidence."""
import hashlib,json,subprocess,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[3];sys.path.insert(0,str(ROOT/'tools'))
from prepare_tqe_bounded_admission_prototype import function
out=Path(__file__).parent
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
old=ROOT/'work/linux510-radio-port/quantenna/wlan/ieee80211_scan_ap.c'
new=ROOT/'outputs/rea/scan-ap-bh-lock-order-20261022T195500Z-r1/source/wlan/ieee80211_scan_ap.c'
assert sha(old)=='be5fb4d720800aacccacc75c53e3d7e9dc9f0a7c28e7307589e9007ff78eae0e'
assert sha(new)=='9a2740433e1140049c6e7e9697cea7aef42c3680cc8cea04c7deea1a07e14f0f'
fixture=ROOT/'outputs/rea/scan-ap-bh-lock-order-fixture-20261022T195900Z-r2'
f=(fixture/'fixture.c').read_text()
for p,prefix in [(old,'old'),(new,'new')]:
 for name in ['lock_ap_list','unlock_ap_list']:
  body=function(p.read_text(),name+'(').replace(name+'(',prefix+'_'+name+'(',1)
  assert body in f,('actual fixture body',prefix,name)
r=subprocess.run([str(fixture/'fixture.exe')],capture_output=True,text=True);assert not r.returncode
tests=json.loads(r.stdout);assert tests=={'old_reentrant_windows':2,'new_reentrant_windows':0,'prior_context_cases':3}
arc=ROOT/'outputs/rea/scan-ap-bh-lock-order-arc-20261022T195800Z-r1'
a=json.loads((arc/'result.json').read_text());assert a['input_pins']==1012 and a['inputs_unchanged']
obj=arc/'wlan/ieee80211_scan_ap.o';assert sha(obj)==a['units'][0]['object_sha256'] and obj.stat().st_size==14348
result={'status':'PASS_INDEPENDENT_ISOLATED_SCAN_AP_BH_LOCK_ORDER_SOURCE_ARC_UNIT','hardware_accessed':False,'firmware_eligible':False,'module_built':False,'fresh_actual_old_new_body_fixture':tests,'object_sha256':sha(obj),'object_bytes':obj.stat().st_size,'ARC_dependency_pins':1012,'header_or_wire_changes':False,'limits':['Fixture models process/softirq lock interleaving; excludes hardIRQ and actual scheduler behavior.','Source compiled as one actual ARC unit only; no final module/integrated461 acceptance.','No device access or RF/client/traffic/boot/persistence acceptance.'],'evidence_pins':[{'path':str(p),'bytes':p.stat().st_size,'sha256':sha(p)}for p in[old,new,fixture/'fixture.c',fixture/'fixture.exe',arc/'result.json',arc/'input-pins.json',obj]]}
assert not(out/'result.json').exists();(out/'result.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps({'status':result['status'],'receipt_sha256':sha(out/'result.json')},indent=2))
