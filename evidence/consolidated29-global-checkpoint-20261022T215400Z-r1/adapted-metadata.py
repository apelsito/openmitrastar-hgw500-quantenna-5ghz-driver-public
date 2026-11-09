"""Freeze complete old/new symbol metadata inspection and reject mutations."""
import hashlib,json,sys,copy
from pathlib import Path
ROOT=Path('/workspace/user-home\\Desktop\\MitraStar');out=Path(__file__).parent;sys.path.insert(0,str(ROOT/'tools'));from qualify_f370_qdrv_module import Elf;import importlib.util
spec=importlib.util.spec_from_file_location('c29',ROOT/'outputs/rea/consolidated29-residual-20261022T214700Z-r7/adapted-strict-residual.py');checker=importlib.util.module_from_spec(spec);spec.loader.exec_module(checker);table,CHANGED,NEW,REN,normpath=[getattr(checker,n)for n in ['table','CHANGED','NEW','REN','normpath']]
control=ROOT/'outputs/rea/consolidated29-control-joint-20261022T214400Z-r1';candidate=ROOT/'outputs/rea/consolidated29-full-pair-20261022T214100Z-r2/a-joint';report={};negative=[]
for c in ['qdrv','wlan']:
 o,n=Elf(control/c/(c+'.ko')),Elf(candidate/c/(c+'.ko'));os,ns=table(o,c),table(n,c,True);matched=[];removed=[]
 for k,(i,s)in os.items():
  if not s['name']or s['type']==3:continue
  if k not in ns:
   literal=o.sections[s['section']]['data'][s['value']:s['value']+s['size']]if s['section']<len(o.sections)else b''
   assert c=='wlan'and k[0].startswith('__func__.'),'unexpectedmissingmetadata '+str(k)
   choices=[x for x in n.syms if n.symsection(x)=='.rodata'and x['name'].startswith('__func__.')and n.sections[x['section']]['data'][x['value']:x['value']+x['size']]==literal]
   if choices:ns[k]=(-1,choices[0])
   else:assert literal==b'ieee80211_beacon_alloc_ie\0';removed.append(k);continue
  _,t=ns[k]
  # Functionliteraloccurrences shiftafterallocatorliteral removal: bindbybytes.
  if o.symsection(s)=='.rodata'and s['name'].startswith('__func__.'):
   literal=o.sections[s['section']]['data'][s['value']:s['value']+s['size']]
   choices=[x for x in n.syms if n.symsection(x)=='.rodata'and x['name'].startswith('__func__.')and n.sections[x['section']]['data'][x['value']:x['value']+x['size']]==literal]
   if not choices:assert literal==b'ieee80211_beacon_alloc_ie\0';removed.append(k);continue
   t=choices[0]
  assert(s['type'],s['bind'],s['other'],o.symsection(s))==(t['type'],t['bind'],t['other'],n.symsection(t)),('oldattrs',k)
  if k[0]not in CHANGED[c]and k[0]not in ['ieee80211_get_prichan_list_by_operating_class','ieee80211_compile_action_measurement_11k','ieee80211com_media_change','ieee80211_priv_args']:assert s['size']==t['size'],('oldsize',k)
  if s['type']==1 and s['size']and k[0]not in ['ieee80211_priv_args','ieee80211_iw_handler_def']and not k[0].startswith('__ksymtab_'):
   x=o.sections[s['section']]['data'][s['value']:s['value']+s['size']];y=n.sections[t['section']]['data'][t['value']:t['value']+t['size']];assert x==y,('oldobjectpayload',k)
  matched.append({'key':k,'old_value':s['value'],'new_value':t['value'],'old_size':s['size'],'new_size':t['size'],'type':s['type'],'bind':s['bind'],'other':s['other']})
 if c=='wlan':
  exported={'om_beacon_reserved_remove','om_beacon_reserved_update','om_ieee80211_beacon_build_status','om_ieee80211_beacon_frame_status'}
  for name in exported:
   s=next(s for s in n.syms if s['name']==name);assert(s['type'],s['bind'],s['other'],n.symsection(s))==(2,1,0,'.text');record=next(s for s in n.syms if s['name']=='__ksymtab_'+name);assert(record['size'],record['type'],record['bind'],record['other'],n.symsection(record))==(12,1,0,0,'__ksymtab');rels={r['offset']-record['value']:r for r in n.relas if r['section']=='__ksymtab'and record['value']<=r['offset']<record['value']+12};assert set(rels)=={0,4,8};assert n.syms[rels[0]['symbol']]['name']==name
   for pos,expected in [(4,name),(8,'')]:r=rels[pos];s=n.syms[r['symbol']];assert n.cstring(n.symsection(s),s['value']+r['addend'])==expected
 # Exactnewnamedmembership followsstatic inspection; noextraordinarydata/symbol.
 oldnames={REN[c].get(s['name'],s['name'])for s in o.syms if s['name']};newnames={REN[c].get(s['name'],s['name'])for s in n.syms if s['name']};added=newnames-oldnames
 expected={'om_ieee80211_beacon_frame_status'}if c=='qdrv'else NEW[c]|{'memmove'}|{p+x for p in ['__kstrtab_','__kstrtabns_','__ksymtab_']for x in ['om_beacon_reserved_remove','om_beacon_reserved_update','om_ieee80211_beacon_build_status','om_ieee80211_beacon_frame_status']}
 assert added==expected,('newnamedmembership',added^expected)
 report[c]={'old_named_metadata_inspected':matched,'removed_exact_allocator_literal':removed,'new_named_membership':sorted(added),'new_exports0_4_8_verified':4 if c=='wlan'else 0}
(out/'complete-symbol-metadata.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({c:{'oldchecked':len(r['old_named_metadata_inspected']),'newnames':len(r['new_named_membership'])}for c,r in report.items()},indent=2));print(hashlib.sha256((out/'complete-symbol-metadata.json').read_bytes()).hexdigest())
