.cpu cortex-m4
.arch armv7-m
.thumb

.global _start

_start:
	mov		r7, #1
	mov		r0, #32
	svc		0
