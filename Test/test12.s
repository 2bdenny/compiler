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
# i := #2

addi $sp, $sp, -4
	li $v1, 2

	sw $v1, 0($fp)
# READ v_v0

	sw $fp, 0($sp)
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	addi $sp, $sp, -4
	move $fp, $sp
	jal read
	addi $sp, $sp, 4
	lw $ra, 0($sp)
	addi $sp, $sp, 4
	lw $fp, 0($sp)
addi $sp, $sp, -4
	sw $v0, -4($fp)
# n := v_v0

addi $sp, $sp, -4
	lw $v1, -4($fp)
	sw $v1, -8($fp)
# LABEL L_0 :

L_0:
# IF i < n GOTO L_1

	lw $v1, 0($fp)
	lw $t0, -8($fp)
	blt $v1, $t0, L_1

# GOTO L_14

	j L_14

# LABEL L_1 :

L_1:
# j := #2

addi $sp, $sp, -4
	li $v1, 2

	sw $v1, -12($fp)
# LABEL L_2 :

L_2:
# flag := #1

addi $sp, $sp, -4
	li $v1, 1

	sw $v1, -16($fp)
# LABEL L_3 :

L_3:
# LABEL L_4 :

L_4:
# LABEL L_5 :

L_5:
# IF flag == #1 GOTO @

	lw $v1, -16($fp)
	li $t0, 1
	beq $v1, $t0, @

# GOTO @

	j @

# v_v1 := i && flag

# IF j < v_v1 GOTO L_6

addi $sp, $sp, -4
	lw $v1, -12($fp)
	lw $t0, -20($fp)
	blt $v1, $t0, L_6

# GOTO L_10

	j L_10

# LABEL L_6 :

L_6:
# v_v2 := i / j

addi $sp, $sp, -4
	lw $t0, 0($fp)
	lw $t1, -12($fp)
	div $t0, $t1
	mflo $t0
	sw $t0, -24($fp)
# v_v3 := v_v2 * j

addi $sp, $sp, -4
	lw $t0, -24($fp)
	lw $t1, -12($fp)
	mul $t0, $t0, $t1
	sw $t0, -28($fp)
# v_v4 := v_v3 - i

addi $sp, $sp, -4
	lw $t0, -28($fp)
	lw $t1, 0($fp)
	sub $t0, $t0, $t1
	sw $t0, -32($fp)
# IF v_v4 == #0 GOTO L_7

	lw $v1, -32($fp)
	li $t0, 0
	beq $v1, $t0, L_7

# GOTO L_8

	j L_8

# LABEL L_7 :

L_7:
# flag := #0

	li $v1, 0

	sw $v1, -16($fp)
# LABEL L_8 :

L_8:
# v_v5 := j + #1

addi $sp, $sp, -4
	lw $t0, -12($fp)
	addi $t0, $t0, 1

	sw $t0, -36($fp)
# j := v_v5

	lw $v1, -36($fp)
	sw $v1, -12($fp)
# LABEL L_9 :

L_9:
# GOTO L_4

	j L_4

# LABEL L_10 :

L_10:
# IF flag == #1 GOTO L_11

	lw $v1, -16($fp)
	li $t0, 1
	beq $v1, $t0, L_11

# GOTO L_12

	j L_12

# LABEL L_11 :

L_11:
# ARG i

# WRITE i

	lw $a0, 0($fp)
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
# LABEL L_12 :

L_12:
# v_v6 := i + #1

addi $sp, $sp, -4
	lw $t0, 0($fp)
	addi $t0, $t0, 1

	sw $t0, -40($fp)
# i := v_v6

	lw $v1, -40($fp)
	sw $v1, 0($fp)
# LABEL L_13 :

L_13:
# GOTO L_0

	j L_0

# LABEL L_14 :

L_14:
# RETURN #0

	li $v0, 0

addi $sp, $sp, 44
	jr $ra
# LABEL L_15 :

L_15:
