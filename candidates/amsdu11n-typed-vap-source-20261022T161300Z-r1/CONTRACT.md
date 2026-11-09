# OEM484 typed VAP contract — 22 October 2026

Status is SOURCE_ONLY. No hardware or installed runtime changes occurred.
No full ARC module, ancillary module closure, aggregation or client acceptance
is claimed by this source preparation and host C fixture.

The immutable parent is the18-record source candidate under
`outputs/rea/duplicate-rts-source-candidate-20261022T155600Z-r1`.
Its source and the independent capability19 prototype remain unchanged.

Exact shipped OEM evidence closes the source design:

- `ieee80211_send_mgmt` reads VAP bytec54 (existing `iv_tx_amsdu`) and a
  distinct bytec55. The ADDBA A-MSDU bit is set when the first is true and
  either `(peer_flags & 0x4008) != 8` or the second is true.
- `ieee80211_vap_setup` sets both bytes to1. Its `param_2` is the netdev;
  the VAP private space starts300bytes into it, yieldingc54/c55 fromf54/f55.
- `qdrv_vap_init` allocates and zeroes its complete private VAP object before
  invoking `ieee80211_vap_setup`. Teardown owns that same VAP allocation; the
  new scalar requires no new callback, timer, buffer, secret or reference.
- WLAN484 setter compares the distinct flag with `!!value`; only a transition
  writes it, sends the full original value via `ieee80211_param_to_qdrv`, and
  requests reassociation `(vap,0,1)`. Getter reads the same VAP scalar.
- QDRV's existing generic SETPARAM path sends command11, payload76bytes
  `{id,value,len,data[64]}`. MuC's exact484 case writes its VAP/node byte80.
- A fresh MuC consumer additionally checks11n/non-VHT and other peer flags
  before selecting aggregation-size policy. This does not establish throughput.

Candidate implementation:

- Append `u8 iv_tx_amsdu_11n` at the end of `struct ieee80211vap`.
- Initialize it to1 in the actual VAP setup beside the existing A-MSDU default.
- Add484 table aliases `txamsdu_11n/get_txamsdu_11n`, exact changed-bool setter
  and direct VAP getter in WLAN.
- Replace only the existing ADDBA A-MSDU predicate with the exact two-flag
  condition using `IEEE80211_NODE_HT/IEEE80211_NODE_VHT`, values8/4000 proven
  in the retained `qtn/shared_defs.h`.
- Keep existing `iv_tx_amsdu`, QDRV generic wire structure and all older fields.

Append-only text preserves every previous VAP member position. The resulting
VAP size can change offsets in enclosing `struct qdrv_vap`; therefore all QDRV,
WLAN, scan, crypto and other VAP/header consumers must be rebuilt together.
This source candidate is not safe to combine with old ancillary modules.
ARC compiler witnesses and a complete module delta audit remain required.

Host actual-C fixtures passed:

- 262144 combinations of all16-bit peer flags, base A-MSDU boolean and distinct
  11n boolean. Existing BA parameter bits remain unchanged except the one bit.
- 28 setter cases covering zero, positive/non-boolean, negative, INT32_MIN and
  INT32_MAX, both flag defaults and existing base A-MSDU values. Full input
  forwarding and reassociation occur only on boolean transition.
- Explicit HT-only disabled11n, VHT unaffected and base-policy disabled cases.
- Typed default1 and unchanged baseline header prefix text checked.

Expected full private table after a qualified build:586 records,583 exact OEM
matches,24 missing original records and three retained port additions. Of the20
source-recovered entries,18 are functional cases and two faithfully return
`-EOPNOTSUPP`. These counts are source expectations until full module extraction.

The four binding REA results and every file hash are in evidence-manifest.json.
Both newly started setup/allocator REA runs finished exit0; no analysis started
for this contract remains in progress.
