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
# n := v_v0

addi $sp, $sp, -4
	lw $v1, 0($fp)
	sw $v1, -4($fp)
# LABEL L_0 :

L_0:
# IF n > #0 GOTO L_1

	lw $v1, -4($fp)
	li $t0, 0
	bgt $v1, $t0, L_1

# GOTO L_2

	j L_2

# LABEL L_1 :

L_1:
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
# GOTO L_5

	j L_5

# LABEL L_2 :

L_2:
# IF n < #0 GOTO L_3

	lw $v1, -4($fp)
	li $t0, 0
	blt $v1, $t0, L_3

# GOTO L_4

	j L_4

# LABEL L_3 :

L_3:
# ARG #-1

# WRITE #-1

	li $a0, -1

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
# GOTO L_5

	j L_5

# LABEL L_4 :

L_4:
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
# LABEL L_5 :

L_5:
# RETURN #0

	li $v0, 0

addi $sp, $sp, 8
	jr $ra
# LABEL L_6 :

L_6:
