"""Split decoded comment views into bounded client chunks; raw catalog unchanged."""
import argparse,gzip,json
from pathlib import Path

def main():
    p=argparse.ArgumentParser();p.add_argument('--catalog',required=True,type=Path);p.add_argument('--records',type=int,default=10000);a=p.parse_args()
    target=a.catalog/'client-chunks';target.mkdir(exist_ok=False);result={}
    m=json.loads((a.catalog/'manifest.json').read_text())
    for group in m['groups']:
        folder=target/group['group'];folder.mkdir();records=[];chunks=[];total=0
        def write(records):
            name=f'{len(chunks):04d}.json.gz'
            with gzip.open(folder/name,'wt',encoding='utf-8',compresslevel=6) as f:json.dump(records,f,ensure_ascii=False,separators=(',',':'))
            chunks.append(group['group']+'/'+name)
        with gzip.open(a.catalog/group['group']/'comments.jsonl.gz','rt',encoding='utf-8') as stream:
            for line in stream:
                records.append(json.loads(line));total+=1
                if len(records)==a.records:write(records);records=[]
        if records:write(records)
        assert total==group['comments'];result[group['group']]={'comments':total,'chunks':chunks}
    (target/'manifest.json').write_text(json.dumps(result,indent=2)+'\n',encoding='utf-8')
    print(json.dumps({'groups':len(result),'comments':sum(x['comments'] for x in result.values())}))
if __name__=='__main__':main()
