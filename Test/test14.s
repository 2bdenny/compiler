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
# DEC arr 40

	addi $sp, $sp, -160
# mi := #0

addi $sp, $sp, -4
	li $v1, 0

	sw $v1, -160($fp)
# LABEL L_0 :

L_0:
# LABEL L_1 :

L_1:
# IF mi < #10 GOTO L_2

	lw $v1, -160($fp)
	li $t0, 10
	blt $v1, $t0, L_2

# GOTO L_5

	j L_5

# LABEL L_2 :

L_2:
# v_v0 := mi * #4

addi $sp, $sp, -4
	lw $t0, -160($fp)
	li $t1, 4

	mul $t0, $t0, $t1
	sw $t0, -164($fp)
# v_v1 := #10 - mi

addi $sp, $sp, -4
	li $t0, 10
	lw $t1, -160($fp)
	sub $t0, $t0, $t1
	sw $t0, -168($fp)
# v_v2 := v_v1 * #2

addi $sp, $sp, -4
	lw $t0, -168($fp)
	li $t1, 2

	mul $t0, $t0, $t1
	sw $t0, -172($fp)
# v_v3 := &arr

addi $sp, $sp, -4
	addi $v1, $fp, 0
	sw $v1, -176($fp)
# v_v3 := v_v3 + v_v0

	lw $t0, -176($fp)
	lw $t1, -164($fp)
	add $t0, $t0, $t1
	sw $t0, -176($fp)
# *v_v3 := v_v2

	lw $t0, -172($fp)
	lw $v1, -176($fp)
	sw $t0, 0($v1)
# LABEL L_3 :

L_3:
# v_v4 := mi + #1

addi $sp, $sp, -4
	lw $t0, -160($fp)
	addi $t0, $t0, 1

	sw $t0, -180($fp)
# mi := v_v4

	lw $v1, -180($fp)
	sw $v1, -160($fp)
# LABEL L_4 :

L_4:
# GOTO L_1

	j L_1

# LABEL L_5 :

L_5:
# v_v5 := #2 * #4

addi $sp, $sp, -4
	li $t0, 2
	li $t1, 4

	mul $t0, $t0, $t1
	sw $t0, -184($fp)
# v_v6 := &arr

addi $sp, $sp, -4
	addi $v1, $fp, 0
	sw $v1, -188($fp)
# v_v6 := v_v6 + v_v5

	lw $t0, -188($fp)
	lw $t1, -184($fp)
	add $t0, $t0, $t1
	sw $t0, -188($fp)
# *v_v6 := #5

	li $t0, 5

	lw $v1, -188($fp)
	sw $t0, 0($v1)
# LABEL L_6 :

L_6:
# v_v7 := #7 * #4

addi $sp, $sp, -4
	li $t0, 7
	li $t1, 4

	mul $t0, $t0, $t1
	sw $t0, -192($fp)
# v_v8 := &arr

addi $sp, $sp, -4
	addi $v1, $fp, 0
	sw $v1, -196($fp)
# v_v8 := v_v8 + v_v7

	lw $t0, -196($fp)
	lw $t1, -192($fp)
	add $t0, $t0, $t1
	sw $t0, -196($fp)
# *v_v8 := #16

	li $t0, 16

	lw $v1, -196($fp)
	sw $t0, 0($v1)
# LABEL L_7 :

L_7:
# v_v9 := #9 * #4

addi $sp, $sp, -4
	li $t0, 9
	li $t1, 4

	mul $t0, $t0, $t1
	sw $t0, -200($fp)
# v_v10 := &arr

addi $sp, $sp, -4
	addi $v1, $fp, 0
	sw $v1, -204($fp)
# v_v10 := v_v10 + v_v9

	lw $t0, -204($fp)
	lw $t1, -200($fp)
	add $t0, $t0, $t1
	sw $t0, -204($fp)
# *v_v10 := #15

	li $t0, 15

	lw $v1, -204($fp)
	sw $t0, 0($v1)
# LABEL L_8 :

L_8:
# i := #1

addi $sp, $sp, -4
	li $v1, 1

	sw $v1, -208($fp)
# LABEL L_9 :

L_9:
# LABEL L_10 :

L_10:
# IF i < #10 GOTO L_11

	lw $v1, -208($fp)
	li $t0, 10
	blt $v1, $t0, L_11

# GOTO L_24

	j L_24

# LABEL L_11 :

L_11:
# v_v11 := i * #4

addi $sp, $sp, -4
	lw $t0, -208($fp)
	li $t1, 4

	mul $t0, $t0, $t1
	sw $t0, -212($fp)
# v_v12 := &arr

addi $sp, $sp, -4
	addi $v1, $fp, 0
	sw $v1, -216($fp)
# v_v12 := v_v12 + v_v11

	lw $t0, -216($fp)
	lw $t1, -212($fp)
	add $t0, $t0, $t1
	sw $t0, -216($fp)
# temp := *v_v12

addi $sp, $sp, -4
	lw $v1, -216($fp)
	lw $v1, 0($v1)
	sw $v1, -220($fp)
# LABEL L_12 :

L_12:
# v_v13 := i - #1

addi $sp, $sp, -4
	lw $t0, -208($fp)
	li $t1, 1

	sub $t0, $t0, $t1
	sw $t0, -224($fp)
# j := v_v13

addi $sp, $sp, -4
	lw $v1, -224($fp)
	sw $v1, -228($fp)
# LABEL L_13 :

L_13:
# LABEL L_14 :

L_14:
# IF j >= #0 GOTO L_15

	lw $v1, -228($fp)
	li $t0, 0
	bgt $v1, $t0, L_15

# GOTO L_19

	j L_19

# LABEL L_15 :

L_15:
# v_v14 := j * #4

addi $sp, $sp, -4
	lw $t0, -228($fp)
	li $t1, 4

	mul $t0, $t0, $t1
	sw $t0, -232($fp)
# v_v15 := &arr

addi $sp, $sp, -4
	addi $v1, $fp, 0
	sw $v1, -236($fp)
# v_v15 := v_v15 + v_v14

	lw $t0, -236($fp)
	lw $t1, -232($fp)
	add $t0, $t0, $t1
	sw $t0, -236($fp)
# IF *v_v15 > temp GOTO L_16

	lw $v1, -236($fp)
	lw $v1, 0($v1)
	lw $t0, -220($fp)
	bgt $v1, $t0, L_16

# GOTO L_19

	j L_19

# v_v16 := j && *v_v15

# IF v_v16 != #0 GOTO L_16

addi $sp, $sp, -4
	lw $v1, -240($fp)
	li $t0, 0
	bne $v1, $t0, L_16

# GOTO L_19

	j L_19

# LABEL L_16 :

L_16:
# v_v17 := j + #1

addi $sp, $sp, -4
	lw $t0, -228($fp)
	addi $t0, $t0, 1

	sw $t0, -244($fp)
# v_v18 := v_v17 * #4

addi $sp, $sp, -4
	lw $t0, -244($fp)
	li $t1, 4

	mul $t0, $t0, $t1
	sw $t0, -248($fp)
# v_v19 := j * #4

addi $sp, $sp, -4
	lw $t0, -228($fp)
	li $t1, 4

	mul $t0, $t0, $t1
	sw $t0, -252($fp)
# v_v20 := &arr

addi $sp, $sp, -4
	addi $v1, $fp, 0
	sw $v1, -256($fp)
# v_v20 := v_v20 + v_v18

	lw $t0, -256($fp)
	lw $t1, -248($fp)
	add $t0, $t0, $t1
	sw $t0, -256($fp)
# v_v21 := &arr

addi $sp, $sp, -4
	addi $v1, $fp, 0
	sw $v1, -260($fp)
# v_v21 := v_v21 + v_v19

	lw $t0, -260($fp)
	lw $t1, -252($fp)
	add $t0, $t0, $t1
	sw $t0, -260($fp)
# *v_v20 := *v_v21

	lw $t0, -260($fp)
	lw $t0, 0($t0)
	lw $v1, -256($fp)
	sw $t0, 0($v1)
# LABEL L_17 :

L_17:
# v_v22 := j - #1

addi $sp, $sp, -4
	lw $t0, -228($fp)
	li $t1, 1

	sub $t0, $t0, $t1
	sw $t0, -264($fp)
# j := v_v22

	lw $v1, -264($fp)
	sw $v1, -228($fp)
# LABEL L_18 :

L_18:
# GOTO L_14

	j L_14

# LABEL L_19 :

L_19:
# v_v23 := i - #1

addi $sp, $sp, -4
	lw $t0, -208($fp)
	li $t1, 1

	sub $t0, $t0, $t1
	sw $t0, -268($fp)
# IF j != v_v23 GOTO L_20

	lw $v1, -228($fp)
	lw $t0, -268($fp)
	bne $v1, $t0, L_20

# GOTO L_22

	j L_22

# LABEL L_20 :

L_20:
# v_v24 := j + #1

addi $sp, $sp, -4
	lw $t0, -228($fp)
	addi $t0, $t0, 1

	sw $t0, -272($fp)
# v_v25 := v_v24 * #4

addi $sp, $sp, -4
	lw $t0, -272($fp)
	li $t1, 4

	mul $t0, $t0, $t1
	sw $t0, -276($fp)
# v_v26 := &arr

addi $sp, $sp, -4
	addi $v1, $fp, 0
	sw $v1, -280($fp)
# v_v26 := v_v26 + v_v25

	lw $t0, -280($fp)
	lw $t1, -276($fp)
	add $t0, $t0, $t1
	sw $t0, -280($fp)
# *v_v26 := temp

	lw $t0, -220($fp)
	lw $v1, -280($fp)
	sw $t0, 0($v1)
# LABEL L_21 :

L_21:
# LABEL L_22 :

L_22:
# v_v27 := i + #1

addi $sp, $sp, -4
	lw $t0, -208($fp)
	addi $t0, $t0, 1

	sw $t0, -284($fp)
# i := v_v27

	lw $v1, -284($fp)
	sw $v1, -208($fp)
# LABEL L_23 :

L_23:
# GOTO L_10

	j L_10

# LABEL L_24 :

L_24:
# i := #0

	li $v1, 0

	sw $v1, -208($fp)
# LABEL L_25 :

L_25:
# LABEL L_26 :

L_26:
# IF i < #10 GOTO L_27

	lw $v1, -208($fp)
	li $t0, 10
	blt $v1, $t0, L_27

# GOTO L_30

	j L_30

# LABEL L_27 :

L_27:
# v_v28 := i * #4

addi $sp, $sp, -4
	lw $t0, -208($fp)
	li $t1, 4

	mul $t0, $t0, $t1
	sw $t0, -288($fp)
# v_v29 := &arr

addi $sp, $sp, -4
	addi $v1, $fp, 0
	sw $v1, -292($fp)
# v_v29 := v_v29 + v_v28

	lw $t0, -292($fp)
	lw $t1, -288($fp)
	add $t0, $t0, $t1
	sw $t0, -292($fp)
# ARG *v_v29

# WRITE *v_v29

	lw $v1, -292($fp)
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
# LABEL L_28 :

L_28:
# v_v30 := i + #1

addi $sp, $sp, -4
	lw $t0, -208($fp)
	addi $t0, $t0, 1

	sw $t0, -296($fp)
# i := v_v30

	lw $v1, -296($fp)
	sw $v1, -208($fp)
# LABEL L_29 :

L_29:
# GOTO L_26

	j L_26

# LABEL L_30 :

L_30:
# RETURN #0

	li $v0, 0

addi $sp, $sp, 300
	jr $ra
# LABEL L_31 :

L_31:
