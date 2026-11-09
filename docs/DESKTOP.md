# Client-side HGW desktop — HTTPS only

The router remains a router. A request to its HTTPS origin returns a compact
HTML/CSS/JavaScript desktop. Windows, games and emulator cores run in the client
browser. The router serves files and bounded authenticated management requests;
it does not render frames or execute arbitrary desktop commands.

The owner's decision is HTTPS exclusively. The prototype refuses to initialize
over HTTP. No redirect or fallback HTTP listener is introduced. All resources
are local paths on the same HTTPS origin; games use no CDN, external fonts or
remote requests. Certificate trust must be established for the actual address
or owner-selected local hostname. Private keys are outside Git and firmware
handoffs; certificate policy and installation are a separately qualified change.

Recorded physical memory evidence on 22 October: Linux has 51,976 KiB total
usable memory, 10,360 KiB available at that sample and no swap. Physical NOR is
16 MiB. This is a dated sample, not current capacity or additional free space.
Protected/reserved partitions are not application storage. The standalone
desktop shell is measured locally; the full Doom package previously exceeded
the available rootfs window, so client-side execution does not by itself solve
asset storage. Use measured packaging and explicit owner-approved storage.

No Docker daemon or native graphical environment is required on ARC700 for this
architecture. The browser executes the application. A PC development container
is a different optional build tool. Docker inside this router has no supported
or tested artifact in this project and is not part of the desktop candidate.

## Prototype

`desktop/index.html`, `desktop/style.css` and `desktop/app.js` implement a
dependency-free window shell with local notes, a client-side Snake game, a
future same-origin Doom entry and an information panel. Notes use browser local
storage. There is no fake router console or arbitrary command endpoint.
The Doom launch is deliberately disabled until HTTPS asset packaging qualifies;
the installed historical HTTP Doom route does not satisfy HTTPS-only acceptance.

The first actual loopback TLS/browser test passes: three assets total10,168bytes,
HTTPS byte hashes match, trusted test-certificate fetches pass, plaintext HTTP
is rejected, notes persist in client storage, Snake and three windows work,
and the HTTP client guard disables applications. Browser requests stay on the
same origin. This is local Windows/browser evidence; nothing is installed on
the physical HGW and the port80 service is not changed by this work.

Serve the candidate using an explicit certificate and key:

```sh
python3 tools/serve_https_desktop.py --root desktop \
  --cert /owner/local/hgw-cert.pem --key /owner/local/hgw-key.pem \
  --bind 127.0.0.1 --port 8443
```

This creates one HTTPS listener only when an operator runs it. The offline test
uses loopback TLS and a temporary test CA/certificate; nothing is installed on
the router. The candidate needs browser interaction verification, actual ARC
TLS interoperability, session/auth integration, certificate trust, resource
measurement, slot-fit, recovery and exact owner authorization before deployment.
