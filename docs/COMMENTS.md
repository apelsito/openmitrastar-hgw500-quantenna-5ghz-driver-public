# Preserved comments

Original and port source comments are retained in their exact source files.
The delivered R26 catalog is `evidence/linux73-r26-comments-handoff-20261023T071000Z-r1/`:
179 files,7179 comments and726274 exact bytes. The document, searchable page,
raw stream and source hashes are preserved together. Previous catalogs remain
intact; [the handoff](HANDOFF_LINUX73_R26.md) records the final selected source.

The current R24 catalog is `evidence/linux73-r24-comments-20261023T055800Z-r1/`:
179 files,7176 comments and725675 exact comment bytes. Source hashes and the
document, searchable page and raw bytes are bound by the
[R24 qualification](../evidence/linux73-r24-final-independent-qualification-20261023T060500Z-r1/result.json).
Historical originals remain intact in their previous versioned directories.

R23 has a new exact catalog at `evidence/linux73-r23-comments-20261023T050000Z-r1/`:
178 files,7167 comments and724512 exact comment bytes. R22 retains its own catalog
with7165 comments. Two comments in replaced loader functions remain in the
unchanged R20/R22 files; they are not claimed to be literally present in R23.
The exact sources, catalogs, documents and searchable pages are all preserved.

The preserved Linux7.3 R20 catalog is
`evidence/linux73-r20-comments-20261022T215800Z-r1/`:174 files,7140 comments and
721223 exact comment bytes. Its collected document and searchable page accompany
the raw stream and hashes. The
[ELF/source review](../evidence/linux73-r20-elf44-comment-review-20261022T220000Z-r1/result.json)
confirms all44 original missing record shapes and names without claiming
functional radio acceptance. Earlier catalogs remain intact below.

Linux7.3 R18 has a new immutable catalog at
`evidence/linux73-r18-comments-20261022T214500Z-r1/`:174 files,7137 comments and
720910 exact comment bytes. Its `COMMENTS.md`, searchable `index.html`,
`comments.json`, raw byte stream and source/hash manifests are preserved together.
The [source review](../evidence/linux73-r18-source-comment-review-20261022T214900Z-r2/result.json)
records43 exact OEM aliases and one468 spelling mismatch in R18. R19 corrects
that spelling; the R18 catalog remains tied to R18. Catalogs do not qualify
driver behavior, and sources remain authoritative byte for byte.

The qualified Consolidated32 source has its own versioned catalog at
`evidence/consolidated32-comments-20261022T203500Z-r1/`:129 files,5646 comments
and453320 exact comment bytes, with no source changes or unterminated comments.
Its collected document, searchable page and raw byte stream are separate from
the historical original catalogs below.

The full catalog is generated under `evidence/comments-20261022-r1/`:

* `COMMENTS.md`: collected comments with original file, physical line and offsets.
* `index.html`: local searchable page, with no external requests.
* `comments.json`: comment-to-file/offset mappings and per-comment SHA-256.
* `comments.raw.bin`: exact original comment byte stream.
* `manifest.json`: every selected file and its exact hash and byte size.

The HTML page can be opened as a local file. Serving it on the router later
requires HTTPS-only installation gates. No HTTP server is started by the catalog.

The scanner handles C/C++ string literals, escaped quotes, raw strings and
backslash-newline splicing. It includes comments in inactive preprocessor
branches. It does not implement trigraph conversion or a full C preprocessor;
that limitation is explicit in the output. The source files remain authoritative.

The complete preserved firmware GPL now has a second catalog:
`evidence/full-legacy-comments-20261022-r1/`. It reads original Git blobs,
including both sides of Windows filename case collisions. Across35018 C/C++
files it contains1331535 comments and104725712 original comment bytes. Every
record hash was independently replayed against its compressed exact-byte stream.
Other languages and binary carriers remain fully preserved in Git; they are
not incorrectly parsed as C comments.

`SUMMARY.md` indexes the components; each component has a collected
`COMMENTS.md.gz`, exact`comments.raw.gz`, and searchable JSONL records. The small
`index.html` page loads only the selected component, decompresses and searches
on the client, and renders40 records per page. It requires HTTPS for loading.
The source/comment catalogs are development documentation and are not proposed
for storage in the router's small firmware partition.

The revised viewer in`docs/comments-viewer-20261022-r2/` limits each download
to10000 records (largest compressed chunk1197091bytes) and renders40 at a time.
Its browser test passes over loopback HTTPS: client gzip decompression,
component/block selection and text search, with no JavaScript errors. The
original catalog/viewer remains immutable. Exact raw streams are unchanged.
