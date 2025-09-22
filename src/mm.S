.global memzero
memzero:
    str xzr, [x0], #8
    subs x1, x1, #8
    b.gt memzero
    ret

.global get_el
# Bits 0 - 1 and 4 - 63 are reserved, 3 2 contain the actual exception level
# 3 2 ELn
# 0 0 EL0
# 0 1 EL1
# 1 0 EL2
# 1 1 EL3
get_el:
    mrs x0, CurrentEL
    lsr x0, x0, #2
    ret
