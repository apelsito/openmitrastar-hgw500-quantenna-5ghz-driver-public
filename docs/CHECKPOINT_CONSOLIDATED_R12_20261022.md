# Consolidated driver source/module checkpoint — 22 October 2026

R12 combines the qualifiedLegacy23 lineage with beacon lifetime, unpublished
ioctl cleanup and APP synchronization. It preserves NAC, dynamic SIFS, typed
AMSDU11n, legacy rate and duplicate RTS recovery. CoC491 and channel-set461
remain separate candidates.

All58 C units compile twice independently. The seven resulting modules are
identical between builds and all strong imports resolve through joint modpost.
QDRV684896bytes SHA256
`7131ebbb5cfb46653784799c693be1f9b37815c375fd669bdcfbdf560002a887`.
WLAN636140bytes SHA256
`b7c82d8c31a2154794fba8b0e24e33ff94293e3b497be72c324ad63001bd7612`.

The matching-header control retains828 unchanged QDRV functions and931 WLAN
functions,13960 and8852 residual relocations, all anonymous executable bytes,
allocated data and symbol metadata. The checkpoint freezes1912 QDRV and2231
WLAN metadata rows, four new exports with exact0/4/8 relocations, and complete
operational data/symbol/relocation checks for the other five modules.2464
current input pins and two ELF negative cases pass.

ARC prefix witnesses and the legacy, APP/BH, cleanup, descriptor and Neighbor
Report fixtures pass on the combined source. Matching rebuilt modules are
required because the host structures grow; original wire widths remain intact.

The [sealed checkpoint](../evidence/consolidated23-beacon-r12-sealed-checkpoint-20261022T211300Z-r1/result.json)
has593 private records:27 recovered in source, including four partial BSS and
neighbor advertisement/control records. It deliberately leaves the qualified
combined functional count null. Capability semantics and management action
handlers still need audit; this is not27 accepted physical features.

The qualifiedLegacy23 record count remains the conservative operation recovery
milestone:23 recovered records and21 original gaps. RF, clients, traffic, soak,
persistent boot, rollback and release acceptance remain open. No router access,
radio activation, UART, reboot or firmware write occurred in this work.
