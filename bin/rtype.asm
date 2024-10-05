addi $t0, $zero, 10    # Load 10 into register $t0
addi $t1, $zero, 20    # Load 20 into register $t1
add $t2, $t0, $t1      # Add $t0 + $t1, store result in $t2 (should be 30)
sub $t3, $t1, $t0      # Subtract $t0 from $t1, store result in $t3 (should be 10)
and $t4, $t0, $t1      # Bitwise AND between $t0 and $t1, store result in $t4 (should be 0)
or  $t5, $t0, $t1      # Bitwise OR between $t0 and $t1, store result in $t5 (should be 30)
slt $t6, $t0, $t1      # Set $t6 to 1 if $t0 < $t1, else 0 (should be 1)
jr  $ra                # Jump back to return address
.word 0xfeedfeed       # Halt instruction (exit)