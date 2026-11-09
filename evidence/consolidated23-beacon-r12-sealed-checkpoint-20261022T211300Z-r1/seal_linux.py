"""Seal bounded R12 source58x2/module/data/metadata checkpoint; nohardware."""
import hashlib,json,sys,subprocess,struct
from pathlib import Path
ROOT=Path('/workspace/user-home/Desktop/MitraStar');sys.path.insert(0,str(ROOT/'tools'));from qualify_f370_qdrv_module import Elf;from qualify_consolidated23_beacon_r12 import normpath
out=Path(__file__).parent;sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest();source=ROOT/'outputs/rea/consolidated23-beacon-r12-source-20261022T203200Z-r2';pair=ROOT/'outputs/rea/consolidated23-beacon-r12-complete-pair-20261022T205500Z-r1';control=ROOT/'outputs/rea/consolidated23-beacon-r12-control-joint-20261022T210400Z-r1';first=pair/'first-joint';second=pair/'second-joint';pins={};closures=[]
for directory in [first,second,control]:
 result=json.loads((directory/'result.json').read_text());assert result['status']=='PASS_DEDUP_JOINT_COMPOSITION_ALL_STRONG_IMPORTS'
 table=json.loads((directory/'input-pins.json').read_text())
 for path,h in table.items():assert sha(Path(path))==h,'actualclosurepin '+path;pins[path]=h
 closures.append({'path':str(directory),'pins':len(table),'source_units':58})
metadata=out/'complete-symbol-metadata.json';assert sha(metadata)=='10bb54408bbeb70c0ce986624d550ec661296a3420567c79b4b133a6160764bf';residual=ROOT/'outputs/rea/consolidated23-beacon-r12-global-residual-20261022T211100Z-r8/result.json';r=json.loads(residual.read_text());assert r['status']=='PASS_CONSOLIDATED23_BEACON_R12_QDRV_WLAN_MATCHED_HEADER_RESIDUAL';proof=json.loads((pair/'pair-result.json').read_text());assert proof['reproducible'];mods=[]
for row in proof['modules']:
 c=row['component'];a=first/c/(c+'.ko');b=second/c/(c+'.ko');assert sha(a)==sha(b)==row['sha256']and a.stat().st_size==row['bytes'];mods.append(row);pins[str(a)]=sha(a);pins[str(b)]=sha(b)
ancillary={}
for c in ['wlan_scan_ap','wlan_scan_sta','wlan_ccmp','wlan_tkip','wlan_xauth']:
 a,b=Elf(control/c/(c+'.ko')),Elf(first/c/(c+'.ko'));assert set(a.byname)==set(b.byname)
 for name,s in a.byname.items():
  t=b.byname[name];assert all(s[k]==t[k]for k in ['type','flags','align','entsize']),('ancillarysection',c,name)
  if s['type']==4 or name in ['.symtab','.strtab','.shstrtab','.note.gnu.build-id']:continue
  if name=='.rodata.str1.4':assert normpath(s['data'].decode('latin1'))==t['data'].decode('latin1')
  else:assert s['size']==t['size']and s['data']==t['data'],('ancillarybytes',c,name)
 def sy(e,s):
  sec=e.symsection(s);value=normpath(e.cstring(sec,s['value']))if sec=='.rodata.str1.4'and s['type']!=3 else s['value'];return(s['name'],sec,value,s['size'],s['type'],s['bind'],s['other'])
 from collections import Counter
 assert Counter(sy(a,s)for s in a.syms)==Counter(sy(b,s)for s in b.syms),'ancillarysymbols '+c
 def rel(e,r):
  s=e.syms[r['symbol']];sec=e.symsection(s);target=('string',normpath(e.cstring(sec,s['value']+r['addend'])))if sec=='.rodata.str1.4'else(sy(e,s),r['addend']);return(r['section'],r['offset'],r['type'],target)
 assert Counter(rel(a,r)for r in a.relas)==Counter(rel(b,r)for r in b.relas),'ancillaryrelas '+c;ancillary[c]={'status':'PASS_ALL_CODE_DATA_SYMBOL_RELA_DECLARED_SOURCE_PATH_MAP','symbols':len(a.syms),'relocations':len(a.relas)}
fixtures=[ROOT/'outputs/rea'/name/'result.json'for name in ['consolidated23-beacon-r12-app-fixture-20261022T203600Z-r1','consolidated23-beacon-r12-cleanup-fixture-20261022T203700Z-r1','consolidated23-beacon-r12-legacy23-fixture-20261022T203800Z-r1','consolidated23-beacon-r12-descriptor-fixture-20261022T203900Z-r3','consolidated23-beacon-r12-neighbor-fixture-20261022T204000Z-r1']]
for p in fixtures:assert json.loads(p.read_text())['status'].startswith('PASS_')
abi=ROOT/'outputs/rea/consolidated23-beacon-r12-full-prefix-abi-20261022T204400Z-r2/result.json';ab=json.loads(abi.read_text());assert ab['status']=='PASS_ARC_CONSOLIDATED23_FULL_PARSED_PREFIX_AND_DECLARED_EMBEDDING_EXTENTS'
for path,h in json.loads((abi.parent/'input-pins.json').read_text()).items():assert sha(Path(path))==h;pins[path]=h
# Strong focused ELF negative checks; residual verifier cannot accept changed
# unrelatedbytes orRELA. Preserve mutant files underthisimmutable checkpoint.
from qualify_consolidated23_beacon_r12 import audit
target=first/'wlan/wlan.ko';e=Elf(target);symbol=next(s for s in e.syms if s['name']=='ieee80211_chan2ieee');negative=[]
for name,pos in [('unrelated_code',e.sections[symbol['section']]['offset']+symbol['value']),('allocated_data',e.byname['.rodata']['offset'])]:
 data=bytearray(target.read_bytes());data[pos]^=1;path=out/(name+'.ko');path.write_bytes(data)
 try:audit(control/'wlan/wlan.ko',path,'wlan')
 except Exception as ex:negative.append({'case':name,'rejected':True,'reason':str(ex)})
 else:raise AssertionError('negativeaccepted '+name)
refs=[metadata,residual,pair/'pair-result.json',abi,source/'result.json',ROOT/'tools/qualify_consolidated23_beacon_r12.py',Path(__file__),*fixtures]
for p in refs:pins[str(p)]=sha(p)
for path,h in pins.items():assert sha(Path(path))==h,'checkpointpostpin '+path
result={'status':'PASS_OFFLINE_CONSOLIDATED23_BEACON_R12_SOURCE_MODULE_CHECKPOINT_PENDING_CAPABILITY_AUDIT','hardware_accessed':False,'firmware_eligible':False,'modules':mods,'source_compile_units':116,'closures':closures,'all_current_review_pins':len(pins),'QDRV_WLAN_residual':r['audits'],'complete_old_symbol_metadata_file':str(metadata),'new_exports_0_4_8_checked':4,'ancillary_all_code_data_symbols_RELA':ancillary,'ABI_old_members_checked':ab['old_members_checked'],'fixture_receipts':[str(p)for p in fixtures],'negative_cases':negative,'inherited_SOURCE23_records':23,'added_partial_BSS_neighbor_source_records':4,'combined_source_table_records':27,'private_table_records':593,'qualified_combined_functional_operation_count':None,'limits':['R12source27records isnot27physicalfunctionalops; semanticcapabilityauditstillpending.','SOURCE19beaconfixes+SOURCE23ops combined: changedfunctions source/fixturebound,notuniversalOEMsemantic equivalence.','FourBSS/neighborrecords partialadvertisement/controlimplementation; no80211kmanagementhandlerproof.','ABI906dep witnesspostcompilehashscopeexplicit. Validprefix/declaredembeddingonly, nooldmodulesmix.','NoSMPgeneralVAP/objectlifetime/RFclienttrafficbootpersistentrollback/recovery/releaseacceptance.']};(out/'input-pins.json').write_text(json.dumps(pins,indent=2)+'\n');(out/'result.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps({'status':result['status'],'receipt_sha256':sha(out/'result.json'),'pins':len(pins),'negative_cases':len(negative)},indent=2))
