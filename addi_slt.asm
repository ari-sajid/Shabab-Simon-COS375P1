	addi $t0, $zero, -32
	addi $t1, $zero, 58
	slt  $v0, $t0, $t1 # Is -32 < 58?
	slt  $v1, $t1, $t0 # Is 58 < -32?
	.word 0xfeedfeed
