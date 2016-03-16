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
# FUNCTION convert :

convert:
# PARAM n

addi $sp, $sp, -4
	lw $v1, 4($fp)
	sw $v1, 0($fp)
# IF n == #0 GOTO L_0

	lw $v1, 0($fp)
	li $t0, 0
	beq $v1, $t0, L_0

# GOTO L_1

	j L_1

# LABEL L_0 :

L_0:
# ARG #0

# WRITE #0

	li $a0, 0

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
# GOTO L_6

	j L_6

# LABEL L_1 :

L_1:
# IF n == #1 GOTO L_2

	lw $v1, 0($fp)
	li $t0, 1
	beq $v1, $t0, L_2

# GOTO L_3

	j L_3

# LABEL L_2 :

L_2:
# ARG #1

# WRITE #1

	li $a0, 1

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
# GOTO L_6

	j L_6

# LABEL L_3 :

L_3:
# v_v0 := n / #2

addi $sp, $sp, -4
	lw $t0, 0($fp)
	li $t1, 2

	div $t0, $t1
	mflo $t0
	sw $t0, -4($fp)
# ARG v_v0

# v_v1 := CALL convert

	sw $fp, 0($sp)
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	addi $sp, $sp, -4
	lw $v1, -4($fp)
	sw $v1, 0($sp)
	addi $sp, $sp, -4
	move $fp, $sp
	jal convert

	addi $sp, $sp, 4
	addi $sp, $sp, 4
	lw $ra, 0($sp)
	addi $sp, $sp, 4
	lw $fp, 0($sp)
addi $sp, $sp, -4
	sw $v0, -8($fp)
# v_v2 := v_v1

addi $sp, $sp, -4
	lw $v1, -8($fp)
	sw $v1, -12($fp)
# LABEL L_4 :

L_4:
# v_v3 := n / #2

addi $sp, $sp, -4
	lw $t0, 0($fp)
	li $t1, 2

	div $t0, $t1
	mflo $t0
	sw $t0, -16($fp)
# v_v4 := v_v3 * #2

addi $sp, $sp, -4
	lw $t0, -16($fp)
	li $t1, 2

	mul $t0, $t0, $t1
	sw $t0, -20($fp)
# v_v5 := n - v_v4

addi $sp, $sp, -4
	lw $t0, 0($fp)
	lw $t1, -20($fp)
	sub $t0, $t0, $t1
	sw $t0, -24($fp)
# ARG v_v5

# WRITE v_v5

	lw $a0, -24($fp)
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
# LABEL L_5 :

L_5:
# LABEL L_6 :

L_6:
# RETURN #0

	li $v0, 0

addi $sp, $sp, 28
	jr $ra
# LABEL L_7 :

L_7:
# FUNCTION main :

main:
move $fp, $sp
# READ v_v6

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
# x := v_v6

addi $sp, $sp, -4
	lw $v1, 0($fp)
	sw $v1, -4($fp)
# ARG x

# v_v7 := CALL convert

	sw $fp, 0($sp)
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	addi $sp, $sp, -4
	lw $v1, -4($fp)
	sw $v1, 0($sp)
	addi $sp, $sp, -4
	move $fp, $sp
	jal convert

	addi $sp, $sp, 4
	addi $sp, $sp, 4
	lw $ra, 0($sp)
	addi $sp, $sp, 4
	lw $fp, 0($sp)
addi $sp, $sp, -4
	sw $v0, -8($fp)
# v_v8 := v_v7

addi $sp, $sp, -4
	lw $v1, -8($fp)
	sw $v1, -12($fp)
# LABEL L_8 :

L_8:
# RETURN #0

	li $v0, 0

addi $sp, $sp, 16
	jr $ra
# LABEL L_9 :

L_9:
