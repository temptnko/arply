#!/bin/bash

set -eu

PROGRAM="arply"
BIN_DIR="bin"

CFLAGS="-Wall -Wextra"
INCLUDES="-Isrc/include"

SOURCES="
src/main.c
src/arp/if_setup.c
src/arp/response_parse.c
src/arp/scan_arp.c
src/arp/sock_setup_arp.c
src/arp/sock_setup_ioctl.c
"

echo "cleaning old executable"
rm -f "$BIN_DIR/$PROGRAM"

mkdir -p "$BIN_DIR"

echo "Compiling $PROGRAM"

gcc $CFLAGS $INCLUDES $SOURCES -o "$BIN_DIR/$PROGRAM"

echo "Build successful"
