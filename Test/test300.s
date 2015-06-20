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
	move reg, reg
	addi reg, reg, 0

	li reg, 0
	li reg, 8

	mul reg, reg, reg
	li reg, 1
	li reg, 4

	mul reg, reg, reg
	add reg, reg, reg
	move reg, reg
	addi reg, reg, 16

	move reg, reg
	addi reg, reg, 0

	move reg, reg
	add reg, reg, reg
	lw reg, 0(reg)
	lw reg, 0(reg)
	add reg, reg, reg
	move reg, reg
	blt reg, reg, L_0

	j L_4

L_0:
	move reg, reg
	addi reg, reg, 0

	li reg, 0
	li reg, 8

	mul reg, reg, reg
	li reg, 1
	li reg, 4

	mul reg, reg, reg
	add reg, reg, reg
	move reg, reg
	add reg, reg, reg
	sw reg, 0(reg)
L_1:
	move reg, reg
	addi reg, reg, 16

	move reg, reg
	addi reg, reg, 0

	move reg, reg
	addi reg, reg, 16

	move reg, reg
	addi reg, reg, 0

	lw reg, 0(reg)
	add reg, reg, reg
	sw reg, 0(reg)
L_2:
	addi reg, reg, 1

L_3:
	j L_5

L_4:
L_5:
main:
	li reg, 0

	li reg, 1

	li reg, 4

	li reg, -100

L_6:
	move reg, reg
	addi reg, reg, 0

	li reg, 0
	li reg, 8

	mul reg, reg, reg
	li reg, 1
	li reg, 4

	mul reg, reg, reg
	add reg, reg, reg
	move reg, reg
	add reg, reg, reg
	li reg, 1

	sw reg, 0(reg)
L_7:
	move reg, reg
	addi reg, reg, 16

	move reg, reg
	addi reg, reg, 0

	li reg, 1

	sw reg, 0(reg)
L_8:
	move reg, reg
	move reg, reg
	lw reg, 0(reg)
	sw reg, 0(reg)
	addi reg, reg, 4

	addi reg, reg, 4

	lw reg, 0(reg)
	sw reg, 0(reg)
	addi reg, reg, 4

	addi reg, reg, 4

	lw reg, 0(reg)
	sw reg, 0(reg)
	addi reg, reg, 4

	addi reg, reg, 4

	lw reg, 0(reg)
	sw reg, 0(reg)
	addi reg, reg, 4

	addi reg, reg, 4

	lw reg, 0(reg)
	sw reg, 0(reg)
L_9:
L_10:
	li reg, 0
	bne reg, reg, L_11

	j L_19

L_11:
L_12:
	li reg, 0
	bgt reg, reg, L_13

	j L_16

L_13:
	add reg, reg, reg
	move reg, reg
L_14:
	addi reg, reg, -1

	move reg, reg
L_15:
	j L_12

L_16:
	addi reg, reg, -1

	move reg, reg
L_17:
	li reg, 4

L_18:
	j L_10

L_19:
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	jal write
	lw $ra, 0($sp)
	addi $sp, $sp, 4
L_20:
	li reg, 10
	beq reg, reg, L_21

	j L_22

L_21:
	j L_23

L_22:
L_23:
