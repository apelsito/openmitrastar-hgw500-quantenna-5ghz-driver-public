"""Finite immutable typed-source-r4 independent pair review; no hardware."""
import copy,hashlib,json,subprocess,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[3];out=Path(__file__).parent;sys.path.insert(0,str(ROOT/'tools'))
from qualify_tqe_r3_typed_batch import audit,NEW,REMOVED
from qualify_f370_qdrv_module import Elf
from prepare_tqe_bounded_admission_prototype import function
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
layout=out/'symbol-layout-review.json'
assert sha(layout)=='7b41e3f89518d8437aacea4cc6af14301fd415a59e03ea479a5bcc70f2311541'
review=json.loads(layout.read_text());build=ROOT/'outputs/rea/driver5g-ops-20261022-tqe-r3-typed-batch-build-r2';source=ROOT/'outputs/rea/driver5g-ops-20261022-tqe-r3-typed-batch-source-r4';parent=ROOT/'outputs/rea/driver5g-ops-20261022-tqe-provider-build-r2';globalgate=ROOT/'outputs/rea/driver5g-ops-20261022-tqe-r3-typed-batch-global-r10'
assert sha(source/'topaz_congest_queue.c')=='3838b149e83fd58d5e1040be62f6598d1c061d415c88186c4298d9f6723d20b5'
proof=json.loads((build/'result.json').read_text());assert proof['reproducible'] and proof['pins_verified']==1140
pins=json.loads((out/'actual-linux-pin-review.json').read_text());assert pins['status']=='PASS_CURRENT_ACTUAL_LINUX_BUILD_AND_GLOBAL_PIN_CLOSURE'
assert [r['pins']for r in pins['reports']]==[1140,527]
reports={};modules=[];negative=[]
for c in ['switch_tqe','topaz_congest_queue']:
 op=parent/f'a/{c}/{c}.ko';np=build/f'a/{c}/{c}.ko';bp=build/f'b/{c}/{c}.ko'
 assert sha(np)==sha(bp)
 for p in[np,bp]:
  row=next(r for r in proof['modules']if r['name']==c and r['lane']==('a'if p==np else'b'))
  assert sha(p)==row['sha256'] and p.stat().st_size==row['bytes'];modules.append(row)
 o,n=Elf(op),Elf(np);r=audit(op,np,c);os={s['name']:s for s in o.syms if s['name']};ns={s['name']:s for s in n.syms if s['name']}
 allowed={r['name']:r for r in review[c]['moved']}
 def check_values(oldsy,newsy):
  assert set(newsy)-set(oldsy)==set(review[c]['additions'])
  for name,s in oldsy.items():
   if name in REMOVED[c]:continue
   t=newsy[name]
   if name in allowed:
    e=allowed[name];assert(s['value'],t['value'],o.symsection(s),s['type'])==(e['old_value'],e['new_value'],e['section'],e['type']),('reviewed canonical value',name)
   else:assert s['value']==t['value'],('unreviewed symbol value',name)
 check_values(os,ns)
 # Every emitted function occupies a unique, non-overlapping bounded range.
 for sec,section in n.byname.items():
  ranges=sorted((s['value'],s['value']+s['size'],s['name'])for s in n.syms if s['type']==2 and s['size']and n.symsection(s)==sec)
  for i,(start,end,name)in enumerate(ranges):
   assert 0<=start<end<=section['size']
   if i and start<ranges[i-1][1]:
    prev=ranges[i-1];assert(start,end)==prev[:2] and {name,prev[2]} in [{'init_module','tqe_module_init'},{'cleanup_module','tqe_module_exit'},{'init_module','topaz_congest_queue_module_init'},{'cleanup_module','topaz_congest_queue_module_exit'}],('function overlap',name)
 # Negative value/membership mutations must be rejected by this supplement.
 x=copy.deepcopy(ns);name=next(name for name,s in os.items()if s['type']==2 and name not in REMOVED[c]);x[name]['value']+=2
 try:check_values(os,x)
 except AssertionError:negative.append(c+':old-function-value mutation rejected')
 else:raise AssertionError('negative function-value mutation accepted')
 x=copy.deepcopy(ns);x['unreviewed_added_symbol']=dict(next(iter(ns.values())))
 try:check_values(os,x)
 except AssertionError:negative.append(c+':new-symbol-membership mutation rejected')
 else:raise AssertionError('negative new symbol mutation accepted')
 reports[c]={**r,'old_named_symbols_checked':len(os),'all_old_values_reviewed':True,'complete_new_membership':review[c]['additions'],'nonoverlapping_function_ranges':True}
fixtures=[]
for dirname,cases,filename in [('tqe-r3-source-r4-independent-typed-publication-20261022T201200Z-r1',142,'actual-typed-batch-publication.c'),('tqe-r3-source-r4-independent-capacity-20261022T201300Z-r1',35212,'actual-provider-batch.c'),('tqe-r3-source-r4-independent-normal-writers-20261022T201500Z-r1',146,'actual-provider-and-four-writers.c')]:
 d=ROOT/'outputs/rea'/dirname;f=json.loads((d/'result.json').read_text());assert f['cases']==cases and f['fixture_sha256']==sha(d/filename)
 p=subprocess.run([str(d/'fixture.exe')],capture_output=True);assert not p.returncode;fixtures.append({'path':str(d),'cases':cases,'source_sha256':sha(d/filename),'executable_sha256':sha(d/'fixture.exe')})
 if cases==142:
  text=(d/filename).read_text();cgq=(source/'topaz_congest_queue.c').read_text();start=cgq.index('#define OMCGQ_HELD_MAX');end=cgq.index('struct topaz_congest_queue *g_congest_queue_ptr',start)
  assert cgq[start:end].replace('try_module_get(THIS_MODULE)','try_module_get(&provider)')in text
  for name in ['omtqe_lhost_quarantined','omtqe_lhost_submit_reserved','omtqe_push_mcast_batch']:assert function((source/'switch_tqe.c').read_text(),name+'(')in text
caller=function((source/'switch_tqe.c').read_text(),'tqe_push_mc_ports(')
assert caller.index('omcgq_batch_admit(')<caller.index('tqe_buf_set_refcounts(')<caller.index('topaz_fwt_sw_mcast_do_per_node(')<caller.index('omcgq_batch_finish(')
refs=[layout,out/'actual-linux-pin-review.json',globalgate/'result.json',globalgate/'complete-audit.json',globalgate/'input-pins.json',build/'result.json',build/'input-pins.json',source/'result.json',*(source/n for n in ['switch_tqe.c','topaz_congest_queue.c','openmitrastar_tqe_bounded_ownership.h']),ROOT/'tools/qualify_tqe_r3_typed_batch.py',ROOT/'tools/test_tqe_r3_actual_typed_publication.py',ROOT/'tools/test_tqe_r3_typed_batch_capacity.py',ROOT/'tools/test_tqe_bounded_provider_pair.py']
result={'status':'PASS_INDEPENDENT_SOURCE_R4_TYPED_BATCH_BOUNDED_PAIR_WITH_EXPLICIT_CALLER_FIXTURE_GAP','hardware_accessed':False,'firmware_eligible':False,'modules':modules,'fresh_actual_module_audits':reports,'source_fixture_replays':fixtures,'negative_value_membership_cases':negative,'actual_linux_current_pin_checks':[1140,527],'caller_static_order_checked':'admit owner BEFORE refcounts BEFORE callbacks BEFORE finish','limits':['142 fixture executes actual typed callback/provider with fake MMIO/VLAN; expected/fanout inputs supplied by fixture.','Full tqe_push_mc_ports with actual multicast enumeration helpers is not executed by these fixtures and remains pending.','146 fixture contains legacy tqe_push_mcast no longer emitted; it does not prove all active R3 writer paths.','UP/PREEMPT_NONE/nonPCIe only; force unload/foreign firmware writers/HBM waits/recovery remain unqualified.','Refusals retain/quarantine a permanent owner; normal congestion retry is deliberately bypassed. No release/reset/recovery acceptance.','No RF/client/traffic/runtime/boot/persistence/release acceptance.'],'evidence_pins':[{'path':str(p),'bytes':p.stat().st_size,'sha256':sha(p)}for p in refs]}
assert not(out/'result.json').exists();(out/'fresh-module-audits.json').write_text(json.dumps(reports,indent=2)+'\n');(out/'result.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps({'status':result['status'],'receipt_sha256':sha(out/'result.json'),'module_count':len(modules),'negative_cases':len(negative)},indent=2))
