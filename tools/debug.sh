#!/bin/bash
qemu-system-$ARCH -cdrom ../benny.iso -s -S -no-reboot -no-shutdown &
gdb -tui
wait