.data
_prompt: .asciiz "Enter an integer:"
_ret: .asciiz "\n"
.globl main
n: .word 0
_v0: .word 0
_v2: .word 0
_v1: .word 0
m: .word 0
result: .word 0
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
fact:
sw $ra, 0($sp)
addi $sp, $sp, -4
	addi $sp, $sp, 4
	lw $fp, 0($sp)
	lw $v1, 4($sp)
	sw $fp, 4($sp)
	sw $v1, n

	lw $v1, n
	li $t0, 1
	beq $v1, $t0, L_0

	j L_1

L_0:
	lw $v0, n

	addi $sp, $sp, 4
	jr $ra
	j L_2

L_1:
	lw $t0, n
	li $t1, 1

	sub $t0, $t0, $t1
	sw $t0, _v0
	lw $v1, _v0

	sw $v1, 0($sp)
	addi $sp, $sp, -4
	jal fact

	addi $sp, $sp, 4
	lw $ra, 0($sp)
	sw $v0, _v1
	lw $t0, n
	lw $t1, _v1

	mul $t0, $t0, $t1
	sw $t0, _v2
	lw $v0, _v2

	addi $sp, $sp, 4
	jr $ra
L_2:
main:
sw $ra, 0($sp)
addi $sp, $sp, -4
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
	sw $v0, _v3

	lw $v1, _v3

	sw $v1, m
L_4:
	lw $v1, m
	li $t0, 1
	bgt $v1, $t0, L_5

	j L_6

L_5:
	lw $v1, m

	sw $v1, 0($sp)
	addi $sp, $sp, -4
	jal fact

	addi $sp, $sp, 4
	lw $ra, 0($sp)
	sw $v0, _v4
	lw $v1, _v4

	sw $v1, result
	j L_7

L_6:
	li $v1, 1

	sw $v1, result
L_7:
	lw $v1, result

	sw $v1, 0($sp)
	addi $sp, $sp, -4
	lw $a0, result

	addi $sp, $sp, -4
	sw $ra, 0($sp)
	jal write
	lw $ra, 0($sp)
	addi $sp, $sp, 4
L_8:
	li $v0, 0

	addi $sp, $sp, 4
	jr $ra
L_9:
