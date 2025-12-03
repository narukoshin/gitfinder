#!/usr/bin/env bash
set -euo pipefail

# VERSION is used to store the version of the program as defined in src/main.cpp
VERSION=$(sed -n 's/^#define VERSION "\([^"]*\)".*$/\1/p' src/main.cpp)

PROJECT_NAME="gitfinder"
SRC_DIR="."
TOOLCHAINS_DIR="toolchains"
BUILD_TYPE="Release"
RELEASE_DIR="releases"
WRK_DIR=$(pwd)
SAMPLE_CONFIG="config.sample.yml"

WINDOWS_BUILD="build-windows-amd64"
LINUX_BUILD="build-linux-amd64"

info() { echo -e "\033[1;32m[INFO]\033[0m $*"; }
action() { echo -e "\033[1;34m[ACTION]\033[0m $*"; }

# Removes previous builds for linux-amd64 and windows-amd64.
# This function is used to cleanup the build directories to ensure that the
# build process starts from a clean slate.
cleanup() {
      # Cleanup previous builds
      if [ -d "$LINUX_BUILD" ]; then
            action "Deleting $LINUX_BUILD..."
            rm -rf "$LINUX_BUILD"
            info "Deleted $LINUX_BUILD"
      fi

      if [ -d "$WINDOWS_BUILD" ]; then
            action "Deleting $WINDOWS_BUILD..."
            rm -rf "$WINDOWS_BUILD"
            info "Deleted $WINDOWS_BUILD"
      fi
}

# Configures, builds and creates an archive for linux-amd64.
# This function:
#   - Configures the build using cmake with the specified toolchain file and build type.
#   - Enters the build directory and builds the project.
#   - Creates an archive of the project with the specified filename.
#   - Copies the executable to the archive.
#   - Prints information about the build process.
linux() {
      action "Configuring linux-amd64..."
      cmake -S $SRC_DIR -B $LINUX_BUILD -G Ninja \
            -DCMAKE_TOOLCHAIN_FILE=$TOOLCHAINS_DIR/linux-x86_64.cmake \
            -DCMAKE_BUILD_TYPE=$BUILD_TYPE
      info "Finished."

      action "Entering $LINUX_BUILD..."
      cd $LINUX_BUILD

      action "Building linux-amd64..."
      ninja
      info "Finished."

      linux_archive="${PROJECT_NAME}-${VERSION}-linux-amd64.tar.gz"

      if [ ! -d "$WRK_DIR/$RELEASE_DIR" ]; then
            mkdir "$WRK_DIR/$RELEASE_DIR"
      fi

      action "Creating archive $linux_archive..."
      # change filename from main to gitfinder
      tar -czf "$WRK_DIR/$RELEASE_DIR/$linux_archive" --transform='s|^main|gitfinder|' main ../$SAMPLE_CONFIG
      action "Copying executable to the archive "
      info "Finished."
      # reset the working directory
      action "Going back to $WRK_DIR..."
      cd $WRK_DIR
}

# Configures, builds and creates an archive for windows-amd64.
# This function:
#   - Configures the build using cmake with the specified toolchain file and build type.
#   - Enters the build directory and builds the project.
#   - Creates an archive of the project with the specified filename.
#   - Copies the executable to the archive.
#   - Prints information about the build process.
windows() {
      action "Configuring windows-amd64..."
      #                                  "Ninja" didn't work aswell
      #                                            ⤹ 
      cmake -S $SRC_DIR -B $WINDOWS_BUILD -G "Unix Makefiles" \
            -DCMAKE_TOOLCHAIN_FILE=$TOOLCHAINS_DIR/windows-x86_64.cmake \
            -DCMAKE_BUILD_TYPE=$BUILD_TYPE
      info "Finished."
      action "Entering $WINDOWS_BUILD..."
      cd $WINDOWS_BUILD
      action "Building windows-amd64..."
      make
      info "Finished."
      # reset the working directory
      action "Going back to $WRK_DIR..."
      cd $WRK_DIR
}

# Cleanup previous builds.
cleanup
clear

# configures, builds and creates an archive for linux-amd64.
linux

# configures, builds and creates an archive for windows-amd64.
# windows
info "Build complete."