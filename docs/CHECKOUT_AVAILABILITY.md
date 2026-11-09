> Public export: availability below describes the source base. Check `PUBLICATION.json` for excluded native databases and private authoring files.

# Published checkout availability

The table below is derived from the remote base tree used for this documentation
update. It makes newer maintainer-local source selections visible without claiming
that they are already in this clone. A documentation commit does not change
firmware sources, original binaries, toolchain pins or selected submodule commits.

| Entry point | Available in remote base |
| --- | --- |
| `src/CURRENT` | Yes |
| `src/linux-7.3-r39/` | No; newer/local or separately provisioned reference |
| `manifests/linux73-dependencies.json` | Yes |
| `docs/STATUS.md` | Yes |
| `docs/LINUX73_RAM_CORE_R35.md` | No; newer/local or separately provisioned reference |
| `vendor/legacy-gpl/` | Yes |
| `vendor/legacy-firmware-source/` | Yes |
| `components/` | Yes |

## Submodule handoffs

Source and analysis submodule revisions stay pinned. Use the direct child
repository links in [the repository map](REPOSITORY_MAP.md) to read newer handoff
documentation; a parent clone intentionally retains the selected child revision.

## Integrity manifests

Where an artifact manifest includes README/documentation bytes, this update
refreshes only those members and aggregate counts. Existing non-document source
and evidence rows remain unchanged. Historical reports are not rewritten to
pretend that new builds or physical checks were run.
