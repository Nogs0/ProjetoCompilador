.text
	li $t0,1
	move $s1,$t0
	li $t0,1
	move $s2,$t0
	li $t0,3
	move $s3,$t0
	move $a2, $s1
	move $a1, $s2
	move $a0, $s3
	jal FUNC4
	li $v0, 10
	syscall

FUNC4:

	sub $sp, $sp, 4
	sw $a2, 0($sp)
	lw $t2, 0($sp)

	sub $sp, $sp, 4
	sw $a1, 0($sp)
	lw $t1, 0($sp)

	sub $sp, $sp, 4
	sw $a0, 0($sp)
	lw $t0, 0($sp)

	div $s5, $s6
	mflo $t3
	add $t4,$t3,$s7
	li $v0, 1
	move $a0,$t4
	syscall
	li $v0,11
	li $a0,'\n'
	syscall
	li $t5,10
	li $v0, 1
	move $a0,$t5
	syscall

	add $sp, $sp, 12
jr $ra
