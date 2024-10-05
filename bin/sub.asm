addi $t0, $zero, 15    # Load 15 into $t0
addi $t1, $zero, 8     # Load 8 into $t1
sub  $t2, $t0, $t1     # Subtract $t1 from $t0, store result in $t2 (15 - 8 = 7)
sub  $t3, $t1, $t0     # Subtract $t0 from $t1, store result in $t3 (8 - 15 = -7)
sub  $t4, $t0, $zero   # Subtract $zero from $t0, store result in $t4 (15 - 0 = 15)
.word 0xfeedfeed       # Halt instruction (exit)
