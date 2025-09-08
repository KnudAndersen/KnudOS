set disassembly-flavor intel
target remote localhost:26000
symbol-file build/loader.BIN
add-symbol-file build/kernel.BIN
