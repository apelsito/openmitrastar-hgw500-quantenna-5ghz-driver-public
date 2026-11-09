"""Portable offline driver rebuild with pinned external ARC/kernel dependencies.

Compile manifests retain executed flags; paths are remapped to this checkout.
Only a new build/ directory is written. No device or network calls exist here.
"""
from __future__ import annotations

import argparse
from concurrent.futures import ThreadPoolExecutor
import hashlib
import json
import os
from pathlib import Path
import subprocess

from elf_compare import Elf, baseline_compare


def sha(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()


def metadata(path):
    values = {}
    for raw in Elf(path).byname[".modinfo"]["data"].split(b"\0"):
        if b"=" in raw:
            key, value = raw.decode().split("=", 1)
            values.setdefault(key, []).append(value)
    return {key: sorted(value) for key, value in values.items()}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--module", choices=("qdrv", "wlan", "switch_tqe"), required=True)
    parser.add_argument("--repo", type=Path, default=Path(__file__).resolve().parents[1])
    parser.add_argument("--kernel", type=Path, required=True)
    parser.add_argument("--context", type=Path, required=True)
    parser.add_argument("--toolchain-root", type=Path, required=True,
                        help="Root containing arc-arc700-linux-uclibc/")
    parser.add_argument("--peer-symvers", type=Path, required=True)
    parser.add_argument("--parent", type=Path)
    parser.add_argument("--output", type=Path, required=True)
    args = parser.parse_args()
    repo = args.repo.resolve()
    manifest_path = repo / "manifests" / ("build-" + args.module + ".json")
    manifest = json.loads(manifest_path.read_text())
    assert manifest["module"] == args.module
    parent = args.parent or repo / manifest["parent_local_path"]
    assert sha(parent) == manifest["parent_sha256"], "wrong parent module"
    output = args.output.resolve()
    output.relative_to((repo / "build").resolve())
    assert not output.exists(), "immutable output already exists"
    tc = args.toolchain_root / "arc-arc700-linux-uclibc/bin/arc-snps-linux-uclibc-"
    original = manifest["original_workspace"]
    path_map = [
        ("work/linux510-radio-port/quantenna-f364-lifetime/qdrv/../../../sources/Quantenna-GPL", "vendor/legacy-gpl"),
        ("work/linux510-radio-port/quantenna/platform/topaz/../../../../sources/Quantenna-GPL", "vendor/legacy-gpl"),
        ("work/linux510-radio-port/quantenna/wlan/../../../sources/Quantenna-GPL", "vendor/legacy-gpl"),
        ("work/linux510-radio-port/quantenna/wlan/../qdrv/compat/include", "src/compat/include"),
        ("work/linux510-radio-port/quantenna/platform/topaz/../../qdrv/compat/include", "src/compat/include"),
        ("work/linux510-radio-port/quantenna/platform/topaz/../ruby", "src/platform/ruby"),
        ("work/linux510-radio-port/quantenna-f364-lifetime/qdrv", "src/qdrv"),
        ("work/linux510-radio-port/quantenna-f344-peers/platform/topaz", "src/platform/topaz"),
        ("work/linux510-radio-port/quantenna/qdrv/compat/include", "src/compat/include"),
        ("work/linux510-radio-port/quantenna/qdrv", "src/qdrv"),
        ("work/linux510-radio-port/quantenna/platform/topaz", "src/platform/topaz"),
        ("work/linux510-radio-port/quantenna/wlan", "src/wlan"),
        ("work/sources/Quantenna-GPL", "vendor/legacy-gpl"),
    ]

    def remap(value):
        value = value.replace("{kernel}", str(args.kernel)).replace("{context}", str(args.context)).replace("{toolchain_root}", str(args.toolchain_root / "arc-arc700-linux-uclibc"))
        for old, new in path_map:
            value = value.replace("{original_workspace}/" + old, str(repo / new))
        return value.replace("{original_workspace}", original)

    # Pin repository content and the explicit external ABI context before use.
    protected = {manifest_path, Path(__file__), repo / "tools/elf_compare.py", parent,
                 args.peer_symvers, args.context / ".config", args.context / "Module.symvers",
                 args.context / "scripts/mod/modpost", args.context / "scripts/module.lds"}
    for source_tree in (repo / "src", repo / "vendor"):
        protected.update(path for path in source_tree.rglob("*") if path.is_file())
    for context_tree in ("include", "arch/arc/include/generated"):
        protected.update(path for path in (args.context / context_tree).rglob("*") if path.is_file())
    for kernel_tree in ("include", "arch/arc/include"):
        protected.update(path for path in (args.kernel / kernel_tree).rglob("*") if path.is_file())
    protected.update(path for path in args.toolchain_root.rglob("*") if path.is_file() and path.name in ("arc-linux-gcc", "arc-snps-linux-uclibc-gcc", "cc1", "as", "ld", "arc-linux-ld", "arc-snps-linux-uclibc-ld"))
    for path in protected:
        assert path.is_file(), "missing required build input: " + str(path)
    pins = {str(path): sha(path) for path in sorted(protected, key=str)}
    canonical_pins = {str(Path(path).resolve()) for path in pins}
    cached_dependencies = {}

    def canonical_dependency(value):
        if value not in cached_dependencies:
            path = Path(value) if value.startswith("/") else args.context / value
            cached_dependencies[value] = str(path.resolve())
        return cached_dependencies[value]

    output.mkdir(parents=True, exist_ok=False)
    (output / "input-pins.json").write_text(json.dumps(pins, indent=2) + "\n")
    result = {"schema": "openmitrastar.portable-driver-build/v1", "module": args.module,
              "status": "NO_GO_OFFLINE_BUILD", "hardware_accessed": False, "firmware_eligible": False,
              "physical_gates": "UNRUN", "modules": [], "parent_sha256": sha(parent),
              "external_dependencies_pinned": len(pins), "limits": "Complete offline baseline compilation only; no physical traffic, persistence, recovery or release acceptance."}

    def run(command, cwd, log):
        (log.with_suffix(".command.json")).write_text(json.dumps(command, indent=2) + "\n")
        process = subprocess.run(command, cwd=cwd, capture_output=True, timeout=240)
        log.with_suffix(".stdout").write_bytes(process.stdout)
        log.with_suffix(".stderr").write_bytes(process.stderr)
        if process.returncode:
            raise RuntimeError(log.name + ": " + process.stderr.decode(errors="replace")[-2500:])

    def compile_command(template, name, source, folder):
        command = [remap(item) for item in template if not item.startswith("-ffile-prefix-map=")]
        for index, item in enumerate(command):
            if item.startswith("-Wp,-MMD,"):
                command[index] = "-Wp,-MMD," + str(folder / (name + ".d"))
        command[command.index("-o") + 1] = str(folder / (name + ".o"))
        command[-1] = str(source)
        # Ports retain direct includes relative to vendor roots. Compile from
        # the repository source while preserving the original __FILE identity.
        prefix = ["-I" + str(repo / "src/compat/include"),
                  "-I" + str(repo / "src/wlan"), "-I" + str(repo / "src/qdrv"),
                  "-I" + str(repo / "vendor/port-context/include"),
                  "-I" + str(repo / "vendor/port-context/drivers/include/kernel"),
                  "-I" + str(repo / "vendor/port-context/drivers/include/shared")]
        if args.module == "qdrv":
            prefix.append("-ffile-prefix-map=" + str(repo / "src/qdrv") + "=/f364-qdrv/qdrv")
        elif args.module == "wlan":
            prefix.append("-ffile-prefix-map=" + str(repo / "src/wlan") + "=" + original + "/work/linux510-radio-port/quantenna/wlan")
        else:
            prefix.append("-ffile-prefix-map=" + str(repo / "src/platform/topaz") + "=" + original + "/work/linux510-radio-port/quantenna/platform/topaz")
        if args.module == "switch_tqe":
            original_gpl = original + "/work/linux510-radio-port/quantenna/platform/topaz/../../../../sources/Quantenna-GPL"
        elif args.module == "wlan":
            original_gpl = original + "/work/linux510-radio-port/quantenna/wlan/../../../sources/Quantenna-GPL"
        else:
            original_gpl = original + "/work/sources/Quantenna-GPL"
        prefix.append("-ffile-prefix-map=" + str(repo / "vendor/port-context") + "=" + original_gpl)
        prefix += ["-ffile-prefix-map=" + str(repo / "vendor/legacy-gpl") + "=" + original + "/work/sources/Quantenna-GPL",
                   "-ffile-prefix-map=" + str(repo / "src/compat") + "=" + original + "/work/linux510-radio-port/quantenna/qdrv/compat",
                   "-ffile-prefix-map=" + str(folder) + "=/portable-build-output"]
        # -I precedence must use current port ABI overlays before preserved
        # legacy GPL headers. A trailing -I would silently select OEM layouts.
        command[1:1] = prefix
        return command

    try:
        dependencies = output / "dependencies.symvers"
        dependencies.write_text((args.context / "Module.symvers").read_text() + args.peer_symvers.read_text())
        for lane in ("a", "b"):
            folder = output / lane
            folder.mkdir()
            def unit(record):
                name = record["name"]
                source = repo / record["source"]
                run(compile_command(record["command"], name, source, folder), args.context, folder / name)
                dependencies_used = (folder / (name + ".d")).read_text().split(":", 1)[1].replace("\\\n", " ").split()
                missing = sorted({canonical_dependency(dep) for dep in dependencies_used} - canonical_pins)
                if missing:
                    raise ValueError("un-pinned compiler dependencies: " + str(missing))
                (folder / ("." + name + ".o.cmd")).write_text("source_" + str(folder / (name + ".o")) + " := " + str(source) + "\n\ndeps_" + str(folder / (name + ".o")) + " := \\\n" + "".join("  " + dep + " \\\n" for dep in dependencies_used) + "\n")
                return name
            with ThreadPoolExecutor(max_workers=4) as pool:
                for name in pool.map(unit, manifest["units"]):
                    print(json.dumps({"lane": lane, "compiled": name}), flush=True)
            objects = [str(folder / (record["name"] + ".o")) for record in manifest["units"]]
            name = args.module
            (folder / (name + ".mod")).write_text("\n".join(objects) + "\n")
            obj = folder / (name + ".o")
            if len(objects) != 1 or objects[0] != str(obj):
                run([str(tc) + "ld", "-r", "-z", "noexecstack", "--no-warn-rwx-segments", "-o", str(obj), *objects], folder, folder / "link-object")
            command = [str(args.context / "scripts/mod/modpost"), "-e"]
            if manifest["modpost_all_versions"]:
                command.append("-a")
            run(command + ["-i", str(dependencies), "-o", str(folder / "Module.symvers"), str(obj)], folder, folder / "modpost")
            modname = manifest["metadata_unit"]
            run(compile_command(manifest["metadata_command"], modname, folder / (modname + ".c"), folder), args.context, folder / "compile-metadata")
            module = folder / (name + ".ko")
            run([str(tc) + "ld", "-r", "-z", "noexecstack", "--no-warn-rwx-segments", "--build-id=sha1", "-T", str(args.context / "scripts/module.lds"), "-o", str(module), str(obj), str(folder / (modname + ".o"))], folder, folder / "link-module")
            result["modules"].append({"lane": lane, "bytes": module.stat().st_size, "sha256": sha(module), "path": str(module)})
        rebuilt = output / "a" / (args.module + ".ko")
        assert result["modules"][0]["sha256"] == result["modules"][1]["sha256"], "independent compilations differ"
        comparison = baseline_compare(Elf(parent), Elf(rebuilt))
        (output / "full-baseline-comparison.json").write_text(json.dumps(comparison, indent=2) + "\n")
        old, new = metadata(parent), metadata(rebuilt)
        delta = sorted(key for key in set(old) | set(new) if old.get(key) != new.get(key))
        (output / "metadata-comparison.json").write_text(json.dumps({"parent": old, "rebuilt": new, "delta": delta}, indent=2) + "\n")
        assert not comparison["errors"], comparison["errors"]
        assert set(delta) <= set(manifest["declared_metadata_delta"]), "undeclared module metadata delta"
        if manifest["strict_byte_identity"]:
            assert sha(rebuilt) == manifest["parent_sha256"], "strict whole-file identity not reproduced"
        for path, digest in pins.items():
            assert sha(path) == digest, "pinned input changed: " + path
        result.update(status="PASS_OFFLINE_FULL_BASELINE", reproducible=True, protected_inputs_unchanged=True,
                      full_baseline_comparison_status=comparison["status"], functions=comparison["function_count"],
                      relocations=comparison["relocations_total"], symbols=comparison["symbols_total"],
                      allocated_code_bytes=comparison["code_allocated_bytes"], metadata_delta=delta,
                      whole_file_exact=sha(rebuilt) == manifest["parent_sha256"])
    except Exception as exc:
        result["error"] = str(exc)
    (output / "result.json").write_text(json.dumps(result, indent=2) + "\n")
    print(json.dumps(result, indent=2), flush=True)
    return int(result["status"] != "PASS_OFFLINE_FULL_BASELINE")


if __name__ == "__main__":
    raise SystemExit(main())
