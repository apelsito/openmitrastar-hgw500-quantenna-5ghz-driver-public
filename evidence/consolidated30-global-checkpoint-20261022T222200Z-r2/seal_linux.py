"""Exactsource30r2globalgate:modules2x,source/delta/ABI/fixtures/negative/pins.

30privateRECORDS only. Driverphysicalandlifetimegatesremainunrun. Rejects
source30r1missingactualtimerdefinitiongate and binds selectedr2freshfixtures.
"""
import hashlib,json,sys
from pathlib import Path
ROOT=Path('/workspace/user-home/Desktop/MitraStar');sys.path.insert(0,str(ROOT/'tools'));from recover_hgw5g_driver_ops import Elf
out=Path(__file__).parent;sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest();source=ROOT/'outputs/rea/consolidated30-source-20261022T221100Z-r2';build=ROOT/'outputs/rea/consolidated30-incremental-20261022T221200Z-r2';first=ROOT/'outputs/rea/consolidated30-joint-a-20261022T221500Z-r2/joint';second=ROOT/'outputs/rea/consolidated30-joint-b-20261022T221500Z-r2/joint';parent=ROOT/'outputs/rea/consolidated29-full-pair-20261022T214100Z-r2/a-joint';abi=ROOT/'outputs/rea/consolidated30-full-abi-20261022T220600Z-r1';residual=ROOT/'outputs/rea/consolidated30-residual-20261022T221800Z-r4';neg=ROOT/'outputs/rea/consolidated30-negative-20261022T222000Z-r2';pins={}
def normalized(path):
 text=str(path).replace('\\','/');return Path('/mnt/'+text[0].lower()+text[2:])if len(text)>2 and text[1]==':'else(Path(text)if text.startswith('/')else ROOT/text)
def importpins(path):
 raw=json.loads(path.read_text())
 for p,h in raw.items():p=normalized(p);assert sha(p)==h,'actualcurrentpin '+str(p);pins[str(p)]=h
 return len(raw)
br=json.loads((build/'result.json').read_text());assert br['status']=='PASS_CONSOLIDATED30_FOUR_REAL_UNITS_PAIRED_EXACT_PARENT_CLOSURE'and br['reproducible']and br['new_compiled_units']==8 and br['reused_units']==108;importpins(build/'input-pins.json')
for directory in[first,second]:
 r=json.loads((directory/'result.json').read_text());assert r['status']=='PASS_DEDUP_JOINT_COMPOSITION_ALL_STRONG_IMPORTS'and r['complete_modules']==7 and r['modpost_diagnostics']=='empty';importpins(directory/'input-pins.json')
sp=json.loads((source/'result.json').read_text());assert sp['combined_source_records']==30
for rel,h in sp['files'].items():p=source/rel.replace('\\','/');assert sha(p)==h;pins[str(p)]=h
ar=json.loads((abi/'result.json').read_text());assert ar['status']=='PASS_ARC_CONSOLIDATED30_ALL_COMBINED_OLD_MEMBERS_SIZES_AND_POLICY_PADDING'and ar['policy_offset']==22667 and ar['host_delta']==0;assert ar['old_members_checked']=={'ieee80211com':386,'ieee80211vap':210,'qdrv_wlan':72,'qdrv_vap':23,'ieee80211_beacon_offsets':18,'ieee80211_beacon_param_t':8};importpins(abi/'actual/input-pins.json')
# R2changedonlywirelesstimerbody; ABIheader/qdrvincludeclosureidenticalR1.
oldsrc=ROOT/'outputs/rea/consolidated30-source-20261022T220000Z-r1'
for rel in['headers/net80211/ieee80211_var.h','headers/net80211/ieee80211_proto.h','headers/net80211/ieee80211_beacon_desc.h','source/qdrv/qdrv_wlan.c']:assert sha(oldsrc/rel)==sha(source/rel),'ABIreuse mismatch '+rel
rr=json.loads((residual/'result.json').read_text());assert rr['status']=='PASS_CONSOLIDATED30_COMPLETE_MODULE_RESIDUALS_PENDING_GLOBAL_PIN_GATE';assert rr['audits']['qdrv']['unchanged_functions']==829 and rr['audits']['wlan']['unchanged_functions']==962
for p,h in rr['tool_pins'].items():p=normalized(p);assert sha(p)==h;pins[str(p)]=h
negative=json.loads((neg/'result.json').read_text());assert negative['status']=='PASS_ELEVEN_CONSOLIDATED30_PARSED_NEGATIVE_DELTA_CASES'and negative['cases']==11 and all(r['rejected']for r in negative['rows']);importpins(neg/'actual/input-pins.json')
mods=json.loads((first/'result.json').read_text())['modules'];ancillary={}
for row in mods:
 c=row['component'];a=first/c/(c+'.ko');b=second/c/(c+'.ko');assert sha(a)==sha(b)==row['sha256']and a.stat().st_size==row['bytes'];pins[str(a)]=sha(a);pins[str(b)]=sha(b)
 if c not in['qdrv','wlan']:assert sha(a)==sha(parent/c/(c+'.ko'));ancillary[c]={'status':'BYTE_IDENTICAL_WHOLE_PARENT29_MODULE','sha256':sha(a)}
records=Elf(first/'wlan/wlan.ko').ioctl_records();old=Elf(parent/'wlan/wlan.ko').ioctl_records();rec=lambda r:(r['cmd'],r['set_args'],r['get_args'],r['name']);assert len(records)==596 and[rec(r)for r in records[:595]]==[rec(r)for r in old]and[rec(r)for r in records[595:]]==[(449,18433,0,'20_40_bss_coex')]
fixture_names=['consolidated30-policy-fixture-20261022T221100Z-r2','consolidated30-setter-fixture-20261022T221300Z-r2','consolidated30-public-fixture-20261022T221300Z-r2','consolidated30-parser-geometry-20261022T221300Z-r2','consolidated30-geometry-fixture-20261022T220100Z-r1','consolidated30-coc-fixture-20261022T220100Z-r1','consolidated30-coc-interleaving-20261022T220900Z-r1'];fr=[]
for name in fixture_names:
 d=ROOT/'outputs/rea'/name;r=json.loads((d/'result.json').read_text());assert r['status'].startswith('PASS_');fr.append({'path':str(d/'result.json'),'status':r['status']})
 for p in d.rglob('*'):
  if p.is_file():pins[str(p)]=sha(p)
# Bind inheritedunchangedR12CAP/App/CoC/BWsemantics tosamefunctionbytes.
from prepare_tqe_bounded_admission_prototype import function
coex=ROOT/'outputs/rea/coex449-source26-bss-geometry-20261022T192600Z-r3'
timer=function((source/'source/wlan/ieee80211_wireless.c').read_text(),'void ieee80211_start_obss_scan_timer(');assert timer==function((coex/'source/wlan/ieee80211_wireless.c').read_text(),'void ieee80211_start_obss_scan_timer(')
for name in['ieee80211.c','ieee80211_beacon.c','ieee80211_beacon_desc.c','ieee80211_output.c']:assert sha(source/'source/wlan'/name)==sha(ROOT/'outputs/rea/consolidated29-source-20261022T212500Z-r1/source/wlan'/name)
for p in[source/'result.json',build/'result.json',abi/'result.json',residual/'result.json',neg/'result.json',Path(__file__),ROOT/'tools/build_consolidated30_incremental.py',ROOT/'tools/prepare_consolidated30_coex449.py',ROOT/'outputs/rea/consolidated29-global-checkpoint-20261022T215400Z-r1/result.json',ROOT/'outputs/rea/coex449-r3-independent-final-modules-20261022T220300Z-r1/result.json']:pins[str(p)]=sha(p)
for p,h in pins.items():assert sha(Path(p))==h,'postfinalpin '+p
result={'status':'PASS_OFFLINE_CONSOLIDATED30_R2_RECORDS_PAIRED_MODULE_GLOBAL_CHECKPOINT','hardware_accessed':False,'firmware_eligible':False,'offline_original_records_reconstructed':30,'remaining_original_records':14,'private_table_records':596,'faithful_unsupported_inherited':2,'fullfunctional_driver_operations':None,'excluded':['461'],'modules':mods,'fresh_changed_source_units':8,'reused_exact_parent29_objects':108,'all_current_review_pins':len(pins),'ABI_old_members_checked':ar['old_members_checked'],'policy_padding_offset':22667,'host_sizes_and_old_fields_unchanged':True,'complete_module_audits':rr['audits'],'ancillary_exact_parent29_modules':ancillary,'parsed_negative_cases':negative['cases'],'fixture_receipts':fr,'selected_timer_source_matches_qualified449':True,'old_source30_r1_rejected_missing_timer_guard':str(oldsrc/'rejection.json'),'limits':['30privateRECORDS withset/getseparate; not30physical/fullfeatures.','InheritedvalidinitializedretainedCoC/NACsnapshot/twoEOPNOTSUPP boundaries retained.','NULL/ANYCBSSguard, publiclengthbounds/492493advertisementdeviations documented.','Otherfullrecv_mgmtparserpathsnotfullyreplayed; actualpolicyfragments/geometry/BWcallbacks boundedproofonly.','Wire/hostABIprefixwitnessandpairedsourceclosureoffline; noRFclienttrafficcoldbootpersistence/rollback/recovery/releaseacceptance.']};(out/'input-pins.json').write_text(json.dumps(pins,indent=2)+'\n');(out/'result.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps({'status':result['status'],'receipt_sha256':sha(out/'result.json'),'pins':len(pins),'offline_records':30,'remaining':14},indent=2))
