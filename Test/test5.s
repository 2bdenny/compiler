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
# v_v0 := #0 * #4

addi $sp, $sp, -4
	li $t0, 0
	li $t1, 4

	mul $t0, $t0, $t1
	sw $t0, -4($fp)
# v_v1 := #1 * #4

addi $sp, $sp, -4
	li $t0, 1
	li $t1, 4

	mul $t0, $t0, $t1
	sw $t0, -8($fp)
# v_v2 := &temp

addi $sp, $sp, -4
	addi $v1, $fp, 0
	sw $v1, -12($fp)
# v_v2 := v_v2 + v_v0

	lw $t0, -12($fp)
	lw $t1, -4($fp)
	add $t0, $t0, $t1
	sw $t0, -12($fp)
# v_v3 := &temp

addi $sp, $sp, -4
	addi $v1, $fp, 0
	sw $v1, -16($fp)
# v_v3 := v_v3 + v_v1

	lw $t0, -16($fp)
	lw $t1, -8($fp)
	add $t0, $t0, $t1
	sw $t0, -16($fp)
# v_v4 := *v_v2 + *v_v3

addi $sp, $sp, -4
lw $v1, -12($fp)
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
# DEC r 8

	addi $sp, $sp, -32
# i := #0

addi $sp, $sp, -4
	li $v1, 0

	sw $v1, -64($fp)
# j := #0

addi $sp, $sp, -4
	li $v1, 0

	sw $v1, -68($fp)
# LABEL L_1 :

L_1:
# IF i < #2 GOTO L_2

	lw $v1, -64($fp)
	li $t0, 2
	blt $v1, $t0, L_2

# GOTO L_12

	j L_12

# LABEL L_2 :

L_2:
# LABEL L_3 :

L_3:
# IF j < #2 GOTO L_4

	lw $v1, -68($fp)
	li $t0, 2
	blt $v1, $t0, L_4

# GOTO L_7

	j L_7

# LABEL L_4 :

L_4:
# v_v5 := j * #4

addi $sp, $sp, -4
	lw $t0, -68($fp)
	li $t1, 4

	mul $t0, $t0, $t1
	sw $t0, -72($fp)
# v_v6 := i + j

addi $sp, $sp, -4
	lw $t0, -64($fp)
	lw $t1, -68($fp)
	add $t0, $t0, $t1
	sw $t0, -76($fp)
# v_v7 := &op

addi $sp, $sp, -4
	addi $v1, $fp, 0
	sw $v1, -80($fp)
# v_v7 := v_v7 + v_v5

	lw $t0, -80($fp)
	lw $t1, -72($fp)
	add $t0, $t0, $t1
	sw $t0, -80($fp)
# *v_v7 := v_v6

	lw $t0, -76($fp)
	lw $v1, -80($fp)
	sw $t0, 0($v1)
# LABEL L_5 :

L_5:
# v_v8 := j + #1

addi $sp, $sp, -4
	lw $t0, -68($fp)
	addi $t0, $t0, 1

	sw $t0, -84($fp)
# j := v_v8

	lw $v1, -84($fp)
	sw $v1, -68($fp)
# LABEL L_6 :

L_6:
# GOTO L_3

	j L_3

# LABEL L_7 :

L_7:
# v_v9 := #0 * #8

addi $sp, $sp, -4
	li $t0, 0
	li $t1, 8

	mul $t0, $t0, $t1
	sw $t0, -88($fp)
# v_v10 := i * #4

addi $sp, $sp, -4
	lw $t0, -64($fp)
	li $t1, 4

	mul $t0, $t0, $t1
	sw $t0, -92($fp)
# v_v11 := v_v10 + v_v9

addi $sp, $sp, -4
	lw $t0, -92($fp)
	lw $t1, -88($fp)
	add $t0, $t0, $t1
	sw $t0, -96($fp)
# ARG op

# v_v12 := &r

addi $sp, $sp, -4
	addi $v1, $fp, -32
	sw $v1, -100($fp)
# v_v12 := v_v12 + v_v11

	lw $t0, -100($fp)
	lw $t1, -96($fp)
	add $t0, $t0, $t1
	sw $t0, -100($fp)
# v_v13 := CALL add

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
	sw $v0, -104($fp)
# *v_v12 := v_v13

	lw $t0, -104($fp)
	lw $v1, -100($fp)
	sw $t0, 0($v1)
# LABEL L_8 :

L_8:
# v_v14 := #0 * #8

addi $sp, $sp, -4
	li $t0, 0
	li $t1, 8

	mul $t0, $t0, $t1
	sw $t0, -108($fp)
# v_v15 := i * #4

addi $sp, $sp, -4
	lw $t0, -64($fp)
	li $t1, 4

	mul $t0, $t0, $t1
	sw $t0, -112($fp)
# v_v16 := v_v15 + v_v14

addi $sp, $sp, -4
	lw $t0, -112($fp)
	lw $t1, -108($fp)
	add $t0, $t0, $t1
	sw $t0, -116($fp)
# v_v17 := &r

addi $sp, $sp, -4
	addi $v1, $fp, -32
	sw $v1, -120($fp)
# v_v17 := v_v17 + v_v16

	lw $t0, -120($fp)
	lw $t1, -116($fp)
	add $t0, $t0, $t1
	sw $t0, -120($fp)
# ARG *v_v17

# WRITE *v_v17

	lw $v1, -120($fp)
	lw $a0, 0($v1)
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
# LABEL L_9 :

L_9:
# v_v18 := i + #1

addi $sp, $sp, -4
	lw $t0, -64($fp)
	addi $t0, $t0, 1

	sw $t0, -124($fp)
# i := v_v18

	lw $v1, -124($fp)
	sw $v1, -64($fp)
# LABEL L_10 :

L_10:
# j := #0

	li $v1, 0

	sw $v1, -68($fp)
# LABEL L_11 :

L_11:
# GOTO L_1

	j L_1

# LABEL L_12 :

L_12:
# RETURN #0

	li $v0, 0

addi $sp, $sp, 128
	jr $ra
# LABEL L_13 :

L_13:
