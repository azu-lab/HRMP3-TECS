OUTPUT_FORMAT("elf32-littlearm", "elf32-bigarm","elf32-littlearm")
OUTPUT_ARCH(arm)

MEMORY {
	ROM : ORIGIN = 0x1000000, LENGTH = 0x800000
	RAM : ORIGIN = 0x1800000, LENGTH = 0x800000
}

SECTIONS {
	.discard (NOLOAD) : {
		start.o(.discard) cfg_attsec4.o(.discard) kernel_cfg.o(.discard) kernel_mem.o(.discard) libkernel.a(.discard) libc.a(.discard) libgcc.a(.discard) 
	}

	.text_kernel 0x1000000 : ALIGN(4) {
		__start_mp_text_kernel = .;
		__start_mo_text_kernel = .;
		__start_text_kernel = .;
		start.o(.text .text.*) cfg_attsec4.o(.text .text.*) kernel_cfg.o(.text .text.*) kernel_mem.o(.text .text.*) libkernel.a(.text .text.*) *(.kernel_add07) *(.kernel_code_CLS_PRC1) *(.kernel_code_CLS_PRC2) *(.kernel_code_CLS_PRC3) *(.kernel_code_CLS_PRC4) *(.kernel_code_CLS_ALL_PRC1) *(.kernel_code_CLS_ALL_PRC2) *(.kernel_code_CLS_ALL_PRC3) *(.kernel_code_CLS_ALL_PRC4) 
		__end_text_kernel = .;
		__end_mo_text_kernel = .;
		__aend_mo_text_kernel = ALIGN(4K);
		__aend_mp_text_kernel = ALIGN(4K);
	} > ROM

	.rodata_kernel ALIGN(4K) : ALIGN(4) {
		__start_mp_rodata_kernel = .;
		__start_mo_rodata_kernel = .;
		__start_rodata_kernel = .;
		start.o(.rodata .rodata.*) cfg_attsec4.o(.rodata .rodata.*) KEEP(kernel_cfg.o(.rodata .rodata.*)) kernel_mem.o(.rodata .rodata.*) libkernel.a(.rodata .rodata.*) *(.kernel_add03) 
		__end_rodata_kernel = .;
		__end_mo_rodata_kernel = .;
		__aend_mo_rodata_kernel = ALIGN(4K);
		__aend_mp_rodata_kernel = ALIGN(4K);
	} > ROM

	.text_kernel_A7 ALIGN(4K) : ALIGN(4) {
		__start_mp_text_kernel_A7 = .;
		__start_mo_text_kernel_A7 = .;
		__start_text_kernel_A7 = .;
		*(.kernel_add08) 
		__end_text_kernel_A7 = .;
		__end_mo_text_kernel_A7 = .;
		__aend_mo_text_kernel_A7 = ALIGN(4K);
		__aend_mp_text_kernel_A7 = ALIGN(4K);
	} > ROM

	.linkonly_kernel ALIGN(4K) : ALIGN(4) {
		__start_mp_linkonly_kernel = .;
		__start_mo_linkonly_kernel = .;
		__start_linkonly_kernel = .;
		*(.kernel_add04) 
		__end_linkonly_kernel = .;
		__end_mo_linkonly_kernel = .;
		__aend_mo_linkonly_kernel = ALIGN(4K);
		__aend_mp_linkonly_kernel = ALIGN(4K);
	} > ROM

	.text_shared ALIGN(4K) : ALIGN(4) {
		__start_mp_text_shared = .;
		__start_mo_text_shared = .;
		__start_text_shared = .;
		libc.a(.text .text.*) libgcc.a(.text .text.*) 
		__end_text_shared = .;
		__end_mo_text_shared = .;
		__aend_mo_text_shared = ALIGN(4K);
		__aend_mp_text_shared = ALIGN(4K);
	} > ROM

	.rodata_shared ALIGN(4K) : ALIGN(4) {
		__start_mp_rodata_shared = .;
		__start_mo_rodata_shared = .;
		__start_rodata_shared = .;
		libc.a(.rodata .rodata.*) libgcc.a(.rodata .rodata.*) 
	} > ROM

	.preinit_array ALIGN(4) : {
		PROVIDE_HIDDEN (__preinit_array_start = .);
		KEEP (*(.preinit_array))
		PROVIDE_HIDDEN (__preinit_array_end = .);
	} > ROM
	.init_array ALIGN(4) : {
		PROVIDE_HIDDEN (__init_array_start = .);
		KEEP (*(SORT(.init_array.*)))
		KEEP (*(.init_array))
		PROVIDE_HIDDEN (__init_array_end = .);
	} > ROM
	.fini_array ALIGN(4) : {
		PROVIDE_HIDDEN (__fini_array_start = .);
		KEEP (*(SORT(.fini_array.*)))
		KEEP (*(.fini_array))
		PROVIDE_HIDDEN (__fini_array_end = .);
	} > ROM
	.ARM.exidx ALIGN(4) : {
		__exidx_start = .;
		*(.ARM.exidx* .gnu.linkonce.armexidx.*)
		__exidx_end = .;
	} > ROM
	.jcr ALIGN(4) : {
		KEEP (*(.jcr))
	} > ROM

	__end_rodata_shared = .;
	__end_mo_rodata_shared = .;
	__aend_mo_rodata_shared = ALIGN(4K);
	__aend_mp_rodata_shared = ALIGN(4K);

	.ROM_align (NOLOAD) : {
		. = ALIGN(4K);
	} > ROM

	.RAM_text_kernel_Ad 0x1800000 : ALIGN(4) {
		__start_mp_RAM_text_kernel = .;
		__start_mo_RAM_text_kernel = .;
		__start_RAM_text_kernel_Ad = .;
		*(.kernel_add17) 
		__end_RAM_text_kernel_Ad = .;
	} > RAM AT > ROM
	__start_RAM_itext_kernel_Ad = LOADADDR(.RAM_text_kernel_Ad);

	.RAM_text_kernel_A15 ALIGN(4) : ALIGN(4) {
		__start_RAM_text_kernel_A15 = .;
		*(.kernel_add27) 
		__end_RAM_text_kernel_A15 = .;
		__end_mo_RAM_text_kernel = .;
		__aend_mo_RAM_text_kernel = ALIGN(4K);
		__aend_mp_RAM_text_kernel = ALIGN(4K);
	} > RAM

	.RAM_rodata_kernel_A9 ALIGN(4K) : ALIGN(4) {
		__start_mp_RAM_rodata_kernel = .;
		__start_mo_RAM_rodata_kernel = .;
		__start_RAM_rodata_kernel_A9 = .;
		*(.kernel_add13) 
		__end_RAM_rodata_kernel_A9 = .;
	} > RAM AT > ROM
	__start_RAM_irodata_kernel_A9 = LOADADDR(.RAM_rodata_kernel_A9);

	.RAM_rodata_kernel_A11 ALIGN(4) : ALIGN(4) {
		__start_RAM_rodata_kernel_A11 = .;
		*(.kernel_add23) 
		__end_RAM_rodata_kernel_A11 = .;
		__end_mo_RAM_rodata_kernel = .;
		__aend_mo_RAM_rodata_kernel = ALIGN(4K);
		__aend_mp_RAM_rodata_kernel = ALIGN(4K);
	} > RAM

	.RAM_text_kernel_Af ALIGN(4K) : ALIGN(4) {
		__start_mp_RAM_text_kernel_A7 = .;
		__start_mo_RAM_text_kernel_A7 = .;
		__start_RAM_text_kernel_Af = .;
		*(.kernel_add18) 
		__end_RAM_text_kernel_Af = .;
	} > RAM AT > ROM
	__start_RAM_itext_kernel_Af = LOADADDR(.RAM_text_kernel_Af);

	.RAM_text_kernel_A17 ALIGN(4) : ALIGN(4) {
		__start_RAM_text_kernel_A17 = .;
		*(.kernel_add28) 
		__end_RAM_text_kernel_A17 = .;
		__end_mo_RAM_text_kernel_A7 = .;
		__aend_mo_RAM_text_kernel_A7 = ALIGN(4K);
		__aend_mp_RAM_text_kernel_A7 = ALIGN(4K);
	} > RAM

	.RAM_linkonly_kernel_Ab ALIGN(4K) : ALIGN(4) {
		__start_mp_RAM_linkonly_kernel = .;
		__start_mo_RAM_linkonly_kernel = .;
		__start_RAM_linkonly_kernel_Ab = .;
		*(.kernel_add14) 
		__end_RAM_linkonly_kernel_Ab = .;
	} > RAM AT > ROM
	__start_RAM_ilinkonly_kernel_Ab = LOADADDR(.RAM_linkonly_kernel_Ab);

	.RAM_linkonly_kernel_A13 ALIGN(4) : ALIGN(4) {
		__start_RAM_linkonly_kernel_A13 = .;
		*(.kernel_add24) 
		__end_RAM_linkonly_kernel_A13 = .;
		__end_mo_RAM_linkonly_kernel = .;
		__aend_mo_RAM_linkonly_kernel = ALIGN(4K);
		__aend_mp_RAM_linkonly_kernel = ALIGN(4K);
	} > RAM

	.data_kernel_Ac ALIGN(4K) : ALIGN(4) {
		__start_mp_rwdata_kernel_A4 = .;
		__start_mo_rwdata_kernel_A4 = .;
		__start_data_kernel_Ac = .;
		*(.kernel_add15) 
		__end_data_kernel_Ac = .;
	} > RAM AT > ROM
	__start_idata_kernel_Ac = LOADADDR(.data_kernel_Ac);

	.bss_kernel_A14 ALIGN(4) (NOLOAD) : ALIGN(4) {
		__start_bss_kernel_A14 = .;
		*(.kernel_add25) 
		__end_bss_kernel_A14 = .;
		__end_mo_rwdata_kernel_A4 = .;
		__aend_mo_rwdata_kernel_A4 = ALIGN(4K);
		__aend_mp_rwdata_kernel_A4 = ALIGN(4K);
	} > RAM

	.data_kernel ALIGN(4K) : ALIGN(4) {
		__start_mp_rwdata_kernel = .;
		__start_mo_rwdata_kernel = .;
		__start_data_kernel = .;
		start.o(.data .data.*) cfg_attsec4.o(.data .data.*) kernel_cfg.o(.data .data.*) kernel_mem.o(.data .data.*) libkernel.a(.data .data.*) *(.kernel_add11) 
		__end_data_kernel = .;
	} > RAM AT > ROM
	__start_idata_kernel = LOADADDR(.data_kernel);

	.bss_kernel ALIGN(4) (NOLOAD) : ALIGN(4) {
		__start_bss_kernel = .;
		start.o(.bss .bss.* COMMON) cfg_attsec4.o(.bss .bss.* COMMON) kernel_cfg.o(.bss .bss.* COMMON) kernel_mem.o(.bss .bss.* COMMON) libkernel.a(.bss .bss.* COMMON) *(.kernel_add21) 
		__end_bss_kernel = .;
	} > RAM

	.noinit_kernel ALIGN(4) (NOLOAD) : ALIGN(4) {
		__start_noinit_kernel = .;
		start.o(.noinit) cfg_attsec4.o(.noinit) kernel_cfg.o(.noinit) kernel_mem.o(.noinit) libkernel.a(.noinit) *(.kernel_data_CLS_PRC1) *(.kernel_data_CLS_PRC2) *(.kernel_data_CLS_PRC3) *(.kernel_data_CLS_PRC4) *(.kernel_data_CLS_ALL_PRC1) *(.kernel_data_CLS_ALL_PRC2) *(.kernel_data_CLS_ALL_PRC3) *(.kernel_data_CLS_ALL_PRC4) *(.system_stack_CLS_PRC1) *(.system_stack_CLS_PRC2) *(.system_stack_CLS_PRC3) *(.system_stack_CLS_PRC4) *(.system_stack_CLS_ALL_PRC1) *(.system_stack_CLS_ALL_PRC2) *(.system_stack_CLS_ALL_PRC3) *(.system_stack_CLS_ALL_PRC4) 
		__end_noinit_kernel = .;
		__end_mo_rwdata_kernel = .;
		__aend_mo_rwdata_kernel = ALIGN(4K);
		__aend_mp_rwdata_kernel = ALIGN(4K);
	} > RAM

	.data_kernel_Ae ALIGN(4K) : ALIGN(4) {
		__start_mp_rwdata_kernel_A6 = .;
		__start_mo_rwdata_kernel_A6 = .;
		__start_data_kernel_Ae = .;
		*(.kernel_add16) 
		__end_data_kernel_Ae = .;
	} > RAM AT > ROM
	__start_idata_kernel_Ae = LOADADDR(.data_kernel_Ae);

	.bss_kernel_A16 ALIGN(4) (NOLOAD) : ALIGN(4) {
		__start_bss_kernel_A16 = .;
		*(.kernel_add26) 
		__end_bss_kernel_A16 = .;
		__end_mo_rwdata_kernel_A6 = .;
		__aend_mo_rwdata_kernel_A6 = ALIGN(4K);
		__aend_mp_rwdata_kernel_A6 = ALIGN(4K);
	} > RAM

	.data_kernel_Aa ALIGN(4K) : ALIGN(4) {
		__start_mp_rwdata_kernel_A2 = .;
		__start_mo_rwdata_kernel_A2 = .;
		__start_data_kernel_Aa = .;
		*(.kernel_add12) 
		__end_data_kernel_Aa = .;
	} > RAM AT > ROM
	__start_idata_kernel_Aa = LOADADDR(.data_kernel_Aa);

	.bss_kernel_A12 ALIGN(4) (NOLOAD) : ALIGN(4) {
		__start_bss_kernel_A12 = .;
		*(.kernel_add22) 
		__end_bss_kernel_A12 = .;
		__end_mo_rwdata_kernel_A2 = .;
		__aend_mo_rwdata_kernel_A2 = ALIGN(4K);
		__aend_mp_rwdata_kernel_A2 = ALIGN(4K);
	} > RAM

	.data_shared ALIGN(4K) : ALIGN(4) {
		__start_mp_rwdata_shared = .;
		__start_mo_rwdata_shared = .;
		__start_data_shared = .;
		libc.a(.data .data.*) libgcc.a(.data .data.*) 
		__end_data_shared = .;
	} > RAM AT > ROM
	__start_idata_shared = LOADADDR(.data_shared);

	.bss_shared ALIGN(4) (NOLOAD) : ALIGN(4) {
		__start_bss_shared = .;
		libc.a(.bss .bss.* COMMON) libgcc.a(.bss .bss.* COMMON) 
		__end_bss_shared = .;
	} > RAM

	.noinit_shared ALIGN(4) (NOLOAD) : ALIGN(4) {
		__start_noinit_shared = .;
		libc.a(.noinit) libgcc.a(.noinit) 
		__end_noinit_shared = .;
		__end_mo_rwdata_shared = .;
		__aend_mo_rwdata_shared = ALIGN(4K);
		__aend_mp_rwdata_shared = ALIGN(4K);
	} > RAM

	/* Stabs debugging sections.  */
	.stab          0 : { *(.stab) }
	.stabstr       0 : { *(.stabstr) }
	.stab.excl     0 : { *(.stab.excl) }
	.stab.exclstr  0 : { *(.stab.exclstr) }
	.stab.index    0 : { *(.stab.index) }
	.stab.indexstr 0 : { *(.stab.indexstr) }
	.comment       0 : { *(.comment) }
	/* DWARF debug sections.
	   Symbols in the DWARF debugging sections are relative to the beginning
	   of the section so we begin them at 0.  */
	/* DWARF 1 */
	.debug          0 : { *(.debug) }
	.line           0 : { *(.line) }
	/* GNU DWARF 1 extensions */
	.debug_srcinfo  0 : { *(.debug_srcinfo) }
	.debug_sfnames  0 : { *(.debug_sfnames) }
	/* DWARF 1.1 and DWARF 2 */
	.debug_aranges  0 : { *(.debug_aranges) }
	.debug_pubnames 0 : { *(.debug_pubnames) }
	/* DWARF 2 */
	.debug_info     0 : { *(.debug_info .gnu.linkonce.wi.*) }
	.debug_abbrev   0 : { *(.debug_abbrev) }
	.debug_line     0 : { *(.debug_line) }
	.debug_frame    0 : { *(.debug_frame) }
	.debug_str      0 : { *(.debug_str) }
	.debug_loc      0 : { *(.debug_loc) }
	.debug_macinfo  0 : { *(.debug_macinfo) }
	/* SGI/MIPS DWARF 2 extensions */
	.debug_weaknames 0 : { *(.debug_weaknames) }
	.debug_funcnames 0 : { *(.debug_funcnames) }
	.debug_typenames 0 : { *(.debug_typenames) }
	.debug_varnames  0 : { *(.debug_varnames) }
	/* DWARF 3 */
	.debug_pubtypes 0 : { *(.debug_pubtypes) }
	.debug_ranges   0 : { *(.debug_ranges) }
	/* DWARF Extension.  */
	.debug_macro    0 : { *(.debug_macro) }
	.ARM.attributes 0 : { KEEP (*(.ARM.attributes)) KEEP (*(.gnu.attributes)) }
	.note.gnu.arm.ident 0 : { KEEP (*(.note.gnu.arm.ident)) }

	/DISCARD/ : {
		*(.*)
	}
}
