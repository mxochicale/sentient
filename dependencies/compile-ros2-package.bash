#!/usr/bin/env bash
set -euo pipefail

# Configuration
ZEN_DIR="/home/rosuser/ros2_ws/src/openzenros2/openzen"
CONAN_PROFILE_HOME="${HOME}/.conan2/profiles"

# Helper functions
log() { echo "[$(date +'%Y-%m-%d %H:%M:%S')] $*"; }
die() { echo "ERROR: $*" >&2; exit 1; }

# Verify prerequisites
command -v conan >/dev/null 2>&1 || die "conan not found in PATH"
command -v cmake >/dev/null 2>&1 || die "cmake not found in PATH"

# Change into project directory
cd "$ZEN_DIR" || die "Cannot cd into $ZEN_DIR"

log "Cleaning build directory..."
rm -rf build/

log "Detecting Conan profile..."
conan profile detect --force

log "Installing custom Conan profile..."
mkdir -p "$CONAN_PROFILE_HOME"
cp docker/conan_profile_linux "$CONAN_PROFILE_HOME/linux"

# Only purge cache if explicitly requested (destructive!)
if [[ "${1:-}" == "--purge-cache" ]]; then
    log "Purging Conan local cache..."
    conan remove "*" -c
else
    log "Skipping Conan cache purge (pass --purge-cache to enable)"
fi

log "Installing project dependencies..."
conan install . \
    --output-folder=build \
    --build=missing

log "Installing expected-lite..."
conan install --requires=expected-lite/0.9.0 --build=missing

log "Configuring CMake..."
cmake --preset conan-release

log "Building project..."
cmake --build --preset conan-release

# Post-build fixup
BUILD_OUT_DIR="$ZEN_DIR/build/build/Release"
cd "$BUILD_OUT_DIR" || die "Cannot cd into $BUILD_OUT_DIR"

log "Patching OpenZenConfig.cmake..."
if [[ -f OpenZenConfig.cmake ]]; then
    sed -i 's|OpenZenTargets\.cmake|OpenZenConfigTargets.cmake|' OpenZenConfig.cmake
else
    die "OpenZenConfig.cmake not found in $BUILD_OUT_DIR"
fi

log "Build complete."