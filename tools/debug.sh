#!/bin/bash
qemu-system-$ARCH -cdrom ../benny.iso -s -S -display none -no-reboot &
gdb

wait