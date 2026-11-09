# Typed Linux 7.3 SRAM provider

The provider wraps the recovered OEM first-fit allocator in one exclusive Linux
resource lease and one forced-uncached mapping. Registration is absent by default.
The current module can compile and link without reserving or touching SRAM.

The compiled kernel board caller supplies `om73_sram_board_contract` only after
verifying the exact kernel occupancy, selected firmware carriers and every
indirect writer/reset owner. Nonzero evidence hashes identify that proof; their
presence alone does not establish it. Resource reservation also does not stop a
MuC/AuC/DSP firmware writer. No module parameter or guessed gap can admit an arena.

`om73_sram_acquire` requests an exclusive page-aligned CPU physical range,
`0x88000000 + start` through `0x88000000 + end`, then maps it with `ioremap`.
On this ARC 7.3 implementation, addresses below `0xc0000000` receive a forced
uncached MMU mapping. `ioremap_prot` also forces uncached memory and there is no
ARC `ioremap_cache` implementation. An arbitrary returned virtual address must
never go through the old Quantenna `virt_to_bus` or ARC identity `virt_to_phys`.
The nominal `0x60000000` OEM alias also lacks a current kernel mapping witness.

The SRAM mapping is treated as RAM-like storage for allocator metadata. Only
the recovered core uses a narrow `__force` cast for normal C loads/stores on
that single uncached mapping. There is no cached alias to flush. Device records
must use `io_map` and the explicitly computed `cpu_phys` and `bus` fields.

Allocations use a private owner key and align the CPU physical address, then
apply its delta to the mapped address. The bus address uses the same SRAM
offset relative to `0x80000000`. `extent_offset` and `extent_bytes` retain the
raw reservation used by the core, while `size` denotes useful client bytes.
The DMA adapter uses its private record as owner, preserving separate identity
when two preparation attempts target the same public descriptor structure.

Acquire and close may sleep and are serialized by a lifecycle mutex. Alloc,
free and query have bounded static ledgers and run under a spinlock with IRQs
saved; they allocate no DRAM and perform no sleeping kernel calls. Devices must
be quiesced before free. Close refuses while the core retains any live span.
The acquired lease pins a modular provider until successful close. Mapping
failure releases the exact resource, and failed acquisition balances the pin.

The current DMA SRAM branch must not use a second `ioremap`, `virt_to_bus`,
`virt_to_phys`, cached-alias flush, or DRAM fallback. A failed owned free must
retain or quarantine the private owner rather than freeing its identity.

Offline source fixtures and actual ARC linkage prove these software boundaries.
They do not prove physical SRAM mapping, bus visibility, firmware writer
exclusion, reset lifecycle, or useful RF traffic. The board admission contract
and its carrier/writer lifecycle checks remain required before activation.
