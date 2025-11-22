set -euo pipefail
extension_dir="$(ls -1dt "${HOME}"/.vscode/extensions/ms-vscode.cpptools-* 2>/dev/null | head -n 1 || true)"

if [[ -z "${extension_dir}" ]]; then
    echo "cpptools extension not found under ~/.vscode/extensions." >&2
    exit 1
fi

lldb_mi="${extension_dir}/debugAdapters/lldb-mi/bin/lldb-mi"

if [[ ! -x "${lldb_mi}" ]]; then
    echo "lldb-mi executable not found at ${lldb_mi}." >&2
    exit 1
fi

exec "${lldb_mi}" "$@"
