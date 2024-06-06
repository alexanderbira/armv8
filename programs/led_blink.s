ldr w3, large

ldr w0, gpsel0
ldr w1, gpsel0val
str w1, [w0]

off:
  ldr w0, gpclr0
  ldr w1, gpclr0val
  str w1, [w0]

movz w2, #0
stall1:
  add w2, w2, #1
  cmp w2, w3
  b.eq on
  b stall1

on:
  ldr w0, gpset0
  ldr w1, gpset0val
  str w1, [w0]

movz w2, #0
stall2:
  add w2, w2, #1
  cmp w2, w3
  b.eq off
  b stall2

gpsel0:
  .int 0x3f200000

gpsel0val:
  .int 0x40

gpset0:
  .int 0x3f20001c

gpset0val:
  .int 0x4

gpclr0:
  .int 0x3f200028

gpclr0val:
  .int 0x4

large:
  .int 0xF0000