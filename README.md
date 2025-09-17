## Description
64-bit operating system for x86 written from scratch. Some features are:
1) GRUB support through Multiboot (ver. 1) compliance.
2) 32-bit boot stub and library to setup initialize processor state for 64-bit long mode.
3) ELF binary loader to set up 64-bit binaries.
4) VGA text mode graphics library supporting multiple TTY windows displaying at once.
5) Virtual and physical memory managers and kernel/user space fine-grained dynamic allocators.

Existing in archives (...being conjured...):
1) Routed IRQ lines and initialized hardware interrupt handling with LAPIC and I/O APIC to support hardware timers (PIT, HPET, LAPIC), and external IRQ's e.g. (PS/2 controller).
2) Device driver for PS/2 keyboard controller.

## Requirements
1) A (modern) virtual machine for ``x86_64``. Needs to emulate BIOS firmware and have 1GiB of memory. (If you have ``qemu-system-x86_64``, then there is recipe in the ``Makefile`` to launch and wait for GDB.)



