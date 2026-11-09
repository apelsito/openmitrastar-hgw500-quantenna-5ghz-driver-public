# Private repository and publication status

The owner initially requested private storage, later authorized publication,
then explicitly corrected that request: **keep everything private for now**.
The active7.3 branch must be available in the private repository for AELITA.
No visibility change was performed. A future publication needs a new applicable
owner instruction and resolution of the mixed-license originals in history.

The recorded audit found no firmware/key/credential candidates in35commits and
2014blobs. It also identified proprietary module declarations in the original
FWT sources and an internal distribution note in muc_txrx_stats.h. Those marks
are preserved. They are licensing/provenance questions, not permission to
relicense or delete the originals. Private storage is not a new public license.

Imported code retains original Quantenna/net80211/kernel notices. Their
per-file licenses remain authoritative; see `LICENSES/` and source headers.
Before public release, resolve the exact license and attribution for each
binary-only firmware dependency and any optional game/emulator asset. Do not
describe the entire mixed tree as newly authored or change vendor notices.

Ignored local-only artifacts include original firmware carriers, driver modules,
private device captures and build outputs. Keys, PSKs, calibration, credentials,
client identities and personalized rootfs data must never enter Git history.
The repository contains static source/metadata evidence, not a deployable
owner-specific router image.

Future releases need a versioned immutable manifest, build reproduction,
complete operational delta audit and physical acceptance. HTTPS certificates
and private keys are installed separately and remain outside Git.
