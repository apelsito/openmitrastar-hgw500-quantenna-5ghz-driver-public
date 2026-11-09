"""Fresh actual26r3qualificationpins/modules/residual withsource review bound."""
import hashlib,json,sys
from pathlib import Path
ROOT=Path('/workspace/user-home/Desktop/MitraStar');sys.path.insert(0,str(ROOT/'tools'));from qualify_f370_qdrv_module import Elf;from audit_hgw5g_coex26_r3_modules import audit
out=Path(__file__).parent;sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest();q=ROOT/'outputs/rea/driver5g-ops-20261022-coex26-qualification-r3';proof=json.loads((q/'result.json').read_text());assert proof['status']=='PASS_OFFLINE_COEX26_R3_OEM_BSS_GEOMETRY_TIMER_RX_BW_MODULE_CLOSURE'and proof['pins_verified']==3471
pins=json.loads((q/'qualification-input-pins.json').read_text());assert len(pins)==3471
for path,h in pins.items():assert sha(Path(path))==h,'actualcurrentqualificationpin '+path
parent=ROOT/'outputs/rea/driver5g-ops-20261022-coc25-joint-a-r6';first=ROOT/'outputs/rea/driver5g-ops-20261022-coex26-joint-a-r3';second=ROOT/'outputs/rea/driver5g-ops-20261022-coex26-joint-b-r3';reports={}
for c in ['qdrv','wlan']:reports[c]=audit(Elf(parent/c/(c+'.ko')),Elf(first/c/(c+'.ko')),c)
for row in proof['modules']:
 c=row['component'];a=first/c/(c+'.ko');b=second/c/(c+'.ko');assert sha(a)==sha(b)==row['sha256']and a.stat().st_size==row['bytes']
source=ROOT/'outputs/rea/coex449-source26-bss-geometry-20261022T192600Z-r3';sp=json.loads((source/'result.json').read_text())
for rel,h in sp['changed_sources'].items():assert sha(source/rel)==h
review=ROOT/'outputs/rea/coex449-r3-independent-review-20261022T215800Z-r1/result.json';assert sha(review)=='e3753379acd7a7312f11b8d7dfe76dde007ed63fb8cd8b2fd69bdc63995eeb3b'
for path,h in pins.items():assert sha(Path(path))==h,'postauditpin '+path
refs=[q/'result.json',q/'qualification-input-pins.json',q/'complete-module-audit.json',source/'result.json',review,ROOT/'tools/audit_hgw5g_coex26_r3_modules.py'];result={'status':'PASS_INDEPENDENT_OFFLINE_SOURCE26_R3_BSS_GEOMETRY_MODULE_REVIEW','hardware_accessed':False,'firmware_eligible':False,'original_records_reconstructed':26,'remaining_original_records':18,'private_table_records':592,'actual_current_qualificationpins':3471,'fresh_module_audits':reports,'modules':proof['modules'],'new_input_compiles':2,'retained_r2_changed_compiles':6,'whole_CoC25_objects_reused':108,'negative_cases':11,'source_review':str(review),'limits':['Source26r3only; combinewith29needsnewSOURCE30ABI/pairedmodules/globalreview.','Geometry/parsersourcefixturesactualhostmodel withBSS/currentdifference; noRF/regulatory/client/traffic proof.','InitializedretainedIC/NACsnapshot/twofaithfulunsupported scopepreserved; noforcedunload/externalCSAentireteardown.','No boot/persistence/rollback/recovery/releaseacceptance.'],'evidence_pins':[{'path':str(p),'bytes':p.stat().st_size,'sha256':sha(p)}for p in refs]};(out/'result.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps({'status':result['status'],'receipt_sha256':sha(out/'result.json')},indent=2))
