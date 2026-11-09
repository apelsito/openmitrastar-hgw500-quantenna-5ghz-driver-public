"""Fresh actual source25 same-header module audit and immutable receipt."""
import hashlib,json,sys
from pathlib import Path
ROOT=Path('/workspace/user-home/Desktop/MitraStar');sys.path.insert(0,str(ROOT/'tools'));from audit_hgw5g_coc25_controlled_modules import audit;from qualify_f370_qdrv_module import Elf
out=Path(__file__).parent;sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest();qualification=ROOT/'outputs/rea/driver5g-ops-20261022-coc25-qualification-r6';r=json.loads((qualification/'result.json').read_text());assert r['status']=='PASS_OFFLINE_COC25_INITIALIZED_RETAINED_IC_SEVEN_MODULE_CLOSURE'and r['pins_verified']==2378 and r['original_records_recovered']==25 and r['remaining_original_records']==19
pins=json.loads((qualification/'qualification-input-pins.json').read_text());assert len(pins)==2378
for path,h in pins.items():assert sha(Path(path))==h,'actual qualificationpin drift '+path
control=ROOT/'outputs/rea/driver5g-ops-20261022-coc25-control-joint-r6';first=ROOT/'outputs/rea/driver5g-ops-20261022-coc25-joint-a-r6';second=ROOT/'outputs/rea/driver5g-ops-20261022-coc25-joint-b-r6';reports={}
for c in ['qdrv','wlan']:reports[c]=audit(Elf(control/c/(c+'.ko')),Elf(first/c/(c+'.ko')),c);assert reports[c]['status']=='PASS_COMPLETE_COC25_RESIDUAL_SAME_HEADER_SCOPE'
modules=[]
for row in r['modules']:
 c=row['component'];a=first/c/(c+'.ko');b=second/c/(c+'.ko');assert sha(a)==sha(b)==row['sha256']and a.stat().st_size==row['bytes'];modules.append(row)
source=ROOT/'outputs/rea/coc491-source25-postcompletion-20261022T181500Z-r6';s=json.loads((source/'result.json').read_text())
for rel,h in s['changed_sources'].items():assert sha(source/rel)==h
review=ROOT/'outputs/rea/coc491-source25-r6-independent-review-20261022T210000Z-r1/result.json';assert json.loads(review.read_text())['status']=='PASS_INDEPENDENT_COC491_R6_SOURCE_SEMANTICS_FOR_INITIALIZED_RETAINED_IC_PENDING_FULL_MODULES'
for path,h in pins.items():assert sha(Path(path))==h,'post auditpin drift '+path
refs=[qualification/'result.json',qualification/'qualification-input-pins.json',qualification/'complete-controlled-audit.json',ROOT/'tools/audit_hgw5g_coc25_controlled_modules.py',source/'result.json',review]
result={'status':'PASS_INDEPENDENT_OFFLINE_COC25_INITIALIZED_RETAINED_IC_SEVEN_MODULE_REVIEW','hardware_accessed':False,'firmware_eligible':False,'original_records_recovered':25,'remaining_original_records':19,'private_records':591,'source_fresh_compiles':116,'control_reused_exact_units':54,'control_new_compiles':4,'actual_current_qualification_pins':2378,'fresh_actual_module_audits':reports,'modules':modules,'source_semantics_review':str(review),'negative_qualification_cases':11,'limits':['InitializedretainedIC scope; inheritedpreinitignoredifattachfailure/forcedunload/externalCSAentireteardownexcluded.','Actual165888worker/66callback/64queue/9interleavings+Windowscanceljoin verifiedseparately; scalar snapshotsmaymixgenerations.','Twofaithfulunsupportedentries/NACsnapshotguarddeviationretained.','NoPMbackend/RF/actualCSAcompletion/clienttrafficbootpersistence/rollback/releaseacceptance.'],'evidence_pins':[{'path':str(p),'bytes':p.stat().st_size,'sha256':sha(p)}for p in refs]};(out/'fresh-audit.json').write_text(json.dumps(reports,indent=2)+'\n');(out/'result.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps({'status':result['status'],'receipt_sha256':sha(out/'result.json')},indent=2))
