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
# v_v0 := #6 + #6

addi $sp, $sp, -4
	li $t0, 6

	addi $t0, $t0, 6
	sw $t0, 0($fp)
# v_v1 := #11 * #1

addi $sp, $sp, -4
	li $t0, 11
	li $t1, 1

	mul $t0, $t0, $t1
	sw $t0, -4($fp)
# v_v2 := v_v0 - v_v1

addi $sp, $sp, -4
	lw $t0, 0($fp)
	lw $t1, -4($fp)
	sub $t0, $t0, $t1
	sw $t0, -8($fp)
# a := v_v2

addi $sp, $sp, -4
	lw $v1, -8($fp)
	sw $v1, -12($fp)
# v_v3 := #7 - #4

addi $sp, $sp, -4
	li $t0, 7
	li $t1, 4

	sub $t0, $t0, $t1
	sw $t0, -16($fp)
# b := v_v3

addi $sp, $sp, -4
	lw $v1, -16($fp)
	sw $v1, -20($fp)
# v_v4 := #3 + #4

addi $sp, $sp, -4
	li $t0, 4

	addi $t0, $t0, 3
	sw $t0, -24($fp)
# v_v5 := #5 * #6

addi $sp, $sp, -4
	li $t0, 5
	li $t1, 6

	mul $t0, $t0, $t1
	sw $t0, -28($fp)
# v_v6 := v_v5 / #3

addi $sp, $sp, -4
	lw $t0, -28($fp)
	li $t1, 3

	div $t0, $t1
	mflo $t0
	sw $t0, -32($fp)
# v_v7 := v_v4 + v_v6

addi $sp, $sp, -4
	lw $t0, -24($fp)
	lw $t1, -32($fp)
	add $t0, $t0, $t1
	sw $t0, -36($fp)
# v_v8 := v_v7 - #4

addi $sp, $sp, -4
	lw $t0, -36($fp)
	li $t1, 4

	sub $t0, $t0, $t1
	sw $t0, -40($fp)
# c := v_v8

addi $sp, $sp, -4
	lw $v1, -40($fp)
	sw $v1, -44($fp)
# v_v9 := a + b

addi $sp, $sp, -4
	lw $t0, -12($fp)
	lw $t1, -20($fp)
	add $t0, $t0, $t1
	sw $t0, -48($fp)
# v_v10 := v_v9 - c

addi $sp, $sp, -4
	lw $t0, -48($fp)
	lw $t1, -44($fp)
	sub $t0, $t0, $t1
	sw $t0, -52($fp)
# d := v_v10

addi $sp, $sp, -4
	lw $v1, -52($fp)
	sw $v1, -56($fp)
# v_v11 := a + b

addi $sp, $sp, -4
	lw $t0, -12($fp)
	lw $t1, -20($fp)
	add $t0, $t0, $t1
	sw $t0, -60($fp)
# v_v12 := c * #2

addi $sp, $sp, -4
	lw $t0, -44($fp)
	li $t1, 2

	mul $t0, $t0, $t1
	sw $t0, -64($fp)
# v_v13 := v_v11 + v_v12

addi $sp, $sp, -4
	lw $t0, -60($fp)
	lw $t1, -64($fp)
	add $t0, $t0, $t1
	sw $t0, -68($fp)
# e := v_v13

addi $sp, $sp, -4
	lw $v1, -68($fp)
	sw $v1, -72($fp)
# v_v14 := a + b

addi $sp, $sp, -4
	lw $t0, -12($fp)
	lw $t1, -20($fp)
	add $t0, $t0, $t1
	sw $t0, -76($fp)
# v_v15 := v_v14 + c

addi $sp, $sp, -4
	lw $t0, -76($fp)
	lw $t1, -44($fp)
	add $t0, $t0, $t1
	sw $t0, -80($fp)
# f := v_v15

addi $sp, $sp, -4
	lw $v1, -80($fp)
	sw $v1, -84($fp)
# g1 := #42

addi $sp, $sp, -4
	li $v1, 42

	sw $v1, -88($fp)
# i := #0

addi $sp, $sp, -4
	li $v1, 0

	sw $v1, -92($fp)
# v_v16 := a + b

addi $sp, $sp, -4
	lw $t0, -12($fp)
	lw $t1, -20($fp)
	add $t0, $t0, $t1
	sw $t0, -96($fp)
# v_v17 := v_v16 + c

addi $sp, $sp, -4
	lw $t0, -96($fp)
	lw $t1, -44($fp)
	add $t0, $t0, $t1
	sw $t0, -100($fp)
# v_v18 := v_v17 - d

addi $sp, $sp, -4
	lw $t0, -100($fp)
	lw $t1, -56($fp)
	sub $t0, $t0, $t1
	sw $t0, -104($fp)
# v_v19 := v_v18 - e

addi $sp, $sp, -4
	lw $t0, -104($fp)
	lw $t1, -72($fp)
	sub $t0, $t0, $t1
	sw $t0, -108($fp)
# v_v20 := v_v19 + f

addi $sp, $sp, -4
	lw $t0, -108($fp)
	lw $t1, -84($fp)
	add $t0, $t0, $t1
	sw $t0, -112($fp)
# f := v_v20

	lw $v1, -112($fp)
	sw $v1, -84($fp)
# LABEL L_0 :

L_0:
# LABEL L_1 :

L_1:
# v_v21 := b - a

addi $sp, $sp, -4
	lw $t0, -20($fp)
	lw $t1, -12($fp)
	sub $t0, $t0, $t1
	sw $t0, -116($fp)
# IF v_v21 < f GOTO L_2

	lw $v1, -116($fp)
	lw $t0, -84($fp)
	blt $v1, $t0, L_2

# GOTO L_11

	j L_11

# LABEL L_2 :

L_2:
# v_v22 := i * #4

addi $sp, $sp, -4
	lw $t0, -92($fp)
	li $t1, 4

	mul $t0, $t0, $t1
	sw $t0, -120($fp)
# v_v23 := g1 + v_v22

addi $sp, $sp, -4
	lw $t0, -88($fp)
	lw $t1, -120($fp)
	add $t0, $t0, $t1
	sw $t0, -124($fp)
# v_v24 := v_v23 + #3

addi $sp, $sp, -4
	lw $t0, -124($fp)
	addi $t0, $t0, 3

	sw $t0, -128($fp)
# v_v25 := v_v24 + #4

addi $sp, $sp, -4
	lw $t0, -128($fp)
	addi $t0, $t0, 4

	sw $t0, -132($fp)
# v_v26 := v_v25 + #5

addi $sp, $sp, -4
	lw $t0, -132($fp)
	addi $t0, $t0, 5

	sw $t0, -136($fp)
# g1 := v_v26

	lw $v1, -136($fp)
	sw $v1, -88($fp)
# LABEL L_3 :

L_3:
# v_v27 := f - b

addi $sp, $sp, -4
	lw $t0, -84($fp)
	lw $t1, -20($fp)
	sub $t0, $t0, $t1
	sw $t0, -140($fp)
# v_v28 := a * #2

addi $sp, $sp, -4
	lw $t0, -12($fp)
	li $t1, 2

	mul $t0, $t0, $t1
	sw $t0, -144($fp)
# v_v29 := v_v27 + v_v28

addi $sp, $sp, -4
	lw $t0, -140($fp)
	lw $t1, -144($fp)
	add $t0, $t0, $t1
	sw $t0, -148($fp)
# v_v30 := c * d

addi $sp, $sp, -4
	lw $t0, -44($fp)
	lw $t1, -56($fp)
	mul $t0, $t0, $t1
	sw $t0, -152($fp)
# v_v31 := v_v29 + v_v30

addi $sp, $sp, -4
	lw $t0, -148($fp)
	lw $t1, -152($fp)
	add $t0, $t0, $t1
	sw $t0, -156($fp)
# v_v32 := v_v31 - f

addi $sp, $sp, -4
	lw $t0, -156($fp)
	lw $t1, -84($fp)
	sub $t0, $t0, $t1
	sw $t0, -160($fp)
# g := v_v32

addi $sp, $sp, -4
	lw $v1, -160($fp)
	sw $v1, -164($fp)
# LABEL L_4 :

L_4:
# v_v33 := i + #1

addi $sp, $sp, -4
	lw $t0, -92($fp)
	addi $t0, $t0, 1

	sw $t0, -168($fp)
# v_v34 := v_v33 + #0

addi $sp, $sp, -4
	lw $t0, -168($fp)
	addi $t0, $t0, 0

	sw $t0, -172($fp)
# i := v_v34

	lw $v1, -172($fp)
	sw $v1, -92($fp)
# LABEL L_5 :

L_5:
# v_v35 := i + #3

addi $sp, $sp, -4
	lw $t0, -92($fp)
	addi $t0, $t0, 3

	sw $t0, -176($fp)
# v_v36 := v_v35 + #1

addi $sp, $sp, -4
	lw $t0, -176($fp)
	addi $t0, $t0, 1

	sw $t0, -180($fp)
# i := v_v36

	lw $v1, -180($fp)
	sw $v1, -92($fp)
# LABEL L_6 :

L_6:
# v_v37 := i - #2

addi $sp, $sp, -4
	lw $t0, -92($fp)
	li $t1, 2

	sub $t0, $t0, $t1
	sw $t0, -184($fp)
# v_v38 := v_v37 - #2

addi $sp, $sp, -4
	lw $t0, -184($fp)
	li $t1, 2

	sub $t0, $t0, $t1
	sw $t0, -188($fp)
# i := v_v38

	lw $v1, -188($fp)
	sw $v1, -92($fp)
# LABEL L_7 :

L_7:
# v_v39 := i / #3

addi $sp, $sp, -4
	lw $t0, -92($fp)
	li $t1, 3

	div $t0, $t1
	mflo $t0
	sw $t0, -192($fp)
# v_v40 := v_v39 * #3

addi $sp, $sp, -4
	lw $t0, -192($fp)
	li $t1, 3

	mul $t0, $t0, $t1
	sw $t0, -196($fp)
# v_v41 := i - v_v40

addi $sp, $sp, -4
	lw $t0, -92($fp)
	lw $t1, -196($fp)
	sub $t0, $t0, $t1
	sw $t0, -200($fp)
# v_v42 := a - a

addi $sp, $sp, -4
	lw $t0, -12($fp)
	lw $t1, -12($fp)
	sub $t0, $t0, $t1
	sw $t0, -204($fp)
# v_v43 := v_v42 + b

addi $sp, $sp, -4
	lw $t0, -204($fp)
	lw $t1, -20($fp)
	add $t0, $t0, $t1
	sw $t0, -208($fp)
# v_v44 := v_v43 - b

addi $sp, $sp, -4
	lw $t0, -208($fp)
	lw $t1, -20($fp)
	sub $t0, $t0, $t1
	sw $t0, -212($fp)
# v_v45 := v_v44 + c

addi $sp, $sp, -4
	lw $t0, -212($fp)
	lw $t1, -44($fp)
	add $t0, $t0, $t1
	sw $t0, -216($fp)
# v_v46 := v_v45 - c

addi $sp, $sp, -4
	lw $t0, -216($fp)
	lw $t1, -44($fp)
	sub $t0, $t0, $t1
	sw $t0, -220($fp)
# IF v_v41 == v_v46 GOTO L_8

	lw $v1, -200($fp)
	lw $t0, -220($fp)
	beq $v1, $t0, L_8

# GOTO L_9

	j L_9

# LABEL L_8 :

L_8:
# v_v47 := f + #1

addi $sp, $sp, -4
	lw $t0, -84($fp)
	addi $t0, $t0, 1

	sw $t0, -224($fp)
# v_v48 := v_v47 + #1

addi $sp, $sp, -4
	lw $t0, -224($fp)
	addi $t0, $t0, 1

	sw $t0, -228($fp)
# f := v_v48

	lw $v1, -228($fp)
	sw $v1, -84($fp)
# LABEL L_9 :

L_9:
# v_v49 := f - #2

addi $sp, $sp, -4
	lw $t0, -84($fp)
	li $t1, 2

	sub $t0, $t0, $t1
	sw $t0, -232($fp)
# v_v50 := v_v49 + #1

addi $sp, $sp, -4
	lw $t0, -232($fp)
	addi $t0, $t0, 1

	sw $t0, -236($fp)
# f := v_v50

	lw $v1, -236($fp)
	sw $v1, -84($fp)
# LABEL L_10 :

L_10:
# GOTO L_1

	j L_1

# LABEL L_11 :

L_11:
# v_v51 := g1 - #2

addi $sp, $sp, -4
	lw $t0, -88($fp)
	li $t1, 2

	sub $t0, $t0, $t1
	sw $t0, -240($fp)
# v_v52 := v_v51 + #3

addi $sp, $sp, -4
	lw $t0, -240($fp)
	addi $t0, $t0, 3

	sw $t0, -244($fp)
# h := v_v52

addi $sp, $sp, -4
	lw $v1, -244($fp)
	sw $v1, -248($fp)
# LABEL L_12 :

L_12:
# ARG h

# WRITE h

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
# LABEL L_13 :

L_13:
# i := g1

	lw $v1, -88($fp)
	sw $v1, -92($fp)
# LABEL L_14 :

L_14:
# LABEL L_15 :

L_15:
# v_v53 := #1200 + #22

addi $sp, $sp, -4
	li $t0, 22

	addi $t0, $t0, 1200
	sw $t0, -252($fp)
# IF g1 >= v_v53 GOTO L_16

	lw $v1, -88($fp)
	lw $t0, -252($fp)
	bgt $v1, $t0, L_16

# GOTO L_20

	j L_20

# LABEL L_16 :

L_16:
# v_v54 := g1 + #1024

addi $sp, $sp, -4
	lw $t0, -88($fp)
	addi $t0, $t0, 1024

	sw $t0, -256($fp)
# i := v_v54

	lw $v1, -256($fp)
	sw $v1, -92($fp)
# LABEL L_17 :

L_17:
# v_v55 := g1 - #1

addi $sp, $sp, -4
	lw $t0, -88($fp)
	li $t1, 1

	sub $t0, $t0, $t1
	sw $t0, -260($fp)
# g1 := v_v55

	lw $v1, -260($fp)
	sw $v1, -88($fp)
# LABEL L_18 :

L_18:
# i := g1

	lw $v1, -88($fp)
	sw $v1, -92($fp)
# LABEL L_19 :

L_19:
# GOTO L_15

	j L_15

# LABEL L_20 :

L_20:
# ARG g1

# WRITE g1

	lw $a0, -88($fp)
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
# LABEL L_21 :

L_21:
# v_v56 := a + b

addi $sp, $sp, -4
	lw $t0, -12($fp)
	lw $t1, -20($fp)
	add $t0, $t0, $t1
	sw $t0, -264($fp)
# a := v_v56

	lw $v1, -264($fp)
	sw $v1, -12($fp)
# LABEL L_22 :

L_22:
# v_v57 := a + b

addi $sp, $sp, -4
	lw $t0, -12($fp)
	lw $t1, -20($fp)
	add $t0, $t0, $t1
	sw $t0, -268($fp)
# b := v_v57

	lw $v1, -268($fp)
	sw $v1, -20($fp)
# LABEL L_23 :

L_23:
# v_v58 := a + b

addi $sp, $sp, -4
	lw $t0, -12($fp)
	lw $t1, -20($fp)
	add $t0, $t0, $t1
	sw $t0, -272($fp)
# c := v_v58

	lw $v1, -272($fp)
	sw $v1, -44($fp)
# LABEL L_24 :

L_24:
# ARG c

# WRITE c

	lw $a0, -44($fp)
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
# LABEL L_25 :

L_25:
# RETURN #0

	li $v0, 0

addi $sp, $sp, 276
	jr $ra
# LABEL L_26 :

L_26:
