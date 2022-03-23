.data
TEXT: .asciiz "Hello there"

.text
main:
	la $a0, TEXT
	jal str_len
	
	la $a0, TEXT
	move $a1, $v0
		
	jal string_print
	j exit

str_len:
	move $t0, $a0
	li $t1, 0

sl_loop:
	lb  $t2, 0($t0)
	beq $t2, $0, sl_return
	
	addi, $t1, $t1, 1
	
	addi $t0, $t0, 1
	j sl_loop
sl_return:
	add $v0, $t1, $0
	jr $ra
	
string_print:
	move  $t0, $a0
	move  $t1, $a1
	li  $t2, 0
	
sp_loop:
	bge $t2, $t1, sp_return
	lb  $t3, 0($t0)
	
	move $a0, $t3,
	li $v0, 11
	syscall
	
	addi $t0, $t0, 1
	addi $t2, $t2, 1
	
	j sp_loop

sp_return:
	jr $ra
	
exit:
	li  $v0, 10
	li  $a0, 0
	syscall
