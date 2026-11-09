# Source and byte preservation

`manifests/seed-provenance.json` pins every seeded file by byte size and SHA-256.
Git attributes disable newline conversion for all files. Sources and comments
were copied without decoding or rewriting them. Original vendor blobs come
from Quantenna-GPL commit `116a8b0cd80d75e6bc3b8d21646f9c7fa4ce1cf9`.

That GPL release is v37.4.0.62.1. This physical HGW OEM binary lineage is
v37.4.0.89. The original source for every OEM extension is not available in
the retained GPL. The full imported `drivers`, `include` and `common` trees
include radio, platform, bootcfg, FWT, radar headers and related interfaces.
The original full firmware carrier remains byte-identical in ignored
`local-only/original-firmware/`. It is not converted into an invented source
tree or committed as a personal firmware image.

`vendor/port-context` preserves changed shared headers separately. `src/qdrv`
is F364/F365 lineage; `src/wlan` is the actual retained F365 WLAN source, not
the similar F344 peer. F370 and the ioctl aliases are separate candidates.

REA derives assembly and pseudocode. ARC700 admission uses a project-local
REA adaptation, Ghidra and the retained ARC toolchain. OEM flags 0x303 need
the isolated ABI3 lane. Synthetic linked addresses never describe observed
hardware addresses. Analysis ELFs stay local and must never be installed.

The comment document and page are decoded views. Exact files, offsets,
per-comment hashes and `comments.raw.bin` retain the byte-authoritative record.
An unavailable binary-only coprocessor component remains marked binary-only;
its original high-level source cannot be inferred as present from pseudocode.

The complete original GPL firmware tree is also pinned as Git submodule
`vendor/legacy-firmware-source`, preserving its original history and every blob.
This adds the old kernel, bootloader, buildroot, host, configs and distributed
source/binary carriers without flattening a second kernel into the radio port.
`manifests/legacy-firmware-source.json` records every original blob identity.
On Windows eight original uppercase kernel header names collide with lowercase
names. Their exact bytes remain accessible through `git show HEAD:path`; use a
case-sensitive Linux checkout for compilation and full-source extraction. No
original blob is edited to work around the filesystem. The first comment catalog
covers the imported radio source and port. The later full Git-blob catalog also
covers all35018 C/C++ files, including the old kernel and Buildroot:1331535
comments/104725712 exact bytes, with every raw record hash replayed.
