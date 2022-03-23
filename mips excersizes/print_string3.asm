.data
TEXT: .asciiz "Some text"

.text
main:
	la $a0, TEXT
	jal string_print
	
	j exit

string_print:
	move $t0, $a0
	
	lb $t1, 0($t0)
	beqz $t1, return
	
loop:
	move $a0, $t1,
	li $v0, 11
	syscall
	
	addi $t0, $t0, 1
	
	lb $t1, 0($t0)
	beqz $t1, return
	j loop

return: 
	jr $ra

exit:
	li  $v0, 10
	li  $a0, 0
	syscall