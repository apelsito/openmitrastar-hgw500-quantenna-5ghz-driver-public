import json,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[3];sys.path.insert(0,str(ROOT/'tools'))
from qualify_f370_qdrv_module import Elf
out=Path(__file__).parent;reports={}
for c in ['switch_tqe','topaz_congest_queue']:
 old=Elf(ROOT/f'outputs/rea/driver5g-ops-20261022-tqe-provider-build-r2/a/{c}/{c}.ko')
 new=Elf(ROOT/f'outputs/rea/driver5g-ops-20261022-tqe-r3-typed-batch-build-r2/a/{c}/{c}.ko')
 os={s['name']:s for s in old.syms if s['name']};ns={s['name']:s for s in new.syms if s['name']}
 additions=sorted(set(ns)-set(os));moved=[]
 for name,s in os.items():
  if name not in ns:continue
  t=ns[name]
  if s['value']!=t['value']:moved.append({'name':name,'section':old.symsection(s),'type':s['type'],'old_value':s['value'],'new_value':t['value'],'size':s['size']})
 reports[c]={'additions':additions,'moved':moved}
assert not(out/'symbol-layout-review.json').exists();(out/'symbol-layout-review.json').write_text(json.dumps(reports,indent=2)+'\n')
print(json.dumps({c:{'additions':r['additions'],'moved':r['moved']}for c,r in reports.items()},indent=2))
