# 6510 CPU ILLEGAL INSTRUCTION SET

```
  JAM                                                                   JAM

                                                        N V - B D I Z C
  Operation:  [locks up machine]                         who cares? :)

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  |  Implied       |   JAM                 |   $02   |    1    |    -     |
  |  Implied       |   JAM                 |   $12   |    1    |    -     |
  |  Implied       |   JAM                 |   $22   |    1    |    -     |
  |  Implied       |   JAM                 |   $32   |    1    |    -     |
  |  Implied       |   JAM                 |   $42   |    1    |    -     |
  |  Implied       |   JAM                 |   $52   |    1    |    -     |
  |  Implied       |   JAM                 |   $62   |    1    |    -     |
  |  Implied       |   JAM                 |   $72   |    1    |    -     |
  |  Implied       |   JAM                 |   $92   |    1    |    -     |
  |  Implied       |   JAM                 |   $B2   |    1    |    -     |
  |  Implied       |   JAM                 |   $D2   |    1    |    -     |
  |  Implied       |   JAM                 |   $F2   |    1    |    -     |
  +----------------+-----------------------+---------+---------+----------+

  Also known as CRS, KIL, HLT.

  Note: Data Bus will be set to $FF


  SLO                   SLO ASL memory, ORA memory                      SLO

                                                        N V - B D I Z C
  Operation:  A <- (M << 1) \/ A                        / . . . . . / /

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  |  ZeroPage      |   SLO $FF             |   $07   |    2    |    5     |
  |  ZeroPage,X    |   SLO $FF,X           |   $17   |    2    |    6     |
  |  Absolute      |   SLO $FFFF           |   $0F   |    3    |    6     |
  |  Absolute,X    |   SLO $FFFF,X         |   $1F   |    3    |    7     |
  |  Absolute,Y    |   SLO $FFFF,Y         |   $1B   |    3    |    7     |
  |  (Indirect,X)  |   SLO ($FF,X)         |   $03   |    2    |    8     |
  |  (Indirect),Y  |   SLO ($FF),Y         |   $13   |    2    |    8     |
  +----------------+-----------------------+---------+---------+----------+

  Also known as ASO.


  ANC           ANC AND #immediate, copy accu-bit 7 to carry            ANC

                                                        N V - B D I Z C
  Operation:  A <- A /\ M, C <- A7                      / . . . . . / A7

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  |  Immediate     |   ANC #$FF            |   $0B   |    2    |    2     |
  |  Immediate     |   ANC #$FF            |   $2B   |    2    |    2     |
  +----------------+-----------------------+---------+---------+----------+


  RLA                   RLA ROL memory, AND memory                      RLA

                                                        N V - B D I Z C
  Operation:  A <- (M << 1) /\ (A)                      / . . . . . / /

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  |  ZeroPage      |   RLA $FF             |   $27   |    2    |    5     |
  |  ZeroPage,X    |   RLA $FF,X           |   $37   |    2    |    6     |
  |  Absolute      |   RLA $FFFF           |   $2F   |    3    |    6     |
  |  Absolute,X    |   RLA $FFFF,X         |   $3F   |    3    |    7     |
  |  Absolute,Y    |   RLA $FFFF,Y         |   $3B   |    3    |    7     |
  |  (Indirect,X)  |   RLA ($FF,X)         |   $23   |    2    |    8     |
  |  (Indirect),Y  |   RLA ($FF),Y         |   $33   |    2    |    8     |
  +----------------+-----------------------+---------+---------+----------+


  SRE                   SRE LSR memory, EOR memory                      SRE

                                                        N V - B D I Z C
  Operation:  A <- (M >> 1) \-/ A                       / . . . . . / /

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  |  ZeroPage      |   SRE $FF             |   $47   |    2    |    5     |
  |  ZeroPage,X    |   SRE $FF,X           |   $57   |    2    |    6     |
  |  Absolute      |   SRE $FFFF           |   $4F   |    3    |    6     |
  |  Absolute,X    |   SRE $FFFF,X         |   $5F   |    3    |    7     |
  |  Absolute,Y    |   SRE $FFFF,Y         |   $5B   |    3    |    7     |
  |  (Indirect,X)  |   SRE ($FF,X)         |   $43   |    2    |    8     |
  |  (Indirect),Y  |   SRE ($FF),Y         |   $53   |    2    |    8     |
  +----------------+-----------------------+---------+---------+----------+

  Also known as LSE.


  ASR                   ASR AND #immediate, LSR accu                    ASR

                                                        N V - B D I Z C
  Operation:  A <- [(A /\ M) >> 1]                      / . . . . . / /

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  |  Immediate     |   ASR #$FF            |   $4B   |    2    |    2     |
  +----------------+-----------------------+---------+---------+----------+

  Also known as ALR.


  RRA                   RRA ROR memory, ADC memory                      RRA

                                                        N V - B D I Z C
  Operation:  A <- (M >> 1) + (A) + C                   / / . . . . / /

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  |  ZeroPage      |   RRA $FF             |   $67   |    2    |    5     |
  |  ZeroPage,X    |   RRA $FF,X           |   $77   |    2    |    6     |
  |  Absolute      |   RRA $FFFF           |   $6F   |    3    |    6     |
  |  Absolute,X    |   RRA $FFFF,X         |   $7F   |    3    |    7     |
  |  Absolute,Y    |   RRA $FFFF,Y         |   $7B   |    3    |    7     |
  |  (Indirect,X)  |   RRA ($FF,X)         |   $63   |    2    |    8     |
  |  (Indirect),Y  |   RRA ($FF),Y         |   $73   |    2    |    8     |
  +----------------+-----------------------+---------+---------+----------+


  ARR                   ARR AND #immediate, ROR accu                    ARR

                                                        N V - B D I Z C
  Operation:  A <- [(A /\ M) >> 1]                      / / . . . . / /

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  |  Immediate     |   ARR #$FF            |   $6B   |    2    |    2     |
  +----------------+-----------------------+---------+---------+----------+

  Note: Part of this command are some ADC mechanisms. Following effects
  appear after AND but before ROR: the V-Flag is set according to
  [(A and #mem)+#mem], Bit 0 does NOT go into carry, but Bit 7 is
  exchanged with the carry.


  SAX                SAX Accu AND X-Register into memory                SAX

                                                        N V - B D I Z C
  Operation:  M <- (A) /\ (X)                           . . . . . . . .

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  |  ZeroPage      |   SAX $FF             |   $87   |    2    |    3     |
  |  ZeroPage,Y    |   SAX $FF,Y           |   $97   |    2    |    4     |
  |  Absolute      |   SAX $FFFF           |   $8F   |    3    |    4     |
  |  (Indirect,X)  |   SAX ($FF,X)         |   $83   |    2    |    6     |
  +----------------+-----------------------+---------+---------+----------+

  Note: The A /\ X operation is a result of A and X put onto the bus
  at the same time.


  ANE                                                                   ANE

                                                        N V - B D I Z C
  Operation:  M <- [(A)/\$EE] \/ (X)/\(M)               / . . . . . / .

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  |  Immediate     |   ANE #$FF            |   $8B   |    2    |    2     |
  +----------------+-----------------------+---------+---------+----------+

  Also known as XAA: A <- [(X) /\ (M)]

  Warning: This opcode is said to be unstable!


  SHA                                                                   SHA

                                                        N V - B D I Z C
  Operation:  M <- (A) /\ (X) /\ (PCH+1)                . . . . . . . .

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  |  Absolute,X    |   SHA $FFFF,X         |   $93   |    3    |    5     |
  |  Absolute,Y    |   SHA $FFFF,Y         |   $9F   |    3    |    5     |
  +----------------+-----------------------+---------+---------+----------+

  Also known as AHX

  Note: Sometimes the "/\ (PCH+1)" drops off. Also page boundary crossing
  won't work as expected (the bank where the value is stored may be equal
  to the value stored).

  Warning: This opcode is said to be unstable!


  SHS                                                                   SHS

  Operation:  X <- (A) /\ (X), S <- (X)                 N V - B D I Z C
              M <- (X) /\ (PCH+1)                       . . . . . . . .

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  |  Absolute,Y    |   SHS $FFFF,Y         |   $9B   |    3    |    5     |
  +----------------+-----------------------+---------+---------+----------+

  Also known as TAS.

  Warning: This opcode is said to be unstable!


  SHY                                                                   SHY

                                                        N V - B D I Z C
  Operation:  M <- (Y) /\ (PCH+1)                       . . . . . . . .

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  |  Absolute,X    |   SHY $FFFF,X         |   $9C   |    3    |    5     |
  +----------------+-----------------------+---------+---------+----------+

  Note: Sometimes the "/\ (PCH+1)" drops off. Also page boundary crossing
  won't work as expected (the bank where the value is stored may be equal
  to the value stored).

  Warning: This opcode is said to be unstable!


  SHX                                                                   SHX

                                                        N V - B D I Z C
  Operation:  M <- (X) /\ (PCH+1)                       . . . . . . . .

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  |  Absolute,Y    |   SHX $FFFF,Y         |   $9E   |    3    |    5     |
  +----------------+-----------------------+---------+---------+----------+

  Note: Sometimes the "/\ (PCH+1)" drops off. Also page boundary crossing
  won't work as expected (the bank where the value is stored may be equal
  to the value stored).

  Warning: This opcode is said to be unstable!


  LAX                        LAX LDA memory, TAX                        LAX

                                                        N V - B D I Z C
  Operation:  A <- M, X <- A                            / . . . . . / .

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  |  ZeroPage      |   LAX $FF             |   $A7   |    2    |    3     |
  |  ZeroPage,Y    |   LAX $FF,Y           |   $B7   |    2    |    4     |
  |  Absolute      |   LAX $FFFF           |   $AF   |    3    |    4     |
  |  Absolute,Y    |   LAX $FFFF,Y         |   $BF   |    3    |    4*    |
  |  (Indirect,X)  |   LAX ($FF,X)         |   $A3   |    2    |    6     |
  |  (Indirect),Y  |   LAX ($FF),Y         |   $B3   |    2    |    5*    |
  +----------------+-----------------------+---------+---------+----------+
  * Add 1 if page boundary is crossed.


  LXA                                                                   LXA

  Operation:  X04 <- (X04) /\ M04                       N V - B D I Z C
              A04 <- (A04) /\ M04                       / . . . . . / .

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  |  Immediate     |   LXA #$FF            |   $AB   |    2    |    2     |
  +----------------+-----------------------+---------+---------+----------+

  Also known as LAX #$FF (immediate).

  Warning: This opcode is said to be unstable!


  LAE            LAE Stack-Pointer AND with memory, TSX, TXA            LAE

                                                        N V - B D I Z C
  Operation:  S,X,A <- (S /\ M)                         / . . . . . / .

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  |  Absolute,Y    |   LAE $FFFF,Y         |   $BB   |    3    |    4*    |
  +----------------+-----------------------+---------+---------+----------+
  * Add 1 if page boundary is crossed.

  Also known as LAS, LAR.

  Warning: This opcode is said to be unstable!


  DCP                    DCP DEC memory, CMP memory                     DCP

                                                        N V - B D I Z C
  Operation:  M <- (M)-1, (A-M) -> NZC                  / . . . . . / /

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  |  ZeroPage      |   DCP $FF             |   $C7   |    2    |    5     |
  |  ZeroPage,X    |   DCP $FF,X           |   $D7   |    2    |    6     |
  |  Absolute      |   DCP $FFFF           |   $CF   |    3    |    6     |
  |  Absolute,X    |   DCP $FFFF,X         |   $DF   |    3    |    7     |
  |  Absolute,Y    |   DCP $FFFF,Y         |   $DB   |    3    |    7     |
  |  (Indirect,X)  |   DCP ($FF,X)         |   $C3   |    2    |    8     |
  |  (Indirect),Y  |   DCP ($FF),Y         |   $D3   |    2    |    8     |
  +----------------+-----------------------+---------+---------+----------+


  SBX SBX Accu AND X-Register, subtract operand, result into X-Register SBX

                                                        N V - B D I Z C
  Operation:  X <- (X)/\(A) - M                         / . . . . . / /

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  |  Immediate     |   SBX #$FF            |   $CB   |    2    |    2     |
  +----------------+-----------------------+---------+---------+----------+

  Note: Performs CMP and DEX at the same time, so the subtraction
  ignores flags 'C' and 'D' (like CMP, unlike SBC).

  Also known as AXS.


  ISB                    ISB INC memory, SBC memory                     ISB

                                                        N V - B D I Z C
  Operation:  M <- (M) + 1, A <- (A) - M - ~C           / / . . . . / /

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  |  ZeroPage      |   ISB $FF             |   $E7   |    2    |    5     |
  |  ZeroPage,X    |   ISB $FF,X           |   $F7   |    2    |    6     |
  |  Absolute      |   ISB $FFFF           |   $EF   |    3    |    6     |
  |  Absolute,X    |   ISB $FFFF,X         |   $FF   |    3    |    7     |
  |  Absolute,Y    |   ISB $FFFF,Y         |   $FB   |    3    |    7     |
  |  (Indirect,X)  |   ISB ($FF,X)         |   $E3   |    2    |    8     |
  |  (Indirect),Y  |   ISB ($FF),Y         |   $F3   |    2    |    8     |
  +----------------+-----------------------+---------+---------+----------+

  Also known as ISC.
```
