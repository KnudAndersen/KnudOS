set disassembly-flavor intel
target remote localhost:26000
symbol-file build/bin_32/load_32.BIN
add-symbol-file build/bin/kernel.BIN
