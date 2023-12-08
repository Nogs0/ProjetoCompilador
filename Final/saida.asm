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
	move $s5, $a2
	move $s6, $a1
	move $s7, $a0
	div $s5, $s6
	mflo $t0
	add $t1,$t0,$s7
	li $v0, 1
	move $a0,$t1
	syscall
	li $v0,11
	li $a0,'\n'
	syscall
	li $t2,10
	li $v0, 1
	move $a0,$t2
	syscall
jr $ra
