"""Source-only29 convergence checkpoint; no linkedmodule/physical promotion."""
import hashlib,json,sys
from pathlib import Path
ROOT=Path('/workspace/user-home/Desktop/MitraStar');sys.path.insert(0,str(ROOT/'tools'));from recover_hgw5g_driver_ops import Elf
out=Path(__file__).parent;sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest();source=ROOT/'outputs/rea/consolidated29-source-20261022T212500Z-r1';syntax=ROOT/'outputs/rea/consolidated29-syntax-20261022T212700Z-r1';abi=ROOT/'outputs/rea/consolidated29-full-prefix-abi-20261022T212900Z-r1';sp=json.loads((source/'result.json').read_text());assert sp['combined_source_records']==29 and sp['preserved_CoC_source_helpers'];pins={}
for rel,h in sp['files'].items():p=source/rel.replace('\\','/');assert sha(p)==h;pins[str(p)]=h
for directory in [syntax,abi]:
 for path,h in json.loads((directory/'input-pins.json').read_text()).items():assert sha(Path(path))==h;pins[path]=h
sr=json.loads((syntax/'result.json').read_text());ar=json.loads((abi/'result.json').read_text());assert sr['status']=='PASS_ACTUAL_TYPED_HEADER_SYNTAX_UNITS'and len(sr['units'])==6;assert ar['old_members_checked']=={'ieee80211com':384,'ieee80211vap':210,'qdrv_wlan':72,'qdrv_vap':23,'ieee80211_beacon_offsets':17,'ieee80211_beacon_param_t':5}
object=syntax/'a/wlan/ieee80211_wireless.o';records=Elf(object).ioctl_records();assert len(records)==595
base=Elf(ROOT/'outputs/rea/driver5g-ops-20261022-coc25-joint-a-r6/wlan/wlan.ko').ioctl_records();r=lambda row:(row['cmd'],row['set_args'],row['get_args'],row['name']);assert[r(x)for x in records[:591]]==[r(x)for x in base]
assert {r(x)for x in records[591:]}=={(493,18433,0,'bss_tm'),(493,0,18433,'get_bss_tm'),(492,18433,0,'neigh_repo'),(492,0,18433,'get_neigh_repo')}
fixtures=['consolidated29-coc-fixture-20261022T212600Z-r1','consolidated29-coc-interleaving-20261022T212800Z-r1','consolidated29-coc-ioctl-20261022T213500Z-r1','consolidated29-legacy-fixture-20261022T212600Z-r1','consolidated29-app-fixture-20261022T212600Z-r1','consolidated29-descriptor-fixture-20261022T212800Z-r1','consolidated29-cleanup-fixture-20261022T212800Z-r1','consolidated29-neighbor-fixture-20261022T213100Z-r1','consolidated29-capability-paths-20261022T213500Z-r1'];fr=[]
for name in fixtures:
 d=ROOT/'outputs/rea'/name;r=json.loads((d/'result.json').read_text());assert r['status'].startswith('PASS_');fr.append({'path':str(d/'result.json'),'status':r['status']});pins[str(d/'result.json')]=sha(d/'result.json')
 for p in d.rglob('*'):
  if p.is_file():pins[str(p)]=sha(p)
for p in [source/'result.json',syntax/'result.json',abi/'result.json',object,Path(__file__)]:pins[str(p)]=sha(p)
for path,h in pins.items():assert sha(Path(path))==h
result={'status':'PASS_SOURCE_ONLY_CONSOLIDATED29_PRESERVED25_ABI_AND_FIXTURES_PENDING_LINKED_MODULES','hardware_accessed':False,'firmware_eligible':False,'source29_private_records':29,'qualified_combined_records':None,'actual_ARC_private_table_object_records':595,'inherited_COC25_table_records_preserved':591,'added_records':records[591:],'old_members_checked':ar['old_members_checked'],'actual_current_pins':len(pins),'source_units_compiled_for_syntax':6,'fixture_receipts':fr,'excluded':['449','461'],'limits':['Full58source2x/jointmodules/currentglobalresidual/delta stillpending.','29recordssourceonly; inherited25validinit/NAC/unsupported and492/493advertisementdeviationsretained.','NoRFclienttrafficbootpersistence/rollback/recovery/releaseacceptance.']};(out/'input-pins.json').write_text(json.dumps(pins,indent=2)+'\n');(out/'result.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps({'status':result['status'],'receipt_sha256':sha(out/'result.json'),'actual_current_pins':len(pins)},indent=2))
