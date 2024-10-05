addi $t0, $zero, 100   # Load 100 into $t0
addi $t1, $zero, 50    # Load 50 into $t1
jal  func              # Jump and link to `func`, save return address in $ra
.word 0xfeedfeed       # Halt instruction (exit)

func:
    add  $t2, $t0, $t1  # Add $t0 and $t1, store result in $t2 (should be 150)
    jr   $ra            # Return to address stored in $ra
