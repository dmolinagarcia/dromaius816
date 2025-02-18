# CENTRAL PROCESSING UNIT (CPU) 65816 Exclusive Instruction set

```
  BRL                       BRL Branch Always Long                      BRL

 65816 only!
                                                        N V M X D I Z C
  Operation:  PC+r -> PC where -32768 < r < 32767       . . . . . . . .

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  | RelativeLong   |   BRL $FFFFFF         |   $82   |    3    |     3    |
  +----------------+-----------------------+---------+---------+----------+


  COP                          COP Coprocessor                          COP

 65816 only!
                                                        N V M X D I Z C
  Operation:  Coprocessor Instruction                   . . . . 0 1 . .

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  | Immediate/Stack|   COP #$FF            |   $02   |    2    |    7/8   |
  +----------------+-----------------------+---------+---------+----------+
  For penalty cycles on the 65816, check the desired addressing mode.


  JML                           JML Jump Long                           JML

 65816 only!
                                                        N V M X D I Z C
  Operation:  W -> PC, B -> PB                          . . . . . . . .

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  | (Abs.Ind.Long) |   JML ($FFFF)         |   $DC   |    3    |     6    |
  +----------------+-----------------------+---------+---------+----------+


  JSL                      JSL Jump Subroutine Long                     JSL

 65816 only!
                                                        N V M X D I Z C
  Operation:  PB -> Bs, S-1 -S, PC -> Ws, S-2 -> S,     . . . . . . . .
              W -> PC,  B -> PB

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  | AbsoluteLong   |   JSL $FFFFFF         |   $22   |    4    |     8    |
  +----------------+-----------------------+---------+---------+----------+
   Note: JSL is equivalent to JSR with long addressing forced.


  MVN                      MVN Block Move Negative                      MVN

 65816 only!
                                                        N V M X D I Z C
  Operation:                                            . . . . . . . .

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  | Block Negative |   MVN $FF,$FF         |   $54   |    3    |  7/byte  |
  +----------------+-----------------------+---------+---------+----------+
   X-Register = Source Address
   Y-Register = Destination Address
   Accumulator = Number of Bytes to move -1
   $FF,$FF = Source Bank, Destination Bank

   Attention: In memory the order of the Bank-bytes is the other way
   around (i.e. Dest, Src)! Some assemblers use this order as syntax,
   although it is not correct.

   Keep also in mind that the Data-Bank-Register will be set to Dest-Bank.

   MVN is used when the destination start address is lower (more negative)
   than the source start address.


  MVP                      MVP Block Move Positive                      MVP

 65816 only!
                                                        N V M X D I Z C
  Operation:                                            . . . . . . . .

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  | Block Positive |   MVP $FF,$FF         |   $44   |    3    |  7/byte  |
  +----------------+-----------------------+---------+---------+----------+
   X-Register = Source Address
   Y-Register = Destination Address
   Accumulator = Number of Bytes to move -1
   $FF,$FF = Source Bank, Destination Bank

   Attention: In memory the order of the Bank-bytes is the other way
   around (i.e. Dest, Src)! Some assemblers use this order as syntax,
   although it is not correct.

   Keep also in mind that the Data-Bank-Register will be set to Dest-Bank.

   MVP is used when the destination start address is higher (more positive)
   than the source start address.


  REP                       REP Reset Status Bits                       REP

 65816 only!
                                                        N V M X D I Z C
  Operation:  P /\ /B -> P                              / / / / / / / /

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  | Immediate      |   REP #$FF            |   $C2   |    2    |     3    |
  +----------------+-----------------------+---------+---------+----------+


  RTL                  RTL Return from Subroutine Long                  RTL

 65816 only!
                                                        N V M X D I Z C
  Operation:  S+2 -> S, Ws+1 -> PC, S+1 -> S, Bs -> PB  . . . . . . . .

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  | Implied/Stack  |   RTL                 |   $6B   |    1    |     6    |
  +----------------+-----------------------+---------+---------+----------+


  SEP                   SEP Set Processor Status Bits                   SEP

 65816 only!
                                                        N V M X D I Z C
  Operation:  P V B -> P                                / / / / / / / /

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  | Immediate      |   SEP #$FF            |   $E2   |    2    |     3    |
  +----------------+-----------------------+---------+---------+----------+


  STP                         STP Stop the Clock                        STP

 65816 only!
                                                        N V M X D I Z C
  Operation:  Stop the clock. Needs reset to continue.  . . . . . . . .

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  | Implied        |   STP                 |   $DB   |    1    |    3+    |
  +----------------+-----------------------+---------+---------+----------+


  WAI                       WAI Wait for Interrupt                      WAI

 65816 only!
                                                        N V M X D I Z C
  Operation:  Wait for interrupt. RDY held low until    . . . . . . . .
              Interrupt.

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  | Implied        |   WAI                 |   $CB   |    1    |    3+    |
  +----------------+-----------------------+---------+---------+----------+


  WDM                 WDM Reserved for Future Expansion                 WDM

 65816 only!
                                                        N V M X D I Z C
  Operation:  At the moment treated like NOP.           . . . . . . . .

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  | Immediate      |   WDM #$FF            |   $42   |    2    |     2    |
  +----------------+-----------------------+---------+---------+----------+
  WDM is the first byte of a multi-byte instruction set to be built in future
  versions of the processor. This instruction should NOT be used by now.
  (WDM -> William D. Mensch jr., 65816 designer)


  XBA                       XBA Exchange AH and AL                      XBA

 65816 only!
                                                        N V M X D I Z C
  Operation:  Swap AH and AL. Status bits reflect       / . . . . . / .
              final condition of AL.

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  | Implied        |   XBA                 |   $EB   |    1    |     3    |
  +----------------+-----------------------+---------+---------+----------+


  BRA                         BRA Branch Always                         BRA

 65816 only!
                                                        N V M X D I Z C
  Operation:  PC+r -> PC                                . . . . . . . .

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  | Relative       |   BRA $FFFF           |   $80   |    2    |     2    |
  +----------------+-----------------------+---------+---------+----------+


  PEA            PEA Push Effective Absolute Address on Stack           PEA
                     (or Push Immediate Data on Stack)

 65816 only!
                                                        N V M X D I Z C
  Operation:  W -> Ws, S-2 ->S                          . . . . . . . .

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  | Immediate/Stack|   PEA #$FFFF          |   $F4   |    3    |     5    |
  +----------------+-----------------------+---------+---------+----------+

  Note: Sometimes PEA is said to have Absolute-addressing-mode. But it
  definately behaves like Immediate.


  PEI            PEI Push Effective Indirect Address on Stack           PEI

 65816 only!
                                                        N V M X D I Z C
  Operation:  W -> Ws, S-2 ->S                          . . . . . . . .

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  |(Indirect)/Stack|   PEI ($FF)           |   $D4   |    2    |     6    |
  +----------------+-----------------------+---------+---------+----------+
  Add one cycle if low byte of direct page is <>0.


  PER    PER Push Effective Program Counter Relative Address on Stack   PER

 65816 only!
                                                        N V M X D I Z C
  Operation:  W -> Ws, S-2 ->S                          . . . . . . . .

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  | RelativeLong/S |   PER $FFFFFF         |   $62   |    3    |     6    |
  +----------------+-----------------------+---------+---------+----------+


  PHB                PHB Push Data Bank Register on Stack               PHB

 65816 only!
                                                        N V M X D I Z C
  Operation:  DB->Bs, S-1 ->S                           . . . . . . . .

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  | Implied/Stack  |   PHB                 |   $8B   |    1    |     3    |
  +----------------+-----------------------+---------+---------+----------+


  PHD                 PHD Push Direct Register on Stack                 PHD

 65816 only!
                                                        N V M X D I Z C
  Operation:  D ->Ws, S-2 ->S                           . . . . . . . .

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  | Implied/Stack  |   PHD                 |   $0B   |    1    |     4    |
  +----------------+-----------------------+---------+---------+----------+


  PHK              PHK Push Program Bank Register on Stack              PHK

 65816 only!
                                                        N V M X D I Z C
  Operation:  PB->Bs, S-1 ->S                           . . . . . . . .

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  | Implied/Stack  |   PHK                 |   $4B   |    1    |     3    |
  +----------------+-----------------------+---------+---------+----------+


  PHX                     PHX Push Index X on Stack                     PHX

 65816 only!
                                                        N V M X D I Z C
  Operation:  X-Ws, S-2 -> S                            . . . . . . . .

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  | Implied/Stack  |   PHX                 |   $DA   |    1    |    3/4   |
  +----------------+-----------------------+---------+---------+----------+
  For penalty cycles on the 65816, check the desired addressing mode.


  PHY                     PHY Push index Y on Stack                     PHY

 65816 only!
                                                        N V M X D I Z C
  Operation:  Y ->Ws, S-2 ->S                           . . . . . . . .

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  | Implied/Stack  |   PHY                 |   $5A   |    1    |    3/4   |
  +----------------+-----------------------+---------+---------+----------+
  For penalty cycles on the 65816, check the desired addressing mode.


  PLB               PLB Pull Data Bank Register from Stack              PLB

 65816 only!
                                                        N V M X D I Z C
  Operation:  S+1 ->S, Bs -> DB                         / . . . . . / .

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  | Implied/Stack  |   PLB                 |   $AB   |    1    |     4    |
  +----------------+-----------------------+---------+---------+----------+


  PLD                PLD Pull Direct Register from Stack                PLD

 65816 only!
                                                        N V M X D I Z C
  Operation:  S+2 ->S, Ws -> D                          / . . . . . / .

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  | Implied/Stack  |   PLD                 |   $2B   |    1    |     5    |
  +----------------+-----------------------+---------+---------+----------+


  PLX                    PLX Pull Index X from Stack                    PLX

 65816 only!
                                                        N V M X D I Z C
  Operation:  S+2 ->S, Ws->X                            / . . . . . / .

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  | Implied/Stack  |   PLX                 |   $FA   |    1    |    4/5   |
  +----------------+-----------------------+---------+---------+----------+
  For penalty cycles on the 65816, check the desired addressing mode.


  PLY                    PLY Pull Index Y form Stack                    PLY

 65816 only!
                                                        N V M X D I Z C
  Operation:  S+2 ->S, Ws->Y                            / . . . . . / .

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  | Implied/Stack  |   PLY                 |   $7A   |    1    |    4/5   |
  +----------------+-----------------------+---------+---------+----------+
  For penalty cycles on the 65816, check the desired addressing mode.


  TCD            TCD Transfer Accumulator to Direct Register            TCD

 65816 only!
                                                        N V M X D I Z C
  Operation:  A -> D                                    / . . . . . / .

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  | Implied        |   TCD                 |   $5B   |    1    |     2    |
  +----------------+-----------------------+---------+---------+----------+


  TCS         TCS Transfer Accumulator to Stack Pointer Register        TCS

 65816 only!
                                                        N V M X D I Z C
  Operation:  A -> S                                    . . . . . . . .

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  | Implied        |   TCS                 |   $1B   |    1    |    2/    |
  +----------------+-----------------------+---------+---------+----------+


  TDC            TDC Transfer Direct Register to Accumulator            TDC

 65816 only!
                                                        N V M X D I Z C
  Operation:  D -> A                                    / . . . . . / .

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  | Implied        |   TDC                 |   $7B   |    1    |     2    |
  +----------------+-----------------------+---------+---------+----------+


  TSC         TSC Transfer Stack Pointer Register to Accumulator        TSC

 65816 only!
                                                        N V M X D I Z C
  Operation:  S -> A                                    / . . . . . / .

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  | Implied        |   TSC                 |   $3B   |    1    |     2    |
  +----------------+-----------------------+---------+---------+----------+


  TXY                  TXY Transfer Index X to Index Y                  TXY

 65816 only!
                                                        N V M X D I Z C
  Operation:  X -> Y                                    / . . . . . / .

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  | Implied        |   TXY                 |   $9B   |    1    |     2    |
  +----------------+-----------------------+---------+---------+----------+


  TYX                  TYX Transfer Index Y to Index X                  TYX

 65816 only!
                                                        N V M X D I Z C
  Operation:  Y -> X                                    / . . . . . / .

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  | Implied        |   TYX                 |   $BB   |    1    |     2    |
  +----------------+-----------------------+---------+---------+----------+


  XCE               XCE Exchange Carry and Emulation Bits               XCE

 65816 only!
                                                        N V M X D I Z C
  Operation:                                            . . . . . . . /

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  | Implied        |   XCE                 |   $FB   |    1    |     2    |
  +----------------+-----------------------+---------+---------+----------+


  STZ                      STZ Store zero in memory                     STZ

 65816 only!
                                                        N V M X D I Z C
  Operation:  0 -> M                                    . . . . . . . .

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  | Absolute       |   STZ $FFFF           |   $9C   |    3    |    4     |
  | Absolute,X     |   STZ $FFFF,X         |   $9E   |    3    |    5     |
  | Direct Page    |   STZ $FF             |   $64   |    2    |    3     |
  | Direct Page,X  |   STZ $FF,X           |   $74   |    2    |    4     |
  +----------------+-----------------------+---------+---------+----------+
  For penalty cycles on the 65816, check the desired addressing mode.


  TSB                       TSB Test and Set Bit                        TSB

 65816 only!
                                                        N V M X D I Z C
  Operation:  A V W -> W                                . . . . . . / .

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  | Absolute       |   TSB $FFFF           |   $0C   |    3    |    4     |
  | Direct Page    |   TSB $FF             |   $04   |    2    |    3     |
  +----------------+-----------------------+---------+---------+----------+
  For penalty cycles on the 65816, check the desired addressing mode.


  TRB                      TRB Test and Reset Bit                       TRB

 65816 only!
                                                        N V M X D I Z C
  Operation:  /A /\ W -> W                              . . . . . . / .

  +----------------+-----------------------+---------+---------+----------+
  | Addressing Mode| Assembly Language Form| OP CODE |No. Bytes|No. Cycles|
  +----------------+-----------------------+---------+---------+----------+
  | Absolute       |   TRB $FFFF           |   $1C   |    3    |    4     |
  | Direct Page    |   TRB $FF             |   $14   |    2    |    3     |
  +----------------+-----------------------+---------+---------+----------+
  For penalty cycles on the 65816, check the desired addressing mode.
```