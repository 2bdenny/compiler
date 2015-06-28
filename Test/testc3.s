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
# FUNCTION mod :

mod:
# PARAM mody

addi $sp, $sp, -4
	lw $v1, 4($fp)
	sw $v1, 0($fp)
# PARAM modx

addi $sp, $sp, -4
	lw $v1, 8($fp)
	sw $v1, -4($fp)
# v_v0 := modx / mody

addi $sp, $sp, -4
	lw $t0, -4($fp)
	lw $t1, 0($fp)
	div $t0, $t1
	mflo $t0
	sw $t0, -8($fp)
# v_v1 := v_v0 * mody

addi $sp, $sp, -4
	lw $t0, -8($fp)
	lw $t1, 0($fp)
	mul $t0, $t0, $t1
	sw $t0, -12($fp)
# v_v2 := modx - v_v1

addi $sp, $sp, -4
	lw $t0, -4($fp)
	lw $t1, -12($fp)
	sub $t0, $t0, $t1
	sw $t0, -16($fp)
# RETURN v_v2

	lw $v0, -16($fp)
addi $sp, $sp, 20
	jr $ra
# LABEL L_0 :

L_0:
# FUNCTION printHexDigit :

printHexDigit:
# PARAM digit6

addi $sp, $sp, -4
	lw $v1, 4($fp)
	sw $v1, 0($fp)
# IF digit6 < #10 GOTO L_1

	lw $v1, 0($fp)
	li $t0, 10
	blt $v1, $t0, L_1

# GOTO L_2

	j L_2

# LABEL L_1 :

L_1:
# ARG digit6

# WRITE digit6

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
# GOTO L_3

	j L_3

# LABEL L_2 :

L_2:
# v_v3 := #0 - digit6

addi $sp, $sp, -4
	li $t0, 0
	lw $t1, 0($fp)
	sub $t0, $t0, $t1
	sw $t0, -4($fp)
# ARG v_v3

# WRITE v_v3

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
# RETURN #0

	li $v0, 0

addi $sp, $sp, 8
	jr $ra
# LABEL L_4 :

L_4:
# FUNCTION printHex :

printHex:
# PARAM number5

addi $sp, $sp, -4
	lw $v1, 4($fp)
	sw $v1, 0($fp)
# DEC a5 12

	addi $sp, $sp, -48
# j5 := #0

addi $sp, $sp, -4
	li $v1, 0

	sw $v1, -52($fp)
# LABEL L_5 :

L_5:
# IF j5 < #3 GOTO L_6

	lw $v1, -52($fp)
	li $t0, 3
	blt $v1, $t0, L_6

# GOTO L_10

	j L_10

# LABEL L_6 :

L_6:
# v_v4 := j5 * #4

addi $sp, $sp, -4
	lw $t0, -52($fp)
	li $t1, 4

	mul $t0, $t0, $t1
	sw $t0, -56($fp)
# ARG number5

# ARG #16

# v_v5 := &a5

addi $sp, $sp, -4
	addi $v1, $fp, -4
	sw $v1, -60($fp)
# v_v5 := v_v5 + v_v4

	lw $t0, -60($fp)
	lw $t1, -56($fp)
	add $t0, $t0, $t1
	sw $t0, -60($fp)
# v_v6 := CALL mod

	sw $fp, 0($sp)
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	addi $sp, $sp, -4
	lw $v1, 0($fp)
	sw $v1, 0($sp)
	addi $sp, $sp, -4
	lw $t0, 1($fp)
	sw $t0, 0($sp)
	addi $sp, $sp, -4
	move $fp, $sp
	jal mod

	addi $sp, $sp, 4
	addi $sp, $sp, 4
	addi $sp, $sp, 4
	lw $ra, 0($sp)
	addi $sp, $sp, 4
	lw $fp, 0($sp)
addi $sp, $sp, -4
	sw $v0, -64($fp)
# *v_v5 := v_v6

	lw $t0, -64($fp)
	lw $v1, -60($fp)
	sw $t0, 0($v1)
# LABEL L_7 :

L_7:
# v_v7 := number5 / #16

addi $sp, $sp, -4
	lw $t0, 0($fp)
	li $t1, 16

	div $t0, $t1
	mflo $t0
	sw $t0, -68($fp)
# number5 := v_v7

	lw $v1, -68($fp)
	sw $v1, 0($fp)
# LABEL L_8 :

L_8:
# v_v8 := j5 + #1

addi $sp, $sp, -4
	lw $t0, -52($fp)
	addi $t0, $t0, 1

	sw $t0, -72($fp)
# j5 := v_v8

	lw $v1, -72($fp)
	sw $v1, -52($fp)
# LABEL L_9 :

L_9:
# GOTO L_5

	j L_5

# LABEL L_10 :

L_10:
# j5 := #2

	li $v1, 2

	sw $v1, -52($fp)
# LABEL L_11 :

L_11:
# LABEL L_12 :

L_12:
# IF j5 >= #0 GOTO L_13

	lw $v1, -52($fp)
	li $t0, 0
	bgt $v1, $t0, L_13

# GOTO L_16

	j L_16

# LABEL L_13 :

L_13:
# v_v9 := j5 * #4

addi $sp, $sp, -4
	lw $t0, -52($fp)
	li $t1, 4

	mul $t0, $t0, $t1
	sw $t0, -76($fp)
# v_v10 := &a5

addi $sp, $sp, -4
	addi $v1, $fp, -4
	sw $v1, -80($fp)
# v_v10 := v_v10 + v_v9

	lw $t0, -80($fp)
	lw $t1, -76($fp)
	add $t0, $t0, $t1
	sw $t0, -80($fp)
# ARG *v_v10

# v_v11 := CALL printHexDigit

	sw $fp, 0($sp)
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	addi $sp, $sp, -4
	lw $v1, -80($fp)
	sw $v1, 0($sp)
	addi $sp, $sp, -4
	move $fp, $sp
	jal printHexDigit

	addi $sp, $sp, 4
	addi $sp, $sp, 4
	lw $ra, 0($sp)
	addi $sp, $sp, 4
	lw $fp, 0($sp)
addi $sp, $sp, -4
	sw $v0, -84($fp)
# v_v12 := v_v11

addi $sp, $sp, -4
	lw $v1, -84($fp)
	sw $v1, -88($fp)
# LABEL L_14 :

L_14:
# v_v13 := j5 - #1

addi $sp, $sp, -4
	lw $t0, -52($fp)
	li $t1, 1

	sub $t0, $t0, $t1
	sw $t0, -92($fp)
# j5 := v_v13

	lw $v1, -92($fp)
	sw $v1, -52($fp)
# LABEL L_15 :

L_15:
# GOTO L_12

	j L_12

# LABEL L_16 :

L_16:
# RETURN #0

	li $v0, 0

addi $sp, $sp, 96
	jr $ra
# LABEL L_17 :

L_17:
# FUNCTION perfect_num :

perfect_num:
# i := #490

addi $sp, $sp, -4
	li $v1, 490

	sw $v1, 0($fp)
# LABEL L_18 :

L_18:
# IF i < #500 GOTO L_19

	lw $v1, 0($fp)
	li $t0, 500
	blt $v1, $t0, L_19

# GOTO L_31

	j L_31

# LABEL L_19 :

L_19:
# sum := #0

addi $sp, $sp, -4
	li $v1, 0

	sw $v1, -4($fp)
# LABEL L_20 :

L_20:
# j := #1

addi $sp, $sp, -4
	li $v1, 1

	sw $v1, -8($fp)
# LABEL L_21 :

L_21:
# LABEL L_22 :

L_22:
# v_v14 := i / #2

addi $sp, $sp, -4
	lw $t0, 0($fp)
	li $t1, 2

	div $t0, $t1
	mflo $t0
	sw $t0, -12($fp)
# IF j <= v_v14 GOTO L_23

	lw $v1, -8($fp)
	lw $t0, -12($fp)
	blt $v1, $t0, L_23

# GOTO L_27

	j L_27

# LABEL L_23 :

L_23:
# ARG i

# ARG j

# v_v15 := CALL mod

	sw $fp, 0($sp)
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	addi $sp, $sp, -4
	lw $v1, 0($fp)
	sw $v1, 0($sp)
	addi $sp, $sp, -4
	lw $t0, -8($fp)
	sw $t0, 0($sp)
	addi $sp, $sp, -4
	move $fp, $sp
	jal mod

	addi $sp, $sp, 4
	addi $sp, $sp, 4
	addi $sp, $sp, 4
	lw $ra, 0($sp)
	addi $sp, $sp, 4
	lw $fp, 0($sp)
addi $sp, $sp, -4
	sw $v0, -16($fp)
# IF v_v15 == #0 GOTO L_24

	lw $v1, -16($fp)
	li $t0, 0
	beq $v1, $t0, L_24

# GOTO L_25

	j L_25

# LABEL L_24 :

L_24:
# v_v16 := sum + j

addi $sp, $sp, -4
	lw $t0, -4($fp)
	lw $t1, -8($fp)
	add $t0, $t0, $t1
	sw $t0, -20($fp)
# sum := v_v16

	lw $v1, -20($fp)
	sw $v1, -4($fp)
# LABEL L_25 :

L_25:
# v_v17 := j + #1

addi $sp, $sp, -4
	lw $t0, -8($fp)
	addi $t0, $t0, 1

	sw $t0, -24($fp)
# j := v_v17

	lw $v1, -24($fp)
	sw $v1, -8($fp)
# LABEL L_26 :

L_26:
# GOTO L_22

	j L_22

# LABEL L_27 :

L_27:
# IF sum == i GOTO L_28

	lw $v1, -4($fp)
	lw $t0, 0($fp)
	beq $v1, $t0, L_28

# GOTO L_29

	j L_29

# LABEL L_28 :

L_28:
# ARG i

# v_v18 := CALL printHex

	sw $fp, 0($sp)
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	addi $sp, $sp, -4
	lw $v1, 0($fp)
	sw $v1, 0($sp)
	addi $sp, $sp, -4
	move $fp, $sp
	jal printHex

	addi $sp, $sp, 4
	addi $sp, $sp, 4
	lw $ra, 0($sp)
	addi $sp, $sp, 4
	lw $fp, 0($sp)
addi $sp, $sp, -4
	sw $v0, -28($fp)
# v_v19 := v_v18

addi $sp, $sp, -4
	lw $v1, -28($fp)
	sw $v1, -32($fp)
# LABEL L_29 :

L_29:
# v_v20 := i + #1

addi $sp, $sp, -4
	lw $t0, 0($fp)
	addi $t0, $t0, 1

	sw $t0, -36($fp)
# i := v_v20

	lw $v1, -36($fp)
	sw $v1, 0($fp)
# LABEL L_30 :

L_30:
# GOTO L_18

	j L_18

# LABEL L_31 :

L_31:
# RETURN #0

	li $v0, 0

addi $sp, $sp, 40
	jr $ra
# LABEL L_32 :

L_32:
# FUNCTION main :

main:
move $fp, $sp
# v_v21 := CALL perfect_num

	sw $fp, 0($sp)
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	addi $sp, $sp, -4
	move $fp, $sp
	jal perfect_num

	addi $sp, $sp, 4
	lw $ra, 0($sp)
	addi $sp, $sp, 4
	lw $fp, 0($sp)
addi $sp, $sp, -4
	sw $v0, 0($fp)
# v_v22 := v_v21

addi $sp, $sp, -4
	lw $v1, 0($fp)
	sw $v1, -4($fp)
# LABEL L_33 :

L_33:
# RETURN #0

	li $v0, 0

addi $sp, $sp, 8
	jr $ra
# LABEL L_34 :

L_34:
