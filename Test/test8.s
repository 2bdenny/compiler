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
# n := #12

addi $sp, $sp, -4
	li $v1, 12

	sw $v1, 0($fp)
# LABEL L_0 :

L_0:
# i := #1

addi $sp, $sp, -4
	li $v1, 1

	sw $v1, -4($fp)
# LABEL L_1 :

L_1:
# LABEL L_2 :

L_2:
# IF i < n GOTO L_3

	lw $v1, -4($fp)
	lw $t0, 0($fp)
	blt $v1, $t0, L_3

# GOTO L_7

	j L_7

# LABEL L_3 :

L_3:
# v_v0 := n / i

addi $sp, $sp, -4
	lw $t0, 0($fp)
	lw $t1, -4($fp)
	div $t0, $t1
	mflo $t0
	sw $t0, -8($fp)
# v_v1 := v_v0 * i

addi $sp, $sp, -4
	lw $t0, -8($fp)
	lw $t1, -4($fp)
	mul $t0, $t0, $t1
	sw $t0, -12($fp)
# IF v_v1 == n GOTO L_4

	lw $v1, -12($fp)
	lw $t0, 0($fp)
	beq $v1, $t0, L_4

# GOTO L_5

	j L_5

# LABEL L_4 :

L_4:
# ARG i

# WRITE i

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
# LABEL L_5 :

L_5:
# v_v2 := i + #1

addi $sp, $sp, -4
	lw $t0, -4($fp)
	addi $t0, $t0, 1

	sw $t0, -16($fp)
# i := v_v2

	lw $v1, -16($fp)
	sw $v1, -4($fp)
# LABEL L_6 :

L_6:
# GOTO L_2

	j L_2

# LABEL L_7 :

L_7:
# RETURN #0

	li $v0, 0

addi $sp, $sp, 20
	jr $ra
# LABEL L_8 :

L_8:
