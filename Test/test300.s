.data
_prompt: .asciiz "Enter an integer:"
_ret: .asciiz "\n"
.globl main
v0: .word 0
v2: .word 0
m: .word 0
result: .word 0
v3: .word 0
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
fact:
	move $v1, $sp
	addi $sp, $sp, -4
	li $v1, 1
	beq $v1, $v1, L_0

	j L_1

L_0:
	move $v0, $v1
	jr $ra
	j L_2

L_1:
	lw $t0, n
	li $t1, 1

	sub $t0, $t0, $t1
	sw $t0, v0
	move $t0, $v1
	move $sp, $t0
	addi $sp, $sp, 4
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	jal fact

	addi $sp, $sp, 4
	move $v1, $v0
	lw $t0, n
	lw $t1, 1

	mul $t0, $t0, $t1
	sw $t0, v2
	move $v0, $v1
	jr $ra
L_2:
main:
	li $t0, 1

	sw $t0, m
	li $t0, 2

	sw $t0, result
	li $v1, 1

	sw $v1, m
L_3:
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	jal read
	lw $ra, 0($sp)
	addi $sp, $sp, 4
	move $v1, $v0
	lw $v1, v3

	sw $v1, m
L_4:
	li $v1, 1
	bgt $v1, $v1, L_5

	j L_6

L_5:
	move $v1, $v1
	move $sp, $v1
	addi $sp, $sp, 4
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	jal fact

	addi $sp, $sp, 4
	move $v1, $v0
	lw $v1, v4

	sw $v1, result
	j L_7

L_6:
	li $v1, 1

	sw $v1, result
L_7:
	move $v1, $v1
	move $sp, $v1
	addi $sp, $sp, 4
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	jal write
	lw $ra, 0($sp)
	addi $sp, $sp, 4
L_8:
	li $v0, 0

	jr $ra
L_9:
