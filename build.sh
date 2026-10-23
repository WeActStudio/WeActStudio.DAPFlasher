#!/usr/bin/env bash
#
# WeAct Studio DAP Flasher - build script
#
# Run it in an MSYS2 MINGW64 shell (D:\msys64\mingw64.exe):
#   pacman -S mingw-w64-x86_64-qt5-static mingw-w64-x86_64-hidapi \
#             mingw-w64-x86_64-libusb
#
# Usage:
#   ./build.sh            # build Release (GUI + console companion)
#   ./build.sh ts         # only refresh the translation (.ts -> .qm)
#   ./build.sh pack       # compress the built exe with UPX
#   ./build.sh clean      # remove the build directories
#
set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# ----------------------------------------------------------------------------
#  Qt6 fix: Qt6's qmake probes the compiler (mkspecs/features/toolchain.prf)
#  by running it through $$system(), i.e. QProcess. On Windows QProcess
#  resolves the command interpreter through %COMSPEC%. MSYS2 shells do NOT
#  export COMSPEC, so QProcess silently fails to start ANY process: the
#  compiler probe returns empty output with exit code 0 and qmake aborts with
#
#      Variable QMAKE_CXX.COMPILER_MACROS is not defined.
#      Project ERROR: failed to parse default include paths from compiler output
#
#  Qt5's qmake does not probe the compiler, which is why only Qt6 was hit.
#  Exporting COMSPEC (and PATHEXT) makes the Qt6 build work from MSYS2.
# ----------------------------------------------------------------------------
if [ -z "${COMSPEC}" ]; then
    # NOTE: the path has to reach cmd.exe with NATIVE backslashes. A forward
    # slash path ("C:/WINDOWS/System32/cmd.exe") makes the CRT's _popen() fail
    # with "The syntax of the command is incorrect", which in turn breaks
    # windres' preprocessor invocation for src/app.rc.
    #
    # Do NOT build it by string concatenation in bash: SYSTEMROOT already
    # carries backslashes ("C:\WINDOWS"), and the "\\" of a literal suffix
    # collapses to a single "/" in double quotes, which yields the mixed
    # "C:\WINDOWS//System32//cmd.exe" that cmd.exe refuses to start. The
    # failure is silent - qmake's compiler probe comes back with empty output
    # and exit code 0 - and looks like
    #     Variable QMAKE_CXX.COMPILER_MACROS is not defined.
    #     Project ERROR: failed to parse default include paths from compiler output
    # cygpath produces the backslash form unambiguously, so use it when it is
    # available and only fall back to a literal path otherwise.
    if command -v cygpath >/dev/null 2>&1; then
        windir_unix="$(cygpath -u "${SYSTEMROOT:-${WINDIR:-C:/WINDOWS}}" 2>/dev/null)"
        export COMSPEC="$(cygpath -w "${windir_unix}/System32/cmd.exe" 2>/dev/null)"
    fi
    if [ -z "${COMSPEC}" ] || [ ! -f "${COMSPEC}" ]; then
        export COMSPEC='C:\WINDOWS\System32\cmd.exe'
    fi
fi
if [ -z "${PATHEXT}" ]; then
    export PATHEXT=".COM;.EXE;.BAT;.CMD"
fi

# Native Windows programs (qmake / python.exe) do not understand MSYS POSIX
# paths (/h/xxx would be read as H:/h/xxx), so keep a Windows style copy too.
if command -v cygpath >/dev/null 2>&1; then
    SCRIPT_DIR_WIN="$(cygpath -w "${SCRIPT_DIR}")"
else
    SCRIPT_DIR_WIN="${SCRIPT_DIR}"
fi

PRO_FILE="WeActStudioDAPFlasher.pro"
EXE_NAME="WeAct Studio DAP Flasher.exe"

CLI_PRO_FILE="WeActStudioDAPFlasher_CLI.pro"
CLI_EXE_NAME="WeAct Studio DAP Flasher CLI.exe"

# Where the object files go. A RAM disk is a lot faster than the source
# volume for this, and nothing under the build directories has to survive a
# reboot, so the RAM disk is used whenever it is mounted. Override with
# DAP_BUILD_DIR / DAP_CLI_BUILD_DIR, or fall back to the source tree when
# there is no RAM disk.
if [ -z "${DAP_BUILD_DIR}" ] && [ -d /r ]; then
    DAP_BUILD_DIR="/r/dapflasher-build/build"
    DAP_CLI_BUILD_DIR="/r/dapflasher-build/build-cli"
fi

BUILD_DIR="${DAP_BUILD_DIR:-${SCRIPT_DIR}/build}"
CLI_BUILD_DIR="${DAP_CLI_BUILD_DIR:-${SCRIPT_DIR}/build-cli}"

# qmake of the MSYS2 static Qt installation. Qt6 static is preferred; to force
# Qt5 run:  QMAKE_BIN=/mingw64/qt5-static/bin/qmake ./build.sh
if [ -z "${QMAKE_BIN}" ]; then
    if [ -x "/mingw64/qt6-static/bin/qmake" ]; then
        QMAKE_BIN="/mingw64/qt6-static/bin/qmake"
    else
        QMAKE_BIN="/mingw64/qt5-static/bin/qmake"
    fi
fi
QT_BIN_DIR="$(dirname "${QMAKE_BIN}")"

# Windows style MSYS prefix needed by the native g++/qmake (see the .pro)
if [ -z "${MSYS_PREFIX_WIN}" ] && command -v cygpath >/dev/null 2>&1; then
    MSYS_PREFIX_WIN="$(cygpath -m "${MSYSTEM_PREFIX:-/mingw64}")"
fi
export MSYS_PREFIX_WIN

# Qt6's lupdate-pro needs an explicit qmake spec
if [ -d "${QT_BIN_DIR}/../share/qt6/mkspecs/win32-g++" ]; then
    export QMAKESPEC="$(cygpath -m "${QT_BIN_DIR}/../share/qt6/mkspecs/win32-g++")"
fi

# ----------------------------------------------------------------------------
#  UI translations (Simplified Chinese, Russian, German, Spanish)
# ----------------------------------------------------------------------------
LANGUAGES="zh_CN ru de es"

generate_translations()
{
    local ts_dir="${SCRIPT_DIR}/translations"
    local qm_dir="${SCRIPT_DIR}/src/translations"

    mkdir -p "${ts_dir}" "${qm_dir}"

    if command -v python >/dev/null 2>&1; then
        PYTHON_BIN=python
    elif command -v python3 >/dev/null 2>&1; then
        PYTHON_BIN=python3
    else
        PYTHON_BIN=""
    fi

    # Scanning the source directory instead of the .pro file: on a static Qt
    # lupdate-pro cannot evaluate the project and fails with "Failed to
    # generate project description", which would leave the .ts files stale.
    if [ -x "${QT_BIN_DIR}/lupdate" ]; then
        LUPDATE_BIN="${QT_BIN_DIR}/lupdate"
    else
        LUPDATE_BIN="${QT_BIN_DIR}/lupdate-pro"
    fi

    local ts_args=""
    for lang in ${LANGUAGES}; do
        ts_args="${ts_args} translations/app_${lang}.ts"
    done

    if [ -n "${PYTHON_BIN}" ] && [ -x "${LUPDATE_BIN}" ]; then
        echo "==> Updating the translation sources (lupdate) ..."
        ( cd "${SCRIPT_DIR}" && "${LUPDATE_BIN}" -no-obsolete src -ts ${ts_args} ) \
            || echo "Warning: lupdate failed, keeping the existing .ts"
        "${PYTHON_BIN}" "${SCRIPT_DIR_WIN}/tools/i18n_fill.py" "${SCRIPT_DIR_WIN}"
    else
        echo "==> Skipping lupdate (python or lupdate missing), using the existing .ts"
    fi

    echo "==> Compiling the translations (lrelease) ..."
    for lang in ${LANGUAGES}; do
        if [ -f "${ts_dir}/app_${lang}.ts" ]; then
            "${QT_BIN_DIR}/lrelease" "${SCRIPT_DIR_WIN}/translations/app_${lang}.ts" \
                -qm "${SCRIPT_DIR_WIN}/src/translations/app_${lang}.qm"
        fi
    done
    touch "${SCRIPT_DIR}/src/translations.qrc"
}

if [ "$1" = "ts" ]; then
    generate_translations
    exit 0
fi

if [ "$1" = "clean" ]; then
    echo "==> Cleaning ${BUILD_DIR} ${CLI_BUILD_DIR}"
    rm -rf "${BUILD_DIR}" "${CLI_BUILD_DIR}" 2>/dev/null || true
    exit 0
fi

if [ "$1" = "pack" ]; then
    EXE_PATH="${BUILD_DIR}/${EXE_NAME}"
    if [ ! -f "${EXE_PATH}" ]; then
        echo "Error: ${EXE_PATH} not found, run ./build.sh first" >&2
        exit 1
    fi
    if ! command -v upx >/dev/null 2>&1; then
        echo "Error: upx not found, install it with: pacman -S mingw-w64-x86_64-upx" >&2
        exit 1
    fi
    echo "==> Compressing with UPX ..."
    upx --best --lzma "${EXE_PATH}"
    echo "    size: $(du -h "${EXE_PATH}" | cut -f1)"
    if [ -f "${CLI_BUILD_DIR}/${CLI_EXE_NAME}" ]; then
        upx --best --lzma "${CLI_BUILD_DIR}/${CLI_EXE_NAME}"
    fi
    exit 0
fi

if [ ! -x "${QMAKE_BIN}" ]; then
    echo "Error: qmake not found -> ${QMAKE_BIN}" >&2
    echo "Install it with: pacman -S mingw-w64-x86_64-qt5-static" >&2
    exit 1
fi

# ----------------------------------------------------------------------------
#  A Qt version switch leaves object files behind that were compiled against
#  another set of headers; start from scratch when the version changes.
# ----------------------------------------------------------------------------
qmake_fingerprint() { "${QMAKE_BIN}" -query QT_VERSION 2>/dev/null; }

# ----------------------------------------------------------------------------
#  .qmake.stash: the build directory has to have one, and it has to be made
#  somewhere else.
#
#  qmake probes the compiler on the first run in a build directory and caches
#  the result (macros, default include and library paths) in .qmake.stash.
#  The probe is written for cmd.exe - "g++ -E ... 2>NUL" - and it does NOT
#  survive being run through MSYS2's /bin/sh: "2>NUL" is then read as a
#  redirect to a file literally named NUL, the probe comes back empty, and
#  qmake stops with
#
#      Variable QMAKE_CXX.COMPILER_MACROS is not defined.
#      Project ERROR: failed to parse default include paths from compiler output
#
#  A build directory that already carries a stash never runs the probe, which
#  is why this stays hidden while the build directory lives on the source
#  volume. It bites when the build directory is on the RAM disk that a reboot
#  empties: the stash goes with it and every build after the reboot fails.
#
#  The stash is Qt-version and toolchain specific, not project specific, so a
#  copy from the source tree's build directory is a valid substitute - the
#  paths inside it point at the compiler, not at the project. Restore it when
#  the RAM disk lost it, and let qmake regenerate it when the copy is absent
#  as well.
# ----------------------------------------------------------------------------
RESTORE_SOURCE_DIRS="${DAP_BUILD_DIR:+${SCRIPT_DIR}/build} ${DAP_CLI_BUILD_DIR:+${SCRIPT_DIR}/build-cli}"

restore_qmake_stash()
{
    local dir="$1"
    local stash="${dir}/.qmake.stash"

    [ -f "${stash}" ] && return 0

    local candidate
    for candidate in ${RESTORE_SOURCE_DIRS}; do
        [ -f "${candidate}/.qmake.stash" ] || continue
        cp -f "${candidate}/.qmake.stash" "${stash}" || return 0
        echo "==> Restored .qmake.stash in ${dir} (from ${candidate})"
        return 0
    done
}

purge_if_qt_changed()
{
    local dir="$1"
    local stamp="${dir}/.qt_version"
    local now
    now="$(qmake_fingerprint)"

    if [ -f "${stamp}" ] && [ "$(cat "${stamp}")" != "${now}" ]; then
        echo "==> Qt version changed ($(cat "${stamp}") -> ${now}), cleaning ${dir}"
        rm -rf "${dir}"
    fi
    mkdir -p "${dir}"
    echo "${now}" > "${stamp}"
    restore_qmake_stash "${dir}"
}

echo "==> Qt version: $("${QMAKE_BIN}" -v | tail -1)"
echo "==> MSYSTEM=${MSYSTEM:-unknown}  MSYSTEM_PREFIX=${MSYSTEM_PREFIX:-unknown}"

generate_translations

JOBS="$(nproc 2>/dev/null || echo 4)"

echo
echo "==> Generating the Makefile (GUI) ..."
purge_if_qt_changed "${BUILD_DIR}"
cd "${BUILD_DIR}"
"${QMAKE_BIN}" "${SCRIPT_DIR_WIN}/${PRO_FILE}"

echo "==> Compiling ..."
make -j"${JOBS}"

EXE_PATH=""
for candidate in "${BUILD_DIR}/${EXE_NAME}" "${BUILD_DIR}/release/${EXE_NAME}"; do
    if [ -f "${candidate}" ]; then
        EXE_PATH="${candidate}"
        break
    fi
done

if [ -z "${EXE_PATH}" ]; then
    echo "Error: the graphical program was not produced" >&2
    exit 1
fi

echo
echo "==> Generating the Makefile (console companion) ..."
purge_if_qt_changed "${CLI_BUILD_DIR}"
cd "${CLI_BUILD_DIR}"
"${QMAKE_BIN}" "${SCRIPT_DIR_WIN}/${CLI_PRO_FILE}"

echo "==> Compiling ..."
make -j"${JOBS}"

CLI_PATH=""
for candidate in "${CLI_BUILD_DIR}/${CLI_EXE_NAME}" "${CLI_BUILD_DIR}/release/${CLI_EXE_NAME}"; do
    if [ -f "${candidate}" ]; then
        CLI_PATH="${candidate}"
        break
    fi
done

if [ -z "${CLI_PATH}" ]; then
    echo "Error: the console companion was not produced" >&2
    exit 1
fi

# The graphical program looks for the companion next to itself
cp -f "${CLI_PATH}" "${BUILD_DIR}/${CLI_EXE_NAME}"

echo
echo "==> Build succeeded"
echo "    program: ${EXE_PATH}"
echo "    size:    $(du -h "${EXE_PATH}" | cut -f1)"
echo "    console: ${BUILD_DIR}/${CLI_EXE_NAME}"
echo "    size:    $(du -h "${BUILD_DIR}/${CLI_EXE_NAME}" | cut -f1)"
