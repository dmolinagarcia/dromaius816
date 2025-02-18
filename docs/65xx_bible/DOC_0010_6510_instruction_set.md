# CENTRAL PROCESSING UNIT (CPU) 6510 Instruction Set

```
  ADC               Add memory to accumulator with carry                ADC

  Operation:  A + M + C -> A, C                         N V - B D I Z C
                                                        / / . . . . / /

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  |  Immediate     |   ADC #$FF            |   $69   |    2    |    2     |
  |  ZeroPage      |   ADC $FF             |   $65   |    2    |    3     |
  |  ZeroPage,X    |   ADC $FF,X           |   $75   |    2    |    4     |
  |  Absolute      |   ADC $FFFF           |   $6D   |    3    |    4     |
  |  Absolute,X    |   ADC $FFFF,X         |   $7D   |    3    |    4*    |
  |  Absolute,Y    |   ADC $FFFF,Y         |   $79   |    3    |    4*    |
  |  (Indirect,X)  |   ADC ($FF,X)         |   $61   |    2    |    6     |
  |  (Indirect),Y  |   ADC ($FF),Y         |   $71   |    2    |    5*    |
  +----------------+-----------------------+---------+---------+----------+
  * Add 1 if page boundary is crossed.
  For penalty cycles on the 65816, check the desired addressing mode.

  Note: See CPU-Bugs for a description how flags are affected.

 65816 Extensions:

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  | AbsoluteLong   |   ADC $FFFFFF         |   $6F   |    4    |     5    |
  | AbsoluteLong,X |   ADC $FFFFFF,X       |   $7F   |    4    |     5    |
  | (Indirect)     |   ADC ($FF)           |   $72   |    2    |     5    |
  | [Indirect Long]|   ADC [$FF]           |   $67   |    2    |     6    |
  | [Ind.Long],Y   |   ADC [$FF],Y         |   $77   |    2    |     6    |
  | Relative,S     |   ADC $FF,S           |   $63   |    2    |     4    |
  | (Indirect,S),Y |   ADC ($FF,S),Y       |   $73   |    2    |     7    |
  +----------------+-----------------------+---------+---------+----------+


  AND                  "AND" memory with accumulator                    AND

  Operation:  A /\ M -> A                               N V - B D I Z C
                                                        / . . . . . / .

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  |  Immediate     |   AND #$FF            |   $29   |    2    |    2     |
  |  ZeroPage      |   AND $FF             |   $25   |    2    |    3     |
  |  ZeroPage,X    |   AND $FF,X           |   $35   |    2    |    4     |
  |  Absolute      |   AND $FFFF           |   $2D   |    3    |    4     |
  |  Absolute,X    |   AND $FFFF,X         |   $3D   |    3    |    4*    |
  |  Absolute,Y    |   AND $FFFF,Y         |   $39   |    3    |    4*    |
  |  (Indirect,X)  |   AND ($FF,X)         |   $21   |    2    |    6     |
  |  (Indirect),Y  |   AND ($FF),Y         |   $31   |    2    |    5*    |
  +----------------+-----------------------+---------+---------+----------+
  * Add 1 if page boundary is crossed.
  For penalty cycles on the 65816, check the desired addressing mode.

 65816 Extensions:

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  | AbsoluteLong   |   AND $FFFFFF         |   $2F   |    4    |     5    |
  | AbsoluteLong,X |   AND $FFFFFF,X       |   $3F   |    4    |     5    |
  | (Indirect)     |   AND ($FF)           |   $32   |    2    |     5    |
  | [Indirect Long]|   AND [$FF]           |   $27   |    2    |     6    |
  | [Ind.Long],Y   |   AND [$FF],Y         |   $37   |    2    |     6    |
  | Relative,S     |   AND $FF,S           |   $23   |    2    |     4    |
  | (Indirect,S),Y |   AND ($FF,S),Y       |   $33   |    2    |     7    |
  +----------------+-----------------------+---------+---------+----------+


  ASL          ASL Shift Left One Bit (Memory or Accumulator)           ASL

                   +-+-+-+-+-+-+-+-+
  Operation:  C <- |7|6|5|4|3|2|1|0| <- 0               N V - B D I Z C
                   +-+-+-+-+-+-+-+-+                    / . . . . . / /

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  |  Accumulator   |   ASL A               |   $0A   |    1    |    2     |
  |  ZeroPage      |   ASL $FF             |   $06   |    2    |    5     |
  |  ZeroPage,X    |   ASL $FF,X           |   $16   |    2    |    6     |
  |  Absolute      |   ASL $FFFF           |   $0E   |    3    |    6     |
  |  Absolute,X    |   ASL $FFFF,X         |   $1E   |    3    |    7     |
  +----------------+-----------------------+---------+---------+----------+
  For penalty cycles on the 65816, check the desired addressing mode.


  BCC                     BCC Branch on Carry Clear                     BCC

  Operation:  Branch on C = 0                           N V - B D I Z C
                                                        . . . . . . . .

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  |  Relative      |   BCC $FFFF           |   $90   |    2    |    2*    |
  +----------------+-----------------------+---------+---------+----------+
  * Add 1 if branch occurs to same page.
  * Add 2 if branch occurs to different page (see explanation).


  BCS                      BCS Branch on carry set                      BCS

  Operation:  Branch on C = 1                           N V - B D I Z C
                                                        . . . . . . . .

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  |  Relative      |   BCS $FFFF           |   $B0   |    2    |    2*    |
  +----------------+-----------------------+---------+---------+----------+
  * Add 1 if branch occurs to same page.
  * Add 2 if branch occurs to different page (see explanation).


  BEQ                    BEQ Branch on result zero                      BEQ

  Operation:  Branch on Z = 1                           N V - B D I Z C
                                                        . . . . . . . .

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  |  Relative      |   BEQ $FFFF           |   $F0   |    2    |    2*    |
  +----------------+-----------------------+---------+---------+----------+
  * Add 1 if branch occurs to same page.
  * Add 2 if branch occurs to different page (see explanation).


  BIT             BIT Test bits in memory with accumulator              BIT

  Operation:  A /\ M -> Z, M7 -> N, M6 -> V             N V - B D I Z C
                                                        M7M6. . . . / .
  Bit 6 and 7 are transferred to the status register.
  If the result of A /\ M is zero then Z = 1, otherwise Z=0.

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  |  ZeroPage      |   BIT $FF             |   $24   |    2    |    3     |
  |  Absolute      |   BIT $FFFF           |   $2C   |    3    |    4     |
  +----------------+-----------------------+---------+---------+----------+
  For penalty cycles on the 65816, check the desired addressing mode.

 65816 Extensions:

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  | Immediate      |   BIT #$FF            |   $89   |    2    |    2     |
  | Direct Page,X  |   BIT $FF,X           |   $34   |    2    |    3     |
  | Absolute,X     |   BIT $FFFF,X         |   $3C   |    3    |    4     |
  +----------------+-----------------------+---------+---------+----------+


  BMI                    BMI Branch on result minus                     BMI

  Operation:  Branch on N = 1                           N V - B D I Z C
                                                        . . . . . . . .

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  |  Relative      |   BMI $FFFF           |   $30   |    2    |    2*    |
  +----------------+-----------------------+---------+---------+----------+
  * Add 1 if branch occurs to same page.
  * Add 2 if branch occurs to different page (see explanation).


  BNE                   BNE Branch on result not zero                   BNE

  Operation:  Branch on Z = 0                           N V - B D I Z C
                                                        . . . . . . . .

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  |  Relative      |   BNE $FFFF           |   $D0   |    2    |    2*    |
  +----------------+-----------------------+---------+---------+----------+
  * Add 1 if branch occurs to same page.
  * Add 2 if branch occurs to different page (see explanation).


  BPL                     BPL Branch on result plus                     BPL

  Operation:  Branch on N = 0                           N V - B D I Z C
                                                        . . . . . . . .

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  |  Relative      |   BPL $FFFF           |   $10   |    2    |    2*    |
  +----------------+-----------------------+---------+---------+----------+
  * Add 1 if branch occurs to same page.
  * Add 2 if branch occurs to different page (see explanation).


  BRK                          BRK Force Break                          BRK

  Operation:  Forced Interrupt;                         N V - B D I Z C
              PC + 2 to Stack, P to Stack               . . . 1 0 1 . .

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  |  Implied/Stack |   BRK                 |   $00   |    1    |    7     |
  +----------------+-----------------------+---------+---------+----------+
  For penalty cycles on the 65816, check the desired addressing mode.

  1. A BRK command cannot be masked by setting I.
  2. Although even official documents say BRK uses implied addressing-mode,
     it is rather an immediate one, but the operand is thrown away. This is
     why PC+2 is pushed onto stack rather than PC+1. So, better use a NOP
     directly after BRK to avoid trouble.
  3. To be very precise: The B-flag is not set by BRK. It is rather set all
     the time, only when an external IRQ is recognized it will be cleared for
     some cycles, so you can distinguish between BRK and IRQ.


  BVC                   BVC Branch on overflow clear                    BVC

  Operation:  Branch on V = 0                           N V - B D I Z C
                                                        . . . . . . . .

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  |  Relative      |   BVC $FFFF           |   $50   |    2    |    2*    |
  +----------------+-----------------------+---------+---------+----------+
  * Add 1 if branch occurs to same page.
  * Add 2 if branch occurs to different page (see explanation).


  BVS                    BVS Branch on overflow set                     BVS

  Operation:  Branch on V = 1                           N V - B D I Z C
                                                        . . . . . . . .

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  |  Relative      |   BVS $FFFF           |   $70   |    2    |    2*    |
  +----------------+-----------------------+---------+---------+----------+
  * Add 1 if branch occurs to same page.
  * Add 2 if branch occurs to different page (see explanation).


  CLC                       CLC Clear carry flag                        CLC

  Operation:  0 -> C                                    N V - B D I Z C
                                                        . . . . . . . 0

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  |  Implied       |   CLC                 |   $18   |    1    |    2     |
  +----------------+-----------------------+---------+---------+----------+


  CLD                      CLD Clear decimal mode                       CLD

  Operation:  0 -> D                                    N V - B D I Z C
                                                        . . . . 0 . . .

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  |  Implied       |   CLD                 |   $D8   |    1    |    2     |
  +----------------+-----------------------+---------+---------+----------+


  CLI                  CLI Clear interrupt disable bit                  CLI

  Operation: 0 -> I                                     N V - B D I Z C
                                                        . . . . . 0 . .

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  |  Implied       |   CLI                 |   $58   |    1    |    2     |
  +----------------+-----------------------+---------+---------+----------+


  CLV                      CLV Clear overflow flag                      CLV

  Operation: 0 -> V                                     N V - B D I Z C
                                                        . 0 . . . . . .

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  |  Implied       |   CLV                 |   $B8   |    1    |    2     |
  +----------------+-----------------------+---------+---------+----------+


  CMP                CMP Compare memory and accumulator                 CMP

  Operation:  A - M                                     N V - B D I Z C
                                                        / . . . . . / /

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  |  Immediate     |   CMP #$FF            |   $C9   |    2    |    2     |
  |  ZeroPage      |   CMP $FF             |   $C5   |    2    |    3     |
  |  ZeroPage,X    |   CMP $FF,X           |   $D5   |    2    |    4     |
  |  Absolute      |   CMP $FFFF           |   $CD   |    3    |    4     |
  |  Absolute,X    |   CMP $FFFF,X         |   $DD   |    3    |    4*    |
  |  Absolute,Y    |   CMP $FFFF,Y         |   $D9   |    3    |    4*    |
  |  (Indirect,X)  |   CMP ($FF,X)         |   $C1   |    2    |    6     |
  |  (Indirect),Y  |   CMP ($FF),Y         |   $D1   |    2    |    5*    |
  +----------------+-----------------------+---------+---------+----------+
  * Add 1 if page boundary is crossed.
  For penalty cycles on the 65816, check the desired addressing mode.

 65816 Extensions:

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  | AbsoluteLong   |   CMP $FFFFFF         |   $CF   |    4    |     5    |
  | AbsoluteLong,X |   CMP $FFFFFF,X       |   $DF   |    4    |     5    |
  | (Indirect)     |   CMP ($FF)           |   $D2   |    2    |     5    |
  | [Indirect Long]|   CMP [$FF]           |   $C7   |    2    |     6    |
  | [Ind.Long],Y   |   CMP [$FF],Y         |   $D7   |    2    |     6    |
  | Relative,S     |   CMP $FF,S           |   $C3   |    2    |     4    |
  | (Indirect,S),Y |   CMP ($FF,S),Y       |   $D3   |    2    |     7    |
  +----------------+-----------------------+---------+---------+----------+


  CPX                  CPX Compare Memory and Index X                   CPX

  Operation:  X - M                                     N V - B D I Z C
                                                        / . . . . . / /

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  |  Immediate     |   CPX #$FF            |   $E0   |    2    |    2     |
  |  ZeroPage      |   CPX $FF             |   $E4   |    2    |    3     |
  |  Absolute      |   CPX $FFFF           |   $EC   |    3    |    4     |
  +----------------+-----------------------+---------+---------+----------+
  For penalty cycles on the 65816, check the desired addressing mode.


  CPY                  CPY Compare memory and index Y                   CPY

  Operation:  Y - M                                     N V - B D I Z C
                                                        / . . . . . / /

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  |  Immediate     |   CPY #$FF            |   $C0   |    2    |    2     |
  |  ZeroPage      |   CPY $FF             |   $C4   |    2    |    3     |
  |  Absolute      |   CPY $FFFF           |   $CC   |    3    |    4     |
  +----------------+-----------------------+---------+---------+----------+
  For penalty cycles on the 65816, check the desired addressing mode.


  DEC                   DEC Decrement memory by one                     DEC

  Operation:  M - 1 -> M                                N V - B D I Z C
                                                        / . . . . . / .

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  |  ZeroPage      |   DEC $FF             |   $C6   |    2    |    5     |
  |  ZeroPage,X    |   DEC $FF,X           |   $D6   |    2    |    6     |
  |  Absolute      |   DEC $FFFF           |   $CE   |    3    |    6     |
  |  Absolute,X    |   DEC $FFFF,X         |   $DE   |    3    |    7     |
  +----------------+-----------------------+---------+---------+----------+
  For penalty cycles on the 65816, check the desired addressing mode.

 65816 Extensions:

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  | Implied        |   DEC                 |   $3A   |    1    |    2     |
  +----------------+-----------------------+---------+---------+----------+


  DEX                   DEX Decrement index X by one                    DEX

  Operation:  X - 1 -> X                                N V - B D I Z C
                                                        / . . . . . / .

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  |  Implied       |   DEX                 |   $CA   |    1    |    2     |
  +----------------+-----------------------+---------+---------+----------+


  DEY                   DEY Decrement index Y by one                    DEY

  Operation:  Y - 1 -> Y                                N V - B D I Z C
                                                        / . . . . . / .

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  |  Implied       |   DEY                 |   $88   |    1    |    2     |
  +----------------+-----------------------+---------+---------+----------+


  EOR            EOR "Exclusive-Or" memory with accumulator             EOR

  Operation:  A EOR M -> A                              N V - B D I Z C
                                                        / . . . . . / .

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  |  Immediate     |   EOR #$FF            |   $49   |    2    |    2     |
  |  ZeroPage      |   EOR $FF             |   $45   |    2    |    3     |
  |  ZeroPage,X    |   EOR $FF,X           |   $55   |    2    |    4     |
  |  Absolute      |   EOR $FFFF           |   $4D   |    3    |    4     |
  |  Absolute,X    |   EOR $FFFF,X         |   $5D   |    3    |    4*    |
  |  Absolute,Y    |   EOR $FFFF,Y         |   $59   |    3    |    4*    |
  |  (Indirect,X)  |   EOR ($FF,X)         |   $41   |    2    |    6     |
  |  (Indirect),Y  |   EOR ($FF),Y         |   $51   |    2    |    5*    |
  +----------------+-----------------------+---------+---------+----------+
  * Add 1 if page boundary is crossed.
  For penalty cycles on the 65816, check the desired addressing mode.

 65816 Extensions:

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  | AbsoluteLong   |   EOR $FFFFFF         |   $4F   |    4    |     5    |
  | AbsoluteLong,X |   EOR $FFFFFF,X       |   $5F   |    4    |     5    |
  | (Indirect)     |   EOR ($FF)           |   $52   |    2    |     5    |
  | [Indirect Long]|   EOR [$FF]           |   $47   |    2    |     6    |
  | [Ind.Long],Y   |   EOR [$FF],Y         |   $57   |    2    |     6    |
  | Relative,S     |   EOR $FF,S           |   $43   |    2    |     4    |
  | (Indirect,S),Y |   EOR ($FF,S),Y       |   $53   |    2    |     7    |
  +----------------+-----------------------+---------+---------+----------+


  INC                    INC Increment memory by one                    INC

  Operation:  M + 1 -> M                                N V - B D I Z C
                                                        / . . . . . / .

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  |  ZeroPage      |   INC $FF             |   $E6   |    2    |    5     |
  |  ZeroPage,X    |   INC $FF,X           |   $F6   |    2    |    6     |
  |  Absolute      |   INC $FFFF           |   $EE   |    3    |    6     |
  |  Absolute,X    |   INC $FFFF,X         |   $FE   |    3    |    7     |
  +----------------+-----------------------+---------+---------+----------+
  For penalty cycles on the 65816, check the desired addressing mode.

 65816 Extensions:

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  | Implied        |   INC                 |   $1A   |    1    |    2     |
  +----------------+-----------------------+---------+---------+----------+


  INX                    INX Increment Index X by one                   INX

  Operation:  X + 1 -> X                                N V - B D I Z C
                                                        / . . . . . / .

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  |  Implied       |   INX                 |   $E8   |    1    |    2     |
  +----------------+-----------------------+---------+---------+----------+


  INY                    INY Increment Index Y by one                   INY

  Operation:  X + 1 -> X                                N V - B D I Z C
                                                        / . . . . . / .

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  |  Implied       |   INY                 |   $C8   |    1    |    2     |
  +----------------+-----------------------+---------+---------+----------+


  JMP                     JMP Jump to new location                      JMP

  Operation:  (PC + 1) -> PCL                           N V - B D I Z C
              (PC + 2) -> PCH                           . . . . . . . .

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  |  Absolute      |   JMP $FFFF           |   $4C   |    3    |    3     |
  | (Abs.Indirect) |   JMP ($FFFF)         |   $6C   |    3    |    5     |
  +----------------+-----------------------+---------+---------+----------+

 65816 Extensions:

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  | (Abs.Ind.,X)   |   JMP ($FFFF,X)       |   $7C   |    3    |     6    |
  | AbsoluteLong   |   JMP $FFFFFF         |   $5C   |    4    |     4    |
  +----------------+-----------------------+---------+---------+----------+
   See also: JML


  JSR          JSR Jump to new location saving return address           JSR

  Operation:  PC + 2 to Stack, (PC + 1) -> PCL          N V - B D I Z C
                               (PC + 2) -> PCH          . . . . . . . .

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  |  Absolute      |   JSR $FFFF           |   $20   |    3    |    6     |
  +----------------+-----------------------+---------+---------+----------+

 65816 Extensions:

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  | (Abs.Ind.,X)   |   JSR ($FFFF,X)       |   $FC   |    3    |     6    |
  +----------------+-----------------------+---------+---------+----------+
   See also: JSL


  LDA                  LDA Load accumulator with memory                 LDA

  Operation:  M -> A                                    N V - B D I Z C
                                                        / . . . . . / .

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  |  Immediate     |   LDA #$FF            |   $A9   |    2    |    2     |
  |  ZeroPage      |   LDA $FF             |   $A5   |    2    |    3     |
  |  ZeroPage,X    |   LDA $FF,X           |   $B5   |    2    |    4     |
  |  Absolute      |   LDA $FFFF           |   $AD   |    3    |    4     |
  |  Absolute,X    |   LDA $FFFF,X         |   $BD   |    3    |    4*    |
  |  Absolute,Y    |   LDA $FFFF,Y         |   $B9   |    3    |    4*    |
  |  (Indirect,X)  |   LDA ($FF,X)         |   $A1   |    2    |    6     |
  |  (Indirect),Y  |   LDA ($FF),Y         |   $B1   |    2    |    5*    |
  +----------------+-----------------------+---------+---------+----------+
  * Add 1 if page boundary is crossed.
  For penalty cycles on the 65816, check the desired addressing mode.

 65816 Extensions:

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  | AbsoluteLong   |   LDA $FFFFFF         |   $AF   |    4    |     5    |
  | AbsoluteLong,X |   LDA $FFFFFF,X       |   $BF   |    4    |     5    |
  | (Indirect)     |   LDA ($FF)           |   $B2   |    2    |     5    |
  | [Indirect Long]|   LDA [$FF]           |   $A7   |    2    |     6    |
  | [Ind.Long],Y   |   LDA [$FF],Y         |   $B7   |    2    |     6    |
  | Relative,S     |   LDA $FF,S           |   $A3   |    2    |     4    |
  | (Indirect,S),Y |   LDA ($FF,S),Y       |   $B3   |    2    |     7    |
  +----------------+-----------------------+---------+---------+----------+


  LDX                   LDX Load index X with memory                    LDX

  Operation:  M -> X                                    N V - B D I Z C
                                                        / . . . . . / .

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  |  Immediate     |   LDX #$FF            |   $A2   |    2    |    2     |
  |  ZeroPage      |   LDX $FF             |   $A6   |    2    |    3     |
  |  ZeroPage,Y    |   LDX $FF,Y           |   $B6   |    2    |    4     |
  |  Absolute      |   LDX $FFFF           |   $AE   |    3    |    4     |
  |  Absolute,Y    |   LDX $FFFF,Y         |   $BE   |    3    |    4*    |
  +----------------+-----------------------+---------+---------+----------+
  * Add 1 when page boundary is crossed.
  For penalty cycles on the 65816, check the desired addressing mode.


  LDY                   LDY Load index Y with memory                    LDY

  Operation:  M -> Y                                    N V - B D I Z C
                                                        / . . . . . / .

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  |  Immediate     |   LDY #$FF            |   $A0   |    2    |    2     |
  |  ZeroPage      |   LDY $FF             |   $A4   |    2    |    3     |
  |  ZeroPage,X    |   LDY $FF,X           |   $B4   |    2    |    4     |
  |  Absolute      |   LDY $FFFF           |   $AC   |    3    |    4     |
  |  Absolute,X    |   LDY $FFFF,X         |   $BC   |    3    |    4*    |
  +----------------+-----------------------+---------+---------+----------+
  * Add 1 when page boundary is crossed.
  For penalty cycles on the 65816, check the desired addressing mode.


  LSR          LSR Shift right one bit (memory or accumulator)          LSR

                   +-+-+-+-+-+-+-+-+
  Operation:  0 -> |7|6|5|4|3|2|1|0| -> C               N V - B D I Z C
                   +-+-+-+-+-+-+-+-+                    0 . . . . . / /

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  |  Accumulator   |   LSR A               |   $4A   |    1    |    2     |
  |  ZeroPage      |   LSR $FF             |   $46   |    2    |    5     |
  |  ZeroPage,X    |   LSR $FF,X           |   $56   |    2    |    6     |
  |  Absolute      |   LSR $FFFF           |   $4E   |    3    |    6     |
  |  Absolute,X    |   LSR $FFFF,X         |   $5E   |    3    |    7     |
  +----------------+-----------------------+---------+---------+----------+
  For penalty cycles on the 65816, check the desired addressing mode.


  NOP                         NOP No operation                          NOP

  Operation:  No Operation                              N V - B D I Z C
                                                        . . . . . . . .

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  |  Implied       |   NOP                 |   $EA   |    1    |    2     |
  +----------------+-----------------------+---------+---------+----------+

 Illegal Versions:

  Can be used like BIT to skip the next one or two bytes,
  but this one doesn't change flags.

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  |  Implied       |   NOP                 |   $1A   |    1    |    2     |
  |  Implied       |   NOP                 |   $3A   |    1    |    2     |
  |  Implied       |   NOP                 |   $5A   |    1    |    2     |
  |  Implied       |   NOP                 |   $7A   |    1    |    2     |
  |  Implied       |   NOP                 |   $DA   |    1    |    2     |
  |  Implied       |   NOP                 |   $FA   |    1    |    2     |
  |  Immediate     |   NOP #$FF            |   $80   |    2    |    2     |
  |  Immediate     |   NOP #$FF            |   $82   |    2    |    2     |
  |  Immediate     |   NOP #$FF            |   $89   |    2    |    2     |
  |  Immediate     |   NOP #$FF            |   $C2   |    2    |    2     |
  |  Immediate     |   NOP #$FF            |   $E2   |    2    |    2     |
  |  ZeroPage      |   NOP $FF             |   $04   |    2    |    3     |
  |  ZeroPage      |   NOP $FF             |   $44   |    2    |    3     |
  |  ZeroPage      |   NOP $FF             |   $64   |    2    |    3     |
  |  ZeroPage,X    |   NOP $FF,X           |   $14   |    2    |    4     |
  |  ZeroPage,X    |   NOP $FF,X           |   $34   |    2    |    4     |
  |  ZeroPage,X    |   NOP $FF,X           |   $54   |    2    |    4     |
  |  ZeroPage,X    |   NOP $FF,X           |   $74   |    2    |    4     |
  |  ZeroPage,X    |   NOP $FF,X           |   $D4   |    2    |    4     |
  |  ZeroPage,X    |   NOP $FF,X           |   $F4   |    2    |    4     |
  |  Absolute      |   NOP $FFFF           |   $0C   |    3    |    4     |
  |  Absolute,X    |   NOP $FFFF,X         |   $1C   |    3    |    4*    |
  |  Absolute,X    |   NOP $FFFF,X         |   $3C   |    3    |    4*    |
  |  Absolute,X    |   NOP $FFFF,X         |   $5C   |    3    |    4*    |
  |  Absolute,X    |   NOP $FFFF,X         |   $7C   |    3    |    4*    |
  |  Absolute,X    |   NOP $FFFF,X         |   $DC   |    3    |    4*    |
  |  Absolute,X    |   NOP $FFFF,X         |   $FC   |    3    |    4*    |
  +----------------+-----------------------+---------+---------+----------+
  * Add 1 if page boundary is crossed.


  ORA                 ORA "OR" memory with accumulator                  ORA

  Operation: A V M -> A                                 N V - B D I Z C
                                                        / . . . . . / .

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  |  Immediate     |   ORA #$FF            |   $09   |    2    |    2     |
  |  ZeroPage      |   ORA $FF             |   $05   |    2    |    3     |
  |  ZeroPage,X    |   ORA $FF,X           |   $15   |    2    |    4     |
  |  Absolute      |   ORA $FFFF           |   $0D   |    3    |    4     |
  |  Absolute,X    |   ORA $FFFF,X         |   $1D   |    3    |    4*    |
  |  Absolute,Y    |   ORA $FFFF,Y         |   $19   |    3    |    4*    |
  |  (Indirect,X)  |   ORA ($FF,X)         |   $01   |    2    |    6     |
  |  (Indirect),Y  |   ORA ($FF),Y         |   $11   |    2    |    5*    |
  +----------------+-----------------------+---------+---------+----------+
  * Add 1 on page crossing
  For penalty cycles on the 65816, check the desired addressing mode.

 65816 Extensions:

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  | AbsoluteLong   |   ORA $FFFFFF         |   $0F   |    4    |     5    |
  | AbsoluteLong,X |   ORA $FFFFFF,X       |   $1F   |    4    |     5    |
  | (Indirect)     |   ORA ($FF)           |   $12   |    2    |     5    |
  | [Indirect Long]|   ORA [$FF]           |   $07   |    2    |     6    |
  | [Ind.Long],Y   |   ORA [$FF],Y         |   $17   |    2    |     6    |
  | Relative,S     |   ORA $FF,S           |   $03   |    2    |     4    |
  | (Indirect,S),Y |   ORA ($FF,S),Y       |   $13   |    2    |     7    |
  +----------------+-----------------------+---------+---------+----------+


  PHA                   PHA Push accumulator on stack                   PHA

  Operation:  A to Stack                                N V - B D I Z C
                                                        . . . . . . . .

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  |  Implied/Stack |   PHA                 |   $48   |    1    |    3     |
  +----------------+-----------------------+---------+---------+----------+
  For penalty cycles on the 65816, check the desired addressing mode.


  PHP                 PHP Push processor status on stack                PHP

  Operation:  P to Stack                                N V - B D I Z C
                                                        . . . . . . . .

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  |  Implied/Stack |   PHP                 |   $08   |    1    |    3     |
  +----------------+-----------------------+---------+---------+----------+


  PLA                 PLA Pull accumulator from stack                   PLA

  Operation:  A from Stack                              N V - B D I Z C
                                                        / . . . . . / .

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  |  Implied/Stack |   PLA                 |   $68   |    1    |    4     |
  +----------------+-----------------------+---------+---------+----------+
  For penalty cycles on the 65816, check the desired addressing mode.


  PLP               PLP Pull processor status from stack                PLP

  Operation:  P from Stack                              N V - B D I Z C
                                                          From Stack

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  |  Implied/Stack |   PLP                 |   $28   |    1    |    4     |
  +----------------+-----------------------+---------+---------+----------+


  ROL          ROL Rotate one bit left (memory or accumulator)          ROL

               +------------------------------+
               |                              |
               |   +-+-+-+-+-+-+-+-+    +-+   |
  Operation:   +-< |7|6|5|4|3|2|1|0| <- |C| <-+         N V - B D I Z C
                   +-+-+-+-+-+-+-+-+    +-+             / . . . . . / /

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  |  Accumulator   |   ROL A               |   $2A   |    1    |    2     |
  |  ZeroPage      |   ROL $FF             |   $26   |    2    |    5     |
  |  ZeroPage,X    |   ROL $FF,X           |   $36   |    2    |    6     |
  |  Absolute      |   ROL $FFFF           |   $2E   |    3    |    6     |
  |  Absolute,X    |   ROL $FFFF,X         |   $3E   |    3    |    7     |
  +----------------+-----------------------+---------+---------+----------+
  For penalty cycles on the 65816, check the desired addressing mode.


  ROR          ROR Rotate one bit right (memory or accumulator)         ROR

               +------------------------------+
               |                              |
               |   +-+    +-+-+-+-+-+-+-+-+   |
  Operation:   +-> |C| -> |7|6|5|4|3|2|1|0| >-+         N V - B D I Z C
                   +-+    +-+-+-+-+-+-+-+-+             / . . . . . / /

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  |  Accumulator   |   ROR A               |   $6A   |    1    |    2     |
  |  ZeroPage      |   ROR $FF             |   $66   |    2    |    5     |
  |  ZeroPage,X    |   ROR $FF,X           |   $76   |    2    |    6     |
  |  Absolute      |   ROR $FFFF           |   $6E   |    3    |    6     |
  |  Absolute,X    |   ROR $FFFF,X         |   $7E   |    3    |    7     |
  +----------------+-----------------------+---------+---------+----------+
  For penalty cycles on the 65816, check the desired addressing mode.

    Note: ROR instruction is available on MCS650X microprocessors after
          June, 1976.


  RTI                    RTI Return from interrupt                      RTI

  Operation:  P from Stack, PC from Stack               N V - B D I Z C
                                                          From Stack

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  |  Implied/Stack |   RTI                 |   $40   |    1    |    6     |
  +----------------+-----------------------+---------+---------+----------+


  RTS                    RTS Return from subroutine                     RTS

  Operation:  PC from Stack, PC + 1 -> PC               N V - B D I Z C
                                                        . . . . . . . .

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  |  Implied/Stack |   RTS                 |   $60   |    1    |    6     |
  +----------------+-----------------------+---------+---------+----------+
   See also: RTL


  SBC          SBC Subtract memory from accumulator with borrow         SBC

  Operation:  A - M - ~C -> A                           N V - B D I Z C
         -                                              / / . . . . / /
    Note:C = Borrow
  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  |  Immediate     |   SBC #$FF            |   $E9   |    2    |    2     |
  |  ZeroPage      |   SBC $FF             |   $E5   |    2    |    3     |
  |  ZeroPage,X    |   SBC $FF,X           |   $F5   |    2    |    4     |
  |  Absolute      |   SBC $FFFF           |   $ED   |    3    |    4     |
  |  Absolute,X    |   SBC $FFFF,X         |   $FD   |    3    |    4*    |
  |  Absolute,Y    |   SBC $FFFF,Y         |   $F9   |    3    |    4*    |
  |  (Indirect,X)  |   SBC ($FF,X)         |   $E1   |    2    |    6     |
  |  (Indirect),Y  |   SBC ($FF),Y         |   $F1   |    2    |    5*    |
  +----------------+-----------------------+---------+---------+----------+
  * Add 1 when page boundary is crossed.
  For penalty cycles on the 65816, check the desired addressing mode.

  Note: See CPU-Bugs for a description how flags are affected.

 Illegal Version:

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  |  Immediate     |   SBC #$FF            |   $EB   |    2    |    2     |
  +----------------+-----------------------+---------+---------+----------+
  There doesn't seem to be a difference to the legal SBC.

 65816 Extensions:

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  | AbsoluteLong   |   SBC $FFFFFF         |   $EF   |    4    |     5    |
  | AbsoluteLong,X |   SBC $FFFFFF,X       |   $FF   |    4    |     5    |
  | (Indirect)     |   SBC ($FF)           |   $F2   |    2    |     5    |
  | [Indirect Long]|   SBC [$FF]           |   $E7   |    2    |     6    |
  | [Ind.Long],Y   |   SBC [$FF],Y         |   $F7   |    2    |     6    |
  | Relative,S     |   SBC $FF,S           |   $E3   |    2    |     4    |
  | (Indirect,S),Y |   SBC ($FF,S),Y       |   $F3   |    2    |     7    |
  +----------------+-----------------------+---------+---------+----------+


  SEC                        SEC Set carry flag                         SEC

  Operation:  1 -> C                                    N V - B D I Z C
                                                        . . . . . . . 1

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  |  Implied       |   SEC                 |   $38   |    1    |    2     |
  +----------------+-----------------------+---------+---------+----------+


  SED                       SED Set decimal mode                        SED

  Operation:  1 -> D                                    N V - B D I Z C
                                                        . . . . 1 . . .

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  |  Implied       |   SED                 |   $F8   |    1    |    2     |
  +----------------+-----------------------+---------+---------+----------+


  SEI                 SEI Set interrupt disable status                  SEI

  Operation:  1 -> I                                    N V - B D I Z C
                                                        . . . . . 1 . .

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  |  Implied       |   SEI                 |   $78   |    1    |    2     |
  +----------------+-----------------------+---------+---------+----------+


  STA                  STA Store accumulator in memory                  STA

  Operation:  A -> M                                    N V - B D I Z C
                                                        . . . . . . . .

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  |  ZeroPage      |   STA $FF             |   $85   |    2    |    3     |
  |  ZeroPage,X    |   STA $FF,X           |   $95   |    2    |    4     |
  |  Absolute      |   STA $FFFF           |   $8D   |    3    |    4     |
  |  Absolute,X    |   STA $FFFF,X         |   $9D   |    3    |    5     |
  |  Absolute,Y    |   STA $FFFF,Y         |   $99   |    3    |    5     |
  |  (Indirect,X)  |   STA ($FF,X)         |   $81   |    2    |    6     |
  |  (Indirect),Y  |   STA ($FF),Y         |   $91   |    2    |    6     |
  +----------------+-----------------------+---------+---------+----------+
  For penalty cycles on the 65816, check the desired addressing mode.

 65816 Extensions:

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  | AbsoluteLong   |   STA $FFFFFF         |   $8F   |    4    |     5    |
  | AbsoluteLong,X |   STA $FFFFFF,X       |   $9F   |    4    |     5    |
  | (Indirect)     |   STA ($FF)           |   $92   |    2    |     5    |
  | [Indirect Long]|   STA [$FF]           |   $87   |    2    |     6    |
  | [Ind.Long],Y   |   STA [$FF],Y         |   $97   |    2    |     6    |
  | Relative,S     |   STA $FF,S           |   $83   |    2    |     4    |
  | (Indirect,S),Y |   STA ($FF,S),Y       |   $93   |    2    |     7    |
  +----------------+-----------------------+---------+---------+----------+


  STX                    STX Store index X in memory                    STX

  Operation: X -> M                                     N V - B D I Z C
                                                        . . . . . . . .

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  |  ZeroPage      |   STX $FF             |   $86   |    2    |    3     |
  |  ZeroPage,Y    |   STX $FF,Y           |   $96   |    2    |    4     |
  |  Absolute      |   STX $FFFF           |   $8E   |    3    |    4     |
  +----------------+-----------------------+---------+---------+----------+
  For penalty cycles on the 65816, check the desired addressing mode.


  STY                    STY Store index Y in memory                    STY

  Operation: Y -> M                                     N V - B D I Z C
                                                        . . . . . . . .

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  |  ZeroPage      |   STY $FF             |   $84   |    2    |    3     |
  |  ZeroPage,X    |   STY $FF,X           |   $94   |    2    |    4     |
  |  Absolute      |   STY $FFFF           |   $8C   |    3    |    4     |
  +----------------+-----------------------+---------+---------+----------+
  For penalty cycles on the 65816, check the desired addressing mode.


  TAX                TAX Transfer accumulator to index X                TAX

  Operation:  A -> X                                    N V - B D I Z C
                                                        / . . . . . / .

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  |  Implied       |   TAX                 |   $AA   |    1    |    2     |
  +----------------+-----------------------+---------+---------+----------+


  TAY                TAY Transfer accumulator to index Y                TAY

  Operation:  A -> Y                                    N V - B D I Z C
                                                        / . . . . . / .

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  |  Implied       |   TAY                 |   $A8   |    1    |    2     |
  +----------------+-----------------------+---------+---------+----------+


  TSX              TSX Transfer stack pointer to index X                TSX

  Operation:  S -> X                                    N V - B D I Z C
                                                        / . . . . . / .

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  |  Implied       |   TSX                 |   $BA   |    1    |    2     |
  +----------------+-----------------------+---------+---------+----------+


  TXA                TXA Transfer index X to accumulator                TXA

  Operation:  X -> A                                    N V - B D I Z C
                                                        / . . . . . / .

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  |  Implied       |   TXA                 |   $8A   |    1    |    2     |
  +----------------+-----------------------+---------+---------+----------+

  TXS              TXS Transfer index X to stack pointer                TXS

  Operation:  X -> S                                    N V - B D I Z C
                                                        . . . . . . . .

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  |  Implied       |   TXS                 |   $9A   |    1    |    2     |
  +----------------+-----------------------+---------+---------+----------+


  TYA                TYA Transfer index Y to accumulator                TYA

  Operation:  Y -> A                                    N V - B D I Z C
                                                        / . . . . . / .

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  |  Implied       |   TYA                 |   $98   |    1    |    2     |
  +----------------+-----------------------+---------+---------+----------+

```