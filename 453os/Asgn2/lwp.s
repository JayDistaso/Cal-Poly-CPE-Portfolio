	.file	"lwp.c"
	.section	.debug_abbrev,"",@progbits
.Ldebug_abbrev0:
	.section	.debug_info,"",@progbits
.Ldebug_info0:
	.section	.debug_line,"",@progbits
.Ldebug_line0:
	.text
.Ltext0:
	.local	rootContext
	.comm	rootContext,704,32
	.local	tidRunning
	.comm	tidRunning,8,8
	.local	rootThread
	.comm	rootThread,8,8
	.data
	.align 32
	.type	RoundRobin, @object
	.size	RoundRobin, 40
RoundRobin:
	.quad	0
	.quad	0
	.quad	rr_admit
	.quad	rr_remove
	.quad	rr_next
	.align 8
	.type	currentScheduler, @object
	.size	currentScheduler, 8
currentScheduler:
	.quad	RoundRobin
	.local	rootOfSched
	.comm	rootOfSched,8,8
	.local	numberOfProcs
	.comm	numberOfProcs,8,8
	.text
.globl lwp_create
	.type	lwp_create, @function
lwp_create:
.LFB0:
	.file 1 "./lwp.c"
	.loc 1 23 0
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%rbx
	subq	$600, %rsp
	movq	%rdi, -584(%rbp)
	movq	%rsi, -592(%rbp)
	movq	%rdx, -600(%rbp)
	.loc 1 32 0
	movl	tid.3311(%rip), %eax
	cmpl	$-4, %eax
	jbe	.L2
	.cfi_offset 3, -24
	.loc 1 33 0
	movq	$-1, %rax
	jmp	.L3
.L2:
	.loc 1 36 0
	movq	-600(%rbp), %rax
	addq	$102400, %rax
	movl	$8, %esi
	movq	%rax, %rdi
	call	calloc
	movq	%rax, -56(%rbp)
	.loc 1 38 0
	cmpq	$0, -56(%rbp)
	jne	.L4
	.loc 1 39 0
	movq	$-1, %rax
	jmp	.L3
.L4:
	.loc 1 42 0
	movl	$704, %esi
	movl	$1, %edi
	call	calloc
	movq	%rax, -32(%rbp)
	.loc 1 44 0
	movq	numberOfProcs(%rip), %rax
	addq	$1, %rax
	movq	%rax, numberOfProcs(%rip)
	.loc 1 45 0
	movl	tid.3311(%rip), %eax
	movslq	%eax, %rcx
	movq	-32(%rbp), %rdx
	movq	%rcx, (%rdx)
	addl	$1, %eax
	movl	%eax, tid.3311(%rip)
	.loc 1 46 0
	movq	-32(%rbp), %rax
	movq	-56(%rbp), %rdx
	movq	%rdx, 8(%rax)
	.loc 1 47 0
	movq	-600(%rbp), %rax
	leaq	102400(%rax), %rdx
	movq	-32(%rbp), %rax
	movq	%rdx, 16(%rax)
	.loc 1 48 0
	leaq	-576(%rbp), %rbx
	movl	$0, %eax
	movl	$64, %edx
	movq	%rbx, %rdi
	movq	%rdx, %rcx
	rep stosq
	movb	$127, -576(%rbp)
	movb	$3, -575(%rbp)
	movb	$-128, -552(%rbp)
	movb	$31, -551(%rbp)
	movb	$-1, -548(%rbp)
	movb	$-1, -547(%rbp)
	movb	$37, -400(%rbp)
	movb	$37, -399(%rbp)
	movb	$37, -398(%rbp)
	movb	$37, -397(%rbp)
	movb	$37, -396(%rbp)
	movb	$37, -395(%rbp)
	movb	$37, -394(%rbp)
	movb	$37, -393(%rbp)
	movb	$37, -392(%rbp)
	movb	$37, -391(%rbp)
	movb	$37, -390(%rbp)
	movb	$37, -389(%rbp)
	movb	$37, -388(%rbp)
	movb	$37, -387(%rbp)
	movb	$37, -386(%rbp)
	movb	$37, -385(%rbp)
	movb	$-1, -360(%rbp)
	movb	$-1, -355(%rbp)
	movq	-32(%rbp), %rax
	leaq	160(%rax), %rbx
	leaq	-576(%rbp), %rdx
	movl	$64, %eax
	movq	%rbx, %rdi
	movq	%rdx, %rsi
	movq	%rax, %rcx
	rep movsq
	.loc 1 50 0
	movq	-32(%rbp), %rax
	movq	8(%rax), %rax
	movq	-600(%rbp), %rdx
	addq	$102400, %rdx
	salq	$3, %rdx
	addq	%rdx, %rax
	movq	%rax, -48(%rbp)
	.loc 1 58 0
	subq	$8, -48(%rbp)
	.loc 1 59 0
	movl	$lwp_exit, %edx
	movq	-48(%rbp), %rax
	movq	%rdx, (%rax)
	.loc 1 60 0
	subq	$8, -48(%rbp)
	.loc 1 61 0
	movl	$lwp_exit, %edx
	movq	-48(%rbp), %rax
	movq	%rdx, (%rax)
	.loc 1 62 0
	movq	-48(%rbp), %rax
	movq	%rax, -40(%rbp)
	.loc 1 63 0
	subq	$8, -48(%rbp)
	.loc 1 64 0
	movq	-584(%rbp), %rdx
	movq	-48(%rbp), %rax
	movq	%rdx, (%rax)
	.loc 1 65 0
	subq	$8, -48(%rbp)
	.loc 1 66 0
	movq	-40(%rbp), %rdx
	movq	-48(%rbp), %rax
	movq	%rdx, (%rax)
	.loc 1 79 0
	movq	-32(%rbp), %rax
	movq	-48(%rbp), %rdx
	movq	%rdx, 24(%rax)
	.loc 1 80 0
	movq	-48(%rbp), %rdx
	movq	-32(%rbp), %rax
	movq	%rdx, 80(%rax)
	.loc 1 81 0
	movq	-48(%rbp), %rdx
	movq	-32(%rbp), %rax
	movq	%rdx, 88(%rax)
	.loc 1 84 0
	movq	-592(%rbp), %rdx
	movq	-32(%rbp), %rax
	movq	%rdx, 72(%rax)
	.loc 1 85 0
	movq	currentScheduler(%rip), %rax
	movq	16(%rax), %rdx
	movq	-32(%rbp), %rax
	movq	%rax, %rdi
	call	*%rdx
	.loc 1 86 0
	movq	rootThread(%rip), %rax
	movq	%rax, -24(%rbp)
	.loc 1 87 0
	movq	rootThread(%rip), %rax
	testq	%rax, %rax
	jne	.L10
	.loc 1 88 0
	movq	-32(%rbp), %rax
	movq	%rax, rootThread(%rip)
	jmp	.L6
.L8:
	.loc 1 92 0
	movq	-24(%rbp), %rax
	movq	672(%rax), %rax
	movq	%rax, -24(%rbp)
	jmp	.L7
.L10:
	.loc 1 91 0
	nop
.L7:
	movq	-24(%rbp), %rax
	movq	672(%rax), %rax
	testq	%rax, %rax
	jne	.L8
	.loc 1 94 0
	movq	-24(%rbp), %rax
	movq	-32(%rbp), %rdx
	movq	%rdx, 672(%rax)
.L6:
	.loc 1 98 0
	movq	-32(%rbp), %rax
	movq	(%rax), %rax
.L3:
	.loc 1 99 0
	addq	$600, %rsp
	popq	%rbx
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	lwp_create, .-lwp_create
.globl lwp_exit
	.type	lwp_exit, @function
lwp_exit:
.LFB1:
	.loc 1 101 0
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	.loc 1 105 0
	movq	currentScheduler(%rip), %rax
	movq	32(%rax), %rax
	call	*%rax
	movq	%rax, -24(%rbp)
	.loc 1 106 0
	movq	tidRunning(%rip), %rax
	movq	%rax, %rdi
	call	tid2thread
	movq	8(%rax), %rax
	movq	%rax, -16(%rbp)
	.loc 1 107 0
	movq	tidRunning(%rip), %rax
	movq	%rax, %rdi
	call	tid2thread
	movq	%rax, -8(%rbp)
	.loc 1 108 0
	cmpq	$0, -24(%rbp)
	je	.L12
	.loc 1 109 0
	movq	-24(%rbp), %rax
	movq	24(%rax), %rax
#APP
# 109 "./lwp.c" 1
	movq  %rax,%rsp
# 0 "" 2
	.loc 1 110 0
#NO_APP
	movq	-16(%rbp), %rdx
	movq	-8(%rbp), %rcx
	movq	-24(%rbp), %rax
	movq	%rcx, %rsi
	movq	%rax, %rdi
	call	lwp_exitOntoSafeStack
	jmp	.L14
.L12:
	.loc 1 113 0
	movq	rootContext+24(%rip), %rax
#APP
# 113 "./lwp.c" 1
	movq  %rax,%rsp
# 0 "" 2
	.loc 1 114 0
#NO_APP
	movq	-16(%rbp), %rdx
	movq	-8(%rbp), %rax
	movq	%rax, %rsi
	movl	$rootContext, %edi
	call	lwp_exitOntoSafeStack
.L14:
	.loc 1 118 0
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1:
	.size	lwp_exit, .-lwp_exit
.globl lwp_exitOntoSafeStack
	.type	lwp_exitOntoSafeStack, @function
lwp_exitOntoSafeStack:
.LFB2:
	.loc 1 120 0
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	movq	%rdi, -24(%rbp)
	movq	%rsi, -32(%rbp)
	movq	%rdx, -40(%rbp)
	.loc 1 123 0
	movq	currentScheduler(%rip), %rax
	movq	24(%rax), %rdx
	movq	-32(%rbp), %rax
	movq	%rax, %rdi
	call	*%rdx
	.loc 1 124 0
	movq	rootThread(%rip), %rax
	movq	%rax, -8(%rbp)
	.loc 1 125 0
	movq	rootThread(%rip), %rax
	cmpq	-32(%rbp), %rax
	jne	.L24
	.loc 1 126 0
	movq	rootThread(%rip), %rax
	movq	672(%rax), %rax
	movq	%rax, rootThread(%rip)
	jmp	.L17
.L20:
	.loc 1 130 0
	movq	-8(%rbp), %rax
	movq	672(%rax), %rax
	cmpq	-32(%rbp), %rax
	jne	.L19
	.loc 1 131 0
	movq	-8(%rbp), %rax
	movq	672(%rax), %rax
	movq	672(%rax), %rdx
	movq	-8(%rbp), %rax
	movq	%rdx, 672(%rax)
	jmp	.L18
.L19:
	.loc 1 134 0
	movq	-8(%rbp), %rax
	movq	672(%rax), %rax
	movq	%rax, -8(%rbp)
	jmp	.L18
.L24:
	.loc 1 129 0
	nop
.L18:
	movq	-8(%rbp), %rax
	movq	672(%rax), %rax
	testq	%rax, %rax
	jne	.L20
.L17:
	.loc 1 138 0
	movq	-40(%rbp), %rax
	movq	%rax, %rdi
	call	free
	.loc 1 139 0
	movq	-32(%rbp), %rax
	movq	%rax, %rdi
	call	free
	.loc 1 141 0
	movq	numberOfProcs(%rip), %rax
	subq	$1, %rax
	movq	%rax, numberOfProcs(%rip)
	.loc 1 142 0
	movq	numberOfProcs(%rip), %rax
	testq	%rax, %rax
	jne	.L21
	.loc 1 143 0
	movq	$0, tidRunning(%rip)
	.loc 1 144 0
	call	lwp_stop
	.loc 1 145 0
	jmp	.L23
.L21:
	.loc 1 147 0
	movq	-24(%rbp), %rax
	addq	$32, %rax
	movq	%rax, %rsi
	movl	$0, %edi
	call	swap_rfiles
.L23:
	.loc 1 150 0
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE2:
	.size	lwp_exitOntoSafeStack, .-lwp_exitOntoSafeStack
.globl lwp_gettid
	.type	lwp_gettid, @function
lwp_gettid:
.LFB3:
	.loc 1 153 0
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	.loc 1 156 0
	movq	tidRunning(%rip), %rax
	.loc 1 157 0
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE3:
	.size	lwp_gettid, .-lwp_gettid
.globl lwp_yield
	.type	lwp_yield, @function
lwp_yield:
.LFB4:
	.loc 1 160 0
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	.loc 1 163 0
	movq	tidRunning(%rip), %rax
	testq	%rax, %rax
	je	.L32
.L28:
	.loc 1 166 0
	movq	currentScheduler(%rip), %rax
	movq	32(%rax), %rax
	call	*%rax
	movq	%rax, -8(%rbp)
	.loc 1 167 0
	cmpq	$0, -8(%rbp)
	je	.L30
	.loc 1 168 0
	movq	tidRunning(%rip), %rax
	movq	%rax, %rdi
	call	tid2thread
	movq	-8(%rbp), %rdx
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	context_switch
	jmp	.L31
.L30:
	.loc 1 171 0
	call	lwp_stop
	jmp	.L31
.L32:
	.loc 1 164 0
	nop
.L31:
	.loc 1 175 0
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE4:
	.size	lwp_yield, .-lwp_yield
.globl lwp_start
	.type	lwp_start, @function
lwp_start:
.LFB5:
	.loc 1 178 0
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%rbx
	subq	$536, %rsp
	.loc 1 180 0
	movq	$0, rootContext(%rip)
	.loc 1 181 0
	leaq	-544(%rbp), %rbx
	.cfi_offset 3, -24
	movl	$0, %eax
	movl	$64, %edx
	movq	%rbx, %rdi
	movq	%rdx, %rcx
	rep stosq
	movb	$127, -544(%rbp)
	movb	$3, -543(%rbp)
	movb	$-128, -520(%rbp)
	movb	$31, -519(%rbp)
	movb	$-1, -516(%rbp)
	movb	$-1, -515(%rbp)
	movb	$37, -368(%rbp)
	movb	$37, -367(%rbp)
	movb	$37, -366(%rbp)
	movb	$37, -365(%rbp)
	movb	$37, -364(%rbp)
	movb	$37, -363(%rbp)
	movb	$37, -362(%rbp)
	movb	$37, -361(%rbp)
	movb	$37, -360(%rbp)
	movb	$37, -359(%rbp)
	movb	$37, -358(%rbp)
	movb	$37, -357(%rbp)
	movb	$37, -356(%rbp)
	movb	$37, -355(%rbp)
	movb	$37, -354(%rbp)
	movb	$37, -353(%rbp)
	movb	$-1, -328(%rbp)
	movb	$-1, -323(%rbp)
	movl	$rootContext+160, %edx
	leaq	-544(%rbp), %rbx
	movl	$64, %eax
	movq	%rdx, %rdi
	movq	%rbx, %rsi
	movq	%rax, %rcx
	rep movsq
	.loc 1 184 0
	movq	tidRunning(%rip), %rax
	testq	%rax, %rax
	jne	.L38
	movq	numberOfProcs(%rip), %rax
	testq	%rax, %rax
	je	.L39
.L35:
	.loc 1 187 0
	movq	currentScheduler(%rip), %rax
	movq	32(%rax), %rax
	call	*%rax
	movq	%rax, -24(%rbp)
	.loc 1 189 0
	cmpq	$0, -24(%rbp)
	je	.L37
	.loc 1 190 0
	movl	$0, %esi
	movl	$rootContext+32, %edi
	call	swap_rfiles
	.loc 1 192 0
	movq	-24(%rbp), %rax
	movq	%rax, %rsi
	movl	$rootContext, %edi
	call	context_switch
	jmp	.L37
.L38:
	.loc 1 185 0
	nop
	jmp	.L37
.L39:
	nop
.L37:
	.loc 1 196 0
	addq	$536, %rsp
	popq	%rbx
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE5:
	.size	lwp_start, .-lwp_start
.globl lwp_stop
	.type	lwp_stop, @function
lwp_stop:
.LFB6:
	.loc 1 199 0
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	.loc 1 202 0
	movq	tidRunning(%rip), %rax
	testq	%rax, %rax
	je	.L44
.L41:
	.loc 1 205 0
	movq	tidRunning(%rip), %rax
	movq	%rax, %rdi
	call	tid2thread
	movl	$rootContext, %esi
	movq	%rax, %rdi
	call	context_switch
	jmp	.L43
.L44:
	.loc 1 203 0
	nop
.L43:
	.loc 1 206 0
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
	.size	lwp_stop, .-lwp_stop
.globl lwp_set_scheduler
	.type	lwp_set_scheduler, @function
lwp_set_scheduler:
.LFB7:
	.loc 1 209 0
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movq	%rdi, -24(%rbp)
	.loc 1 214 0
	movq	-24(%rbp), %rax
	movq	(%rax), %rax
	testq	%rax, %rax
	je	.L46
	.loc 1 215 0
	movq	-24(%rbp), %rax
	movq	(%rax), %rax
	call	*%rax
.L46:
	.loc 1 217 0
	movl	$0, -12(%rbp)
	jmp	.L47
.L48:
	.loc 1 218 0
	movq	currentScheduler(%rip), %rax
	movq	32(%rax), %rax
	call	*%rax
	movq	%rax, -8(%rbp)
	.loc 1 219 0
	movq	currentScheduler(%rip), %rax
	movq	24(%rax), %rdx
	movq	-8(%rbp), %rax
	movq	%rax, %rdi
	call	*%rdx
	.loc 1 220 0
	movq	-24(%rbp), %rax
	movq	16(%rax), %rdx
	movq	-8(%rbp), %rax
	movq	%rax, %rdi
	call	*%rdx
	.loc 1 217 0
	addl	$1, -12(%rbp)
.L47:
	movl	-12(%rbp), %eax
	movslq	%eax, %rdx
	movq	numberOfProcs(%rip), %rax
	cmpq	%rax, %rdx
	jb	.L48
	.loc 1 222 0
	movq	currentScheduler(%rip), %rax
	movq	8(%rax), %rax
	testq	%rax, %rax
	je	.L49
	.loc 1 223 0
	movq	currentScheduler(%rip), %rax
	movq	8(%rax), %rax
	call	*%rax
.L49:
	.loc 1 225 0
	movq	-24(%rbp), %rax
	movq	%rax, currentScheduler(%rip)
	.loc 1 226 0
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE7:
	.size	lwp_set_scheduler, .-lwp_set_scheduler
.globl lwp_get_scheduler
	.type	lwp_get_scheduler, @function
lwp_get_scheduler:
.LFB8:
	.loc 1 229 0
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	.loc 1 230 0
	movq	currentScheduler(%rip), %rax
	.loc 1 231 0
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE8:
	.size	lwp_get_scheduler, .-lwp_get_scheduler
.globl tid2thread
	.type	tid2thread, @function
tid2thread:
.LFB9:
	.loc 1 234 0
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -24(%rbp)
	.loc 1 237 0
	cmpq	$0, -24(%rbp)
	jne	.L61
	.loc 1 238 0
	movl	$rootContext, %eax
	jmp	.L55
.L61:
	.loc 1 240 0
	nop
.L60:
	.loc 1 243 0
	movq	rootThread(%rip), %rax
	movq	%rax, -8(%rbp)
	.loc 1 244 0
	jmp	.L56
.L58:
	.loc 1 245 0
	movq	-8(%rbp), %rax
	movq	(%rax), %rax
	cmpq	-24(%rbp), %rax
	jne	.L57
	.loc 1 246 0
	movq	-8(%rbp), %rax
	jmp	.L55
.L57:
	.loc 1 249 0
	movq	-8(%rbp), %rax
	movq	672(%rax), %rax
	movq	%rax, -8(%rbp)
.L56:
	.loc 1 244 0
	cmpq	$0, -8(%rbp)
	jne	.L58
	.loc 1 253 0
	movl	$0, %eax
.L55:
	.loc 1 254 0
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE9:
	.size	tid2thread, .-tid2thread
.globl rr_next
	.type	rr_next, @function
rr_next:
.LFB10:
	.loc 1 257 0
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	.loc 1 261 0
	movq	next.3497(%rip), %rax
	testq	%rax, %rax
	jne	.L63
	.loc 1 262 0
	movq	rootOfSched(%rip), %rax
	movq	%rax, next.3497(%rip)
	.loc 1 263 0
	jmp	.L64
.L65:
	.loc 1 264 0
	movq	next.3497(%rip), %rax
	movq	696(%rax), %rax
	movq	%rax, next.3497(%rip)
.L64:
	.loc 1 263 0
	movq	next.3497(%rip), %rax
	movq	696(%rax), %rax
	testq	%rax, %rax
	jne	.L65
.L63:
	.loc 1 267 0
	movq	next.3497(%rip), %rax
	movq	%rax, -8(%rbp)
	.loc 1 268 0
	movq	numberOfProcs(%rip), %rax
	testq	%rax, %rax
	jne	.L66
	.loc 1 269 0
	movl	$0, %eax
	jmp	.L67
.L66:
	.loc 1 272 0
	movq	next.3497(%rip), %rax
	movq	688(%rax), %rax
	testq	%rax, %rax
	je	.L73
	.loc 1 273 0
	movq	next.3497(%rip), %rax
	movq	688(%rax), %rax
	movq	%rax, next.3497(%rip)
	jmp	.L69
.L71:
	.loc 1 278 0
	movq	next.3497(%rip), %rax
	movq	696(%rax), %rax
	movq	%rax, next.3497(%rip)
	jmp	.L70
.L73:
	.loc 1 277 0
	nop
.L70:
	movq	next.3497(%rip), %rax
	movq	696(%rax), %rax
	testq	%rax, %rax
	jne	.L71
.L69:
	.loc 1 281 0
	movq	-8(%rbp), %rax
.L67:
	.loc 1 282 0
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE10:
	.size	rr_next, .-rr_next
.globl rr_admit
	.type	rr_admit, @function
rr_admit:
.LFB11:
	.loc 1 285 0
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -8(%rbp)
	.loc 1 288 0
	movq	rootOfSched(%rip), %rax
	testq	%rax, %rax
	jne	.L75
	.loc 1 289 0
	movq	-8(%rbp), %rax
	movq	%rax, rootOfSched(%rip)
	jmp	.L77
.L75:
	.loc 1 292 0
	movq	rootOfSched(%rip), %rdx
	movq	-8(%rbp), %rax
	movq	%rdx, 696(%rax)
	.loc 1 293 0
	movq	rootOfSched(%rip), %rax
	movq	-8(%rbp), %rdx
	movq	%rdx, 688(%rax)
	.loc 1 294 0
	movq	-8(%rbp), %rax
	movq	%rax, rootOfSched(%rip)
.L77:
	.loc 1 296 0
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE11:
	.size	rr_admit, .-rr_admit
.globl rr_remove
	.type	rr_remove, @function
rr_remove:
.LFB12:
	.loc 1 299 0
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -24(%rbp)
	.loc 1 302 0
	movq	-24(%rbp), %rax
	movq	696(%rax), %rax
	movq	%rax, -8(%rbp)
	.loc 1 303 0
	cmpq	$0, -8(%rbp)
	je	.L79
	.loc 1 304 0
	movq	-24(%rbp), %rax
	movq	688(%rax), %rdx
	movq	-8(%rbp), %rax
	movq	%rdx, 688(%rax)
.L79:
	.loc 1 306 0
	movq	-24(%rbp), %rax
	movq	688(%rax), %rax
	movq	%rax, -8(%rbp)
	.loc 1 307 0
	cmpq	$0, -8(%rbp)
	je	.L80
	.loc 1 308 0
	movq	-24(%rbp), %rax
	movq	696(%rax), %rdx
	movq	-8(%rbp), %rax
	movq	%rdx, 696(%rax)
.L80:
	.loc 1 310 0
	movq	rootOfSched(%rip), %rax
	cmpq	%rax, -24(%rbp)
	jne	.L82
	.loc 1 311 0
	movq	-24(%rbp), %rax
	movq	696(%rax), %rax
	movq	%rax, rootOfSched(%rip)
.L82:
	.loc 1 313 0
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE12:
	.size	rr_remove, .-rr_remove
.globl context_switch
	.type	context_switch, @function
context_switch:
.LFB13:
	.loc 1 319 0
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	.loc 1 326 0
	movq	-16(%rbp), %rax
	movq	(%rax), %rax
	movq	%rax, tidRunning(%rip)
	.loc 1 346 0
	movq	-16(%rbp), %rax
	leaq	32(%rax), %rdx
	movq	-8(%rbp), %rax
	addq	$32, %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	swap_rfiles
	.loc 1 347 0
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE13:
	.size	context_switch, .-context_switch
	.local	next.3497
	.comm	next.3497,8,8
	.data
	.align 4
	.type	tid.3311, @object
	.size	tid.3311, 4
tid.3311:
	.long	1
	.text
.Letext0:
	.file 2 "/usr/lib/gcc/x86_64-redhat-linux/4.4.7/include/stddef.h"
	.file 3 "/usr/include/stdint.h"
	.file 4 "fp.h"
	.file 5 "lwp.h"
	.section	.debug_info
	.long	0x975
	.value	0x3
	.long	.Ldebug_abbrev0
	.byte	0x8
	.uleb128 0x1
	.long	.LASF101
	.byte	0x1
	.long	.LASF102
	.long	.LASF103
	.quad	.Ltext0
	.quad	.Letext0
	.long	.Ldebug_line0
	.uleb128 0x2
	.byte	0x1
	.byte	0x8
	.long	.LASF0
	.uleb128 0x2
	.byte	0x2
	.byte	0x7
	.long	.LASF1
	.uleb128 0x2
	.byte	0x4
	.byte	0x7
	.long	.LASF2
	.uleb128 0x2
	.byte	0x8
	.byte	0x7
	.long	.LASF3
	.uleb128 0x2
	.byte	0x1
	.byte	0x6
	.long	.LASF4
	.uleb128 0x2
	.byte	0x2
	.byte	0x5
	.long	.LASF5
	.uleb128 0x3
	.byte	0x4
	.byte	0x5
	.string	"int"
	.uleb128 0x2
	.byte	0x8
	.byte	0x5
	.long	.LASF6
	.uleb128 0x4
	.byte	0x8
	.uleb128 0x2
	.byte	0x1
	.byte	0x6
	.long	.LASF7
	.uleb128 0x5
	.long	.LASF10
	.byte	0x2
	.byte	0xd3
	.long	0x42
	.uleb128 0x2
	.byte	0x8
	.byte	0x7
	.long	.LASF8
	.uleb128 0x2
	.byte	0x8
	.byte	0x5
	.long	.LASF9
	.uleb128 0x5
	.long	.LASF11
	.byte	0x3
	.byte	0x31
	.long	0x2d
	.uleb128 0x5
	.long	.LASF12
	.byte	0x3
	.byte	0x32
	.long	0x34
	.uleb128 0x5
	.long	.LASF13
	.byte	0x3
	.byte	0x34
	.long	0x3b
	.uleb128 0x6
	.long	.LASF50
	.value	0x200
	.byte	0x4
	.byte	0xc
	.long	0x2fb
	.uleb128 0x7
	.long	.LASF14
	.byte	0x4
	.byte	0x10
	.long	0x92
	.sleb128 0
	.uleb128 0x7
	.long	.LASF15
	.byte	0x4
	.byte	0x11
	.long	0x92
	.sleb128 2
	.uleb128 0x7
	.long	.LASF16
	.byte	0x4
	.byte	0x12
	.long	0x9d
	.sleb128 4
	.uleb128 0x8
	.string	"fop"
	.byte	0x4
	.byte	0x13
	.long	0x92
	.sleb128 8
	.uleb128 0x7
	.long	.LASF17
	.byte	0x4
	.byte	0x14
	.long	0x87
	.sleb128 10
	.uleb128 0x8
	.string	"ftw"
	.byte	0x4
	.byte	0x15
	.long	0x87
	.sleb128 11
	.uleb128 0x8
	.string	"fsw"
	.byte	0x4
	.byte	0x16
	.long	0x92
	.sleb128 12
	.uleb128 0x8
	.string	"fcw"
	.byte	0x4
	.byte	0x17
	.long	0x92
	.sleb128 14
	.uleb128 0x7
	.long	.LASF18
	.byte	0x4
	.byte	0x18
	.long	0x9d
	.sleb128 16
	.uleb128 0x7
	.long	.LASF19
	.byte	0x4
	.byte	0x19
	.long	0x9d
	.sleb128 20
	.uleb128 0x7
	.long	.LASF20
	.byte	0x4
	.byte	0x1a
	.long	0x92
	.sleb128 24
	.uleb128 0x7
	.long	.LASF21
	.byte	0x4
	.byte	0x1b
	.long	0x92
	.sleb128 26
	.uleb128 0x7
	.long	.LASF22
	.byte	0x4
	.byte	0x1c
	.long	0x9d
	.sleb128 28
	.uleb128 0x7
	.long	.LASF23
	.byte	0x4
	.byte	0x1e
	.long	0x2fb
	.sleb128 32
	.uleb128 0x8
	.string	"st0"
	.byte	0x4
	.byte	0x1e
	.long	0x30b
	.sleb128 38
	.uleb128 0x7
	.long	.LASF24
	.byte	0x4
	.byte	0x1f
	.long	0x2fb
	.sleb128 54
	.uleb128 0x8
	.string	"st1"
	.byte	0x4
	.byte	0x1f
	.long	0x30b
	.sleb128 60
	.uleb128 0x7
	.long	.LASF25
	.byte	0x4
	.byte	0x20
	.long	0x2fb
	.sleb128 76
	.uleb128 0x8
	.string	"st2"
	.byte	0x4
	.byte	0x20
	.long	0x30b
	.sleb128 82
	.uleb128 0x7
	.long	.LASF26
	.byte	0x4
	.byte	0x21
	.long	0x2fb
	.sleb128 98
	.uleb128 0x8
	.string	"st3"
	.byte	0x4
	.byte	0x21
	.long	0x30b
	.sleb128 104
	.uleb128 0x7
	.long	.LASF27
	.byte	0x4
	.byte	0x22
	.long	0x2fb
	.sleb128 120
	.uleb128 0x8
	.string	"st4"
	.byte	0x4
	.byte	0x22
	.long	0x30b
	.sleb128 126
	.uleb128 0x7
	.long	.LASF28
	.byte	0x4
	.byte	0x23
	.long	0x2fb
	.sleb128 142
	.uleb128 0x8
	.string	"st5"
	.byte	0x4
	.byte	0x23
	.long	0x30b
	.sleb128 148
	.uleb128 0x7
	.long	.LASF29
	.byte	0x4
	.byte	0x24
	.long	0x2fb
	.sleb128 164
	.uleb128 0x8
	.string	"st6"
	.byte	0x4
	.byte	0x24
	.long	0x30b
	.sleb128 170
	.uleb128 0x7
	.long	.LASF30
	.byte	0x4
	.byte	0x25
	.long	0x2fb
	.sleb128 186
	.uleb128 0x8
	.string	"st7"
	.byte	0x4
	.byte	0x25
	.long	0x30b
	.sleb128 192
	.uleb128 0x7
	.long	.LASF31
	.byte	0x4
	.byte	0x27
	.long	0x312
	.sleb128 208
	.uleb128 0x7
	.long	.LASF32
	.byte	0x4
	.byte	0x28
	.long	0x312
	.sleb128 224
	.uleb128 0x7
	.long	.LASF33
	.byte	0x4
	.byte	0x29
	.long	0x312
	.sleb128 240
	.uleb128 0x7
	.long	.LASF34
	.byte	0x4
	.byte	0x2a
	.long	0x312
	.sleb128 256
	.uleb128 0x7
	.long	.LASF35
	.byte	0x4
	.byte	0x2b
	.long	0x312
	.sleb128 272
	.uleb128 0x7
	.long	.LASF36
	.byte	0x4
	.byte	0x2c
	.long	0x312
	.sleb128 288
	.uleb128 0x7
	.long	.LASF37
	.byte	0x4
	.byte	0x2d
	.long	0x312
	.sleb128 304
	.uleb128 0x7
	.long	.LASF38
	.byte	0x4
	.byte	0x2e
	.long	0x312
	.sleb128 320
	.uleb128 0x7
	.long	.LASF39
	.byte	0x4
	.byte	0x2f
	.long	0x312
	.sleb128 336
	.uleb128 0x7
	.long	.LASF40
	.byte	0x4
	.byte	0x30
	.long	0x312
	.sleb128 352
	.uleb128 0x7
	.long	.LASF41
	.byte	0x4
	.byte	0x31
	.long	0x312
	.sleb128 368
	.uleb128 0x7
	.long	.LASF42
	.byte	0x4
	.byte	0x32
	.long	0x312
	.sleb128 384
	.uleb128 0x7
	.long	.LASF43
	.byte	0x4
	.byte	0x33
	.long	0x312
	.sleb128 400
	.uleb128 0x7
	.long	.LASF44
	.byte	0x4
	.byte	0x34
	.long	0x312
	.sleb128 416
	.uleb128 0x7
	.long	.LASF45
	.byte	0x4
	.byte	0x35
	.long	0x312
	.sleb128 432
	.uleb128 0x7
	.long	.LASF46
	.byte	0x4
	.byte	0x36
	.long	0x312
	.sleb128 448
	.uleb128 0x7
	.long	.LASF47
	.byte	0x4
	.byte	0x37
	.long	0x319
	.sleb128 464
	.byte	0x0
	.uleb128 0x9
	.long	0x87
	.long	0x30b
	.uleb128 0xa
	.long	0x42
	.byte	0x5
	.byte	0x0
	.uleb128 0x2
	.byte	0x10
	.byte	0x4
	.long	.LASF48
	.uleb128 0x2
	.byte	0x10
	.byte	0x4
	.long	.LASF49
	.uleb128 0x9
	.long	0x87
	.long	0x329
	.uleb128 0xa
	.long	0x42
	.byte	0x2f
	.byte	0x0
	.uleb128 0x6
	.long	.LASF51
	.value	0x280
	.byte	0x5
	.byte	0x10
	.long	0x40a
	.uleb128 0x8
	.string	"rax"
	.byte	0x5
	.byte	0x11
	.long	0x42
	.sleb128 0
	.uleb128 0x8
	.string	"rbx"
	.byte	0x5
	.byte	0x12
	.long	0x42
	.sleb128 8
	.uleb128 0x8
	.string	"rcx"
	.byte	0x5
	.byte	0x13
	.long	0x42
	.sleb128 16
	.uleb128 0x8
	.string	"rdx"
	.byte	0x5
	.byte	0x14
	.long	0x42
	.sleb128 24
	.uleb128 0x8
	.string	"rsi"
	.byte	0x5
	.byte	0x15
	.long	0x42
	.sleb128 32
	.uleb128 0x8
	.string	"rdi"
	.byte	0x5
	.byte	0x16
	.long	0x42
	.sleb128 40
	.uleb128 0x8
	.string	"rbp"
	.byte	0x5
	.byte	0x17
	.long	0x42
	.sleb128 48
	.uleb128 0x8
	.string	"rsp"
	.byte	0x5
	.byte	0x18
	.long	0x42
	.sleb128 56
	.uleb128 0x8
	.string	"r8"
	.byte	0x5
	.byte	0x19
	.long	0x42
	.sleb128 64
	.uleb128 0x8
	.string	"r9"
	.byte	0x5
	.byte	0x1a
	.long	0x42
	.sleb128 72
	.uleb128 0x8
	.string	"r10"
	.byte	0x5
	.byte	0x1b
	.long	0x42
	.sleb128 80
	.uleb128 0x8
	.string	"r11"
	.byte	0x5
	.byte	0x1c
	.long	0x42
	.sleb128 88
	.uleb128 0x8
	.string	"r12"
	.byte	0x5
	.byte	0x1d
	.long	0x42
	.sleb128 96
	.uleb128 0x8
	.string	"r13"
	.byte	0x5
	.byte	0x1e
	.long	0x42
	.sleb128 104
	.uleb128 0x8
	.string	"r14"
	.byte	0x5
	.byte	0x1f
	.long	0x42
	.sleb128 112
	.uleb128 0x8
	.string	"r15"
	.byte	0x5
	.byte	0x20
	.long	0x42
	.sleb128 120
	.uleb128 0x7
	.long	.LASF50
	.byte	0x5
	.byte	0x21
	.long	0xa8
	.sleb128 128
	.byte	0x0
	.uleb128 0x5
	.long	.LASF52
	.byte	0x5
	.byte	0x22
	.long	0x329
	.uleb128 0x5
	.long	.LASF53
	.byte	0x5
	.byte	0x27
	.long	0x42
	.uleb128 0x5
	.long	.LASF54
	.byte	0x5
	.byte	0x2a
	.long	0x42b
	.uleb128 0xb
	.byte	0x8
	.long	0x431
	.uleb128 0x6
	.long	.LASF55
	.value	0x2c0
	.byte	0x5
	.byte	0x2b
	.long	0x4ae
	.uleb128 0x8
	.string	"tid"
	.byte	0x5
	.byte	0x2c
	.long	0x415
	.sleb128 0
	.uleb128 0x7
	.long	.LASF56
	.byte	0x5
	.byte	0x2d
	.long	0x4ae
	.sleb128 8
	.uleb128 0x7
	.long	.LASF57
	.byte	0x5
	.byte	0x2e
	.long	0x6e
	.sleb128 16
	.uleb128 0x8
	.string	"sp"
	.byte	0x5
	.byte	0x2f
	.long	0x4ae
	.sleb128 24
	.uleb128 0x7
	.long	.LASF58
	.byte	0x5
	.byte	0x30
	.long	0x40a
	.sleb128 32
	.uleb128 0x7
	.long	.LASF59
	.byte	0x5
	.byte	0x31
	.long	0x420
	.sleb128 672
	.uleb128 0x7
	.long	.LASF60
	.byte	0x5
	.byte	0x32
	.long	0x420
	.sleb128 680
	.uleb128 0x7
	.long	.LASF61
	.byte	0x5
	.byte	0x33
	.long	0x420
	.sleb128 688
	.uleb128 0x7
	.long	.LASF62
	.byte	0x5
	.byte	0x34
	.long	0x420
	.sleb128 696
	.byte	0x0
	.uleb128 0xb
	.byte	0x8
	.long	0x42
	.uleb128 0x5
	.long	.LASF63
	.byte	0x5
	.byte	0x35
	.long	0x431
	.uleb128 0x5
	.long	.LASF64
	.byte	0x5
	.byte	0x37
	.long	0x4ca
	.uleb128 0xb
	.byte	0x8
	.long	0x4d0
	.uleb128 0xc
	.byte	0x1
	.long	0x4dc
	.uleb128 0xd
	.long	0x65
	.byte	0x0
	.uleb128 0xe
	.long	.LASF65
	.byte	0x28
	.byte	0x5
	.byte	0x3a
	.long	0x525
	.uleb128 0x7
	.long	.LASF66
	.byte	0x5
	.byte	0x3b
	.long	0x527
	.sleb128 0
	.uleb128 0x7
	.long	.LASF67
	.byte	0x5
	.byte	0x3c
	.long	0x527
	.sleb128 8
	.uleb128 0x7
	.long	.LASF68
	.byte	0x5
	.byte	0x3d
	.long	0x539
	.sleb128 16
	.uleb128 0x7
	.long	.LASF69
	.byte	0x5
	.byte	0x3e
	.long	0x539
	.sleb128 24
	.uleb128 0x7
	.long	.LASF70
	.byte	0x5
	.byte	0x3f
	.long	0x545
	.sleb128 32
	.byte	0x0
	.uleb128 0xf
	.byte	0x1
	.uleb128 0xb
	.byte	0x8
	.long	0x525
	.uleb128 0xc
	.byte	0x1
	.long	0x539
	.uleb128 0xd
	.long	0x420
	.byte	0x0
	.uleb128 0xb
	.byte	0x8
	.long	0x52d
	.uleb128 0x10
	.byte	0x1
	.long	0x420
	.uleb128 0xb
	.byte	0x8
	.long	0x53f
	.uleb128 0x5
	.long	.LASF65
	.byte	0x5
	.byte	0x40
	.long	0x556
	.uleb128 0xb
	.byte	0x8
	.long	0x4dc
	.uleb128 0x11
	.byte	0x1
	.long	.LASF85
	.byte	0x1
	.byte	0x17
	.byte	0x1
	.long	0x415
	.quad	.LFB0
	.quad	.LFE0
	.byte	0x1
	.byte	0x9c
	.long	0x609
	.uleb128 0x12
	.long	.LASF71
	.byte	0x1
	.byte	0x17
	.long	0x4bf
	.byte	0x3
	.byte	0x91
	.sleb128 -600
	.uleb128 0x13
	.string	"arg"
	.byte	0x1
	.byte	0x17
	.long	0x65
	.byte	0x3
	.byte	0x91
	.sleb128 -608
	.uleb128 0x12
	.long	.LASF57
	.byte	0x1
	.byte	0x17
	.long	0x6e
	.byte	0x3
	.byte	0x91
	.sleb128 -616
	.uleb128 0x14
	.string	"tid"
	.byte	0x1
	.byte	0x19
	.long	0x57
	.byte	0x9
	.byte	0x3
	.quad	tid.3311
	.uleb128 0x15
	.long	.LASF56
	.byte	0x1
	.byte	0x1b
	.long	0x4ae
	.byte	0x3
	.byte	0x91
	.sleb128 -72
	.uleb128 0x15
	.long	.LASF72
	.byte	0x1
	.byte	0x1d
	.long	0x4ae
	.byte	0x2
	.byte	0x91
	.sleb128 -64
	.uleb128 0x15
	.long	.LASF73
	.byte	0x1
	.byte	0x1e
	.long	0x4ae
	.byte	0x2
	.byte	0x91
	.sleb128 -56
	.uleb128 0x15
	.long	.LASF74
	.byte	0x1
	.byte	0x2a
	.long	0x609
	.byte	0x2
	.byte	0x91
	.sleb128 -48
	.uleb128 0x15
	.long	.LASF75
	.byte	0x1
	.byte	0x56
	.long	0x420
	.byte	0x2
	.byte	0x91
	.sleb128 -40
	.byte	0x0
	.uleb128 0xb
	.byte	0x8
	.long	0x4b4
	.uleb128 0x16
	.byte	0x1
	.long	.LASF78
	.byte	0x1
	.byte	0x65
	.byte	0x1
	.quad	.LFB1
	.quad	.LFE1
	.byte	0x1
	.byte	0x9c
	.long	0x659
	.uleb128 0x15
	.long	.LASF75
	.byte	0x1
	.byte	0x69
	.long	0x420
	.byte	0x2
	.byte	0x91
	.sleb128 -40
	.uleb128 0x15
	.long	.LASF76
	.byte	0x1
	.byte	0x6a
	.long	0x4ae
	.byte	0x2
	.byte	0x91
	.sleb128 -32
	.uleb128 0x15
	.long	.LASF77
	.byte	0x1
	.byte	0x6b
	.long	0x420
	.byte	0x2
	.byte	0x91
	.sleb128 -24
	.byte	0x0
	.uleb128 0x16
	.byte	0x1
	.long	.LASF79
	.byte	0x1
	.byte	0x78
	.byte	0x1
	.quad	.LFB2
	.quad	.LFE2
	.byte	0x1
	.byte	0x9c
	.long	0x6b1
	.uleb128 0x12
	.long	.LASF70
	.byte	0x1
	.byte	0x78
	.long	0x420
	.byte	0x2
	.byte	0x91
	.sleb128 -40
	.uleb128 0x12
	.long	.LASF77
	.byte	0x1
	.byte	0x78
	.long	0x420
	.byte	0x2
	.byte	0x91
	.sleb128 -48
	.uleb128 0x12
	.long	.LASF76
	.byte	0x1
	.byte	0x78
	.long	0x4ae
	.byte	0x2
	.byte	0x91
	.sleb128 -56
	.uleb128 0x15
	.long	.LASF75
	.byte	0x1
	.byte	0x7c
	.long	0x420
	.byte	0x2
	.byte	0x91
	.sleb128 -24
	.byte	0x0
	.uleb128 0x17
	.byte	0x1
	.long	.LASF83
	.byte	0x1
	.byte	0x99
	.byte	0x1
	.long	0x415
	.quad	.LFB3
	.quad	.LFE3
	.byte	0x1
	.byte	0x9c
	.uleb128 0x16
	.byte	0x1
	.long	.LASF80
	.byte	0x1
	.byte	0xa0
	.byte	0x1
	.quad	.LFB4
	.quad	.LFE4
	.byte	0x1
	.byte	0x9c
	.long	0x6fe
	.uleb128 0x15
	.long	.LASF75
	.byte	0x1
	.byte	0xa6
	.long	0x420
	.byte	0x2
	.byte	0x91
	.sleb128 -24
	.byte	0x0
	.uleb128 0x16
	.byte	0x1
	.long	.LASF81
	.byte	0x1
	.byte	0xb2
	.byte	0x1
	.quad	.LFB5
	.quad	.LFE5
	.byte	0x1
	.byte	0x9c
	.long	0x72c
	.uleb128 0x15
	.long	.LASF75
	.byte	0x1
	.byte	0xbb
	.long	0x420
	.byte	0x2
	.byte	0x91
	.sleb128 -40
	.byte	0x0
	.uleb128 0x18
	.byte	0x1
	.long	.LASF104
	.byte	0x1
	.byte	0xc7
	.byte	0x1
	.quad	.LFB6
	.quad	.LFE6
	.byte	0x1
	.byte	0x9c
	.uleb128 0x16
	.byte	0x1
	.long	.LASF82
	.byte	0x1
	.byte	0xd1
	.byte	0x1
	.quad	.LFB7
	.quad	.LFE7
	.byte	0x1
	.byte	0x9c
	.long	0x78f
	.uleb128 0x13
	.string	"fun"
	.byte	0x1
	.byte	0xd1
	.long	0x54b
	.byte	0x2
	.byte	0x91
	.sleb128 -40
	.uleb128 0x14
	.string	"i"
	.byte	0x1
	.byte	0xd4
	.long	0x57
	.byte	0x2
	.byte	0x91
	.sleb128 -28
	.uleb128 0x15
	.long	.LASF75
	.byte	0x1
	.byte	0xd5
	.long	0x420
	.byte	0x2
	.byte	0x91
	.sleb128 -24
	.byte	0x0
	.uleb128 0x17
	.byte	0x1
	.long	.LASF84
	.byte	0x1
	.byte	0xe5
	.byte	0x1
	.long	0x54b
	.quad	.LFB8
	.quad	.LFE8
	.byte	0x1
	.byte	0x9c
	.uleb128 0x11
	.byte	0x1
	.long	.LASF86
	.byte	0x1
	.byte	0xea
	.byte	0x1
	.long	0x420
	.quad	.LFB9
	.quad	.LFE9
	.byte	0x1
	.byte	0x9c
	.long	0x7ee
	.uleb128 0x13
	.string	"tid"
	.byte	0x1
	.byte	0xea
	.long	0x415
	.byte	0x2
	.byte	0x91
	.sleb128 -40
	.uleb128 0x15
	.long	.LASF75
	.byte	0x1
	.byte	0xf3
	.long	0x420
	.byte	0x2
	.byte	0x91
	.sleb128 -24
	.byte	0x0
	.uleb128 0x19
	.byte	0x1
	.long	.LASF87
	.byte	0x1
	.value	0x101
	.long	0x420
	.quad	.LFB10
	.quad	.LFE10
	.byte	0x1
	.byte	0x9c
	.long	0x837
	.uleb128 0x1a
	.long	.LASF70
	.byte	0x1
	.value	0x104
	.long	0x420
	.byte	0x9
	.byte	0x3
	.quad	next.3497
	.uleb128 0x1a
	.long	.LASF75
	.byte	0x1
	.value	0x10b
	.long	0x420
	.byte	0x2
	.byte	0x91
	.sleb128 -24
	.byte	0x0
	.uleb128 0x1b
	.byte	0x1
	.long	.LASF88
	.byte	0x1
	.value	0x11d
	.byte	0x1
	.quad	.LFB11
	.quad	.LFE11
	.byte	0x1
	.byte	0x9c
	.long	0x867
	.uleb128 0x1c
	.string	"new"
	.byte	0x1
	.value	0x11d
	.long	0x420
	.byte	0x2
	.byte	0x91
	.sleb128 -24
	.byte	0x0
	.uleb128 0x1b
	.byte	0x1
	.long	.LASF89
	.byte	0x1
	.value	0x12b
	.byte	0x1
	.quad	.LFB12
	.quad	.LFE12
	.byte	0x1
	.byte	0x9c
	.long	0x8a6
	.uleb128 0x1d
	.long	.LASF90
	.byte	0x1
	.value	0x12b
	.long	0x420
	.byte	0x2
	.byte	0x91
	.sleb128 -40
	.uleb128 0x1a
	.long	.LASF75
	.byte	0x1
	.value	0x12e
	.long	0x420
	.byte	0x2
	.byte	0x91
	.sleb128 -24
	.byte	0x0
	.uleb128 0x1b
	.byte	0x1
	.long	.LASF91
	.byte	0x1
	.value	0x13f
	.byte	0x1
	.quad	.LFB13
	.quad	.LFE13
	.byte	0x1
	.byte	0x9c
	.long	0x8e5
	.uleb128 0x1d
	.long	.LASF92
	.byte	0x1
	.value	0x13f
	.long	0x609
	.byte	0x2
	.byte	0x91
	.sleb128 -24
	.uleb128 0x1d
	.long	.LASF93
	.byte	0x1
	.value	0x13f
	.long	0x609
	.byte	0x2
	.byte	0x91
	.sleb128 -32
	.byte	0x0
	.uleb128 0x15
	.long	.LASF94
	.byte	0x1
	.byte	0xd
	.long	0x4b4
	.byte	0x9
	.byte	0x3
	.quad	rootContext
	.uleb128 0x15
	.long	.LASF95
	.byte	0x1
	.byte	0xe
	.long	0x415
	.byte	0x9
	.byte	0x3
	.quad	tidRunning
	.uleb128 0x15
	.long	.LASF96
	.byte	0x1
	.byte	0xf
	.long	0x420
	.byte	0x9
	.byte	0x3
	.quad	rootThread
	.uleb128 0x15
	.long	.LASF97
	.byte	0x1
	.byte	0x10
	.long	0x4dc
	.byte	0x9
	.byte	0x3
	.quad	RoundRobin
	.uleb128 0x15
	.long	.LASF98
	.byte	0x1
	.byte	0x11
	.long	0x54b
	.byte	0x9
	.byte	0x3
	.quad	currentScheduler
	.uleb128 0x15
	.long	.LASF99
	.byte	0x1
	.byte	0x12
	.long	0x420
	.byte	0x9
	.byte	0x3
	.quad	rootOfSched
	.uleb128 0x15
	.long	.LASF100
	.byte	0x1
	.byte	0x13
	.long	0x42
	.byte	0x9
	.byte	0x3
	.quad	numberOfProcs
	.byte	0x0
	.section	.debug_abbrev
	.uleb128 0x1
	.uleb128 0x11
	.byte	0x1
	.uleb128 0x25
	.uleb128 0xe
	.uleb128 0x13
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x1b
	.uleb128 0xe
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x10
	.uleb128 0x6
	.byte	0x0
	.byte	0x0
	.uleb128 0x2
	.uleb128 0x24
	.byte	0x0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0xe
	.byte	0x0
	.byte	0x0
	.uleb128 0x3
	.uleb128 0x24
	.byte	0x0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0x8
	.byte	0x0
	.byte	0x0
	.uleb128 0x4
	.uleb128 0xf
	.byte	0x0
	.uleb128 0xb
	.uleb128 0xb
	.byte	0x0
	.byte	0x0
	.uleb128 0x5
	.uleb128 0x16
	.byte	0x0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x6
	.uleb128 0x13
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0xb
	.uleb128 0x5
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x7
	.uleb128 0xd
	.byte	0x0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0xd
	.byte	0x0
	.byte	0x0
	.uleb128 0x8
	.uleb128 0xd
	.byte	0x0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0xd
	.byte	0x0
	.byte	0x0
	.uleb128 0x9
	.uleb128 0x1
	.byte	0x1
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x1
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0xa
	.uleb128 0x21
	.byte	0x0
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2f
	.uleb128 0xb
	.byte	0x0
	.byte	0x0
	.uleb128 0xb
	.uleb128 0xf
	.byte	0x0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0xc
	.uleb128 0x15
	.byte	0x1
	.uleb128 0x27
	.uleb128 0xc
	.uleb128 0x1
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0xd
	.uleb128 0x5
	.byte	0x0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0xe
	.uleb128 0x13
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0xf
	.uleb128 0x15
	.byte	0x0
	.uleb128 0x27
	.uleb128 0xc
	.byte	0x0
	.byte	0x0
	.uleb128 0x10
	.uleb128 0x15
	.byte	0x0
	.uleb128 0x27
	.uleb128 0xc
	.uleb128 0x49
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x11
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0xc
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x40
	.uleb128 0xa
	.uleb128 0x1
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x12
	.uleb128 0x5
	.byte	0x0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0xa
	.byte	0x0
	.byte	0x0
	.uleb128 0x13
	.uleb128 0x5
	.byte	0x0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0xa
	.byte	0x0
	.byte	0x0
	.uleb128 0x14
	.uleb128 0x34
	.byte	0x0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0xa
	.byte	0x0
	.byte	0x0
	.uleb128 0x15
	.uleb128 0x34
	.byte	0x0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0xa
	.byte	0x0
	.byte	0x0
	.uleb128 0x16
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0xc
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x40
	.uleb128 0xa
	.uleb128 0x1
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x17
	.uleb128 0x2e
	.byte	0x0
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0xc
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x40
	.uleb128 0xa
	.byte	0x0
	.byte	0x0
	.uleb128 0x18
	.uleb128 0x2e
	.byte	0x0
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0xc
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x40
	.uleb128 0xa
	.byte	0x0
	.byte	0x0
	.uleb128 0x19
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x40
	.uleb128 0xa
	.uleb128 0x1
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x1a
	.uleb128 0x34
	.byte	0x0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0xa
	.byte	0x0
	.byte	0x0
	.uleb128 0x1b
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x27
	.uleb128 0xc
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x40
	.uleb128 0xa
	.uleb128 0x1
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x1c
	.uleb128 0x5
	.byte	0x0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0xa
	.byte	0x0
	.byte	0x0
	.uleb128 0x1d
	.uleb128 0x5
	.byte	0x0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0xa
	.byte	0x0
	.byte	0x0
	.byte	0x0
	.section	.debug_pubnames,"",@progbits
	.long	0xf1
	.value	0x2
	.long	.Ldebug_info0
	.long	0x979
	.long	0x55c
	.string	"lwp_create"
	.long	0x60f
	.string	"lwp_exit"
	.long	0x659
	.string	"lwp_exitOntoSafeStack"
	.long	0x6b1
	.string	"lwp_gettid"
	.long	0x6d0
	.string	"lwp_yield"
	.long	0x6fe
	.string	"lwp_start"
	.long	0x72c
	.string	"lwp_stop"
	.long	0x747
	.string	"lwp_set_scheduler"
	.long	0x78f
	.string	"lwp_get_scheduler"
	.long	0x7ae
	.string	"tid2thread"
	.long	0x7ee
	.string	"rr_next"
	.long	0x837
	.string	"rr_admit"
	.long	0x867
	.string	"rr_remove"
	.long	0x8a6
	.string	"context_switch"
	.long	0x0
	.section	.debug_pubtypes,"",@progbits
	.long	0xbc
	.value	0x2
	.long	.Ldebug_info0
	.long	0x979
	.long	0x6e
	.string	"size_t"
	.long	0x87
	.string	"uint8_t"
	.long	0x92
	.string	"uint16_t"
	.long	0x9d
	.string	"uint32_t"
	.long	0xa8
	.string	"fxsave"
	.long	0x329
	.string	"registers"
	.long	0x40a
	.string	"rfile"
	.long	0x415
	.string	"tid_t"
	.long	0x420
	.string	"thread"
	.long	0x431
	.string	"threadinfo_st"
	.long	0x4b4
	.string	"context"
	.long	0x4bf
	.string	"lwpfun"
	.long	0x4dc
	.string	"scheduler"
	.long	0x54b
	.string	"scheduler"
	.long	0x0
	.section	.debug_aranges,"",@progbits
	.long	0x2c
	.value	0x2
	.long	.Ldebug_info0
	.byte	0x8
	.byte	0x0
	.value	0x0
	.value	0x0
	.quad	.Ltext0
	.quad	.Letext0-.Ltext0
	.quad	0x0
	.quad	0x0
	.section	.debug_str,"MS",@progbits,1
.LASF53:
	.string	"tid_t"
.LASF57:
	.string	"stacksize"
.LASF96:
	.string	"rootThread"
.LASF69:
	.string	"remove"
.LASF81:
	.string	"lwp_start"
.LASF79:
	.string	"lwp_exitOntoSafeStack"
.LASF88:
	.string	"rr_admit"
.LASF98:
	.string	"currentScheduler"
.LASF62:
	.string	"sched_two"
.LASF2:
	.string	"unsigned int"
.LASF103:
	.string	"//home/jdistaso/453os/Asgn2"
.LASF70:
	.string	"next"
.LASF50:
	.string	"fxsave"
.LASF102:
	.string	"./lwp.c"
.LASF72:
	.string	"stackPointer"
.LASF92:
	.string	"sourceContext"
.LASF19:
	.string	"mxcsr"
.LASF31:
	.string	"xmm0"
.LASF32:
	.string	"xmm1"
.LASF33:
	.string	"xmm2"
.LASF34:
	.string	"xmm3"
.LASF35:
	.string	"xmm4"
.LASF36:
	.string	"xmm5"
.LASF37:
	.string	"xmm6"
.LASF38:
	.string	"xmm7"
.LASF39:
	.string	"xmm8"
.LASF40:
	.string	"xmm9"
.LASF68:
	.string	"admit"
.LASF13:
	.string	"uint32_t"
.LASF59:
	.string	"lib_one"
.LASF8:
	.string	"long long unsigned int"
.LASF64:
	.string	"lwpfun"
.LASF14:
	.string	"pad1"
.LASF17:
	.string	"pad2"
.LASF20:
	.string	"pad3"
.LASF47:
	.string	"pad4"
.LASF97:
	.string	"RoundRobin"
.LASF67:
	.string	"shutdown"
.LASF99:
	.string	"rootOfSched"
.LASF86:
	.string	"tid2thread"
.LASF85:
	.string	"lwp_create"
.LASF10:
	.string	"size_t"
.LASF77:
	.string	"ptrToThreadToContextToBeFreed"
.LASF71:
	.string	"func"
.LASF15:
	.string	"cs_ip_bits"
.LASF49:
	.string	"__float128"
.LASF100:
	.string	"numberOfProcs"
.LASF65:
	.string	"scheduler"
.LASF41:
	.string	"xmm10"
.LASF42:
	.string	"xmm11"
.LASF43:
	.string	"xmm12"
.LASF44:
	.string	"xmm13"
.LASF45:
	.string	"xmm14"
.LASF46:
	.string	"xmm15"
.LASF7:
	.string	"char"
.LASF55:
	.string	"threadinfo_st"
.LASF87:
	.string	"rr_next"
.LASF11:
	.string	"uint8_t"
.LASF90:
	.string	"victim"
.LASF104:
	.string	"lwp_stop"
.LASF18:
	.string	"mxcsr_mask"
.LASF61:
	.string	"sched_one"
.LASF23:
	.string	"padst0"
.LASF24:
	.string	"padst1"
.LASF25:
	.string	"padst2"
.LASF26:
	.string	"padst3"
.LASF27:
	.string	"padst4"
.LASF28:
	.string	"padst5"
.LASF29:
	.string	"padst6"
.LASF30:
	.string	"padst7"
.LASF101:
	.string	"GNU C 4.4.7 20120313 (Red Hat 4.4.7-16)"
.LASF9:
	.string	"long long int"
.LASF93:
	.string	"destinationContext"
.LASF63:
	.string	"context"
.LASF94:
	.string	"rootContext"
.LASF78:
	.string	"lwp_exit"
.LASF48:
	.string	"long double"
.LASF12:
	.string	"uint16_t"
.LASF89:
	.string	"rr_remove"
.LASF73:
	.string	"retPtr"
.LASF5:
	.string	"short int"
.LASF6:
	.string	"long int"
.LASF74:
	.string	"newContext"
.LASF75:
	.string	"temp"
.LASF91:
	.string	"context_switch"
.LASF21:
	.string	"fpu_dp_bits_high"
.LASF56:
	.string	"stack"
.LASF83:
	.string	"lwp_gettid"
.LASF66:
	.string	"init"
.LASF80:
	.string	"lwp_yield"
.LASF3:
	.string	"long unsigned int"
.LASF52:
	.string	"rfile"
.LASF54:
	.string	"thread"
.LASF60:
	.string	"lib_two"
.LASF76:
	.string	"ptrToStackToBeFreed"
.LASF95:
	.string	"tidRunning"
.LASF0:
	.string	"unsigned char"
.LASF58:
	.string	"state"
.LASF82:
	.string	"lwp_set_scheduler"
.LASF51:
	.string	"registers"
.LASF4:
	.string	"signed char"
.LASF1:
	.string	"short unsigned int"
.LASF22:
	.string	"fpu_dp_bits_low"
.LASF84:
	.string	"lwp_get_scheduler"
.LASF16:
	.string	"fpu_ip_bits"
	.ident	"GCC: (GNU) 4.4.7 20120313 (Red Hat 4.4.7-16)"
	.section	.note.GNU-stack,"",@progbits
