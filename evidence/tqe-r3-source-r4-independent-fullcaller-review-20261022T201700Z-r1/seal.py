"""Independent source-bound full multicast caller fixture receipt, no hardware."""
import hashlib,json,subprocess,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[3];out=Path(__file__).parent;sys.path.insert(0,str(ROOT/'tools'))
from prepare_tqe_bounded_admission_prototype import function
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
source=ROOT/'outputs/rea/driver5g-ops-20261022-tqe-r3-typed-batch-source-r4'
fixture=ROOT/'outputs/rea/tqe-r3-source-r4-independent-fullcaller-20261022T201600Z-r1'
text=(fixture/'actual-full-multicast-caller.c').read_text();f=json.loads((fixture/'result.json').read_text());assert f['cases']==259 and sha(fixture/'actual-full-multicast-caller.c')==f['fixture_sha256']
for name in ['omtqe_lhost_quarantined','omtqe_lhost_submit_reserved','omtqe_push_mcast_batch','tqe_push_mc_ports']:assert function((source/'switch_tqe.c').read_text(),name+'(')in text,('actual caller/body',name)
fw=ROOT/'work/sources/Quantenna-GPL/include/qtn/topaz_fwt_cpuif.h'
for name in ['topaz_fwt_sw_count_bits','topaz_fwt_sw_mcast_node_is_set','topaz_fwt_sw_mcast_enqueues','topaz_fwt_sw_mcast_do_per_node']:assert function(fw.read_text(),name+'(')in text,('actual OEM helper',name)
assert 'return (port == TOPAZ_TQE_WMAC_PORT);'in function(fw.read_text(),'topaz_fwt_sw_mcast_port_has_nodes(')
assert 'static int topaz_fwt_sw_mcast_port_has_nodes(uint8_t port){return port==2;}'in text
q=(source/'topaz_congest_queue.c').read_text();begin=q.index('#define OMCGQ_HELD_MAX');end=q.index('struct topaz_congest_queue *g_congest_queue_ptr',begin);assert q[begin:end].replace('try_module_get(THIS_MODULE)','try_module_get(&provider)')in text
r=subprocess.run([str(fixture/'fixture.exe')],capture_output=True);assert not r.returncode and json.loads(r.stdout)['cases']==259
parent=ROOT/'outputs/rea/tqe-r3-source-r4-independent-final-review-20261022T201400Z-r1/result.json';assert sha(parent)=='8cebb3834cca745d21d42163688b7b5a8691f1825130ced0e43c5d3a2537d8e0'
refs=[parent,fixture/'result.json',fixture/'actual-full-multicast-caller.c',fixture/'fixture.exe',source/'switch_tqe.c',source/'topaz_congest_queue.c',fw,ROOT/'tools/test_tqe_r3_full_multicast_caller.py']
result={'status':'PASS_INDEPENDENT_ACTUAL_MULTICAST_CALLER_ENUMERATION_TYPED_OWNER_SAMPLE','hardware_accessed':False,'firmware_eligible':False,'cases':259,'actual_tqe_push_mc_ports':True,'actual_typed_callback_provider':True,'actual_OEM_enumeration_helpers':4,'stub_port_has_nodes_verified_equivalent_WMAC2':True,'previous_fullcaller_fixture_gap_closed_for_these_cases':True,'limits':['All255nonzeroportbitmap cases use zero node bitmap/inputEMAC0; maximum134fanout case uses128WMACnodes withallports.','Incomplete enumeration/node filtering cases outsidethese samples are not universally exhausted.','Expectedcounts computedbyactualOEM helper: allports/no-nodecases additionally check publicationcount equality.','Real HBM refcount/cache/MMIO/module primitives are fakes. Owner-before-EC order is asserted; actualhardwareprimitiveacceptance remains unrun.','tqe_rx_multicast IP/FWTlookup/packetdecoder entrypoint is not executed bythisfixture.','146legacycallbackfixture remains historicalscalar-path evidence; notallactiveR3writers.','No recovery/foreignfirmwarewriter/forceunload/physicalRFclienttrafficpersistence/release acceptance.'],'evidence_pins':[{'path':str(p),'bytes':p.stat().st_size,'sha256':sha(p)}for p in refs]}
assert not(out/'result.json').exists();(out/'result.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps({'status':result['status'],'receipt_sha256':sha(out/'result.json')},indent=2))
