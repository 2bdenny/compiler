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
fib:
	move $v1, $sp
	addi $sp, $sp, -4
	move $t0, $sp
	addi $sp, $sp, -4
	move $t1, $sp
	addi $sp, $sp, -4
	move $t2, $t1
	addi $t3, $t2, 0

	li $t4, 0
	li $t4, 8

	mul $t4, $t4, $t4
	li $t5, 1
	li $t5, 4

	mul $t5, $t5, $t5
	add $t6, $t5, $t4
	move $t7, $t1
	addi $s0, $t7, 16

	move $s1, $s0
	addi $s2, $s1, 0

	move $s3, $t3
	add $s3, $s3, $t6
	lw $s4, 0($s3)
	lw $s4, 0($s2)
	add $t3, $s4, $s4
	move $s4, $t3
	blt $v1, $t0, L_0

	j L_4

L_0:
	move $t3, $t1
	addi $t3, $t3, 0

	li $s5, 0
	li $s5, 8

	mul $t3, $s5, $s5
	li $s5, 1
	li $s5, 4

	mul $t3, $s5, $s5
	add $t3, $t3, $t3
	move $t3, $t3
	add $t3, $t3, $t3
	sw $s4, 0($t3)
L_1:
	move $t3, $t1
	addi $t3, $t3, 16

	move $t3, $t3
	addi $t4, $t3, 0

	move $t4, $t1
	addi $t4, $t4, 16

	move $t4, $t4
	addi $t4, $t4, 0

	lw $s5, 0($t4)
	add $t4, $s5, $s4
	sw $t4, 0($t4)
L_2:
	addi $t4, $v1, 1

	move $s5, $t1
	move $sp, $s5
	addi $sp, $sp, 4
	move $s5, $t0
	move $sp, $s5
	addi $sp, $sp, 4
	move $s5, $t4
	move $sp, $s5
	addi $sp, $sp, 4
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	jal fib

	addi $sp, $sp, 4
	move $t4, $v0
	move $v0, $t4
	jr $ra
L_3:
	j L_5

L_4:
	move $v0, $s4
	jr $ra
L_5:
main:
	li $s5, 0

	li $s6, 1

	li $s7, 4

	li $s5, -100

L_6:
	move $t4, $s8
	addi $t4, $t4, 0

	li $t8, 0
	li $t8, 8

	mul $t5, $t8, $t8
	li $t8, 1
	li $t8, 4

	mul $t5, $t8, $t8
	add $t5, $t5, $t5
	move $t5, $t4
	add $t5, $t5, $t5
	li $t8, 1

	sw $t8, 0($t5)
L_7:
	move $t5, $s8
	addi $t5, $t5, 16

	move $t5, $t5
	addi $t5, $t5, 0

	li $t8, 1

	sw $t8, 0($t5)
L_8:
	move $t5, $t8
	move $t5, $s8
	lw $t9, 0($t5)
	sw $t9, 0($t5)
	addi $t5, $t5, 4

	addi $t5, $t5, 4

	lw $t9, 0($t5)
	sw $t9, 0($t5)
	addi $t5, $t5, 4

	addi $t5, $t5, 4

	lw $t9, 0($t5)
	sw $t9, 0($t5)
	addi $t5, $t5, 4

	addi $t5, $t5, 4

	lw $t9, 0($t5)
	sw $t9, 0($t5)
	addi $t5, $t5, 4

	addi $t5, $t5, 4

	lw $t9, 0($t5)
	sw $t9, 0($t5)
L_9:
L_10:
	li $t9, 0
	bne $s6, $t9, L_11

	j L_19

L_11:
L_12:
	li $t9, 0
	bgt $s7, $t9, L_13

	j L_16

L_13:
	add $t6, $s5, $s7
	move $s5, $t6
L_14:
	addi $t6, $s7, -1

	move $s7, $t6
L_15:
	j L_12

L_16:
	addi $t6, $s6, -1

	move $s6, $t6
L_17:
	li $s7, 4

L_18:
	j L_10

L_19:
	addi $t9, $fp, 0
	move $sp, $t9
	addi $sp, $sp, 4
	li $t9, 10

	move $sp, $t9
	addi $sp, $sp, 4
	li $t9, 1

	move $sp, $t9
	addi $sp, $sp, 4
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	jal fib

	addi $sp, $sp, 4
	move $t6, $v0
	move $t9, $t6
	move $sp, $t9
	addi $sp, $sp, 4
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	jal write
	lw $ra, 0($sp)
	addi $sp, $sp, 4
L_20:
	li $t9, 10
	beq $s5, $t9, L_21

	j L_22

L_21:
	addi $t9, $fp, 0
	move $sp, $t9
	addi $sp, $sp, 4
	li $t9, 10

	move $sp, $t9
	addi $sp, $sp, 4
	li $t9, 1

	move $sp, $t9
	addi $sp, $sp, 4
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	jal fib

	addi $sp, $sp, 4
	move $t6, $v0
	move $v0, $t6
	jr $ra
	j L_23

L_22:
	move $v0, $s5
	jr $ra
L_23:
