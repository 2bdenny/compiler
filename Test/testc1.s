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
# FUNCTION max :

max:
# PARAM maxy

addi $sp, $sp, -4
	lw $v1, 4($fp)
	sw $v1, 0($fp)
# PARAM maxx

addi $sp, $sp, -4
	lw $v1, 8($fp)
	sw $v1, -4($fp)
# IF maxx > maxy GOTO L_0

	lw $v1, -4($fp)
	lw $t0, 0($fp)
	bgt $v1, $t0, L_0

# GOTO L_1

	j L_1

# LABEL L_0 :

L_0:
# RETURN maxx

	lw $v0, -4($fp)
addi $sp, $sp, 8
	jr $ra
# GOTO L_2

	j L_2

# LABEL L_1 :

L_1:
# RETURN maxy

	lw $v0, 0($fp)
addi $sp, $sp, 8
	jr $ra
# LABEL L_2 :

L_2:
# FUNCTION min :

min:
# PARAM miny

addi $sp, $sp, -4
	lw $v1, 4($fp)
	sw $v1, 0($fp)
# PARAM minx

addi $sp, $sp, -4
	lw $v1, 8($fp)
	sw $v1, -4($fp)
# IF minx < miny GOTO L_3

	lw $v1, -4($fp)
	lw $t0, 0($fp)
	blt $v1, $t0, L_3

# GOTO L_4

	j L_4

# LABEL L_3 :

L_3:
# RETURN minx

	lw $v0, -4($fp)
addi $sp, $sp, 8
	jr $ra
# GOTO L_5

	j L_5

# LABEL L_4 :

L_4:
# RETURN miny

	lw $v0, 0($fp)
addi $sp, $sp, 8
	jr $ra
# LABEL L_5 :

L_5:
# FUNCTION main :

main:
move $fp, $sp
# DEC A 40

	addi $sp, $sp, -160
# v_v0 := #0 * #4

addi $sp, $sp, -4
	li $t0, 0
	li $t1, 4

	mul $t0, $t0, $t1
	sw $t0, -160($fp)
# v_v1 := &A

addi $sp, $sp, -4
	addi $v1, $fp, 0
	sw $v1, -164($fp)
# v_v1 := v_v1 + v_v0

	lw $t0, -164($fp)
	lw $t1, -160($fp)
	add $t0, $t0, $t1
	sw $t0, -164($fp)
# *v_v1 := #2

	li $t0, 2

	lw $v1, -164($fp)
	sw $t0, 0($v1)
# LABEL L_6 :

L_6:
# v_v2 := #1 * #4

addi $sp, $sp, -4
	li $t0, 1
	li $t1, 4

	mul $t0, $t0, $t1
	sw $t0, -168($fp)
# v_v3 := &A

addi $sp, $sp, -4
	addi $v1, $fp, 0
	sw $v1, -172($fp)
# v_v3 := v_v3 + v_v2

	lw $t0, -172($fp)
	lw $t1, -168($fp)
	add $t0, $t0, $t1
	sw $t0, -172($fp)
# *v_v3 := #-1

	li $t0, -1

	lw $v1, -172($fp)
	sw $t0, 0($v1)
# LABEL L_7 :

L_7:
# v_v4 := #2 * #4

addi $sp, $sp, -4
	li $t0, 2
	li $t1, 4

	mul $t0, $t0, $t1
	sw $t0, -176($fp)
# v_v5 := &A

addi $sp, $sp, -4
	addi $v1, $fp, 0
	sw $v1, -180($fp)
# v_v5 := v_v5 + v_v4

	lw $t0, -180($fp)
	lw $t1, -176($fp)
	add $t0, $t0, $t1
	sw $t0, -180($fp)
# *v_v5 := #1

	li $t0, 1

	lw $v1, -180($fp)
	sw $t0, 0($v1)
# LABEL L_8 :

L_8:
# v_v6 := #3 * #4

addi $sp, $sp, -4
	li $t0, 3
	li $t1, 4

	mul $t0, $t0, $t1
	sw $t0, -184($fp)
# v_v7 := &A

addi $sp, $sp, -4
	addi $v1, $fp, 0
	sw $v1, -188($fp)
# v_v7 := v_v7 + v_v6

	lw $t0, -188($fp)
	lw $t1, -184($fp)
	add $t0, $t0, $t1
	sw $t0, -188($fp)
# *v_v7 := #0

	li $t0, 0

	lw $v1, -188($fp)
	sw $t0, 0($v1)
# LABEL L_9 :

L_9:
# v_v8 := #4 * #4

addi $sp, $sp, -4
	li $t0, 4
	li $t1, 4

	mul $t0, $t0, $t1
	sw $t0, -192($fp)
# v_v9 := &A

addi $sp, $sp, -4
	addi $v1, $fp, 0
	sw $v1, -196($fp)
# v_v9 := v_v9 + v_v8

	lw $t0, -196($fp)
	lw $t1, -192($fp)
	add $t0, $t0, $t1
	sw $t0, -196($fp)
# *v_v9 := #2

	li $t0, 2

	lw $v1, -196($fp)
	sw $t0, 0($v1)
# LABEL L_10 :

L_10:
# v_v10 := #5 * #4

addi $sp, $sp, -4
	li $t0, 5
	li $t1, 4

	mul $t0, $t0, $t1
	sw $t0, -200($fp)
# v_v11 := &A

addi $sp, $sp, -4
	addi $v1, $fp, 0
	sw $v1, -204($fp)
# v_v11 := v_v11 + v_v10

	lw $t0, -204($fp)
	lw $t1, -200($fp)
	add $t0, $t0, $t1
	sw $t0, -204($fp)
# *v_v11 := #-10

	li $t0, -10

	lw $v1, -204($fp)
	sw $t0, 0($v1)
# LABEL L_11 :

L_11:
# v_v12 := #6 * #4

addi $sp, $sp, -4
	li $t0, 6
	li $t1, 4

	mul $t0, $t0, $t1
	sw $t0, -208($fp)
# v_v13 := &A

addi $sp, $sp, -4
	addi $v1, $fp, 0
	sw $v1, -212($fp)
# v_v13 := v_v13 + v_v12

	lw $t0, -212($fp)
	lw $t1, -208($fp)
	add $t0, $t0, $t1
	sw $t0, -212($fp)
# *v_v13 := #4

	li $t0, 4

	lw $v1, -212($fp)
	sw $t0, 0($v1)
# LABEL L_12 :

L_12:
# v_v14 := #7 * #4

addi $sp, $sp, -4
	li $t0, 7
	li $t1, 4

	mul $t0, $t0, $t1
	sw $t0, -216($fp)
# v_v15 := &A

addi $sp, $sp, -4
	addi $v1, $fp, 0
	sw $v1, -220($fp)
# v_v15 := v_v15 + v_v14

	lw $t0, -220($fp)
	lw $t1, -216($fp)
	add $t0, $t0, $t1
	sw $t0, -220($fp)
# *v_v15 := #-2

	li $t0, -2

	lw $v1, -220($fp)
	sw $t0, 0($v1)
# LABEL L_13 :

L_13:
# v_v16 := #8 * #4

addi $sp, $sp, -4
	li $t0, 8
	li $t1, 4

	mul $t0, $t0, $t1
	sw $t0, -224($fp)
# v_v17 := &A

addi $sp, $sp, -4
	addi $v1, $fp, 0
	sw $v1, -228($fp)
# v_v17 := v_v17 + v_v16

	lw $t0, -228($fp)
	lw $t1, -224($fp)
	add $t0, $t0, $t1
	sw $t0, -228($fp)
# *v_v17 := #-1

	li $t0, -1

	lw $v1, -228($fp)
	sw $t0, 0($v1)
# LABEL L_14 :

L_14:
# v_v18 := #9 * #4

addi $sp, $sp, -4
	li $t0, 9
	li $t1, 4

	mul $t0, $t0, $t1
	sw $t0, -232($fp)
# v_v19 := &A

addi $sp, $sp, -4
	addi $v1, $fp, 0
	sw $v1, -236($fp)
# v_v19 := v_v19 + v_v18

	lw $t0, -236($fp)
	lw $t1, -232($fp)
	add $t0, $t0, $t1
	sw $t0, -236($fp)
# *v_v19 := #4

	li $t0, 4

	lw $v1, -236($fp)
	sw $t0, 0($v1)
# LABEL L_15 :

L_15:
# v_v20 := #0 * #4

addi $sp, $sp, -4
	li $t0, 0
	li $t1, 4

	mul $t0, $t0, $t1
	sw $t0, -240($fp)
# v_v21 := &A

addi $sp, $sp, -4
	addi $v1, $fp, 0
	sw $v1, -244($fp)
# v_v21 := v_v21 + v_v20

	lw $t0, -244($fp)
	lw $t1, -240($fp)
	add $t0, $t0, $t1
	sw $t0, -244($fp)
# maxP := *v_v21

addi $sp, $sp, -4
	lw $v1, -244($fp)
	lw $v1, 0($v1)
	sw $v1, -248($fp)
# LABEL L_16 :

L_16:
# v_v22 := #0 * #4

addi $sp, $sp, -4
	li $t0, 0
	li $t1, 4

	mul $t0, $t0, $t1
	sw $t0, -252($fp)
# v_v23 := &A

addi $sp, $sp, -4
	addi $v1, $fp, 0
	sw $v1, -256($fp)
# v_v23 := v_v23 + v_v22

	lw $t0, -256($fp)
	lw $t1, -252($fp)
	add $t0, $t0, $t1
	sw $t0, -256($fp)
# localMax := *v_v23

addi $sp, $sp, -4
	lw $v1, -256($fp)
	lw $v1, 0($v1)
	sw $v1, -260($fp)
# LABEL L_17 :

L_17:
# v_v24 := #0 * #4

addi $sp, $sp, -4
	li $t0, 0
	li $t1, 4

	mul $t0, $t0, $t1
	sw $t0, -264($fp)
# v_v25 := &A

addi $sp, $sp, -4
	addi $v1, $fp, 0
	sw $v1, -268($fp)
# v_v25 := v_v25 + v_v24

	lw $t0, -268($fp)
	lw $t1, -264($fp)
	add $t0, $t0, $t1
	sw $t0, -268($fp)
# localMin := *v_v25

addi $sp, $sp, -4
	lw $v1, -268($fp)
	lw $v1, 0($v1)
	sw $v1, -272($fp)
# LABEL L_18 :

L_18:
# i := #1

addi $sp, $sp, -4
	li $v1, 1

	sw $v1, -276($fp)
# LABEL L_19 :

L_19:
# LABEL L_20 :

L_20:
# IF i < #10 GOTO L_21

	lw $v1, -276($fp)
	li $t0, 10
	blt $v1, $t0, L_21

# GOTO L_26

	j L_26

# LABEL L_21 :

L_21:
# tmpLocalMax := localMax

addi $sp, $sp, -4
	lw $v1, -260($fp)
	sw $v1, -280($fp)
# v_v26 := i * #4

addi $sp, $sp, -4
	lw $t0, -276($fp)
	li $t1, 4

	mul $t0, $t0, $t1
	sw $t0, -284($fp)
# v_v27 := i * #4

addi $sp, $sp, -4
	lw $t0, -276($fp)
	li $t1, 4

	mul $t0, $t0, $t1
	sw $t0, -288($fp)
# v_v28 := &A

addi $sp, $sp, -4
	addi $v1, $fp, 0
	sw $v1, -292($fp)
# v_v28 := v_v28 + v_v27

	lw $t0, -292($fp)
	lw $t1, -288($fp)
	add $t0, $t0, $t1
	sw $t0, -292($fp)
# v_v29 := *v_v28 * localMax

addi $sp, $sp, -4
	lw $t0, -292($fp)
	lw $t1, -260($fp)
	lw $t0, 0($t0)
	mul $t0, $t0, $t1
	sw $t0, -296($fp)
# v_v30 := i * #4

addi $sp, $sp, -4
	lw $t0, -276($fp)
	li $t1, 4

	mul $t0, $t0, $t1
	sw $t0, -300($fp)
# v_v31 := &A

addi $sp, $sp, -4
	addi $v1, $fp, 0
	sw $v1, -304($fp)
# v_v31 := v_v31 + v_v30

	lw $t0, -304($fp)
	lw $t1, -300($fp)
	add $t0, $t0, $t1
	sw $t0, -304($fp)
# v_v32 := *v_v31 * localMin

addi $sp, $sp, -4
	lw $t0, -304($fp)
	lw $t1, -272($fp)
	lw $t0, 0($t0)
	mul $t0, $t0, $t1
	sw $t0, -308($fp)
# ARG v_v29

# ARG v_v32

# v_v33 := &A

addi $sp, $sp, -4
	addi $v1, $fp, 0
	sw $v1, -312($fp)
# v_v33 := v_v33 + v_v26

	lw $t0, -312($fp)
	lw $t1, -284($fp)
	add $t0, $t0, $t1
	sw $t0, -312($fp)
# ARG *v_v33

# v_v34 := CALL max

	sw $fp, 0($sp)
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	addi $sp, $sp, -4
	lw $v1, -296($fp)
	sw $v1, 0($sp)
	addi $sp, $sp, -4
	lw $t0, -308($fp)
	sw $t0, 0($sp)
	addi $sp, $sp, -4
	lw $t1, -312($fp)
	sw $t1, 0($sp)
	addi $sp, $sp, -4
	move $fp, $sp
	jal max

	addi $sp, $sp, 4
	addi $sp, $sp, 4
	addi $sp, $sp, 4
	addi $sp, $sp, 4
	lw $ra, 0($sp)
	addi $sp, $sp, 4
	lw $fp, 0($sp)
addi $sp, $sp, -4
	sw $v0, -316($fp)
# ARG v_v34

# v_v35 := CALL max

	sw $fp, 0($sp)
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	addi $sp, $sp, -4
	lw $v1, -316($fp)
	sw $v1, 0($sp)
	addi $sp, $sp, -4
	move $fp, $sp
	jal max

	addi $sp, $sp, 4
	addi $sp, $sp, 4
	lw $ra, 0($sp)
	addi $sp, $sp, 4
	lw $fp, 0($sp)
addi $sp, $sp, -4
	sw $v0, -320($fp)
# localMax := v_v35

	lw $v1, -320($fp)
	sw $v1, -260($fp)
# LABEL L_22 :

L_22:
# v_v36 := i * #4

addi $sp, $sp, -4
	lw $t0, -276($fp)
	li $t1, 4

	mul $t0, $t0, $t1
	sw $t0, -324($fp)
# v_v37 := i * #4

addi $sp, $sp, -4
	lw $t0, -276($fp)
	li $t1, 4

	mul $t0, $t0, $t1
	sw $t0, -328($fp)
# v_v38 := &A

addi $sp, $sp, -4
	addi $v1, $fp, 0
	sw $v1, -332($fp)
# v_v38 := v_v38 + v_v37

	lw $t0, -332($fp)
	lw $t1, -328($fp)
	add $t0, $t0, $t1
	sw $t0, -332($fp)
# v_v39 := *v_v38 * tmpLocalMax

addi $sp, $sp, -4
	lw $t0, -332($fp)
	lw $t1, -280($fp)
	lw $t0, 0($t0)
	mul $t0, $t0, $t1
	sw $t0, -336($fp)
# v_v40 := i * #4

addi $sp, $sp, -4
	lw $t0, -276($fp)
	li $t1, 4

	mul $t0, $t0, $t1
	sw $t0, -340($fp)
# v_v41 := &A

addi $sp, $sp, -4
	addi $v1, $fp, 0
	sw $v1, -344($fp)
# v_v41 := v_v41 + v_v40

	lw $t0, -344($fp)
	lw $t1, -340($fp)
	add $t0, $t0, $t1
	sw $t0, -344($fp)
# v_v42 := *v_v41 * localMin

addi $sp, $sp, -4
	lw $t0, -344($fp)
	lw $t1, -272($fp)
	lw $t0, 0($t0)
	mul $t0, $t0, $t1
	sw $t0, -348($fp)
# ARG v_v39

# ARG v_v42

# v_v43 := &A

addi $sp, $sp, -4
	addi $v1, $fp, 0
	sw $v1, -352($fp)
# v_v43 := v_v43 + v_v36

	lw $t0, -352($fp)
	lw $t1, -324($fp)
	add $t0, $t0, $t1
	sw $t0, -352($fp)
# ARG *v_v43

# v_v44 := CALL min

	sw $fp, 0($sp)
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	addi $sp, $sp, -4
	lw $v1, -336($fp)
	sw $v1, 0($sp)
	addi $sp, $sp, -4
	lw $t0, -348($fp)
	sw $t0, 0($sp)
	addi $sp, $sp, -4
	lw $t1, -352($fp)
	sw $t1, 0($sp)
	addi $sp, $sp, -4
	move $fp, $sp
	jal min

	addi $sp, $sp, 4
	addi $sp, $sp, 4
	addi $sp, $sp, 4
	addi $sp, $sp, 4
	lw $ra, 0($sp)
	addi $sp, $sp, 4
	lw $fp, 0($sp)
addi $sp, $sp, -4
	sw $v0, -356($fp)
# ARG v_v44

# v_v45 := CALL min

	sw $fp, 0($sp)
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	addi $sp, $sp, -4
	lw $v1, -356($fp)
	sw $v1, 0($sp)
	addi $sp, $sp, -4
	move $fp, $sp
	jal min

	addi $sp, $sp, 4
	addi $sp, $sp, 4
	lw $ra, 0($sp)
	addi $sp, $sp, 4
	lw $fp, 0($sp)
addi $sp, $sp, -4
	sw $v0, -360($fp)
# localMin := v_v45

	lw $v1, -360($fp)
	sw $v1, -272($fp)
# LABEL L_23 :

L_23:
# ARG maxP

# ARG localMax

# v_v46 := CALL max

	sw $fp, 0($sp)
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	addi $sp, $sp, -4
	lw $v1, -248($fp)
	sw $v1, 0($sp)
	addi $sp, $sp, -4
	lw $t0, -260($fp)
	sw $t0, 0($sp)
	addi $sp, $sp, -4
	move $fp, $sp
	jal max

	addi $sp, $sp, 4
	addi $sp, $sp, 4
	addi $sp, $sp, 4
	lw $ra, 0($sp)
	addi $sp, $sp, 4
	lw $fp, 0($sp)
addi $sp, $sp, -4
	sw $v0, -364($fp)
# maxP := v_v46

	lw $v1, -364($fp)
	sw $v1, -248($fp)
# LABEL L_24 :

L_24:
# v_v47 := i + #1

addi $sp, $sp, -4
	lw $t0, -276($fp)
	addi $t0, $t0, 1

	sw $t0, -368($fp)
# i := v_v47

	lw $v1, -368($fp)
	sw $v1, -276($fp)
# LABEL L_25 :

L_25:
# GOTO L_20

	j L_20

# LABEL L_26 :

L_26:
# ARG maxP

# WRITE maxP

	lw $a0, -248($fp)
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
# LABEL L_27 :

L_27:
# RETURN #0

	li $v0, 0

addi $sp, $sp, 372
	jr $ra
# LABEL L_28 :

L_28:
