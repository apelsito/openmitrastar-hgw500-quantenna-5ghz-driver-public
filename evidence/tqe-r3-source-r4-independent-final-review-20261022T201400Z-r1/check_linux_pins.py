import hashlib,json
from pathlib import Path
root=Path('/workspace/user-home/Desktop/MitraStar');out=Path(__file__).parent
reports=[]
for name in ['driver5g-ops-20261022-tqe-r3-typed-batch-build-r2','driver5g-ops-20261022-tqe-r3-typed-batch-global-r10']:
 p=root/'outputs/rea'/name/'input-pins.json';pins=json.loads(p.read_text())
 for path,h in pins.items():assert hashlib.sha256(Path(path).read_bytes()).hexdigest()==h,('actual pin drift',path)
 reports.append({'input':str(p),'pins':len(pins),'sha256':hashlib.sha256(p.read_bytes()).hexdigest(),'all_current_actual_bytes_verified':True})
assert not(out/'actual-linux-pin-review.json').exists()
(out/'actual-linux-pin-review.json').write_text(json.dumps({'status':'PASS_CURRENT_ACTUAL_LINUX_BUILD_AND_GLOBAL_PIN_CLOSURE','reports':reports},indent=2)+'\n')
print(json.dumps(reports,indent=2))
