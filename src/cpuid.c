#include <cpuid.h>
#include <asm.h>

// TODO: for robustness, this should happen in the loader
//       so it should be migrated there before testing on real hardware

void check_cpuid_features()
{
	/*
	* TODO: check if cpuid is supported in the first place:
	* The ID flag (bit 21) in the EFLAGS register indicates support for the CPUID instruction.
	* If a software procedure can set and clear this flag, the processor executing the
	* procedure supports the CPUID instruction. This instruction operates the same in
	* non-64-bit modes and 64-bit mode.
	*/
	u32 eax, ebx, ecx, edx;
	/*
	* NX: execute disable.
	* If CPUID.80000001H:EDX.NX [bit 20] = 1, IA32_EFER.NXE may be set to 1, allowing PAE paging and IA-32e
	* paging to disable execute access to selected pages (see Section 4.6). (Processors that do not support CPUID
	* function 80000001H do not allow IA32_EFER.NXE to be set to 1.)
	*/
	cpuid(0x80000001, &eax, &ebx, &ecx, &edx);
	if (!(edx & (1 << 20)))
		halt_forever();
	/*
	* LM: IA-32e mode support.
	* If CPUID.80000001H:EDX.LM [bit 29] = 1, IA32_EFER.LME may be set to 1, enabling IA-32e paging.
	* (Processors that do not support CPUID function 80000001H do not allow IA32_EFER.LME to be set to 1.)
	*/
	cpuid(0x80000001, &eax, &ebx, &ecx, &edx);
	if (!(edx & (1 << 29)))
		halt_forever();
	/*
	* PAE: physical-address extension.
	* If CPUID.01H:EDX.PAE [bit 6] = 1, CR4.PAE may be set to 1, enabling PAE paging (this setting is also required
	* for IA-32e paging).
	*/
	cpuid(0x01, &eax, &ebx, &ecx, &edx);
	if (!(edx & (1 << 6)))
		halt_forever();
}
