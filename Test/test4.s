.data
_prompt: .asciiz "Enter an integer:"
_ret: .asciiz "\n"
.globl main
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
# FUNCTION add :

add:
# PARAM temp

addi $sp, $sp, -4
	lw $v1, 4($fp)
	sw $v1, 0($fp)
# v_v0 := temp

addi $sp, $sp, -4
	lw $v1, 0($fp)
	sw $v1, -4($fp)
# v_v1 := v_v0 + #0

addi $sp, $sp, -4
	lw $t0, -4($fp)
	addi $t0, $t0, 0

	sw $t0, -8($fp)
# v_v2 := temp

addi $sp, $sp, -4
	lw $v1, 0($fp)
	sw $v1, -12($fp)
# v_v3 := v_v2 + #4

addi $sp, $sp, -4
	lw $t0, -12($fp)
	addi $t0, $t0, 4

	sw $t0, -16($fp)
# v_v4 := *v_v1 + *v_v3

addi $sp, $sp, -4
lw $v1, -8($fp)
lw $t0, -16($fp)
	lw $v1, 0($v1)
	lw $t0, 0($t0)
	add $v1, $v1, $t0
	sw $v1, -20($fp)
# RETURN v_v4

	lw $v0, -20($fp)
addi $sp, $sp, 24
	jr $ra
# LABEL L_0 :

L_0:
# FUNCTION main :

main:
move $fp, $sp
# DEC op 8

	addi $sp, $sp, -32
# v_v5 := &op

addi $sp, $sp, -4
	addi $v1, $fp, 0
	sw $v1, -32($fp)
# v_v6 := v_v5 + #0

addi $sp, $sp, -4
	lw $t0, -32($fp)
	addi $t0, $t0, 0

	sw $t0, -36($fp)
# *v_v6 := #1

	li $t0, 1

	lw $v1, -36($fp)
	sw $t0, 0($v1)
# LABEL L_1 :

L_1:
# v_v7 := &op

addi $sp, $sp, -4
	addi $v1, $fp, 0
	sw $v1, -40($fp)
# v_v8 := v_v7 + #4

addi $sp, $sp, -4
	lw $t0, -40($fp)
	addi $t0, $t0, 4

	sw $t0, -44($fp)
# *v_v8 := #2

	li $t0, 2

	lw $v1, -44($fp)
	sw $t0, 0($v1)
# LABEL L_2 :

L_2:
# ARG &op

# v_v9 := CALL add

	sw $fp, 0($sp)
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	addi $sp, $sp, -4
	lw $v1, 0($fp)
	sw $v1, 0($sp)
	addi $sp, $sp, -4
	move $fp, $sp
	jal add

	addi $sp, $sp, 4
	addi $sp, $sp, 4
	lw $ra, 0($sp)
	addi $sp, $sp, 4
	lw $fp, 0($sp)
addi $sp, $sp, -4
	sw $v0, -48($fp)
# n := v_v9

addi $sp, $sp, -4
	lw $v1, -48($fp)
	sw $v1, -52($fp)
# LABEL L_3 :

L_3:
# ARG n

# WRITE n

	lw $a0, -52($fp)
	sw $fp, 0($sp)
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	addi $sp, $sp, -4
	move $fp, $sp
	jal write
	addi $sp, $sp, 4
	lw $ra, 0($sp)
	addi $sp, $sp, 4
	lw $fp, 0($sp)
# LABEL L_4 :

L_4:
# RETURN #0

	li $v0, 0

addi $sp, $sp, 56
	jr $ra
# LABEL L_5 :

L_5:
