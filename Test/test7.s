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
	sw $v0, 0($fp)
# a := v_v0

addi $sp, $sp, -4
	lw $v1, 0($fp)
	sw $v1, -4($fp)
# READ v_v1

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
	sw $v0, -8($fp)
# b := v_v1

addi $sp, $sp, -4
	lw $v1, -8($fp)
	sw $v1, -12($fp)
# READ v_v2

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
	sw $v0, -16($fp)
# c := v_v2

addi $sp, $sp, -4
	lw $v1, -16($fp)
	sw $v1, -20($fp)
# IF a > b GOTO L_0

	lw $v1, -4($fp)
	lw $t0, -12($fp)
	bgt $v1, $t0, L_0

# GOTO L_4

	j L_4

# LABEL L_0 :

L_0:
# v_v3 := a + b

addi $sp, $sp, -4
	lw $t0, -4($fp)
	lw $t1, -12($fp)
	add $t0, $t0, $t1
	sw $t0, -24($fp)
# a := v_v3

	lw $v1, -24($fp)
	sw $v1, -4($fp)
# LABEL L_1 :

L_1:
# v_v4 := a - b

addi $sp, $sp, -4
	lw $t0, -4($fp)
	lw $t1, -12($fp)
	sub $t0, $t0, $t1
	sw $t0, -28($fp)
# b := v_v4

	lw $v1, -28($fp)
	sw $v1, -12($fp)
# LABEL L_2 :

L_2:
# v_v5 := a - b

addi $sp, $sp, -4
	lw $t0, -4($fp)
	lw $t1, -12($fp)
	sub $t0, $t0, $t1
	sw $t0, -32($fp)
# a := v_v5

	lw $v1, -32($fp)
	sw $v1, -4($fp)
# LABEL L_3 :

L_3:
# LABEL L_4 :

L_4:
# IF c < a GOTO L_5

	lw $v1, -20($fp)
	lw $t0, -4($fp)
	blt $v1, $t0, L_5

# GOTO L_10

	j L_10

# LABEL L_5 :

L_5:
# tmp := b

addi $sp, $sp, -4
	lw $v1, -12($fp)
	sw $v1, -36($fp)
# LABEL L_6 :

L_6:
# b := a

	lw $v1, -4($fp)
	sw $v1, -12($fp)
# LABEL L_7 :

L_7:
# a := c

	lw $v1, -20($fp)
	sw $v1, -4($fp)
# LABEL L_8 :

L_8:
# c := tmp

	lw $v1, -36($fp)
	sw $v1, -20($fp)
# LABEL L_9 :

L_9:
# GOTO L_15

	j L_15

# LABEL L_10 :

L_10:
# IF c < b GOTO L_11

	lw $v1, -20($fp)
	lw $t0, -12($fp)
	blt $v1, $t0, L_11

# GOTO L_15

	j L_15

# LABEL L_11 :

L_11:
# v_v6 := b + c

addi $sp, $sp, -4
	lw $t0, -12($fp)
	lw $t1, -20($fp)
	add $t0, $t0, $t1
	sw $t0, -40($fp)
# b := v_v6

	lw $v1, -40($fp)
	sw $v1, -12($fp)
# LABEL L_12 :

L_12:
# v_v7 := b - c

addi $sp, $sp, -4
	lw $t0, -12($fp)
	lw $t1, -20($fp)
	sub $t0, $t0, $t1
	sw $t0, -44($fp)
# c := v_v7

	lw $v1, -44($fp)
	sw $v1, -20($fp)
# LABEL L_13 :

L_13:
# v_v8 := b - c

addi $sp, $sp, -4
	lw $t0, -12($fp)
	lw $t1, -20($fp)
	sub $t0, $t0, $t1
	sw $t0, -48($fp)
# b := v_v8

	lw $v1, -48($fp)
	sw $v1, -12($fp)
# LABEL L_14 :

L_14:
# LABEL L_15 :

L_15:
# ARG a

# WRITE a

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
# LABEL L_16 :

L_16:
# ARG b

# WRITE b

	lw $a0, -12($fp)
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
# LABEL L_17 :

L_17:
# ARG c

# WRITE c

	lw $a0, -20($fp)
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
# LABEL L_18 :

L_18:
# RETURN #0

	li $v0, 0

addi $sp, $sp, 52
	jr $ra
# LABEL L_19 :

L_19:
