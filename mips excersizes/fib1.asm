.data
n: 8

.text
main:
	la $a0, n
	lw $a0, 0($a0)
	jal fib
	
	move $a0, $v0
	li $v0, 1
	syscall
	
	j exit
fib:
	addi, $sp, $sp, 12
	sw $ra, 0($sp)
	sw $s0, 4($sp)
	sw $s1, 8($sp)
	
	move $s1, $a0
	li $t1, 1
	
	blez $s1, return_0
	beq $s1, $t1, return_1

	addi $a0, $s1, -1
	jal fib
	move $s0, $v0 # first fib result
	
	addi $a0, $s1, -2
	jal fib
	move $t3, $v0 # second fib result
	
	add $v0, $s0, $t3
	j return
return_1:
	li $v0, 1
	j return
	
return_0:
	li $v0, 0

return: 
	lw $ra, 0($sp)
	lw $s0, 4($sp)
	lw $s1, 8($sp)
	addi $sp, $sp, -12
	jr $ra

exit:
	li  $v0, 10
	li  $a0, 0
	syscall