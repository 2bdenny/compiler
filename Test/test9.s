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
# DEC a 40

	addi $sp, $sp, -160
# i := #0

addi $sp, $sp, -4
	li $v1, 0

	sw $v1, -160($fp)
# sum := #0

addi $sp, $sp, -4
	li $v1, 0

	sw $v1, -164($fp)
# LABEL L_0 :

L_0:
# IF i < #10 GOTO L_1

	lw $v1, -160($fp)
	li $t0, 10
	blt $v1, $t0, L_1

# GOTO L_4

	j L_4

# LABEL L_1 :

L_1:
# v_v0 := i * #4

addi $sp, $sp, -4
	lw $t0, -160($fp)
	li $t1, 4

	mul $t0, $t0, $t1
	sw $t0, -168($fp)
# v_v1 := &a

addi $sp, $sp, -4
	addi $v1, $fp, 0
	sw $v1, -172($fp)
# v_v1 := v_v1 + v_v0

	lw $t0, -172($fp)
	lw $t1, -168($fp)
	add $t0, $t0, $t1
	sw $t0, -172($fp)
# *v_v1 := i

	lw $t0, -160($fp)
	lw $v1, -172($fp)
	sw $t0, 0($v1)
# LABEL L_2 :

L_2:
# v_v2 := i + #1

addi $sp, $sp, -4
	lw $t0, -160($fp)
	addi $t0, $t0, 1

	sw $t0, -176($fp)
# i := v_v2

	lw $v1, -176($fp)
	sw $v1, -160($fp)
# LABEL L_3 :

L_3:
# GOTO L_0

	j L_0

# LABEL L_4 :

L_4:
# i := #0

	li $v1, 0

	sw $v1, -160($fp)
# LABEL L_5 :

L_5:
# LABEL L_6 :

L_6:
# IF i < #10 GOTO L_7

	lw $v1, -160($fp)
	li $t0, 10
	blt $v1, $t0, L_7

# GOTO L_10

	j L_10

# LABEL L_7 :

L_7:
# v_v3 := i * #4

addi $sp, $sp, -4
	lw $t0, -160($fp)
	li $t1, 4

	mul $t0, $t0, $t1
	sw $t0, -180($fp)
# v_v4 := i * #4

addi $sp, $sp, -4
	lw $t0, -160($fp)
	li $t1, 4

	mul $t0, $t0, $t1
	sw $t0, -184($fp)
# v_v5 := &a

addi $sp, $sp, -4
	addi $v1, $fp, 0
	sw $v1, -188($fp)
# v_v5 := v_v5 + v_v3

	lw $t0, -188($fp)
	lw $t1, -180($fp)
	add $t0, $t0, $t1
	sw $t0, -188($fp)
# v_v6 := &a

addi $sp, $sp, -4
	addi $v1, $fp, 0
	sw $v1, -192($fp)
# v_v6 := v_v6 + v_v4

	lw $t0, -192($fp)
	lw $t1, -184($fp)
	add $t0, $t0, $t1
	sw $t0, -192($fp)
# v_v7 := *v_v5 * *v_v6

addi $sp, $sp, -4
	lw $t0, -188($fp)
	lw $t1, -192($fp)
	lw $t0, 0($t0)
	lw $t1, 0($t1)
	mul $t0, $t0, $t1
	sw $t0, -196($fp)
# v_v8 := sum + v_v7

addi $sp, $sp, -4
	lw $t0, -164($fp)
	lw $t1, -196($fp)
	add $t0, $t0, $t1
	sw $t0, -200($fp)
# sum := v_v8

	lw $v1, -200($fp)
	sw $v1, -164($fp)
# LABEL L_8 :

L_8:
# v_v9 := i + #1

addi $sp, $sp, -4
	lw $t0, -160($fp)
	addi $t0, $t0, 1

	sw $t0, -204($fp)
# i := v_v9

	lw $v1, -204($fp)
	sw $v1, -160($fp)
# LABEL L_9 :

L_9:
# GOTO L_6

	j L_6

# LABEL L_10 :

L_10:
# ARG sum

# WRITE sum

	lw $a0, -164($fp)
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
# LABEL L_11 :

L_11:
# RETURN #0

	li $v0, 0

addi $sp, $sp, 208
	jr $ra
# LABEL L_12 :

L_12:
