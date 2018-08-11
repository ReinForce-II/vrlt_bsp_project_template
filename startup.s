.section .reset_handler
.global Reset_Handler
.global trap_entry
.global trap_handler
.global bss_init
.global main
Reset_Handler:
    .option push
    .option norelax
    la 		gp, __global_pointer
    .option pop
	la		sp, __stack_start
bss_init:
    la 		a0, __bss_start
    la 		a1, __bss_end
bss_loop:
    beq     a0, a1, bss_done
    sw      zero, 0(a0)
    add     a0, a0, 4
    j       bss_loop
bss_done:

sbss_init:
    la 		a0, __sbss_start
    la 		a1, __sbss_end
sbss_loop:
    beq     a0, a1, sbss_done
    sw      zero, 0(a0)
    add     a0, a0, 4
    j       sbss_loop
sbss_done:
    la 		t0, trap_entry
    csrw 	mtvec, t0
    csrsi   mstatus, 0x8
    j    	main


.section .text
infinitLoop:
    j       infinitLoop

.global portSAVE_CONTEXT
.global portRESTORE_CONTEXT
.global xPortStartScheduler
.global vPortYield
.global vTaskIncrementTick
.global vPortEndScheduler
.global xExitStack


/* Macro for saving task context */
.macro portSAVE_CONTEXT
	.global	pxCurrentTCB
	/* make room in stack */
	addi	sp, sp, -4 * 32

	/* Save Context */
	sw	x1, 0x0(sp)
	sw	x2, 1 * 4(sp)
	sw	x3, 2 * 4(sp)
	sw	x4, 3 * 4(sp)
	sw	x5, 4 * 4(sp)
	sw	x6, 5 * 4(sp)
	sw	x7, 6 * 4(sp)
	sw	x8, 7 * 4(sp)
	sw	x9, 8 * 4(sp)
	sw	x10, 9 * 4(sp)
	sw	x11, 10 * 4(sp)
	sw	x12, 11 * 4(sp)
	sw	x13, 12 * 4(sp)
	sw	x14, 13 * 4(sp)
	sw	x15, 14 * 4(sp)
	sw	x16, 15 * 4(sp)
	sw	x17, 16 * 4(sp)
	sw	x18, 17 * 4(sp)
	sw	x19, 18 * 4(sp)
	sw	x20, 19 * 4(sp)
	sw	x21, 20 * 4(sp)
	sw	x22, 21 * 4(sp)
	sw	x23, 22 * 4(sp)
	sw	x24, 23 * 4(sp)
	sw	x25, 24 * 4(sp)
	sw	x26, 25 * 4(sp)
	sw	x27, 26 * 4(sp)
	sw	x28, 27 * 4(sp)
	sw	x29, 28 * 4(sp)
	sw	x30, 29 * 4(sp)
	sw	x31, 30 * 4(sp)
	
	/* sw current stackpointer in task control block (TCB) */
	lw	t0, pxCurrentTCB	//pointer
	sw	sp, 0x0(t0)
	.endm

/* Saves current error program counter (EPC) as task program counter */
.macro portSAVE_EPC
  	csrr	t0, mepc
	sw	t0, 31 * 4(sp)
	.endm

/* Saves current return adress (RA) as task program counter */
.macro portSAVE_RA
	sw	ra, 31 * 4(sp)
	.endm

/* Macro for restoring task context */
.macro portRESTORE_CONTEXT

	.global	pxCurrentTCB
	/* lw stack pointer from the current TCB */
	lw	sp, pxCurrentTCB
	lw	sp, 0x0(sp)

	/* lw task program counter */
	lw	t0, 31 * 4(sp)
  	csrw	mepc, t0

	/* Run in machine mode */
  	li 		t0, 0x1880
  	csrs	mstatus, t0

	/* Resw registers,
	   Skip global pointer because that does not change */
	lw	x1, 0x0(sp)
	lw	x4, 3 * 4(sp)
	lw	x5, 4 * 4(sp)
	lw	x6, 5 * 4(sp)
	lw	x7, 6 * 4(sp)
	lw	x8, 7 * 4(sp)
	lw	x9, 8 * 4(sp)
	lw	x10, 9 * 4(sp)
	lw	x11, 10 * 4(sp)
	lw	x12, 11 * 4(sp)
	lw	x13, 12 * 4(sp)
	lw	x14, 13 * 4(sp)
	lw	x15, 14 * 4(sp)
	lw	x16, 15 * 4(sp)
	lw	x17, 16 * 4(sp)
	lw	x18, 17 * 4(sp)
	lw	x19, 18 * 4(sp)
	lw	x20, 19 * 4(sp)
	lw	x21, 20 * 4(sp)
	lw	x22, 21 * 4(sp)
	lw	x23, 22 * 4(sp)
	lw	x24, 23 * 4(sp)
	lw	x25, 24 * 4(sp)
	lw	x26, 25 * 4(sp)
	lw	x27, 26 * 4(sp)
	lw	x28, 27 * 4(sp)
	lw	x29, 28 * 4(sp)
	lw	x30, 29 * 4(sp)
	lw	x31, 30 * 4(sp)

	addi	sp, sp, 4 * 32
	mret
	.endm

/* Macro for restoring task context */
trap_entry:
	portSAVE_CONTEXT
	portSAVE_EPC
	csrr a0, mcause
	csrr a1, mepc
	jal	trap_handler	
	portRESTORE_CONTEXT

xPortStartScheduler:
	jal		vPortSetupTimer
	portRESTORE_CONTEXT

vPortEndScheduler:
	ret

vPortYield:
	/*
	*  This routine can be called from outside of interrupt handler. This means
	*  interrupts may be enabled at this point. This is probably okay for registers and
	*  stack. However, "mepc" will be overwritten by the interrupt handler if a timer
	*  interrupt happens during the yield. To avoid this, prevent interrupts before starting.
	*  The write to mstatus in the resw context routine will enable interrupts after the
	*  mret. A more fine-grain lock may be possible.
	*/  
	csrci mstatus, 8

	portSAVE_CONTEXT
	portSAVE_RA
	jal	vTaskSwitchContext
	portRESTORE_CONTEXT
