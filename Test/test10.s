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
# FUNCTION sum :

sum:
# PARAM y

addi $sp, $sp, -4
	lw $v1, 4($fp)
	sw $v1, 0($fp)
# PARAM x

addi $sp, $sp, -4
	lw $v1, 8($fp)
	sw $v1, -4($fp)
# v_v0 := x + y

addi $sp, $sp, -4
	lw $t0, -4($fp)
	lw $t1, 0($fp)
	add $t0, $t0, $t1
	sw $t0, -8($fp)
# RETURN v_v0

	lw $v0, -8($fp)
addi $sp, $sp, 12
	jr $ra
# LABEL L_0 :

L_0:
# FUNCTION main :

main:
move $fp, $sp
# a := #21

addi $sp, $sp, -4
	li $v1, 21

	sw $v1, 0($fp)
# LABEL L_1 :

L_1:
# b := #179

addi $sp, $sp, -4
	li $v1, 179

	sw $v1, -4($fp)
# LABEL L_2 :

L_2:
# ARG a

# ARG b

# v_v1 := CALL sum

	sw $fp, 0($sp)
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	addi $sp, $sp, -4
	lw $v1, 0($fp)
	sw $v1, 0($sp)
	addi $sp, $sp, -4
	lw $t0, -4($fp)
	sw $t0, 0($sp)
	addi $sp, $sp, -4
	move $fp, $sp
	jal sum

	addi $sp, $sp, 4
	addi $sp, $sp, 4
	addi $sp, $sp, 4
	lw $ra, 0($sp)
	addi $sp, $sp, 4
	lw $fp, 0($sp)
addi $sp, $sp, -4
	sw $v0, -8($fp)
# ARG v_v1

# WRITE v_v1

	lw $a0, -8($fp)
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

addi $sp, $sp, 12
	jr $ra
# LABEL L_4 :

L_4:
