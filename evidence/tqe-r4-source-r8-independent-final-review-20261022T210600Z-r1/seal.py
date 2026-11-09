"""Immutable R4 bounded semantic/modules/full symbol-value supplement."""
import copy,hashlib,json,subprocess,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[3];out=Path(__file__).parent;sys.path.insert(0,str(ROOT/'tools'));from qualify_tqe_r4_typed_queue import audit;from qualify_f370_qdrv_module import Elf
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest();layout=out/'symbol-layout-review.json';assert sha(layout)=='557c897f63e005643680455b8abe0e75bdaa88c7f74dddb94d138a0033151e65';rows=json.loads(layout.read_text());build=ROOT/'outputs/rea/driver5g-ops-20261022-tqe-r4-typed-queue-build-r5';source=ROOT/'outputs/rea/driver5g-ops-20261022-tqe-r4-typed-queue-source-r8';parent=ROOT/'outputs/rea/driver5g-ops-20261022-tqe-r3-typed-batch-build-r2';globalgate=ROOT/'outputs/rea/driver5g-ops-20261022-tqe-r4-typed-queue-global-r5';proof=json.loads((build/'result.json').read_text());assert proof['reproducible']and proof['pins_verified']==1140;assert sha(source/'topaz_congest_queue.c')=='659c422a7f0c97ddc4c7b08c31df8047ef8e040c97e6c863c719f260a9bb9e6b';pins=json.loads((out/'actual-linux-pin-review.json').read_text());assert[r['pins']for r in pins['reports']]==[1140,1158]
modules=[];reports={};negative=[]
for c in ['switch_tqe','topaz_congest_queue']:
 op=parent/f'a/{c}/{c}.ko';np=build/f'a/{c}/{c}.ko';bp=build/f'b/{c}/{c}.ko';assert sha(op)==rows[c]['old_module_sha256']and sha(np)==rows[c]['new_module_sha256']==sha(bp);reports[c]=audit(op,np,c);o,n=Elf(op),Elf(np);os={s['name']:s for s in o.syms if s['name']};ns={s['name']:s for s in n.syms if s['name']};moved={r['name']:r for r in rows[c]['moved']};rename=rows[c]['rename']
 def check(newsy):
  assert set(newsy)-{rename.get(k,k)for k in os}==set(rows[c]['new_names'])
  for name,s in os.items():
   t=newsy[rename.get(name,name)]
   if name in moved:r=moved[name];assert(s['value'],t['value'],o.symsection(s),s['type'])==(r['old_value'],r['new_value'],r['section'],r['type']),('reviewedoldvalue',name)
   else:assert s['value']==t['value'],('unreviewedoldvalue',name)
 check(ns)
 for kind in ['old_function_value','added_name']:
  mutated=copy.deepcopy(ns)
  if kind=='old_function_value':name=next(k for k,s in os.items()if s['type']==2);mutated[rename.get(name,name)]['value']+=2
  else:mutated['extra_unreviewed']=copy.deepcopy(next(iter(ns.values())))
  try:check(mutated)
  except AssertionError:negative.append(c+':'+kind+' rejected')
  else:raise AssertionError('negative accepted')
 for lane,path in [('a',np),('b',bp)]:
  row=next(r for r in proof['modules']if r['name']==c and r['lane']==lane);assert sha(path)==row['sha256']and path.stat().st_size==row['bytes'];modules.append(row)
 reports[c].update(all_old_named_values_checked=len(os),all_new_membership_exact=len(rows[c]['new_names']))
fixtures=[]
for name,cases,fn in [('tqe-r4-source-r8-independent-queue-20261022T205800Z-r1',10,'actual-typed-queue.c'),('tqe-r4-source-r8-independent-initial-retry-20261022T210700Z-r1',3,'actual-enqueue-retry.c')]:
 d=ROOT/'outputs/rea'/name;r=json.loads((d/'result.json').read_text());assert r['cases']==cases and r['fixture_sha256']==sha(d/fn);p=subprocess.run([str(d/'fixture.exe')],capture_output=True);assert not p.returncode;fixtures.append({'path':str(d),'cases':cases,'fixture_sha256':sha(d/fn),'executable_sha256':sha(d/'fixture.exe')})
atomic=ROOT/'outputs/rea/tqe-r4-source-r8-independent-atomic-commit-20261022T205700Z-r1/result.json';assert json.loads(atomic.read_text())['facts']['newbatch_credit_pointer_preserved'];oldrace=ROOT/'outputs/rea/tqe-r4-source-r7-independent-credit-race-20261022T205300Z-r1/result.json';assert json.loads(oldrace.read_text())['facts']['newbatch_owner_credit_clobbered']
refs=[layout,out/'actual-linux-pin-review.json',build/'result.json',build/'input-pins.json',globalgate/'result.json',globalgate/'input-pins.json',globalgate/'complete-audit.json',source/'result.json',source/'topaz_congest_queue.c',source/'switch_tqe.c',source/'openmitrastar_tqe_bounded_ownership.h',ROOT/'tools/qualify_tqe_r4_typed_queue.py',atomic,oldrace]
result={'status':'PASS_INDEPENDENT_R4_SOURCE_R8_BOUNDED_TYPED_QUEUE_RETRY_PAIR','hardware_accessed':False,'firmware_eligible':False,'modules':modules,'fresh_actual_module_audits':reports,'actual_Linux_current_build_global_pins':[1140,1158],'independent_fixture_replays':fixtures,'symbol_negative_cases':negative,'original_R7_credit_pointer_race_reproduced':True,'source_R8_actual_atomic_success_dequeue_fix_confirmed':True,'limits':['UP/PREEMPT_NONE/nonPCIe only. Permanentmodulepins retained; noreset/recovery/forceunloadacceptance.','MMIO/VLAN/queuecapacity fake; actualprovider/TQEcallbacks/queuexmit tested for13cases, notallnodes/queues/scalarwriters.','Queueclear typedowner refusesHBMfree; originaluntypedHBMwaits/callbacklatency/foreignfirmwarewriters remainunqualified.','105outerfixture fromR3 ishistoricalentryevidence andnotcountedasR4directcallerexecution.','NohardwareRFclienttrafficbootpersistence/releaseacceptance.'],'evidence_pins':[{'path':str(p),'bytes':p.stat().st_size,'sha256':sha(p)}for p in refs]};(out/'result.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps({'status':result['status'],'receipt_sha256':sha(out/'result.json')},indent=2))
