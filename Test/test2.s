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
# FUNCTION fact :

fact:
# PARAM n

addi $sp, $sp, -4
	lw $v1, 4($fp)
	sw $v1, 0($fp)
# IF n == #1 GOTO L_0

	lw $v1, 0($fp)
	li $t0, 1
	beq $v1, $t0, L_0

# GOTO L_1

	j L_1

# LABEL L_0 :

L_0:
# RETURN n

	lw $v0, 0($fp)
addi $sp, $sp, 4
	jr $ra
# GOTO L_2

	j L_2

# LABEL L_1 :

L_1:
# v_v0 := n - #1

addi $sp, $sp, -4
	lw $t0, 0($fp)
	li $t1, 1

	sub $t0, $t0, $t1
	sw $t0, -4($fp)
# ARG v_v0

# v_v1 := CALL fact

	sw $fp, 0($sp)
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	addi $sp, $sp, -4
	lw $v1, -4($fp)
	sw $v1, 0($sp)
	addi $sp, $sp, -4
	move $fp, $sp
	jal fact

	addi $sp, $sp, 4
	addi $sp, $sp, 4
	lw $ra, 0($sp)
	addi $sp, $sp, 4
	lw $fp, 0($sp)
addi $sp, $sp, -4
	sw $v0, -8($fp)
# v_v2 := n * v_v1

addi $sp, $sp, -4
	lw $t0, 0($fp)
	lw $t1, -8($fp)
	mul $t0, $t0, $t1
	sw $t0, -12($fp)
# RETURN v_v2

	lw $v0, -12($fp)
addi $sp, $sp, 16
	jr $ra
# LABEL L_2 :

L_2:
# FUNCTION main :

main:
move $fp, $sp
# READ v_v3

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
# m := v_v3

addi $sp, $sp, -4
	lw $v1, 0($fp)
	sw $v1, -4($fp)
# LABEL L_3 :

L_3:
# IF m > #1 GOTO L_4

	lw $v1, -4($fp)
	li $t0, 1
	bgt $v1, $t0, L_4

# GOTO L_5

	j L_5

# LABEL L_4 :

L_4:
# ARG m

# v_v4 := CALL fact

	sw $fp, 0($sp)
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	addi $sp, $sp, -4
	lw $v1, -4($fp)
	sw $v1, 0($sp)
	addi $sp, $sp, -4
	move $fp, $sp
	jal fact

	addi $sp, $sp, 4
	addi $sp, $sp, 4
	lw $ra, 0($sp)
	addi $sp, $sp, 4
	lw $fp, 0($sp)
addi $sp, $sp, -4
	sw $v0, -8($fp)
# result := v_v4

addi $sp, $sp, -4
	lw $v1, -8($fp)
	sw $v1, -12($fp)
# GOTO L_6

	j L_6

# LABEL L_5 :

L_5:
# result := #1

	li $v1, 1

	sw $v1, -12($fp)
# LABEL L_6 :

L_6:
# ARG result

# WRITE result

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
# LABEL L_7 :

L_7:
# RETURN #0

	li $v0, 0

addi $sp, $sp, 16
	jr $ra
# LABEL L_8 :

L_8:
