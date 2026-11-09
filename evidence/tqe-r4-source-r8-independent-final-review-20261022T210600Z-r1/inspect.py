import hashlib,json,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[3];out=Path(__file__).parent;sys.path.insert(0,str(ROOT/'tools'));from qualify_f370_qdrv_module import Elf
r={}
for c in ['switch_tqe','topaz_congest_queue']:
 op=ROOT/f'outputs/rea/driver5g-ops-20261022-tqe-r3-typed-batch-build-r2/a/{c}/{c}.ko';np=ROOT/f'outputs/rea/driver5g-ops-20261022-tqe-r4-typed-queue-build-r5/a/{c}/{c}.ko';o,n=Elf(op),Elf(np);os={s['name']:s for s in o.syms if s['name']};ns={s['name']:s for s in n.syms if s['name']};rename={'__UNIQUE_ID_description327':'__UNIQUE_ID_description328','__UNIQUE_ID_author328':'__UNIQUE_ID_author329','__UNIQUE_ID_license329':'__UNIQUE_ID_license330'}if c=='topaz_congest_queue'else{};moves=[]
 for name,s in os.items():
  target=rename.get(name,name);t=ns[target]
  if s['value']!=t['value']:moves.append({'name':name,'target':target,'section':o.symsection(s),'type':s['type'],'old_value':s['value'],'new_value':t['value'],'size':s['size']})
 r[c]={'old_module_sha256':hashlib.sha256(op.read_bytes()).hexdigest(),'new_module_sha256':hashlib.sha256(np.read_bytes()).hexdigest(),'rename':rename,'new_names':sorted(set(ns)-{rename.get(name,name)for name in os}),'moved':moves}
(out/'symbol-layout-review.json').write_text(json.dumps(r,indent=2)+'\n');print(json.dumps({c:{'moves':len(x['moved']),'additions':len(x['new_names'])}for c,x in r.items()},indent=2));print(hashlib.sha256((out/'symbol-layout-review.json').read_bytes()).hexdigest())
