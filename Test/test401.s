.data
_prompt: .asciiz "Enter an integer:"
_ret: .asciiz "\n"
.globl main
index: .word 0
_v0: .word 0
index2: .word 0
_v1: .word 0
_v2: .word 0
_v3: .word 0
_v4: .word 0
.text
read:
	li $v0, 4
	la $a0, _prompt
	syscall
	li $v0, 5
	syscall
	jr $ra
write:
	li $v0, 1
	syscall
	li $v0, 4
	la $a0, _ret
	syscall
	move $v0, $0
	jr $ra
main:
sw $ra, 0($sp)
addi $sp, $sp, -4
	li $t0, 1

	sw $t0, index
	lw $t0, index
	addi $t0, $t0, 1

	sw $t0, _v0
	lw $v1, _v0

	sw $v1, index2
	lw $t0, index2
	li $t1, 3

	mul $t0, $t0, $t1
	sw $t0, _v1
	lw $t0, _v1
	addi $t0, $t0, 4

	sw $t0, _v2
	lw $t0, _v2
	li $t1, 5

	div $t0, $t1
	mflo $t0
	sw $t0, _v3
	lw $t0, _v3
	li $t1, 2

	sub $t0, $t0, $t1
	sw $t0, _v4
	lw $v1, _v4

	sw $v1, index2
L_0:
	lw $v1, index2

	sw $v1, 0($sp)
	addi $sp, $sp, -4
	lw $a0, index2

	addi $sp, $sp, -4
	sw $ra, 0($sp)
	jal write
	lw $ra, 0($sp)
	addi $sp, $sp, 4
L_1:
	li $v0, 0

	addi $sp, $sp, 4
	jr $ra
L_2:
