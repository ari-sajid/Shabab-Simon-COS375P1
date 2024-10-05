addi $t0, $zero, 5     # Load 5 into $t0
addi $t1, $zero, 5     # Load 5 into $t1
beq  $t0, $t1, equal   # Branch if $t0 == $t1 (branch should happen)
addi $t2, $zero, 100   # This instruction will be skipped
equal:
addi $t2, $zero, 200   # This instruction will execute (branch target)

addi $t3, $zero, 10    # Load 10 into $t3
addi $t4, $zero, 15    # Load 15 into $t4
beq  $t3, $t4, noteq   # Branch if $t3 == $t4 (no branch should happen)
addi $t5, $zero, 300   # This will execute (no branch)
noteq:
.word 0xfeedfeed       # Halt instruction (exit)
