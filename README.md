## Requirements
1) You need to build cross compilers for the ``i686-elf`` and ``x86_64-elf`` targets according to the GNU binutils schema. By default, these binaries should be found in ``.cross/i686-elf-gcc`` and ``.cross/x86_64-elf-gcc``, relative to the project root.
2) You need [The Netwide Assembler](https://www.nasm.us/), it is expected in ``/usr/bin`` by default. (NOTE: The main constraint is that the assembler should use Intel syntax, and that it supports dwarf (version at least 4) for debugging.) 
3) A (modern) virtual machine for ``x86_64``. Needs to emulate BIOS firmware and have ~1GiB of memory (NOTE: subject to change). (If you have ``qemu-system-x86_64``, then there is recipe in the ``Makefile`` to launch and wait for GDB with the corresponding ``.gdbinit`` config.)

## Description
64-bit operating system for x86 written from scratch. Some features are:
1) GRUB support through Multiboot (ver. 1) compliance.
2) 32-bit boot stub and library to setup initialize processor state for 64-bit long mode.
3) ELF binary loader to set up 64-bit binaries.
4) VGA text mode graphics library supporting multiple TTY instances displaying at once with standard I/O.
5) Virtual and physical memory managers and kernel/user space fine-grained dynamic allocators.
6) ... COMING SOON, BUSY WITH SCHOOl ...




