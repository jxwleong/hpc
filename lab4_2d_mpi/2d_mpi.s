	.file	"2d_mpi.c"
	.text
	.comm	xsize,4,4
	.comm	ysize,4,4
	.globl	f
	.type	f, @function
f:
.LFB6:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	movl	%edi, -36(%rbp)
	movl	%esi, -40(%rbp)
	movl	xsize(%rip), %eax
	cvtsi2sdl	%eax, %xmm1
	movsd	.LC0(%rip), %xmm0
	mulsd	%xmm1, %xmm0
	movsd	.LC1(%rip), %xmm1
	divsd	%xmm1, %xmm0
	movsd	%xmm0, -24(%rbp)
	cvtsi2sdl	-36(%rbp), %xmm0
	divsd	-24(%rbp), %xmm0
	movsd	%xmm0, -16(%rbp)
	cvtsi2sdl	-40(%rbp), %xmm0
	divsd	-24(%rbp), %xmm0
	movsd	%xmm0, -8(%rbp)
	movq	-16(%rbp), %rax
	movq	%rax, %xmm0
	call	sin@PLT
	movsd	%xmm0, -48(%rbp)
	movq	-8(%rbp), %rax
	movq	%rax, %xmm0
	call	cos@PLT
	movsd	-48(%rbp), %xmm1
	addsd	%xmm0, %xmm1
	movsd	.LC2(%rip), %xmm0
	mulsd	%xmm0, %xmm1
	movsd	.LC3(%rip), %xmm0
	addsd	%xmm1, %xmm0
	cvttsd2sil	%xmm0, %eax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
	.size	f, .-f
	.section	.rodata
	.align 8
.LC4:
	.string	"How wide and tall should the picture be? "
.LC5:
	.string	"%d %d"
.LC6:
	.string	"Rank: %d, start: %d end: %d\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB7:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%rbx
	subq	$360, %rsp
	.cfi_offset 3, -24
	movl	%edi, -356(%rbp)
	movq	%rsi, -368(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -24(%rbp)
	xorl	%eax, %eax
	movl	$0, -328(%rbp)
	movl	$0, -324(%rbp)
	movl	$0, -320(%rbp)
	movl	$0, -316(%rbp)
	leaq	-368(%rbp), %rdx
	leaq	-356(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	MPI_Init@PLT
	leaq	-348(%rbp), %rax
	movq	%rax, %rsi
	leaq	ompi_mpi_comm_world(%rip), %rdi
	call	MPI_Comm_size@PLT
	leaq	-344(%rbp), %rax
	movq	%rax, %rsi
	leaq	ompi_mpi_comm_world(%rip), %rdi
	call	MPI_Comm_rank@PLT
	leaq	-340(%rbp), %rdx
	leaq	-288(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	MPI_Get_processor_name@PLT
	movl	-344(%rbp), %eax
	testl	%eax, %eax
	jne	.L4
	leaq	.LC4(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	movq	stdout(%rip), %rax
	movq	%rax, %rdi
	call	fflush@PLT
	leaq	ysize(%rip), %rdx
	leaq	xsize(%rip), %rsi
	leaq	.LC5(%rip), %rdi
	movl	$0, %eax
	call	__isoc99_scanf@PLT
	movl	ysize(%rip), %eax
	cltq
	salq	$3, %rax
	movq	%rax, %rdi
	call	malloc@PLT
	movq	%rax, -296(%rbp)
	movl	xsize(%rip), %eax
	movl	-348(%rbp), %ebx
	cltd
	idivl	%ebx
	movl	%eax, -312(%rbp)
	cmpl	$0, -312(%rbp)
	je	.L4
	movl	$1, -328(%rbp)
.L4:
	leaq	ompi_mpi_comm_world(%rip), %r8
	movl	$0, %ecx
	leaq	ompi_mpi_int(%rip), %rdx
	movl	$1, %esi
	leaq	xsize(%rip), %rdi
	call	MPI_Bcast@PLT
	leaq	ompi_mpi_comm_world(%rip), %r8
	movl	$0, %ecx
	leaq	ompi_mpi_int(%rip), %rdx
	movl	$1, %esi
	leaq	ysize(%rip), %rdi
	call	MPI_Bcast@PLT
	movl	xsize(%rip), %eax
	movl	-348(%rbp), %ecx
	cltd
	idivl	%ecx
	movl	%eax, -308(%rbp)
	movl	-344(%rbp), %eax
	movl	-308(%rbp), %edx
	imull	%edx, %eax
	movl	%eax, -304(%rbp)
	movl	-344(%rbp), %eax
	imull	-308(%rbp), %eax
	movl	%eax, %edx
	movl	-308(%rbp), %eax
	addl	%edx, %eax
	subl	$1, %eax
	movl	%eax, -300(%rbp)
	movl	-344(%rbp), %eax
	movl	-300(%rbp), %ecx
	movl	-304(%rbp), %edx
	movl	%eax, %esi
	leaq	.LC6(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$0, -332(%rbp)
	jmp	.L5
.L12:
	movl	-332(%rbp), %eax
	cltq
	leaq	0(,%rax,8), %rdx
	movq	-296(%rbp), %rax
	addq	%rdx, %rax
	movq	(%rax), %rax
	testq	%rax, %rax
	jne	.L6
	movl	xsize(%rip), %eax
	cltq
	salq	$2, %rax
	movl	-332(%rbp), %edx
	movslq	%edx, %rdx
	leaq	0(,%rdx,8), %rcx
	movq	-296(%rbp), %rdx
	leaq	(%rcx,%rdx), %rbx
	movq	%rax, %rdi
	call	malloc@PLT
	movq	%rax, (%rbx)
.L6:
	movl	-304(%rbp), %eax
	movl	%eax, -336(%rbp)
	jmp	.L7
.L8:
	movl	$32, %edi
	call	putchar@PLT
	addl	$1, -336(%rbp)
.L7:
	movl	-336(%rbp), %eax
	cmpl	-300(%rbp), %eax
	jle	.L8
	movl	-344(%rbp), %eax
	cmpl	%eax, -324(%rbp)
	jne	.L9
	cmpl	$1, -328(%rbp)
	jne	.L9
	movl	-348(%rbp), %eax
	movl	-308(%rbp), %edx
	imull	%edx, %eax
	movl	%eax, -336(%rbp)
	jmp	.L10
.L11:
	movl	-332(%rbp), %eax
	cltq
	leaq	0(,%rax,8), %rdx
	movq	-296(%rbp), %rax
	addq	%rdx, %rax
	movq	(%rax), %rax
	movl	-336(%rbp), %edx
	movslq	%edx, %rdx
	salq	$2, %rdx
	leaq	(%rax,%rdx), %rbx
	movl	-332(%rbp), %edx
	movl	-336(%rbp), %eax
	movl	%edx, %esi
	movl	%eax, %edi
	call	f
	movl	%eax, (%rbx)
	addl	$1, -336(%rbp)
.L10:
	movl	xsize(%rip), %eax
	cmpl	%eax, -336(%rbp)
	jl	.L11
.L9:
	addl	$1, -332(%rbp)
.L5:
	movl	ysize(%rip), %eax
	cmpl	%eax, -332(%rbp)
	jl	.L12
	leaq	ompi_mpi_comm_world(%rip), %rdi
	call	MPI_Barrier@PLT
	call	MPI_Finalize@PLT
	movl	$0, %eax
	movq	-24(%rbp), %rbx
	xorq	%fs:40, %rbx
	je	.L14
	call	__stack_chk_fail@PLT
.L14:
	addq	$360, %rsp
	popq	%rbx
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE7:
	.size	main, .-main
	.section	.rodata
	.align 8
.LC0:
	.long	1413754136
	.long	1074340347
	.align 8
.LC1:
	.long	0
	.long	1078853632
	.align 8
.LC2:
	.long	0
	.long	1078984704
	.align 8
.LC3:
	.long	0
	.long	1080033280
	.ident	"GCC: (Ubuntu 9.4.0-1ubuntu1~20.04.1) 9.4.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	 1f - 0f
	.long	 4f - 1f
	.long	 5
0:
	.string	 "GNU"
1:
	.align 8
	.long	 0xc0000002
	.long	 3f - 2f
2:
	.long	 0x3
3:
	.align 8
4:
