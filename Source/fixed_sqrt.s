.global fixed_sqrt
.align 2
.thumb_func
fixed_sqrt:
swi 0x8
bx lr