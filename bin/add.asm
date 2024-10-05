addi $t0, $zero, 25    # Load 25 into $t0
addi $t1, $zero, 17    # Load 17 into $t1
add  $t2, $t0, $t1     # Add $t0 and $t1, store result in $t2 (25 + 17 = 42)
add  $t3, $t0, $zero   # Add $t0 and $zero, store result in $t3 (25)
add  $t4, $t1, $zero   # Add $t1 and $zero, store result in $t4 (17)
.word 0xfeedfeed       # Halt instruction (exit)