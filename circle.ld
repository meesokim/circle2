/*
 * circle.ld
 */

ENTRY(_start)

SECTIONS
{
	.init : {
		*(.init)
	}

	.text : {
		*(.text*)

		_etext = .;
	}

	.rodata : {
		*(.rodata*)
	}

	.init_array : {
		__init_start = .;

		*(.init_array*)

		__init_end = .;
	}

	.ARM.exidx : {
		__exidx_start = .;

		*(.ARM.exidx*)

		__exidx_end = .;
	}

	.eh_frame : {
		*(.eh_frame*)
	}

	.data : {
		*(.data*)
	}

	.bss : {
		__bss_start = .;

		*(.bss*)
		*(COMMON)

		__bss_end = .;
	}
	
	/* 
	 * Next we put stack for Core0 
	 */
	.stack0 : {
        . = ALIGN(8); 					/* Stack must always be aligned to 8 byte boundary AAPCS32 call standard */
        __stack_start__core0 = .;		/* Label in case we want address of stack core 0 section start */
        . = . + 512;     				/* IRQ stack size core 0 */
        __IRQ_stack_core0 = .;
        . = . + 512;     				/* FIQ stack size core 0 */
        __FIQ_stack_core0 = .;
        . = . + 512;    				/* SVC stack size core 0 */
        __SVC_stack_core0 = .;
        . = . + 16384;    				/* SYS stack size core 0 */
        __SYS_stack_core0 = .;
        __stack_end__core0 = .;  		/* Label in case we want address of stack core 0 section end */
	}

	/* 
	 * Next we put stack for Core1 
	 */
	.stack1 : {
        . = ALIGN(8); 					/* Stack must always be aligned to 8 byte boundary AAPCS32 call standard */
        __stack_start__core1 = .;		/* Label in case we want address of stack core 1 section start */
        . = . + 512;     				/* IRQ stack size core 1 */
        __IRQ_stack_core1 = .;
        . = . + 512;     				/* FIQ stack size core 1 */
        __FIQ_stack_core1 = .;
        . = . + 512;    				/* SVC stack size core 1 */
        __SVC_stack_core1 = .;
        . = . + 4096;    				/* SYS stack size core 1 */
        __SYS_stack_core1 = .;
        __stack_end__core1 = .;  		/* Label in case we want address of stack core 1 section end */
	}

	/* 
	 * Next we put stack for Core2 
	 */
	.stack2 : {
        . = ALIGN(8); 					/* Stack must always be aligned to 8 byte boundary AAPCS32 call standard */
        __stack_start__core2 = .;		/* Label in case we want address of stack core 2 section start */
        . = . + 512;     				/* IRQ stack size core 2 */
        __IRQ_stack_core2 = .;
        . = . + 512;     				/* FIQ stack size core 2 */
        __FIQ_stack_core2 = .;
        . = . + 512;    				/* SVC stack size core 2 */
        __SVC_stack_core2 = .;
        . = . + 4096;    				/* SYS stack size core 2 */
        __SYS_stack_core2 = .;
        __stack_end__core2 = .;  		/* Label in case we want address of stack core 2 section end */
   	}

	/* 
	 * Next we put stack for Core3
	 */
	.stack3 : {
        . = ALIGN(8); 					/* Stack must always be aligned to 8 byte boundary AAPCS32 call standard */
        __stack_start__core3 = .;		/* Label in case we want address of stack core 3 section start */
        . = . + 512;     				/* IRQ stack size core 3 */
        __IRQ_stack_core3 = .;
        . = . + 512;     				/* FIQ stack size core 3 */
        __FIQ_stack_core3 = .;
        . = . + 512;    				/* SVC stack size core 3 */
        __SVC_stack_core3 = .;
        . = . + 4096;    				/* SYS stack size core 3 */
        __SYS_stack_core3 = .;
        __stack_end__core3 = .;  		/* Label in case we want address of stack core 3 section end */
   	}	
}
