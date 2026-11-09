"""Validate distributed source pins while reporting unprovisioned private inputs."""

import json
from pathlib import Path
import subprocess
import sys


def main():
    root = Path(__file__).resolve().parents[1]
    result = subprocess.run(
        [sys.executable, str(root / "tools/verify_repository.py"), "--root", str(root)],
        capture_output=True, text=True, check=False,
    )
    if not result.stdout.strip():
        sys.stderr.write(result.stderr)
        return result.returncode or 1
    original = json.loads(result.stdout)
    errors = []
    missing_inputs = []
    prefix = "preserved copy mismatch: "
    for error in original["errors"]:
        relative = error.removeprefix(prefix)
        if error.startswith(prefix) and relative.startswith("local-only/") and not (root / relative).exists():
            missing_inputs.append(relative)
        else:
            errors.append(error)
    summary = {
        "status": "PASS_DISTRIBUTED_SOURCE_PRESERVATION" if not errors else "FAIL",
        "preserved_files_checked": original["preserved_files"],
        "preservation_manifests": original["preservation_manifests"],
        "unprovisioned_private_inputs": missing_inputs,
        "errors": errors,
        "hardware_accessed": False,
    }
    print(json.dumps(summary, indent=2))
    return int(bool(errors))


if __name__ == "__main__":
    raise SystemExit(main())
