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
# a := #0

addi $sp, $sp, -4
	li $v1, 0

	sw $v1, 0($fp)
# b := #1

addi $sp, $sp, -4
	li $v1, 1

	sw $v1, -4($fp)
# i := #0

addi $sp, $sp, -4
	li $v1, 0

	sw $v1, -8($fp)
# READ _v0

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
	sw $v0, -12($fp)
# n := _v0

addi $sp, $sp, -4
	lw $v1, -12($fp)
	sw $v1, -16($fp)
# LABEL L_0 :

L_0:
# LABEL L_1 :

L_1:
# IF i < n GOTO L_2

	lw $v1, -8($fp)
	lw $t0, -16($fp)
	blt $v1, $t0, L_2

# GOTO L_7

	j L_7

# LABEL L_2 :

L_2:
# _v1 := a + b

addi $sp, $sp, -4
	lw $t0, 0($fp)
	lw $t1, -4($fp)
	add $t0, $t0, $t1
	sw $t0, -20($fp)
# c := _v1

addi $sp, $sp, -4
	lw $v1, -20($fp)
	sw $v1, -24($fp)
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
# LABEL L_3 :

L_3:
# a := b

	lw $v1, -4($fp)
	sw $v1, 0($fp)
# LABEL L_4 :

L_4:
# b := c

	lw $v1, -24($fp)
	sw $v1, -4($fp)
# LABEL L_5 :

L_5:
# _v2 := i + #1

addi $sp, $sp, -4
	lw $t0, -8($fp)
	addi $t0, $t0, 1

	sw $t0, -28($fp)
# i := _v2

	lw $v1, -28($fp)
	sw $v1, -8($fp)
# LABEL L_6 :

L_6:
# GOTO L_1

	j L_1

# LABEL L_7 :

L_7:
# RETURN #0

	li $v0, 0

	addi $sp, $sp, 32
	jr $ra
# LABEL L_8 :

L_8:
