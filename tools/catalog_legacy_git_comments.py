"""Catalog every available C/C++ comment directly from preserved legacy Git blobs.

Exact Git source avoids Windows case collisions, checkout conversion and
symlink ambiguity. Compressed documents/data retain all original comment bytes.
No source writes, device/network operations or huge single-page render required.
"""
import argparse
from collections import defaultdict
import gzip
import hashlib
import json
from pathlib import Path
import subprocess
from datetime import datetime, timezone
from catalog_hgw_firmware_comments import scan_comments as comments,decode,markdown_code,SOURCE_SUFFIXES

def digest(p):
    h=hashlib.sha256()
    with p.open('rb') as f:
        for raw in iter(lambda:f.read(1024*1024),b''):h.update(raw)
    return h.hexdigest()

def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--git-root',type=Path,required=True);p.add_argument('--commit',required=True);p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();a.output.mkdir(parents=True,exist_ok=False)
    head=subprocess.check_output(['git','-C',str(a.git_root),'rev-parse',a.commit],text=True).strip()
    tree=subprocess.check_output(['git','-C',str(a.git_root),'ls-tree','-r','-l','-z',head])
    blobs=[];files=[]
    for record in tree.split(b'\0'):
        if not record:continue
        meta,path=record.split(b'\t',1);mode,kind,oid,size=meta.decode().split();path=path.decode('utf-8')
        row={'path':path,'git_object':oid,'mode':mode,'bytes':int(size) if size!='-' else None};files.append(row)
        if kind=='blob' and mode!='120000' and Path(path).suffix.lower() in SOURCE_SUFFIXES:
            blobs.append(row)
    groups=defaultdict(list)
    for row in blobs:groups[row['path'].split('/')[0]].append(row)
    batch=subprocess.Popen(['git','-C',str(a.git_root),'cat-file','--batch'],stdin=subprocess.PIPE,stdout=subprocess.PIPE)
    results=[]
    try:
        for group,selected in sorted(groups.items()):
            destination=a.output/group;destination.mkdir()
            count=0;stream_offset=0;unterminated=0;source_sha_rows=[]
            with gzip.open(destination/'comments.raw.gz','wb',compresslevel=6) as rawstream, gzip.open(destination/'comments.jsonl.gz','wt',encoding='utf-8',compresslevel=6) as catalog, gzip.open(destination/'COMMENTS.md.gz','wt',encoding='utf-8',compresslevel=6) as document:
                document.write('# Original firmware comments: '+group+'\n\nCommit '+head+'; original byte offsets remain authoritative.\n')
                for row in selected:
                    batch.stdin.write((row['git_object']+'\n').encode());batch.stdin.flush()
                    header=batch.stdout.readline().decode().split();assert header[1]=='blob'
                    size=int(header[2]);raw=batch.stdout.read(size);assert len(raw)==size and batch.stdout.read(1)==b'\n'
                    assert hashlib.sha1(b'blob '+str(size).encode()+b'\0'+raw).hexdigest()==row['git_object'],'Git object bytes differ'
                    found=comments(raw);source_sha_rows.append({**row,'sha256':hashlib.sha256(raw).hexdigest(),'comments':len(found)})
                    for block in found:
                        exact=raw[block['offset']:block['end_offset']];text,encoding=decode(exact);count+=1
                        value={'id':count,'path':row['path'],**block,'raw_stream_offset':stream_offset,'decoded_encoding':encoding,'text':text}
                        rawstream.write(exact);stream_offset+=len(exact)
                        catalog.write(json.dumps(value,ensure_ascii=False,separators=(',',':'))+'\n')
                        document.write('\n## '+row['path']+':'+str(block['line'])+'\n\nSHA256 '+block['sha256']+'; bytes '+str(block['offset'])+'..'+str(block['end_offset'])+'\n\n'+markdown_code(text)+'\n')
                        unterminated+=not block['terminated']
                    if len(source_sha_rows)%1000==0:print(json.dumps({'group':group,'files':len(source_sha_rows),'comments':count}),flush=True)
            # Read exact raw stream back with streaming digest, no bytes dropped.
            raw_sha=hashlib.sha256();read_size=0
            with gzip.open(destination/'comments.raw.gz','rb') as f:
                for chunk in iter(lambda:f.read(1024*1024),b''):raw_sha.update(chunk);read_size+=len(chunk)
            assert read_size==stream_offset
            item={'group':group,'source_files':len(selected),'comments':count,'raw_bytes':stream_offset,'raw_sha256':raw_sha.hexdigest(),'unterminated_comments':unterminated,
                  'files':source_sha_rows,'artifacts':{name:digest(destination/name) for name in ('comments.raw.gz','comments.jsonl.gz','COMMENTS.md.gz')}}
            (destination/'manifest.json').write_text(json.dumps(item,indent=2)+'\n',encoding='utf-8')
            results.append({k:v for k,v in item.items() if k!='files'})
            print(json.dumps({k:item[k] for k in ('group','source_files','comments','raw_bytes')}),flush=True)
    finally:
        batch.stdin.close();batch.stdout.close();batch.wait(timeout=10)
    manifest={'schema':'openmitrastar.full-legacy-git-comments/v1','status':'PASS_EXACT_GIT_BLOB_COMMENT_CATALOG','created_utc':datetime.now(timezone.utc).isoformat(),'commit':head,
              'all_git_files':len(files),'selected_source_files':len(blobs),'comments':sum(x['comments'] for x in results),'raw_bytes':sum(x['raw_bytes'] for x in results),
              'hardware_accessed':False,'source_files_modified':False,'groups':results,
              'limits':['C/C++ lexical comments; other file languages are preserved in Git but are not parsed as C.','No trigraph conversion or preprocessing.','Documents/JSON are decoded views; compressed raw stream and original Git blobs preserve exact bytes.','Original vendor firmware source is not the complete OEM v89 high-level source.']}
    (a.output/'manifest.json').write_text(json.dumps(manifest,indent=2)+'\n',encoding='utf-8')
    (a.output/'SUMMARY.md').write_text('# All preserved legacy C/C++ comments\n\nCommit `'+head+'`.\n\n'+'\n'.join('* '+x['group']+': '+str(x['comments'])+' comments / '+str(x['raw_bytes'])+' original bytes; collected document `'+x['group']+'/COMMENTS.md.gz`.' for x in results)+'\n',encoding='utf-8')
    print(json.dumps({k:manifest[k] for k in ('status','selected_source_files','comments','raw_bytes')},indent=2),flush=True)
    return 0
if __name__=='__main__':raise SystemExit(main())
