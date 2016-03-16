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
# FUNCTION main :

main:
move $fp, $sp
# a := #-10

addi $sp, $sp, -4
	li $v1, -10

	sw $v1, 0($fp)
# b := #30

addi $sp, $sp, -4
	li $v1, 30

	sw $v1, -4($fp)
# c := #20

addi $sp, $sp, -4
	li $v1, 20

	sw $v1, -8($fp)
# LABEL L_0 :

L_0:
# v_v0 := a * a

addi $sp, $sp, -4
	lw $t0, 0($fp)
	lw $t1, 0($fp)
	mul $t0, $t0, $t1
	sw $t0, -12($fp)
# v_v1 := #4 * b

addi $sp, $sp, -4
	li $t0, 4
	lw $t1, -4($fp)
	mul $t0, $t0, $t1
	sw $t0, -16($fp)
# v_v2 := v_v1 * c

addi $sp, $sp, -4
	lw $t0, -16($fp)
	lw $t1, -8($fp)
	mul $t0, $t0, $t1
	sw $t0, -20($fp)
# v_v3 := v_v0 - v_v2

addi $sp, $sp, -4
	lw $t0, -12($fp)
	lw $t1, -20($fp)
	sub $t0, $t0, $t1
	sw $t0, -24($fp)
# x := v_v3

addi $sp, $sp, -4
	lw $v1, -24($fp)
	sw $v1, -28($fp)
# LABEL L_1 :

L_1:
# ARG x

# WRITE x

	lw $a0, -28($fp)
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
# LABEL L_2 :

L_2:
# v_v4 := a + c

addi $sp, $sp, -4
	lw $t0, 0($fp)
	lw $t1, -8($fp)
	add $t0, $t0, $t1
	sw $t0, -32($fp)
# a := v_v4

	lw $v1, -32($fp)
	sw $v1, 0($fp)
# LABEL L_3 :

L_3:
# v_v5 := a - c

addi $sp, $sp, -4
	lw $t0, 0($fp)
	lw $t1, -8($fp)
	sub $t0, $t0, $t1
	sw $t0, -36($fp)
# c := v_v5

	lw $v1, -36($fp)
	sw $v1, -8($fp)
# LABEL L_4 :

L_4:
# v_v6 := a - c

addi $sp, $sp, -4
	lw $t0, 0($fp)
	lw $t1, -8($fp)
	sub $t0, $t0, $t1
	sw $t0, -40($fp)
# a := v_v6

	lw $v1, -40($fp)
	sw $v1, 0($fp)
# LABEL L_5 :

L_5:
# v_v7 := a * #2

addi $sp, $sp, -4
	lw $t0, 0($fp)
	li $t1, 2

	mul $t0, $t0, $t1
	sw $t0, -44($fp)
# v_v8 := c / #3

addi $sp, $sp, -4
	lw $t0, -8($fp)
	li $t1, 3

	div $t0, $t1
	mflo $t0
	sw $t0, -48($fp)
# v_v9 := v_v7 + v_v8

addi $sp, $sp, -4
	lw $t0, -44($fp)
	lw $t1, -48($fp)
	add $t0, $t0, $t1
	sw $t0, -52($fp)
# v_v10 := a / b

addi $sp, $sp, -4
	lw $t0, 0($fp)
	lw $t1, -4($fp)
	div $t0, $t1
	mflo $t0
	sw $t0, -56($fp)
# v_v11 := v_v10 * #2

addi $sp, $sp, -4
	lw $t0, -56($fp)
	li $t1, 2

	mul $t0, $t0, $t1
	sw $t0, -60($fp)
# v_v12 := v_v9 + v_v11

addi $sp, $sp, -4
	lw $t0, -52($fp)
	lw $t1, -60($fp)
	add $t0, $t0, $t1
	sw $t0, -64($fp)
# v_v13 := x / c

addi $sp, $sp, -4
	lw $t0, -28($fp)
	lw $t1, -8($fp)
	div $t0, $t1
	mflo $t0
	sw $t0, -68($fp)
# v_v14 := v_v13 * #2

addi $sp, $sp, -4
	lw $t0, -68($fp)
	li $t1, 2

	mul $t0, $t0, $t1
	sw $t0, -72($fp)
# v_v15 := v_v12 - v_v14

addi $sp, $sp, -4
	lw $t0, -64($fp)
	lw $t1, -72($fp)
	sub $t0, $t0, $t1
	sw $t0, -76($fp)
# b := v_v15

	lw $v1, -76($fp)
	sw $v1, -4($fp)
# LABEL L_6 :

L_6:
# ARG b

# WRITE b

	lw $a0, -4($fp)
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
# LABEL L_7 :

L_7:
# RETURN #0

	li $v0, 0

addi $sp, $sp, 80
	jr $ra
# LABEL L_8 :

L_8:
