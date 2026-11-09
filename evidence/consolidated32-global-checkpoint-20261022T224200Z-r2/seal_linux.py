"""SealcompleteSOURCE32boundedprivate-record checkpoint, nophysicalpromotion."""
import hashlib,json,sys
from pathlib import Path
ROOT=Path('/workspace/user-home/Desktop/MitraStar');sys.path.insert(0,str(ROOT/'tools'));from recover_hgw5g_driver_ops import Elf
out=Path(__file__).parent;sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest();source=ROOT/'outputs/rea/consolidated32-source-20261022T222600Z-r2';pair=ROOT/'outputs/rea/consolidated32-pair-20261022T223400Z-r2';parent=ROOT/'outputs/rea/consolidated30-joint-a-20261022T221500Z-r2/joint';first,second=pair/'a-joint',pair/'b-joint';abi=ROOT/'outputs/rea/consolidated32-scanner-abi-20261022T223100Z-r2';modulegate=ROOT/'outputs/rea/consolidated32-residual-negative-20261022T224000Z-r2';pins={}
def normalized(p):
 text=str(p).replace('\\','/');return Path('/mnt/'+text[0].lower()+text[2:])if len(text)>2 and text[1]==':'else(Path(text)if text.startswith('/')else ROOT/text)
def importmap(p):
 data=json.loads(p.read_text())
 for path,h in data.items():path=normalized(path);assert sha(path)==h,'currentpin '+str(path);pins[str(path)]=h
 return len(data)
pr=json.loads((pair/'result.json').read_text());assert pr['status']=='PASS_CONSOLIDATED32_FOUR_CHANGED_UNITS_PAIRED_SEVEN_MODULES_PENDING_GLOBAL'and pr['reproducible']and pr['new_compiled_units']==8 and pr['reused_parent30_objects']==108;importmap(pair/'input-pins.json')
for d in[first,second]:
 r=json.loads((d/'result.json').read_text());assert r['status']=='PASS_DEDUP_JOINT_COMPOSITION_ALL_STRONG_IMPORTS'and r['complete_modules']==7 and r['modpost_diagnostics']=='empty';importmap(d/'input-pins.json')
ar=json.loads((abi/'result.json').read_text());assert ar['status']=='PASS_ARC_CONSOLIDATED32_PUBLIC_HOST_PREFIX_AND_PRIVATE_SCANNER_APPEND'and ar['old_scanner_members']=={'ieee80211_scan_state':17,'scan_state':8};importmap(abi/'input-pins.json')
sp=json.loads((source/'result.json').read_text());assert sp['source_original_records']==32
for rel,h in sp['files'].items():p=source/rel.replace('\\','/');assert sha(p)==h;pins[str(p)]=h
gate=json.loads((modulegate/'result.json').read_text());assert gate['status']=='PASS_CONSOLIDATED32_COMPLETE_RESIDUALS_AND_ELEVEN_PARSED_NEGATIVES'and gate['negative_count']==11 and all(x['rejected']for x in gate['negative_cases']);assert gate['audits']['wlan']['same_functions']==960 and gate['audits']['wlan_scan_ap']['same_functions']==24;importmap(modulegate/'input-pins.json')
modules=[];unchanged={}
for row in pr['modules']:
 c=row['component'];a=first/c/(c+'.ko');b=second/c/(c+'.ko');assert sha(a)==sha(b)==row['sha256']and a.stat().st_size==row['bytes'];modules.append(row);pins[str(a)]=sha(a);pins[str(b)]=sha(b)
 if c not in['wlan','wlan_scan_ap']:assert sha(a)==sha(parent/c/(c+'.ko'));unchanged[c]={'status':'BYTE_IDENTICAL_WHOLE_PARENT30_MODULE','sha256':sha(a)}
old=Elf(parent/'wlan/wlan.ko').ioctl_records();new=Elf(first/'wlan/wlan.ko').ioctl_records();record=lambda r:(r['cmd'],r['set_args'],r['get_args'],r['name']);assert len(new)==598 and[record(r)for r in new[:596]]==[record(r)for r in old]and[record(r)for r in new[596:]]==[(461,18433,0,'set_bw_autosel'),(461,0,18433,'get_bw_autosel')]
fixture_names=['consolidated32-start-fixture-20261022T222700Z-r2','consolidated32-next-end-fixture-20261022T222700Z-r2','consolidated32-critical-fixture-20261022T222700Z-r2','consolidated32-detach-fixture-20261022T223200Z-r2','consolidated32-449-policy-20261022T222800Z-r2','consolidated32-coc-fixture-20261022T222800Z-r2','consolidated32-app-fixture-20261022T222800Z-r2','consolidated32-policy-fixture-20261022T224100Z-r2'];fr=[]
for name in fixture_names:
 d=ROOT/'outputs/rea'/name;r=json.loads((d/'result.json').read_text());assert r['status'].startswith('PASS_');fr.append({'path':str(d/'result.json'),'status':r['status']})
 for p in d.rglob('*'):
  if p.is_file():pins[str(p)]=sha(p)
 # Cases with input_pins/pins retainbyteprovenance; normalizedWindowspathmaps.
 for key in['input_pins','pins']:
  if isinstance(r.get(key),dict):
   for path,h in r[key].items():p=normalized(path);assert sha(p)==h;pins[str(p)]=h
# Parent30allfields/headerbyteexact, publicscannerheaderappendsAPIonly.
base=ROOT/'outputs/rea/consolidated30-source-20261022T221100Z-r2'
for rel in['headers/net80211/ieee80211_var.h','headers/net80211/ieee80211_proto.h','headers/net80211/ieee80211_beacon_desc.h','source/qdrv/qdrv_wlan.c','source/wlan/ieee80211.c','source/wlan/ieee80211_input.c','source/wlan/ieee80211_node.c','source/wlan/ieee80211_output.c','source/wlan/ieee80211_beacon.c','source/wlan/ieee80211_beacon_desc.c']:assert sha(base/rel)==sha(source/rel)
for p in[pair/'result.json',source/'result.json',abi/'result.json',modulegate/'result.json',ROOT/'tools/audit_consolidated32_modules.py',ROOT/'tools/build_consolidated32_pair.py',ROOT/'outputs/rea/consolidated30-global-checkpoint-20261022T222200Z-r2/result.json',ROOT/'outputs/rea/chanset461-r15-review-handoff-20261022T230000Z-r1/REVIEW.md',Path(__file__)]:pins[str(p)]=sha(p)
for path,h in pins.items():assert sha(Path(path))==h,'postsealcurrentpin '+path
r={'status':'PASS_OFFLINE_CONSOLIDATED32_SCANNER_RECORDS_PAIRED_MODULE_GLOBAL_CHECKPOINT','hardware_accessed':False,'firmware_eligible':False,'offline_original_records_reconstructed':32,'remaining_original_records':12,'private_table_records':598,'faithful_unsupported_inherited':2,'fullfunctional_driver_operations':None,'modules':modules,'fresh_changed_source_compiles':8,'reused_exact_parent30_objects':108,'all_current_review_pins':len(pins),'whole_parent30_modules_exact':unchanged,'complete_WLAN_AP_module_audits':gate['audits'],'parsed_negative_cases':11,'private_scanner_ABI':{'old_public_fields':17,'old_private_fields':8,'sizes':{x['lane']:x['sizes']['scan_state']for x in ar['rows']},'extensions':ar['new_private_extensions']},'host_wire_ABI_unchanged':True,'fixture_receipts':fr,'existing_449_491_492_493_source_paths_preserved':True,'limits':['32privateRECORDS reconstructedwithsetters/gettersseparate, not32physical/fullfeatures.','461HT24APbranchonly;5GHzscannerexistingalgorithmretained, notallrank/scanset/OEMmetricsemanticsuniversalproof.','UP/PREEMPT_NONEvalidcallerRTNL/VAPserialization scope; externalarbitrarycallers/SMP/hardIRQ/realasyncMuClifetimeunqualified.','Sampled13wordsharedscanmetricsmaymixgenerations; nocoherenttupleproof.','Typed40states manuallyinstalledin7foregroundfixturecases; noautomatic40MHzscancoverageclaim.','Inherited449bounds/CoCvalidinit/NACsnapshot/492493advertisementdeviations/twoEOPNOTSUPPscope retained.','NoRFrealclientassociation/bidirectionaltrafficcoldbootpersistence/rollback/recovery/releaseacceptance.']};(out/'input-pins.json').write_text(json.dumps(pins,indent=2)+'\n');(out/'result.json').write_text(json.dumps(r,indent=2)+'\n');print(json.dumps({'status':r['status'],'receipt_sha256':sha(out/'result.json'),'pins':len(pins),'records':32,'remaining':12},indent=2))
