# Channel-set461 source helpers

Six helpers are reconstructed in typed C: original constant templates, first
matching entry, bounded scan-type queue, ranking defaults, frequency edges and
ARC low32 cost arithmetic. Original130 records preserve10,400 bytes exactly,
without relocations or runtime/calibration data. This is partial reconstruction.

Host verification passes46,080 lookups,2,600 exact compiled words,9 bounds
refusals,168 queue cases,780 edge cases,27 default words and100,000 cost cases.
The cost loop is omitted by Ghidra as a non-procedure body; original88 code bytes
and register setup were checked with pinned ARC objdump. Saturated or malformed
queues refuse before overwrite, differing intentionally from OEM behavior.

The actual ARC compilation has1,042 input pins, six intended functions and three
retained header functions. The standalone object is12,836bytes, SHA256
08858015eed781858236499235c9cfa4486d41ab0e6c3d6b70ff0b8d06bd56ad.
This object is not a loadable module or firmware input.

Original comments remain preserved; a new helper comment document/searchable
page is retained separately. No461 table entry is added yet. Metric collection,
final candidate selection, scan callbacks, lifetime and complete driver closure
remain open. The current qualified NAC22 module still has22 OEM table gaps.
No device, RF, client, flash or release acceptance is claimed.
