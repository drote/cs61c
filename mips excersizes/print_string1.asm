.data
TEXT: .asciiz "Hello there"

.text
main:
	la  $a0, TEXT
	jal print_me
	j exit
print_me:
	add $t0, $a0, $0
loop:
	lb  $t1, 0($t0)
	beq $t1, $0, return
	
	li  $v0, 11
	add $a0, $t1, $0
	syscall
	
	addi $t0, $t0, 1
	j loop
return:
	jr $ra
exit:
	li  $v0, 10
	li  $a0, 0
	syscall
	