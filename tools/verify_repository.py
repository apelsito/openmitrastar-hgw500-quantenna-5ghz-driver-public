"""Verify preserved source bytes and Git exclusions; read-only offline checks."""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess

def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
    p=argparse.ArgumentParser();p.add_argument('--root',type=Path,default=Path(__file__).resolve().parents[1]);a=p.parse_args();root=a.root.resolve()
    errors=[];checked=0
    manifests=[root/name for name in ('manifests/seed-provenance.json','manifests/recovery-sync-20261022.json','manifests/recovery-seven-pair-sync-20261022.json')]
    manifests.extend(sorted((root/'manifests').glob('preservation-checkpoint-*.json')))
    preserved={}
    for manifest in manifests:
        for row in json.loads(manifest.read_text(encoding='utf-8'))['files']:
            relative=Path(row['path'])
            if relative.is_absolute() or '..' in relative.parts:
                errors.append('invalid preservation path: '+row['path']);continue
            previous=preserved.get(row['path'])
            contract=(row['bytes'],row['sha256'])
            if previous is not None and previous!=contract:
                errors.append('conflicting preserved contract: '+row['path'])
            preserved[row['path']]=contract
    for relative,(size,digest) in preserved.items():
        path=root/relative;checked+=1
        if not path.is_file() or path.stat().st_size!=size or sha(path)!=digest:errors.append('preserved copy mismatch: '+relative)
    included=subprocess.check_output(['git','-C',str(root),'ls-files','--cached','--others','--exclude-standard','-z']).decode('utf-8').split('\0')
    for path in filter(None,included):
        if path.startswith(('local-only/','build/','.cache/')) or Path(path).suffix.lower() in ('.ko','.o','.elf','.pem','.key','.p12'):errors.append('private/build path included: '+path)
    attr=subprocess.check_output(['git','-C',str(root),'check-attr','text','--','src/qdrv/qdrv_wlan.c','vendor/legacy-gpl/drivers/wlan/ieee80211.c'],text=True)
    if attr.count('text: unset')!=2:errors.append('Git newline conversion is not disabled')
    value={'status':'PASS_REPOSITORY_PRESERVATION_AND_EXCLUSIONS' if not errors else 'FAIL','preserved_files':checked,'preservation_manifests':len(manifests),'included_paths':len(list(filter(None,included))),'errors':errors,'hardware_accessed':False}
    print(json.dumps(value,indent=2));return int(bool(errors))
if __name__=='__main__':raise SystemExit(main())
