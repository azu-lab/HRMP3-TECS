OUTPUT_FORMAT("elf32-littlearm", "elf32-bigarm","elf32-littlearm")
OUTPUT_ARCH(arm)

MEMORY {
	ROM : ORIGIN = 0x1000000, LENGTH = 0x800000
	RAM : ORIGIN = 0x1800000, LENGTH = 0x800000
}

SECTIONS {
	.discard (NOLOAD) : {
		start.o(.discard) cfg_attsec3.o(.discard) kernel_cfg.o(.discard) kernel_mem.o(.discard) libkernel.a(.discard) libc.a(.discard) libgcc.a(.discard) 
	}

	.text_kernel 0x1000000 : ALIGN(4) {
		__start_mp_text_kernel = .;
		__start_mo_text_kernel = .;
		__start_text_kernel = .;
		start.o(.text .text.*) cfg_attsec3.o(.text .text.*) kernel_cfg.o(.text .text.*) kernel_mem.o(.text .text.*) libkernel.a(.text .text.*) *(.kernel_code_CLS_PRC1) *(.kernel_code_CLS_PRC2) *(.kernel_code_CLS_PRC3) *(.kernel_code_CLS_PRC4) *(.kernel_code_CLS_ALL_PRC1) *(.kernel_code_CLS_ALL_PRC2) *(.kernel_code_CLS_ALL_PRC3) *(.kernel_code_CLS_ALL_PRC4) 
		__end_text_kernel = .;
		__end_mo_text_kernel = .;
		__aend_mo_text_kernel = ALIGN(4K);
		__aend_mp_text_kernel = ALIGN(4K);
	} > ROM

	.rodata_kernel ALIGN(4K) : ALIGN(4) {
		__start_mp_rodata_kernel = .;
		__start_mo_rodata_kernel = .;
		__start_rodata_kernel = .;
		start.o(.rodata .rodata.*) cfg_attsec3.o(.rodata .rodata.*) KEEP(kernel_cfg.o(.rodata .rodata.*)) kernel_mem.o(.rodata .rodata.*) libkernel.a(.rodata .rodata.*) 
		__end_rodata_kernel = .;
		__end_mo_rodata_kernel = .;
		__aend_mo_rodata_kernel = ALIGN(4K);
		__aend_mp_rodata_kernel = ALIGN(4K);
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

	.RAM_text_kernel 0x1800000 : ALIGN(4) {
		__start_mp_RAM_text_kernel = .;
		__start_mo_RAM_text_kernel = .;
		__start_RAM_text_kernel = .;
		*(.kernel_add07) 
		__end_RAM_text_kernel = .;
		__end_mo_RAM_text_kernel = .;
		__aend_mo_RAM_text_kernel = ALIGN(4K);
		__aend_mp_RAM_text_kernel = ALIGN(4K);
	} > RAM

	.RAM_rodata_kernel ALIGN(4K) : ALIGN(4) {
		__start_mp_RAM_rodata_kernel = .;
		__start_mo_RAM_rodata_kernel = .;
		__start_RAM_rodata_kernel = .;
		*(.kernel_add03) 
		__end_RAM_rodata_kernel = .;
		__end_mo_RAM_rodata_kernel = .;
		__aend_mo_RAM_rodata_kernel = ALIGN(4K);
		__aend_mp_RAM_rodata_kernel = ALIGN(4K);
	} > RAM

	.RAM_text_kernel_A7 ALIGN(4K) : ALIGN(4) {
		__start_mp_RAM_text_kernel_A7 = .;
		__start_mo_RAM_text_kernel_A7 = .;
		__start_RAM_text_kernel_A7 = .;
		*(.kernel_add08) 
		__end_RAM_text_kernel_A7 = .;
		__end_mo_RAM_text_kernel_A7 = .;
		__aend_mo_RAM_text_kernel_A7 = ALIGN(4K);
		__aend_mp_RAM_text_kernel_A7 = ALIGN(4K);
	} > RAM

	.RAM_linkonly_kernel ALIGN(4K) : ALIGN(4) {
		__start_mp_RAM_linkonly_kernel = .;
		__start_mo_RAM_linkonly_kernel = .;
		__start_RAM_linkonly_kernel = .;
		*(.kernel_add04) 
		__end_RAM_linkonly_kernel = .;
		__end_mo_RAM_linkonly_kernel = .;
		__aend_mo_RAM_linkonly_kernel = ALIGN(4K);
		__aend_mp_RAM_linkonly_kernel = ALIGN(4K);
	} > RAM

	.RAM_text_kernel_A45 ALIGN(4K) : ALIGN(4) {
		__start_mp_RAM_text_kernel_A45 = .;
		__start_mo_RAM_text_kernel_A45 = .;
		__start_RAM_text_kernel_A45 = .;
		*(.kernel_add17) 
		__end_RAM_text_kernel_A45 = .;
		__end_mo_RAM_text_kernel_A45 = .;
		__aend_mo_RAM_text_kernel_A45 = ALIGN(4K);
		__aend_mp_RAM_text_kernel_A45 = ALIGN(4K);
	} > RAM

	.RAM_rodata_kernel_A41 ALIGN(4K) : ALIGN(4) {
		__start_mp_RAM_rodata_kernel_A41 = .;
		__start_mo_RAM_rodata_kernel_A41 = .;
		__start_RAM_rodata_kernel_A41 = .;
		*(.kernel_add13) 
		__end_RAM_rodata_kernel_A41 = .;
		__end_mo_RAM_rodata_kernel_A41 = .;
		__aend_mo_RAM_rodata_kernel_A41 = ALIGN(4K);
		__aend_mp_RAM_rodata_kernel_A41 = ALIGN(4K);
	} > RAM

	.RAM_text_kernel_A47 ALIGN(4K) : ALIGN(4) {
		__start_mp_RAM_text_kernel_A47 = .;
		__start_mo_RAM_text_kernel_A47 = .;
		__start_RAM_text_kernel_A47 = .;
		*(.kernel_add18) 
		__end_RAM_text_kernel_A47 = .;
		__end_mo_RAM_text_kernel_A47 = .;
		__aend_mo_RAM_text_kernel_A47 = ALIGN(4K);
		__aend_mp_RAM_text_kernel_A47 = ALIGN(4K);
	} > RAM

	.RAM_linkonly_kernel_A43 ALIGN(4K) : ALIGN(4) {
		__start_mp_RAM_linkonly_kernel_A43 = .;
		__start_mo_RAM_linkonly_kernel_A43 = .;
		__start_RAM_linkonly_kernel_A43 = .;
		*(.kernel_add14) 
		__end_RAM_linkonly_kernel_A43 = .;
		__end_mo_RAM_linkonly_kernel_A43 = .;
		__aend_mo_RAM_linkonly_kernel_A43 = ALIGN(4K);
		__aend_mp_RAM_linkonly_kernel_A43 = ALIGN(4K);
	} > RAM

	.noinit_kernel_A4 ALIGN(4K) (NOLOAD) : ALIGN(4) {
		__start_mp_rwdata_kernel_A4 = .;
		__start_mo_rwdata_kernel_A4 = .;
		__start_noinit_kernel_A4 = .;
		*(.kernel_add05) 
		__end_noinit_kernel_A4 = .;
		__end_mo_rwdata_kernel_A4 = .;
		__aend_mo_rwdata_kernel_A4 = ALIGN(4K);
		__aend_mp_rwdata_kernel_A4 = ALIGN(4K);
	} > RAM

	.data_kernel ALIGN(4K) : ALIGN(4) {
		__start_mp_rwdata_kernel = .;
		__start_mo_rwdata_kernel = .;
		__start_data_kernel = .;
		start.o(.data .data.*) cfg_attsec3.o(.data .data.*) kernel_cfg.o(.data .data.*) kernel_mem.o(.data .data.*) libkernel.a(.data .data.*) 
		__end_data_kernel = .;
	} > RAM AT > ROM
	__start_idata_kernel = LOADADDR(.data_kernel);

	.bss_kernel ALIGN(4) (NOLOAD) : ALIGN(4) {
		__start_bss_kernel = .;
		start.o(.bss .bss.* COMMON) cfg_attsec3.o(.bss .bss.* COMMON) kernel_cfg.o(.bss .bss.* COMMON) kernel_mem.o(.bss .bss.* COMMON) libkernel.a(.bss .bss.* COMMON) 
		__end_bss_kernel = .;
	} > RAM

	.noinit_kernel ALIGN(4) (NOLOAD) : ALIGN(4) {
		__start_noinit_kernel = .;
		start.o(.noinit) cfg_attsec3.o(.noinit) kernel_cfg.o(.noinit) kernel_mem.o(.noinit) libkernel.a(.noinit) *(.kernel_add01) *(.kernel_data_CLS_PRC1) *(.kernel_data_CLS_PRC2) *(.kernel_data_CLS_PRC3) *(.kernel_data_CLS_PRC4) *(.kernel_data_CLS_ALL_PRC1) *(.kernel_data_CLS_ALL_PRC2) *(.kernel_data_CLS_ALL_PRC3) *(.kernel_data_CLS_ALL_PRC4) *(.system_stack_CLS_PRC1) *(.system_stack_CLS_PRC2) *(.system_stack_CLS_PRC3) *(.system_stack_CLS_PRC4) *(.system_stack_CLS_ALL_PRC1) *(.system_stack_CLS_ALL_PRC2) *(.system_stack_CLS_ALL_PRC3) *(.system_stack_CLS_ALL_PRC4) 
		__end_noinit_kernel = .;
		__end_mo_rwdata_kernel = .;
		__aend_mo_rwdata_kernel = ALIGN(4K);
		__aend_mp_rwdata_kernel = ALIGN(4K);
	} > RAM

	.noinit_kernel_A6 ALIGN(4K) (NOLOAD) : ALIGN(4) {
		__start_mp_rwdata_kernel_A6 = .;
		__start_mo_rwdata_kernel_A6 = .;
		__start_noinit_kernel_A6 = .;
		*(.kernel_add06) 
		__end_noinit_kernel_A6 = .;
		__end_mo_rwdata_kernel_A6 = .;
		__aend_mo_rwdata_kernel_A6 = ALIGN(4K);
		__aend_mp_rwdata_kernel_A6 = ALIGN(4K);
	} > RAM

	.noinit_kernel_A2 ALIGN(4K) (NOLOAD) : ALIGN(4) {
		__start_mp_rwdata_kernel_A2 = .;
		__start_mo_rwdata_kernel_A2 = .;
		__start_noinit_kernel_A2 = .;
		*(.kernel_add02) 
		__end_noinit_kernel_A2 = .;
		__end_mo_rwdata_kernel_A2 = .;
		__aend_mo_rwdata_kernel_A2 = ALIGN(4K);
		__aend_mp_rwdata_kernel_A2 = ALIGN(4K);
	} > RAM

	.noinit_kernel_A44 ALIGN(4K) (NOLOAD) : ALIGN(4) {
		__start_mp_rwdata_kernel_A44 = .;
		__start_mo_rwdata_kernel_A44 = .;
		__start_noinit_kernel_A44 = .;
		*(.kernel_add15) 
		__end_noinit_kernel_A44 = .;
		__end_mo_rwdata_kernel_A44 = .;
		__aend_mo_rwdata_kernel_A44 = ALIGN(4K);
		__aend_mp_rwdata_kernel_A44 = ALIGN(4K);
	} > RAM

	.noinit_kernel_A40 ALIGN(4K) (NOLOAD) : ALIGN(4) {
		__start_mp_rwdata_kernel_A40 = .;
		__start_mo_rwdata_kernel_A40 = .;
		__start_noinit_kernel_A40 = .;
		*(.kernel_add11) 
		__end_noinit_kernel_A40 = .;
		__end_mo_rwdata_kernel_A40 = .;
		__aend_mo_rwdata_kernel_A40 = ALIGN(4K);
		__aend_mp_rwdata_kernel_A40 = ALIGN(4K);
	} > RAM

	.noinit_kernel_A46 ALIGN(4K) (NOLOAD) : ALIGN(4) {
		__start_mp_rwdata_kernel_A46 = .;
		__start_mo_rwdata_kernel_A46 = .;
		__start_noinit_kernel_A46 = .;
		*(.kernel_add16) 
		__end_noinit_kernel_A46 = .;
		__end_mo_rwdata_kernel_A46 = .;
		__aend_mo_rwdata_kernel_A46 = ALIGN(4K);
		__aend_mp_rwdata_kernel_A46 = ALIGN(4K);
	} > RAM

	.noinit_kernel_A42 ALIGN(4K) (NOLOAD) : ALIGN(4) {
		__start_mp_rwdata_kernel_A42 = .;
		__start_mo_rwdata_kernel_A42 = .;
		__start_noinit_kernel_A42 = .;
		*(.kernel_add12) 
		__end_noinit_kernel_A42 = .;
		__end_mo_rwdata_kernel_A42 = .;
		__aend_mo_rwdata_kernel_A42 = ALIGN(4K);
		__aend_mp_rwdata_kernel_A42 = ALIGN(4K);
	} > RAM

	.RAM_stext_kernel ALIGN(4K) : ALIGN(4) {
		__start_mp_RAM_stext_kernel = .;
		__start_mo_RAM_stext_kernel = .;
		__start_RAM_stext_kernel = .;
		*(.kernel_add27) 
		__end_RAM_stext_kernel = .;
		__end_mo_RAM_stext_kernel = .;
		__aend_mo_RAM_stext_kernel = ALIGN(4K);
		__aend_mp_RAM_stext_kernel = ALIGN(4K);
	} > RAM

	.RAM_srodata_kernel ALIGN(4K) : ALIGN(4) {
		__start_mp_RAM_srodata_kernel = .;
		__start_mo_RAM_srodata_kernel = .;
		__start_RAM_srodata_kernel = .;
		*(.kernel_add23) 
		__end_RAM_srodata_kernel = .;
		__end_mo_RAM_srodata_kernel = .;
		__aend_mo_RAM_srodata_kernel = ALIGN(4K);
		__aend_mp_RAM_srodata_kernel = ALIGN(4K);
	} > RAM

	.RAM_stext_kernel_A27 ALIGN(4K) : ALIGN(4) {
		__start_mp_RAM_stext_kernel_A7 = .;
		__start_mo_RAM_stext_kernel_A7 = .;
		__start_RAM_stext_kernel_A27 = .;
		*(.kernel_add28) 
		__end_RAM_stext_kernel_A27 = .;
		__end_mo_RAM_stext_kernel_A7 = .;
		__aend_mo_RAM_stext_kernel_A7 = ALIGN(4K);
		__aend_mp_RAM_stext_kernel_A7 = ALIGN(4K);
	} > RAM

	.RAM_slinkonly_kernel ALIGN(4K) : ALIGN(4) {
		__start_mp_RAM_slinkonly_kernel = .;
		__start_mo_RAM_slinkonly_kernel = .;
		__start_RAM_slinkonly_kernel = .;
		*(.kernel_add24) 
		__end_RAM_slinkonly_kernel = .;
		__end_mo_RAM_slinkonly_kernel = .;
		__aend_mo_RAM_slinkonly_kernel = ALIGN(4K);
		__aend_mp_RAM_slinkonly_kernel = ALIGN(4K);
	} > RAM

	.snoinit_kernel_A24 ALIGN(4K) (NOLOAD) : ALIGN(4) {
		__start_mp_srwdata_kernel_A4 = .;
		__start_mo_srwdata_kernel_A4 = .;
		__start_snoinit_kernel_A24 = .;
		*(.kernel_add25) 
		__end_snoinit_kernel_A24 = .;
		__end_mo_srwdata_kernel_A4 = .;
		__aend_mo_srwdata_kernel_A4 = ALIGN(4K);
		__aend_mp_srwdata_kernel_A4 = ALIGN(4K);
	} > RAM

	.snoinit_kernel ALIGN(4K) (NOLOAD) : ALIGN(4) {
		__start_mp_srwdata_kernel = .;
		__start_mo_srwdata_kernel = .;
		__start_snoinit_kernel = .;
		*(.kernel_add21) 
		__end_snoinit_kernel = .;
		__end_mo_srwdata_kernel = .;
		__aend_mo_srwdata_kernel = ALIGN(4K);
		__aend_mp_srwdata_kernel = ALIGN(4K);
	} > RAM

	.snoinit_kernel_A26 ALIGN(4K) (NOLOAD) : ALIGN(4) {
		__start_mp_srwdata_kernel_A6 = .;
		__start_mo_srwdata_kernel_A6 = .;
		__start_snoinit_kernel_A26 = .;
		*(.kernel_add26) 
		__end_snoinit_kernel_A26 = .;
		__end_mo_srwdata_kernel_A6 = .;
		__aend_mo_srwdata_kernel_A6 = ALIGN(4K);
		__aend_mp_srwdata_kernel_A6 = ALIGN(4K);
	} > RAM

	.snoinit_kernel_A22 ALIGN(4K) (NOLOAD) : ALIGN(4) {
		__start_mp_srwdata_kernel_A2 = .;
		__start_mo_srwdata_kernel_A2 = .;
		__start_snoinit_kernel_A22 = .;
		*(.kernel_add22) 
		__end_snoinit_kernel_A22 = .;
		__end_mo_srwdata_kernel_A2 = .;
		__aend_mo_srwdata_kernel_A2 = ALIGN(4K);
		__aend_mp_srwdata_kernel_A2 = ALIGN(4K);
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
