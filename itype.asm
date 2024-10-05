addi $t0, $zero, -32   # Load -32 into $t0
addi $t1, $zero, 58    # Load 58 into $t1
slt  $v0, $t0, $t1     # Set $v0 to 1 if -32 < 58, else 0 (should be 1)
slt  $v1, $t1, $t0     # Set $v1 to 1 if 58 < -32, else 0 (should be 0)
beq  $t0, $t1, label   # Branch if $t0 == $t1 (should not branch)
label: 
lw   $t2, 0($t1)       # Load word from memory address $t1 into $t2
sw   $t3, 0($t1)       # Store word from $t3 into memory at address $t1
.word 0xfeedfeed       # Halt instruction (exit)