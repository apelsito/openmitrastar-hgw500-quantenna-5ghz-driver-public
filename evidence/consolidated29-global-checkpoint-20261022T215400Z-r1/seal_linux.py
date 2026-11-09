"""Seal source29 two complete modulewaves, matchedcontrol andall currentpins.

Candidatefunctionsemantics boundto25/R12reviews+actual29sourcefixtures. No
hardware/client/release claim; no449/461integration. First55partialobjects
acceptedonlyunderactualsource/dependencyhashchecks;CCMPlateobjectnotused.
"""
import hashlib,json,sys,struct,importlib.util
from collections import Counter
from pathlib import Path
ROOT=Path('/workspace/user-home/Desktop/MitraStar');sys.path.insert(0,str(ROOT/'tools'));from qualify_f370_qdrv_module import Elf;from recover_hgw5g_driver_ops import Elf as OpsElf
out=Path(__file__).parent;sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest();pair=ROOT/'outputs/rea/consolidated29-full-pair-20261022T214100Z-r2';first,second=pair/'a-joint',pair/'b-joint';control=ROOT/'outputs/rea/consolidated29-control-joint-20261022T214400Z-r1';source=ROOT/'outputs/rea/consolidated29-source-20261022T212500Z-r1';abi=ROOT/'outputs/rea/consolidated29-full-prefix-abi-20261022T212900Z-r1';residual=ROOT/'outputs/rea/consolidated29-residual-20261022T214700Z-r7/result.json';r=json.loads(residual.read_text());assert r['status']=='PASS_CONSOLIDATED23_BEACON_R12_QDRV_WLAN_MATCHED_HEADER_RESIDUAL';metadata=out/'complete-symbol-metadata.json';assert sha(metadata)=='2147746db49776102ce94c16a16d24b3bdd7c6c0e0862fb357a6fbb26961f804'
pins={};closures=[]
for directory in[first,second,control]:
 proof=json.loads((directory/'result.json').read_text());assert proof['status']=='PASS_DEDUP_JOINT_COMPOSITION_ALL_STRONG_IMPORTS'and proof['complete_modules']==7
 table=json.loads((directory/'input-pins.json').read_text())
 for path,h in table.items():assert sha(Path(path))==h,'actualcurrentpin '+path;pins[path]=h
 closures.append({'path':str(directory),'current_pins':len(table),'strong_import_contract':'PASS'})
cp=json.loads((pair/'result.json').read_text());assert cp['total_verified_source_compiles']==116 and cp['source_units_firstwave_verified_reused']==55 and cp['source_units_firstwave_recompiled']==3 and cp['source_units_secondwave_fresh']==58 and cp['reproducible'];mods=[]
for row in cp['modules']:
 c=row['component'];a=first/c/(c+'.ko');b=second/c/(c+'.ko');assert sha(a)==sha(b)==row['sha256']and a.stat().st_size==row['bytes'];mods.append(row);pins[str(a)]=sha(a);pins[str(b)]=sha(b)
names=['wlan_scan_ap','wlan_scan_sta','wlan_ccmp','wlan_tkip','wlan_xauth'];ancillary={}
def pathnorm(s):return s.replace('consolidated29-control-source-20261022T213300Z-r1','consolidated29-source-20261022T212500Z-r1')
for c in names:
 o,n=Elf(control/c/(c+'.ko')),Elf(first/c/(c+'.ko'));assert set(o.byname)==set(n.byname)
 for name,s in o.byname.items():
  t=n.byname[name];assert all(s[k]==t[k]for k in['type','flags','align','entsize']),('ancillarymeta',c,name)
  if s['type']==4 or name in['.symtab','.strtab','.shstrtab','.note.gnu.build-id']:continue
  if name=='.rodata.str1.4':assert pathnorm(s['data'].decode('latin1'))==t['data'].decode('latin1')
  else:assert s['size']==t['size']and s['data']==t['data'],('ancillarydata',c,name)
 def sy(e,s):
  sec=e.symsection(s);value=pathnorm(e.cstring(sec,s['value']))if sec=='.rodata.str1.4'and s['type']!=3 else s['value'];return(s['name'],sec,value,s['size'],s['type'],s['bind'],s['other'])
 assert Counter(sy(o,s)for s in o.syms)==Counter(sy(n,s)for s in n.syms),'ancillaryallsymbols '+c
 def rel(e,r):
  s=e.syms[r['symbol']];sec=e.symsection(s);target=('string',pathnorm(e.cstring(sec,s['value']+r['addend'])))if sec=='.rodata.str1.4'else(sy(e,s),r['addend']);return(r['section'],r['offset'],r['type'],target)
 assert Counter(rel(o,r)for r in o.relas)==Counter(rel(n,r)for r in n.relas),'ancillaryallRELA '+c;ancillary[c]={'status':'PASS_COMPLETE_CODE_DATA_SYMBOL_RELA','symbols':len(o.syms),'relocations':len(o.relas)}
ar=json.loads((abi/'result.json').read_text());assert ar['old_members_checked']=={'ieee80211com':384,'ieee80211vap':210,'qdrv_wlan':72,'qdrv_vap':23,'ieee80211_beacon_offsets':17,'ieee80211_beacon_param_t':5}
for path,h in json.loads((abi/'input-pins.json').read_text()).items():assert sha(Path(path))==h;pins[path]=h
fr=[];fixture_names=['consolidated29-coc-fixture-20261022T212600Z-r1','consolidated29-coc-interleaving-20261022T212800Z-r1','consolidated29-coc-ioctl-20261022T213500Z-r1','consolidated29-legacy-fixture-20261022T212600Z-r1','consolidated29-app-fixture-20261022T212600Z-r1','consolidated29-descriptor-fixture-20261022T212800Z-r1','consolidated29-cleanup-fixture-20261022T212800Z-r1','consolidated29-neighbor-fixture-20261022T213100Z-r1','consolidated29-capability-paths-20261022T213500Z-r1']
for name in fixture_names:
 d=ROOT/'outputs/rea'/name;proof=json.loads((d/'result.json').read_text());assert proof['status'].startswith('PASS_');fr.append({'path':str(d/'result.json'),'status':proof['status']})
 for p in d.rglob('*'):
  if p.is_file():pins[str(p)]=sha(p)
records=OpsElf(first/'wlan/wlan.ko').ioctl_records();assert len(records)==595;previous=OpsElf(ROOT/'outputs/rea/driver5g-ops-20261022-coc25-joint-a-r6/wlan/wlan.ko').ioctl_records();rec=lambda x:(x['cmd'],x['set_args'],x['get_args'],x['name']);assert[rec(x)for x in records[:591]]==[rec(x)for x in previous];assert len(records[591:])==4
# Actualinstruction/parserresidual negative mutations; newoutputkeptimmutable.
checker=ROOT/'outputs/rea/consolidated29-residual-20261022T214700Z-r7/adapted-strict-residual.py';spec=importlib.util.spec_from_file_location('c29',checker);m=importlib.util.module_from_spec(spec);spec.loader.exec_module(m);target=first/'wlan/wlan.ko';e=Elf(target);s=next(s for s in e.syms if s['name']=='ieee80211_chan2ieee');negative=[]
for name,pos in [('unrelated_code',e.sections[s['section']]['offset']+s['value']),('allocated_data',e.byname['.rodata']['offset'])]:
 data=bytearray(target.read_bytes());data[pos]^=1;p=out/(name+'.ko');p.write_bytes(data)
 try:m.audit(control/'wlan/wlan.ko',p,'wlan')
 except Exception as ex:negative.append({'case':name,'rejected':True,'reason':str(ex)})
 else:raise AssertionError('negativeaccepted '+name)
refs=[pair/'result.json',source/'result.json',metadata,residual,checker,out/'metadata-adaptation-pins.json',abi/'result.json',ROOT/'outputs/rea/driver5g-ops-20261022-coc25-independent-final-review-r6/result.json',ROOT/'outputs/rea/consolidated23-beacon-r12-capability-audit-20261022T212200Z-r1/result.json',Path(__file__)]
for p in refs:pins[str(p)]=sha(p)
for path,h in pins.items():assert sha(Path(path))==h,'finalpostpin '+path
result={'status':'PASS_OFFLINE_CONSOLIDATED29_RECORDS_SOURCE_MODULE_CHECKPOINT','hardware_accessed':False,'firmware_eligible':False,'offline_original_records_reconstructed':29,'remaining_original_records':15,'private_table_records':595,'inherited_faithful_unsupported_records':2,'fullfunctional_driver_ops':None,'excluded':['449','461'],'modules':mods,'source_compile_count':116,'firstwave_reused_verified_fresh_compiles':55,'firstwave_unqualified_CCMP_recompiled_and_other_remaining':3,'secondwave_fresh_compiles':58,'sameheader_control':{'unchanged_objects':52,'changed_units_precompiled':6,'noadditional_candidatebuildclaim':True},'closures':closures,'all_current_review_pins':len(pins),'QDRV_WLAN_residual':r['audits'],'full_old_symbol_metadata_file':str(metadata),'metadata_old_named_rows':{'qdrv':1913,'wlan':2242},'new_exports_0_4_8_verified':4,'ancillary_full_audits':ancillary,'ABIoldmembers':ar['old_members_checked'],'fixture_receipts':fr,'negative_cases':negative,'limits':['29isoriginalprivateRECORDcountwithsetters/gettersseparate; not29physical/full80211featureclaims.','ValidinitializedretainedICCoCscope; existingpreinitifattachfailure/forcedunload/externalentireCSAteardownexcluded.','492/493disabledbeaconRRMpresenceandshortTLV/associationactualcopiedlengthdeviationsretained.','NACscalar snapshotsmaymixgenerations; twofaithfulunsupportedrecordsretained.','ABIpostcompiledepwitnessscopeexplicit; nooldmodulesmix.','NoRFclientsbidirectionaltrafficcoldbootpersistence/rollback/recovery/releaseacceptance.']};(out/'input-pins.json').write_text(json.dumps(pins,indent=2)+'\n');(out/'result.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps({'status':result['status'],'receipt_sha256':sha(out/'result.json'),'pins':len(pins),'offline_records':29,'remaining':15},indent=2))
