# INSTRUCTION TIMING

```
 (Note: Data is gained fro4m 65816-documentation, but it is downward
        compatible. For the 6510 just ignore specs which do not apply.)

 6510/65816 Addressing modes:

  Absolute -- a
  Absolute (JMP) -- a
  Absolute (JSR) -- a
  Absolute (R-M-W) -- a
  Absolute Indexed -- a,x  a,y
  Absolute Indexed (R-M-W) -- a,x
  Absolute Indexed Indirect -- (a,x)
  Absolute Indirect -- (a)
  Accumulator -- A
  Immediate -- #
  Implied -- i
  Relative -- r
  Stack (Hardware Interrupts) -- s
  Stack (Software Interrupts) -- s
  Stack (Pull) -- s
  Stack (Push) -- s
  Stack (RTI) -- s
  Stack (RTS) -- s
  Zeropage/Direct -- d
  Zeropage/Direct (R-M-W) -- d
  Zeropage/Direct Indexed -- d,x  d,y
  Zeropage/Direct Indexed (R-M-W) -- d,x
  Zeropage/Direct Indexed Indirect -- (d,x)
  Zeropage/Direct Indirect Indexed -- (d),y

 6510 Illegal Addressing modes:

  Absolute Indexed (R-M-W) -- a,y
  Zeropage Indexed Indirect (R-M-W) -- (d,x)
  Zeropage Indirect Indexed (R-M-W) -- (d),y

 65816 Addressing modes:

  Absolute Indexed Indirect (JSR) -- (a,x)
  Absolute Indirect Long (JML) -- (a)
  Absolute Long -- al
  Absolute Long Indexed -- al,x
  Absolute Long (JMP) -- al
  Absolute Long (JSL) -- al
  Block Move Negative (backward) -- xyc
  Block Move Positive (forward) -- xyc
  Direct Indirect -- (d)
  Direct Indirect Long -- [d]
  Direct Indirect Indexed Long -- [d],y
  Implied (XBA) -- i
  Relative Long -- rl
  Stack (PEA) -- s
  Stack (PEI) -- s
  Stack (PER) -- s
  Stack (RTL) -- s
  Stack Relative -- d,s
  Stack Relative Indirect Indexed -- (d,s),y
  Stop the Clock (STP) -- h
  Wait for Interrupt (WAI) -- h


 6510/65816 Addressing mode: Immediate -- #

 (ADC,ANC,AND,ANE,ARR,ASR,BIT,CMP,CPX,CPY,EOR,LDA,LDX,LDY,LXA,NOP,ORA,
  REP,SBC,SBX,SEP)
 (2 and 3 bytes)  (2 and 3 cycles)

    +---------------+------------------+-----------------------+----------+
    |     Cycle     |   Address Bus    |       Data Bus        |Read/Write|
    +---------------+------------------+-----------------------+----------+
    |           1   |  PBR,PC          | Op Code               |    R     |
    |           2   |  PBR,PC+1        | Immediate Data Low    |    R     |
    |       (1) 2a  |  PBR,PC+2        | Immediate Data High   |    R     |
    +---------------+------------------+-----------------------+----------+
    (1) Add 1 cycle and 1 byte for M=0 or X=0 (i.e. 16 bit data).

    See also: Abbreviations


 6510/65816 Addressing mode: Absolute -- a

 (ADC,AND,BIT,CMP,CPX,CPY,EOR,LAX,LDA,LDX,LDY,NOP,ORA,SAX,SBC,STA,STX,
  STY,STZ)
 (3 bytes)  (4 and 5 cycles)

    +---------------+------------------+-----------------------+----------+
    |     Cycle     |   Address Bus    |       Data Bus        |Read/Write|
    +---------------+------------------+-----------------------+----------+
    |           1   |  PBR,PC          | Op Code               |    R     |
    |           2   |  PBR,PC+1        | Absolute Address Low  |    R     |
    |           3   |  PBR,PC+2        | Absolute Address High |    R     |
    |           4   |  DBR,AA          | Data Low              |   R/W    |
    |       (1) 4a  |  DBR,AA+1        | Data High             |   R/W    |
    +---------------+------------------+-----------------------+----------+
    (1) Add 1 cycle for M=0 or X=0 (i.e. 16 bit data).

    See also: Abbreviations


 6510/65816 Addressing mode: Absolute (R-M-W) -- a

 (ASL,DCP,DEC,INC,ISB,LSR,RLA,ROL,ROR,RRA,SLO,SRE,TRB,TSB)
 (3 bytes)  (6 and 8 cycles)

    +---------------+------------------+-----------------------+----------+
    |     Cycle     |   Address Bus    |       Data Bus        |Read/Write|
    +---------------+------------------+-----------------------+----------+
    |           1   |  PBR,PC          | Op Code               |    R     |
    |           2   |  PBR,PC+1        | Absolute Address Low  |    R     |
    |           3   |  PBR,PC+2        | Absolute Address High |    R     |
    |           4   |  DBR,AA          | Data Low              |    R     |
    |       (1) 4a  |  DBR,AA+1        | Data High             |    R     |
    |   (12)(3) 5   |  DBR,AA+2        | Internal Operation    |    R     |
    |       (1) 6a  |  DBR,AA+1        | Data High             |    W     |
    |           6   |  DBR,AA          | Data Low              |    W     |
    +---------------+------------------+-----------------------+----------+
    (1) Add 1 cycle for M=0 or X=0 (i.e. 16 bit data).
    (3) Special case for aborting instruction. This is the last cycle which
        may be aborted or the Status, PBR or DBR registers will be updated.
   (12) Unmodified Data Low is written back to memory in 6502 emulation
        mode (E=1).

    See also: Abbreviations


 6510/65816 Addressing mode: Absolute (JMP) -- a

 (JMP)
 (3 bytes)  (3 cycles)

    +---------------+------------------+-----------------------+----------+
    |     Cycle     |   Address Bus    |       Data Bus        |Read/Write|
    +---------------+------------------+-----------------------+----------+
    |           1   |  PBR,PC          | Op Code               |    R     |
    |           2   |  PBR,PC+1        | NEW PCL               |    R     |
    |           3   |  PBR,PC+2        | NEW PCH               |    R     |
    |           1   |  PBR,NEWPC       | New Op Code           |    R     |
    +---------------+------------------+-----------------------+----------+

    See also: Abbreviations


 6510/65816 Addressing mode: Absolute (JSR) -- a

 (JSR)
 (3 bytes)  (6 cycles)

 On 6510:
    +---------------+------------------+-----------------------+----------+
    |     Cycle     |   Address Bus    |       Data Bus        |Read/Write|
    +---------------+------------------+-----------------------+----------+
    |           1   |  PBR,PC          | Op Code               |    R     |
    |           2   |  PBR,PC+1        | NEW PCL               |    R     |
    |           3   |  0,S             | Internal Operation    |    R     |
    |           4   |  0,S             | Program Counter High  |    W     |
    |           5   |  0,S-1           | Program Counter Low   |    W     |
    |           6   |  PBR,PC+2        | NEW PCH               |    R     |
    |           1   |  PBR,NEWPC       | New Op Code           |    R     |
    +---------------+------------------+-----------------------+----------+

 On 65816:
    +---------------+------------------+-----------------------+----------+
    |     Cycle     |   Address Bus    |       Data Bus        |Read/Write|
    +---------------+------------------+-----------------------+----------+
    |           1   |  PBR,PC          | Op Code               |    R     |
    |           2   |  PBR,PC+1        | NEW PCL               |    R     |
    |           3   |  PBR,PC+2        | NEW PCH               |    R     |
    |           4   |  PBR,PC+2        | Internal Operation    |    R     |
    |           5   |  0,S             | Program Counter High  |    W     |
    |           6   |  0,S-1           | Program Counter Low   |    W     |
    |           1   |  PBR,NEWPC       | New Op Code           |    R     |
    +---------------+------------------+-----------------------+----------+

    See also: Abbreviations


 6510/65816 Addressing mode: Absolute Long -- al

 (ADC,AND,CMP,EOR,LDA,ORA,SBC,STA)
 (4 bytes)  (5 and 6 cycles)

    +---------------+------------------+-----------------------+----------+
    |     Cycle     |   Address Bus    |       Data Bus        |Read/Write|
    +---------------+------------------+-----------------------+----------+
    |           1   |  PBR,PC          | Op Code               |    R     |
    |           2   |  PBR,PC+1        | Absolute Address Low  |    R     |
    |           3   |  PBR,PC+2        | Absolute Address High |    R     |
    |           4   |  PBR,PC+3        | Absolute Address Bank |    R     |
    |           5   |  AAB,AA          | Data Low              |   R/W    |
    |       (1) 5a  |  AAB,AA+1        | Data High             |   R/W    |
    +---------------+------------------+-----------------------+----------+
    (1) Add 1 cycle for M=0 or X=0 (i.e. 16 bit data).

    See also: Abbreviations


 6510/65816 Addressing mode: Absolute Long (JMP) -- al

 (JMP)
 (4 bytes)  (4 cycles)

    +---------------+------------------+-----------------------+----------+
    |     Cycle     |   Address Bus    |       Data Bus        |Read/Write|
    +---------------+------------------+-----------------------+----------+
    |           1   |  PBR,PC          | Op Code               |    R     |
    |           2   |  PBR,PC+1        | NEW PCL               |    R     |
    |           3   |  PBR,PC+2        | NEW PCH               |    R     |
    |           4   |  PBR,PC+3        | NEW PBR               |    R     |
    |           1   |  NEW PBR,PC      | New Op Code           |    R     |
    +---------------+------------------+-----------------------+----------+

    See also: Abbreviations


 6510/65816 Addressing mode: Absolute Long (JSL) -- al

 (JSL)
 (4 bytes)  (7 cycles)

    +---------------+------------------+-----------------------+----------+
    |     Cycle     |   Address Bus    |       Data Bus        |Read/Write|
    +---------------+------------------+-----------------------+----------+
    |           1   |  PBR,PC          | Op Code               |    R     |
    |           2   |  PBR,PC+1        | NEW PCL               |    R     |
    |           3   |  PBR,PC+2        | NEW PCH               |    R     |
    |           4   |  0,S             | Program Bank Register |    W     |
    |           5   |  0,S             | Internal Operation    |    R     |
    |           6   |  PBR,PC+3        | NEW PBR               |    R     |
    |           7   |  0,S-1           | Program Counter High  |    W     |
    |           8   |  0,S-2           | Program Counter Low   |    W     |
    |           1   |  NEW PBR,PC      | New Op Code           |    R     |
    +---------------+------------------+-----------------------+----------+

    See also: Abbreviations


 6510/65816 Addressing mode: Zeropage/Direct -- d

 (ADC,AND,BIT,CMP,CPX,CPY,EOR,LAX,LDA,LDX,LDY,NOP,ORA,SAX,SBC,STA,STX,
  STY,STZ)
 (2 bytes)  (3,4 and 5 cycles)

    +---------------+------------------+-----------------------+----------+
    |     Cycle     |   Address Bus    |       Data Bus        |Read/Write|
    +---------------+------------------+-----------------------+----------+
    |           1   |  PBR,PC          | Op Code               |    R     |
    |           2   |  PBR,PC+1        | Direct Offset         |    R     |
    |       (2) 2a  |  PBR,PC+2        | Internal Operation    |    R     |
    |           3   |  0,D+DO          | Data Low              |   R/W    |
    |       (1) 3a  |  0,D+DO+1        | Data High             |   R/W    |
    +---------------+------------------+-----------------------+----------+
    (1) Add 1 cycle for M=0 or X=0 (i.e. 16 bit data).
    (2) Add 1 cycle for direct register low (DL) not equal 0.

    See also: Abbreviations


 6510/65816 Addressing mode: Zeropage/Direct (R-M-W) -- d

 (ASL,DCP,DEC,INC,ISB,LSR,RLA,ROL,ROR,RRA,SLO,SRE,TRB,TSB)
 (2 bytes)  (5,6,7 and 8 cycles)

    +---------------+------------------+-----------------------+----------+
    |     Cycle     |   Address Bus    |       Data Bus        |Read/Write|
    +---------------+------------------+-----------------------+----------+
    |           1   |  PBR,PC          | Op Code               |    R     |
    |           2   |  PBR,PC+1        | Direct Offset         |    R     |
    |       (2) 3a  |  PBR,PC+1        | Internal Operation    |    R     |
    |           3   |  0,D+DO          | Data Low              |    R     |
    |       (1) 3a  |  0,D+DO+1        | Data High             |    R     |
    |   (12)(3) 4   |  0,D+DO+1        | Internal Operation    |    R     |
    |       (1) 5a  |  0,D+D0+1        | Data High             |    W     |
    |           5   |  0,D+DO          | Data Low              |    W     |
    +---------------+------------------+-----------------------+----------+
    (1) Add 1 cycle for M=0 or X=0 (i.e. 16 bit data).
    (2) Add 1 cycle for direct register low (DL) not equal 0.
    (3) Special case for aborting instruction. This is the last cycle which
        may be aborted or the Status, PBR or DBR registers will be updated.
   (12) Unmodified Data Low is written back to memory in 6502 emulation
        mode (E=1).

    See also: Abbreviations


 6510/65816 Addressing mode: Accumulator -- A

 (ASL,DEC,INC,LSR,ROL,ROR)
 (1 byte)  (2 cycles)

    +---------------+------------------+-----------------------+----------+
    |     Cycle     |   Address Bus    |       Data Bus        |Read/Write|
    +---------------+------------------+-----------------------+----------+
    |           1   |  PBR,PC          | Op Code               |    R     |
    |           2   |  PBR,PC+1        | Internal Operation    |    R     |
    +---------------+------------------+-----------------------+----------+

    See also: Abbreviations


 6510/65816 Addressing mode: Implied -- i

 (CLC,CLD,CLI,CLV,DEX,DEY,INX,INY,JAM,NOP,SEC,SED,SEI,TAX,TAY,TCD,TCS,
  TDC,TSC,TSX,TXA,TXS,TXY,TYA,TYX,XCE)
 (1 byte)  (2 cycles)

    +---------------+------------------+-----------------------+----------+
    |     Cycle     |   Address Bus    |       Data Bus        |Read/Write|
    +---------------+------------------+-----------------------+----------+
    |           1   |  PBR,PC          | Op Code               |    R     |
    |           2   |  PBR,PC+1        | Internal Operation    |    R     |
    +---------------+------------------+-----------------------+----------+

    See also: Abbreviations


 65816 Addressing mode: Implied (XBA) -- i

 (XBA)
 (1 byte)  (3 cycles)

    +---------------+------------------+-----------------------+----------+
    |     Cycle     |   Address Bus    |       Data Bus        |Read/Write|
    +---------------+------------------+-----------------------+----------+
    |           1   |  PBR,PC          | Op Code               |    R     |
    |           2   |  PBR,PC+1        | Internal Operation    |    R     |
    |           3   |  PBR,PC+1        | Internal Operation    |    R     |
    +---------------+------------------+-----------------------+----------+

    See also: Abbreviations


 65816 Addressing mode: Wait for Interrupt -- h

 (WAI)
 (1 byte)  (3 cycles)

    +---------------+------------------+-----------------------+----------+
    |     Cycle     |   Address Bus    |       Data Bus        |Read/Write|
    +---------------+------------------+-----------------------+----------+
    |           1   |  PBR,PC          | Op Code               |    R     |
    |       (9) 2   |  PBR,PC+1        | Internal Operation    |    R     |
    |           3   |  PBR,PC+1        | Internal Operation    |    R     |
    |   IRQ,NMI 1   |  PBR,PC+1        | IRQ(BRK)              |    R     |
    +---------------+------------------+-----------------------+----------+
    (9) Wait at cycle 2 for 2 cycles after /NMI or /IRQ active input.

    See also: Abbreviations


 65816 Addressing mode: Stop the Clock -- h

 (STP)
 (1 byte)  (3 cycles)

    +---------------+------------------+-----------------------+----------+
    |     Cycle     |   Address Bus    |       Data Bus        |Read/Write|
    +---------------+------------------+-----------------------+----------+
    |           1   |  PBR,PC          | Op Code               |    R     |
    |           2   |  PBR,PC+1        | Internal Operation    |    R     |
    |   RES=1   3   |  PBR,PC+1        | Internal Operation    |    R     |
    |   RES=0   1c  |  PBR,PC+1        | RES(BRK)              |    R     |
    |   RES=0   1b  |  PBR,PC+1        | RES(BRK)              |    R     |
    |   RES=1   1a  |  PBR,PC+1        | RES(BRK)              |    R     |
    |           1   |  PBR,PC+1        | BEGIN                 |    R     |
    +---------------+------------------+-----------------------+----------+

    See also: Abbreviations
              Hardware interrupt


 6510/65816 Addressing mode: Zeropage/Direct Indirect Indexed -- (d),y

 (ADC,AND,CMP,EOR,LAX,LDA,ORA,SBC,STA)
 (2 bytes)  (5,6,7 and 8 cycles)

    +---------------+------------------+-----------------------+----------+
    |     Cycle     |   Address Bus    |       Data Bus        |Read/Write|
    +---------------+------------------+-----------------------+----------+
    |           1   |  PBR,PC          | Op Code               |    R     |
    |           2   |  PBR,PC+1        | Direct Offset         |    R     |
    |       (2) 2a  |  PBR,PC+1        | Internal Operation    |    R     |
    |           3   |  0,D+DO          | Absolute Address Low  |    R     |
    |           4   |  0,D+DO+1        | Absolute Address High |    R     |
    |       (4) 4a  |  DBR,AAH,AAL+YL  | Internal Operation    |    R     |
    |           5   |  DBR,AA+Y        | Data Low              |   R/W    |
    |       (1) 5a  |  DBR,AA+Y+1      | Data High             |   R/W    |
    +---------------+------------------+-----------------------+----------+
    (1) Add 1 cycle for M=0 or X=0 (i.e. 16 bit data).
    (2) Add 1 cycle for direct register low (DL) not equal 0.
    (4) Add 1 cycle for indexing across page boundaries, or write, or X=0.
        When X=1 or in the emulation mode, this cycle contains invalid
        addresses.

    See also: Abbreviations


 6510 Illegal Addressing mode: Direct Indexed Indirect (R-M-W) -- (d),y

 (DCP,ISB,RLA,RRA,SLO,SRE)
 (2 bytes)  (7,8 cycles)

    +---------------+------------------+-----------------------+----------+
    |     Cycle     |   Address Bus    |       Data Bus        |Read/Write|
    +---------------+------------------+-----------------------+----------+
    |           1   |  PBR,PC          | Op Code               |    R     |
    |           2   |  PBR,PC+1        | Direct Offset         |    R     |
    |           3   |  0,D+DO          | Absolute Address Low  |    R     |
    |           4   |  0,D+DO+1        | Absolute Address High |    R     |
    |       (4) 4a  |  DBR,AAH,AAL+YL  | Internal Operation    |    R     |
    |           5   |  DBR,AA+Y        | Data Low              |    R     |
    |           6   |  DBR,AA          | Old Data Low          |    W     |
    |           7   |  DBR,AA          | New Data Low          |    W     |
    +---------------+------------------+-----------------------+----------+
    (4) Add 1 cycle for indexing across page boundaries or write.

    See also: Abbreviations


 6510/65816 Addressing mode: Zeropage/Direct Indirect Indexed Long -- [d],y

 (ADC,AND,CMP,EOR,LDA,ORA,SBC,STA)
 (2 bytes)  (6,7 and 8 cycles)

    +---------------+------------------+-----------------------+----------+
    |     Cycle     |   Address Bus    |       Data Bus        |Read/Write|
    +---------------+------------------+-----------------------+----------+
    |           1   |  PBR,PC          | Op Code               |    R     |
    |           2   |  PBR,PC+1        | Direct Offset         |    R     |
    |       (2) 2a  |  PBR,PC+1        | Internal Operation    |    R     |
    |           3   |  0,D+DO          | Absolute Address Low  |    R     |
    |           4   |  0,D+DO+1        | Absolute Address High |    R     |
    |           5   |  0,D+DO+2        | Absolute Address Bank |    R     |
    |           6   |  AAB,AA+Y        | Data Low              |   R/W    |
    |       (1) 6a  |  AAB,AA+Y+1      | Data High             |   R/W    |
    +---------------+------------------+-----------------------+----------+
    (1) Add 1 cycle for M=0 or X=0 (i.e. 16 bit data).
    (2) Add 1 cycle for direct register low (DL) not equal 0.

    See also: Abbreviations


 6510/65816 Addressing mode: Zeropage/Direct Indexed Indirect -- (d,x)

 (ADC,AND,CMP,EOR,LAX,LDA,ORA,SAX,SBC,STA)
 (2 bytes)  (6,7 and 8 cycles)

    +---------------+------------------+-----------------------+----------+
    |     Cycle     |   Address Bus    |       Data Bus        |Read/Write|
    +---------------+------------------+-----------------------+----------+
    |           1   |  PBR,PC          | Op Code               |    R     |
    |           2   |  PBR,PC+1        | Direct Offset         |    R     |
    |       (2) 2a  |  PBR,PC+1        | Internal Operation    |    R     |
    |           3   |  PBR,PC+1        | Internal Operation    |    R     |
    |           4   |  0,D+DO+X        | Absolute Address Low  |    R     |
    |           5   |  0,D+DO+X+1      | Absolute Address High |    R     |
    |           6   |  DBR,AA          | Data Low              |   R/W    |
    |       (1) 6a  |  DBR,AA+1        | Data High             |   R/W    |
    +---------------+------------------+-----------------------+----------+
    (1) Add 1 cycle for M=0 or X=0 (i.e. 16 bit data).
    (2) Add 1 cycle for direct register low (DL) not equal 0.

    See also: Abbreviations


 6510 Illegal Addressing mode: Direct Indexed Indirect (R-M-W) -- (d,x)

 (DCP,ISB,RLA,RRA,SLO,SRE)
 (2 bytes)  (8 cycles)

    +---------------+------------------+-----------------------+----------+
    |     Cycle     |   Address Bus    |       Data Bus        |Read/Write|
    +---------------+------------------+-----------------------+----------+
    |           1   |  PBR,PC          | Op Code               |    R     |
    |           2   |  PBR,PC+1        | Direct Offset         |    R     |
    |           3   |  PBR,PC+1        | Internal Operation    |    R     |
    |           4   |  0,D+DO+X        | Absolute Address Low  |    R     |
    |           5   |  0,D+DO+X+1      | Absolute Address High |    R     |
    |           6   |  DBR,AA          | Data Low              |    R     |
    |           7   |  DBR,AA          | Old Data Low          |    W     |
    |           8   |  DBR,AA          | New Data Low          |    W     |
    +---------------+------------------+-----------------------+----------+

    See also: Abbreviations


 6510/65816 Addressing mode: Zeropage/Direct Indexed -- d,x  d,y

 (ADC,AND,BIT,CMP,EOR,LAX,LDA,LDX,LDY,NOP,ORA,SAX,SBC,STA,STX,STY,STZ)

 (2 bytes)  (4,5 and 6 cycles)

    +---------------+------------------+-----------------------+----------+
    |     Cycle     |   Address Bus    |       Data Bus        |Read/Write|
    +---------------+------------------+-----------------------+----------+
    |           1   |  PBR,PC          | Op Code               |    R     |
    |           2   |  PBR,PC+1        | Direct Offset         |    R     |
    |       (2) 2a  |  PBR,PC+1        | Internal Operation    |    R     |
    |           3   |  PBR,PC+1        | Internal Operation    |    R     |
    |           4   |  0,D+DO+I        | Data Low              |   R/W    |
    |       (1) 4a  |  0,D+DO+I+1      | Data High             |   R/W    |
    +---------------+------------------+-----------------------+----------+
    (1) Add 1 cycle for M=0 or X=0 (i.e. 16 bit data).
    (2) Add 1 cycle for direct register low (DL) not equal 0.

    See also: Abbreviations


 6510/65816 Addressing mode: Zeropage/Direct Indexed (R-M-W) -- d,x

 (ASL,DCP,DEC,INC,ISB,LSR,RLA,ROL,ROR,RRA,SLO,SRE)
 (2 bytes)  (6,7,8 and 9 cycles)

    +---------------+------------------+-----------------------+----------+
    |     Cycle     |   Address Bus    |       Data Bus        |Read/Write|
    +---------------+------------------+-----------------------+----------+
    |           1   |  PBR,PC          | Op Code               |    R     |
    |           2   |  PBR,PC+1        | Direct Offset         |    R     |
    |       (2) 2a  |  PBR,PC+1        | Internal Operation    |    R     |
    |           3   |  PBR,PC+1        | Internal Operation    |    R     |
    |           4   |  0,D+DO+X        | Data Low              |    R     |
    |       (1) 4a  |  0,D+DO+X+1      | Data High             |    R     |
    |   (12)(3) 5   |  0,D+DO+X+1      | Internal Operation    |    R     |
    |       (1) 6a  |  0,D+DO+X+1      | Data High             |    W     |
    |           6   |  0,D+DO+X        | Data Low              |    W     |
    +---------------+------------------+-----------------------+----------+
    (1) Add 1 cycle for M=0 or X=0 (i.e. 16 bit data).
    (2) Add 1 cycle for direct register low (DL) not equal 0.
    (3) Special case for aborting instruction. This is the last cycle which
        may be aborted or the Status, PBR or DBR registers will be updated.
   (12) Unmodified Data Low is written back to memory in 6502 emulation
        mode (E=1).

    See also: Abbreviations


 6510/65816 Addressing mode: Absolute Indexed -- a,x  a,y

 (ADC,AND,BIT,CMP,EOR,LAE,LAX,LDA,LDX,LDY,NOP,ORA,SBC,SHA,SHS,SHX,SHY,
  STA,STZ)
 (3 bytes)  (4,5 and 6 cycles)

    +---------------+------------------+-----------------------+----------+
    |     Cycle     |   Address Bus    |       Data Bus        |Read/Write|
    +---------------+------------------+-----------------------+----------+
    |           1   |  PBR,PC          | Op code               |    R     |
    |           2   |  PBR,PC+1        | Absolute Address Low  |    R     |
    |           3   |  PBR,PC+2        | Absolute Address High |    R     |
    |       (4) 3a  |  DBR,AAH,AAL+IL  | Internal Operation    |    R     |
    |           4   |  DBR,AA+I        | Data Low              |   R/W    |
    |       (1) 4a  |  DBR,AA+I+1      | Data High             |   R/W    |
    +---------------+------------------+-----------------------+----------+
    (1) Add 1 cycle for M=0 or X=0 (i.e. 16 bit data).
    (4) Add 1 cycle for indexing across page boundaries, or write, or X=0.
        When X=1 or in the emulation mode, this cycle contains invalid
        addresses.

    See also: Abbreviations


 6510/65816 Addressing mode: Absolute Indexed (R-M-W) -- a,x
 6510 Illegal Addressing mode: Absolute Indexed (R-M-W) -- a,y

 (ASL,DCP,DEC,INC,ISB,LSR,RLA,ROL,ROR,RRA,SLO,SRE)
 (3 bytes)  (7 and 9 cycles)

    +---------------+------------------+-----------------------+----------+
    |     Cycle     |   Address Bus    |       Data Bus        |Read/Write|
    +---------------+------------------+-----------------------+----------+
    |           1   |  PBR,PC          | Op Code               |    R     |
    |           2   |  PBR,PC+1        | Absolute Address Low  |    R     |
    |           3   |  PBR,PC+2        | Absolute Address High |    R     |
    |           4   |  DBR,AAH,AAL+XL  | Internal Operation    |    R     |
    |           5   |  DBR,AA+X        | Data Low              |    R     |
    |       (1) 5a  |  DBR,AA+X+1      | Data High             |    R     |
    |   (12)(3) 6   |  DBR,AA+X+1      | Internal Operation    |    R     |
    |       (1) 7a  |  DBR,AA+X+1      | Data High             |    W     |
    |           7   |  DBR,AA+X        | Data Low              |    W     |
    +---------------+------------------+-----------------------+----------+
    (1) Add 1 cycle for M=0 or X=0 (i.e. 16 bit data).
    (3) Special case for aborting instruction. This is the last cycle which
        may be aborted or the Status, PBR or DBR registers will be updated.
   (12) Unmodified Data Low is written back to memory in 6502 emulation
        mode (E=1).

    See also: Abbreviations


 65816 Addressing mode: Absolute Long Indexed -- al,x

 (ADC,AND,CMP,EOR,LDA,ORA,SBC,STA)
 (4 bytes)  (5 and 6 cycles)

    +---------------+------------------+-----------------------+----------+
    |     Cycle     |   Address Bus    |       Data Bus        |Read/Write|
    +---------------+------------------+-----------------------+----------+
    |           1   |  PBR,PC          | Op Code               |    R     |
    |           2   |  PBR,PC+1        | Absolute Address Low  |    R     |
    |           3   |  PBR,PC+2        | Absolute Address High |    R     |
    |           4   |  PBR,PC+3        | Absolute Address Bank |    R     |
    |           5   |  AAB,AA+X        | Data Low              |   R/W    |
    |       (1) 5a  |  AAB,AA+X+1      | Data High             |   R/W    |
    +---------------+------------------+-----------------------+----------+
    (1) Add 1 cycle for M=0 or X=0 (i.e. 16 bit data).

    See also: Abbreviations


 6510/65816 Addressing mode: Relative -- r

 (BCC,BCS,BEQ,BMI,BNE,BPL,BRA,BVC,BVS)
 (2 bytes)  (2,3 and 4 cycles)

    +---------------+------------------+-----------------------+----------+
    |     Cycle     |   Address Bus    |       Data Bus        |Read/Write|
    +---------------+------------------+-----------------------+----------+
    |           1   |  PBR,PC          | Op Code               |    R     |
    |           2   |  PBR,PC+1        | Offset                |    R     |
    |       (5) 2a  |  PBR,PC+2        | Internal Operation    |    R     |
    |       (6) 2b  |  PBR,PC+2+OFF    | Internal Operation    |    R     |
    |           1   |  PBR,NewPC       | New Op Code           |    R     |
    +---------------+------------------+-----------------------+----------+
    (5) Add 1 cycle if branch is taken.
    (6) Add 1 cycle if branch is taken across page boundaries in 6502
        emulation mode (E=1).

    See also: Abbreviations


 65816 Addressing mode: Relative Long -- rl

 (BRL)
 (3 bytes)  (4 cycles)

    +---------------+------------------+-----------------------+----------+
    |     Cycle     |   Address Bus    |       Data Bus        |Read/Write|
    +---------------+------------------+-----------------------+----------+
    |           1   |  PBR,PC          | Op Code               |    R     |
    |           2   |  PBR,PC+1        | Offset Low            |    R     |
    |           3   |  PBR,PC+2        | Offset High           |    R     |
    |           4   |  PBR,PC+2        | Internal Operation    |    R     |
    |           1   |  PBR,NewPC       | New Op Code           |    R     |
    +---------------+------------------+-----------------------+----------+

    See also: Abbreviations


 6510/65816 Addressing mode: Absolute Indirect -- (a)

 (JMP)
 (3 bytes)  (5 cycles)

    +---------------+------------------+-----------------------+----------+
    |     Cycle     |   Address Bus    |       Data Bus        |Read/Write|
    +---------------+------------------+-----------------------+----------+
    |           1   |  PBR,PC          | Op Code               |    R     |
    |           2   |  PBR,PC+1        | Absolute Address Low  |    R     |
    |           3   |  PBR,PC+2        | Absolute Address High |    R     |
    |           4   |  0,AA            | NEW PCL               |    R     |
    |           5   |  0,AA+1          | NEW PCH               |    R     |
    |           1   |  PBR,NewPC       | New Op Code           |    R     |
    +---------------+------------------+-----------------------+----------+

    See also: Abbreviations


 65816 Addressing mode: Absolute Indirect Long (JML) -- (a)

 (JML)
 (3 bytes)  (6 cycles)

    +---------------+------------------+-----------------------+----------+
    |     Cycle     |   Address Bus    |       Data Bus        |Read/Write|
    +---------------+------------------+-----------------------+----------+
    |           1   |  PBR,PC          | Op Code               |    R     |
    |           2   |  PBR,PC+1        | Absolute Address Low  |    R     |
    |           3   |  PBR,PC+1        | Absolute Address High |    R     |
    |           4   |  0,AA            | NEW PCL               |    R     |
    |           5   |  0,AA+1          | NEW PCH               |    R     |
    |           6   |  0,AA+2          | NEW PBR               |    R     |
    |           1   |  NEW PBR,PC      | New Op Code           |    R     |
    +---------------+------------------+-----------------------+----------+

    See also: Abbreviations


 65816 Addressing mode: Direct Indirect -- (d)

 (ADC,AND,CMP,EOR,LDA,ORA,SBC,STA)
 (2 bytes)  (5,6 and 7 cycles)

    +---------------+------------------+-----------------------+----------+
    |     Cycle     |   Address Bus    |       Data Bus        |Read/Write|
    +---------------+------------------+-----------------------+----------+
    |           1   |  PBR,PC          | Op Code               |    R     |
    |           2   |  PBR,PC+1        | Direct Offset         |    R     |
    |       (2) 2a  |  PBR,PC+1        | Internal Operation    |    R     |
    |           3   |  0,D+DO          | Absolute Address Low  |    R     |
    |           1   |  0,D+DO+1        | Absolute Address High |    R     |
    |           5   |  DBR,AA          | Data Low              |   R/W    |
    |       (1) 5a  |  DBR,AA+1        | Data Low              |   R/W    |
    +---------------+------------------+-----------------------+----------+
    (1) Add 1 cycle for M=0 or X=0 (i.e. 16 bit data).
    (2) Add 1 cycle for direct register low (DL) not equal 0.

    See also: Abbreviations


 65816 Addressing mode: Direct Indirect Long -- [d]

 (ADC,AND,CMP,EOR,LDA,ORA,SBC,STA)
 (2 bytes)  (6,7 and 8 cycles)

    +---------------+------------------+-----------------------+----------+
    |     Cycle     |   Address Bus    |       Data Bus        |Read/Write|
    +---------------+------------------+-----------------------+----------+
    |           1   |  PBR,PC          | Op Code               |    R     |
    |           2   |  PBR,PC+1        | Direct Offset         |    R     |
    |       (2) 2a  |  PBR,PC+1        | Internal Operation    |    R     |
    |           3   |  0,D+DO          | Absolute Address Low  |    R     |
    |           4   |  0,D+DO+1        | Absolute Address High |    R     |
    |           5   |  0,D+DO+2        | Absolute Address Bank |    R     |
    |           6   |  AAB,AA          | Data Low              |   R/W    |
    |       (1) 6a  |  AAB,AA+1        | Data High             |   R/W    |
    +---------------+------------------+-----------------------+----------+
    (1) Add 1 cycle for M=0 or X=0 (i.e. 16 bit data).
    (2) Add 1 cycle for direct register low (DL) not equal 0.

    See also: Abbreviations


 6510/65816 Addressing mode: Absolute Indexed Indirect -- (a,x)

 (JMP)
 (3 bytes)  (6 cycles)

    +---------------+------------------+-----------------------+----------+
    |     Cycle     |   Address Bus    |       Data Bus        |Read/Write|
    +---------------+------------------+-----------------------+----------+
    |           1   |  PBR,PC          | Op Code               |    R     |
    |           2   |  PBR,PC+1        | Absolute Address Low  |    R     |
    |           3   |  PBR,PC+2        | Absolute Address High |    R     |
    |           4   |  PBR,PC+2        | Internal Operation    |    R     |
    |           5   |  PBR,AA+X        | NEW PCL               |    R     |
    |           6   |  PBR,AA+X+1      | NEW PCH               |    R     |
    |           1   |  PBR,NEWPC       | New Op Code           |    R     |
    +---------------+------------------+-----------------------+----------+

    See also: Abbreviations


 65816 Addressing mode: Absolute Indexed Indirect (JSR) -- (a,x)

 (JSR)
 (3 bytes)  (8 cycles)

    +---------------+------------------+-----------------------+----------+
    |     Cycle     |   Address Bus    |       Data Bus        |Read/Write|
    +---------------+------------------+-----------------------+----------+
    |           1   |  PBR,PC          | Op Code               |    R     |
    |           2   |  PBR,PC+1        | Absolute Address Low  |    R     |
    |           3   |  0,S             | Program Counter High  |    W     |
    |           4   |  0,S-1           | Program Counter Low   |    W     |
    |           5   |  PBR,PC+2        | Absolute Address High |    R     |
    |           6   |  PBR,PC+2        | Internal Operation    |    R     |
    |           7   |  PBR,AA+X        | NEW PCL               |    R     |
    |           8   |  PBR,AA+X+1      | NEW PCH               |    R     |
    |           1   |  PBR,NEWPC       | New Op Code           |    R     |
    +---------------+------------------+-----------------------+----------+

    See also: Abbreviations


 6510/65816 Addressing mode: Stack (Hardware Interrupts) -- s

 (IRQ,NMI,ABORT,RES)
 (0 bytes)  (7 and 8 cycles)

    +---------------+------------------+-----------------------+----------+
    |     Cycle     |   Address Bus    |       Data Bus        |Read/Write|
    +---------------+------------------+-----------------------+----------+
    |           1   |  PBR,PC          | Internal Operation    |    R     |
    |       (3) 2   |  PBR,PC          | Internal Operation    |    R     |
    |       (7) 3   |  0,S             | Program Bank Register |    W     |
    |      (10) 4   |  0,S-1           | Program Counter High  |    W     |
    |      (10) 5   |  0,S-2           | Program Counter Low   |    W     |
    |  (10)(11) 6   |  0,S-3           | Status Register       |    W     |
    |           7   |  0,VA            | Abs.Addr. Vector Low  |    R     |
    |           8   |  0,VA+1          | Abs.Addr. Vector High |    R     |
    |           1   |  0,AAV           | New Op Code           |    R     |
    +---------------+------------------+-----------------------+----------+
    (3) Special case for aborting instruction. This is the last cycle which
        may be aborted or the Status, PBR or DBR registers will be updated.
    (7) Subtract 1 cycle for 6502 emulation mode (E=1).
   (10) R/W remains high during Reset.
   (11) BRK bit 4 equals "0" in Emulation mode.

    See also: Abbreviations


 6510/65816 Addressing mode: Stack (Software Interrupts) -- s

 (BRK,COP)
 (2 bytes)  (7 and 8 cycles)

    +---------------+------------------+-----------------------+----------+
    |     Cycle     |   Address Bus    |       Data Bus        |Read/Write|
    +---------------+------------------+-----------------------+----------+
    |           1   |  PBR,PC          | Op Code               |    R     |
    |       (3) 2   |  PBR,PC+1        | Signature             |    R     |
    |       (7) 3   |  0,S             | Program Bank Register |    W     |
    |           4   |  0,S-1           | Program Counter High  |    W     |
    |           5   |  0,S-2           | Program Counter Low   |    W     |
    |           6   |  0,S-3           | (COP Latches)P        |    W     |
    |           7   |  0,VA            | Abs.Addr. Vector Low  |    R     |
    |           8   |  0,VA+1          | Abs.Addr. Vector High |    R     |
    |           1   |  0,AAV           | New Op Code           |    R     |
    +---------------+------------------+-----------------------+----------+
    (3) Special case for aborting instruction. This is the last cycle which
        may be aborted or the Status, PBR or DBR registers will be updated.
    (7) Subtract 1 cycle for 6502 emulation mode (E=1).

    See also: Abbreviations


 6510/65816 Addressing mode: Stack (RTI) -- s

 (RTI)
 (1 byte)  (6 and 7 cycles)

    +---------------+------------------+-----------------------+----------+
    |     Cycle     |   Address Bus    |       Data Bus        |Read/Write|
    +---------------+------------------+-----------------------+----------+
    |           1   |  PBR,PC          | Op Code               |    R     |
    |           2   |  PBR,PC+1        | Internal Operation    |    R     |
    |       (3) 3   |  PBR,PC+1        | Internal Operation    |    R     |
    |           4   |  0,S+1           | Status Register       |    R     |
    |           5   |  0,S+2           | New PCL               |    R     |
    |           6   |  0,S+3           | New PCH               |    R     |
    |       (7) 7   |  0,S+4           | Program Bank Register |    R     |
    |           1   |  PBR,NewPC       | New Op Code           |    R     |
    +---------------+------------------+-----------------------+----------+
    (3) Special case for aborting instruction. This is the last cycle which
        may be aborted or the Status, PBR or DBR registers will be updated.
    (7) Subtract 1 cycle for 6502 emulation mode (E=1).

    See also: Abbreviations


 6510/65816 Addressing mode: Stack (RTS) -- s

 (RTS)
 (1 byte)  (6 cycles)

    +---------------+------------------+-----------------------+----------+
    |     Cycle     |   Address Bus    |       Data Bus        |Read/Write|
    +---------------+------------------+-----------------------+----------+
    |           1   |  PBR,PC          | Op Code               |    R     |
    |           2   |  PBR,PC+1        | Internal Operation    |    R     |
    |           3   |  PBR,PC+1        | Internal Operation    |    R     |
    |           4   |  0,S+1           | New PCL-1             |    R     |
    |           5   |  0,S+2           | New PCH               |    R     |
    |           6   |  0,S+2           | Internal Operation    |    R     |
    |           1   |  PBR,NewPC       | New Op Code           |    R     |
    +---------------+------------------+-----------------------+----------+

    See also: Abbreviations


 65816 Addressing mode: Stack (RTL) -- s

 (RTL)
 (1 byte)  (6 cycles)

    +---------------+------------------+-----------------------+----------+
    |     Cycle     |   Address Bus    |       Data Bus        |Read/Write|
    +---------------+------------------+-----------------------+----------+
    |           1   |  PBR,PC          | Op Code               |    R     |
    |           2   |  PBR,PC+1        | Internal Operation    |    R     |
    |           3   |  PBR,PC+1        | Internal Operation    |    R     |
    |           4   |  0,S+1           | NEW PCL               |    R     |
    |           5   |  0,S+2           | NEW PCH               |    R     |
    |           6   |  0,S+3           | NEW PBR               |    R     |
    |           1   |  NEWPBR,PC       | New Op Code           |    R     |
    +---------------+------------------+-----------------------+----------+

    See also: Abbreviations


 6510/65816 Addressing mode: Stack (Push) -- s

 (PHA,PHB,PHD,PHK,PHP,PHX,PHY)
 (1 byte)  (3 and 4 cycles)

    +---------------+------------------+-----------------------+----------+
    |     Cycle     |   Address Bus    |       Data Bus        |Read/Write|
    +---------------+------------------+-----------------------+----------+
    |           1   |  PBR,PC          | Op Code               |    R     |
    |           2   |  PBR,PC+1        | Internal Operation    |    R     |
    |       (1) 3a  |  0,S             | Register High         |    W     |
    |           3   |  0,S-1           | Register Low          |    W     |
    +---------------+------------------+-----------------------+----------+
    (1) Add 1 cycle for M=0 or X=0 (i.e. 16 bit data).

    See also: Abbreviations


 6510/65816 Addressing mode: Stack (Pull) -- s

 (PLA,PLB,PLD,PLP,PLX,PLY)
 (1 byte)  (4 and 5 cycles)

    +---------------+------------------+-----------------------+----------+
    |     Cycle     |   Address Bus    |       Data Bus        |Read/Write|
    +---------------+------------------+-----------------------+----------+
    |           1   |  PBR,PC          | Op Code               |    R     |
    |           2   |  PBR,PC+1        | Internal Operation    |    R     |
    |           3   |  PBR,PC+1        | Internal Operation    |    R     |
    |           4   |  0,S+1           | Register Low          |    R     |
    |       (1) 4a  |  0,S+2           | Register High         |    R     |
    +---------------+------------------+-----------------------+----------+
    (1) Add 1 cycle for M=0 or X=0 (i.e. 16 bit data).

    See also: Abbreviations


 65816 Addressing mode: Stack (PEI) -- s

 (PEI)
 (2 bytes)  (6 and 7 cycles)

    +---------------+------------------+-----------------------+----------+
    |     Cycle     |   Address Bus    |       Data Bus        |Read/Write|
    +---------------+------------------+-----------------------+----------+
    |           1   |  PBR,PC          | Op Code               |    R     |
    |           2   |  PBR,PC+1        | Direct Offset         |    R     |
    |       (2) 2a  |  PBR,PC+1        | Internal Operation    |    R     |
    |           3   |  0,D+DO          | Absolute Address Low  |    R     |
    |           4   |  0,D+DO+1        | Absolute Address High |    R     |
    |           5   |  0,S             | Absolute Address High |    W     |
    |           6   |  0,S-1           | Absolute Address Low  |    W     |
    +---------------+------------------+-----------------------+----------+
    (2) Add 1 cycle for direct register low (DL) not equal 0.

    See also: Abbreviations


 65816 Addressing mode: Stack (PEA) -- s

 (PEA)
 (3 bytes)  (5 cycles)

    +---------------+------------------+-----------------------+----------+
    |     Cycle     |   Address Bus    |       Data Bus        |Read/Write|
    +---------------+------------------+-----------------------+----------+
    |           1   |  PBR,PC          | Op Code               |    R     |
    |           2   |  PBR,PC+1        | Absolute Address Low  |    R     |
    |           3   |  PBR,PC+2        | Absolute Address High |    R     |
    |           4   |  0,S             | Absolute Address High |    W     |
    |           5   |  0,S-1           | Absolute Address Low  |    W     |
    +---------------+------------------+-----------------------+----------+

    See also: Abbreviations


 65816 Addressing mode: Stack (PER) -- s

 (PER)
 (3 bytes)  (6 cycles)

    +---------------+------------------+-----------------------+----------+
    |     Cycle     |   Address Bus    |       Data Bus        |Read/Write|
    +---------------+------------------+-----------------------+----------+
    |           1   |  PBR,PC          | Op Code               |    R     |
    |           2   |  PBR,PC+1        | Offset Low            |    R     |
    |           3   |  PBR,PC+2        | Offset High           |    R     |
    |           4   |  PBR,PC+2        | Internal Operation    |    R     |
    |           5   |  0,S             | PCH+Offset+CAR        |    W     |
    |           6   |  0,S-1           | PCL + Offset          |    W     |
    +---------------+------------------+-----------------------+----------+

    See also: Abbreviations


 65816 Addressing mode: Stack Relative -- d,s

 (ADC,AND,CMP,EOR,LDA,ORA,SBC,STA)
 (2 bytes)  (4 and 5 cycles)

    +---------------+------------------+-----------------------+----------+
    |     Cycle     |   Address Bus    |       Data Bus        |Read/Write|
    +---------------+------------------+-----------------------+----------+
    |           1   |  PBR,PC          | Op Code               |    R     |
    |           2   |  PBR,PC+1        | Stack Offset          |    R     |
    |           3   |  PBR,PC+1        | Internal Operation    |    R     |
    |           4   |  0,S+SO          | Data Low              |   R/W    |
    |       (1) 4a  |  0,S+SO+1        | Data High             |   R/W    |
    +---------------+------------------+-----------------------+----------+
    (1) Add 1 cycle for M=0 or X=0 (i.e. 16 bit data).

    See also: Abbreviations


 65816 Addressing mode: Stack Relative Indirect Indexed -- (d,s),y

 (ADC,AND,CMP,EOR,LDA,ORA,SBC,STA)
 (2 bytes)  (7 and 8 cycles)

    +---------------+------------------+-----------------------+----------+
    |     Cycle     |   Address Bus    |       Data Bus        |Read/Write|
    +---------------+------------------+-----------------------+----------+
    |           1   |  PBR,PC          | Op Code               |    R     |
    |           2   |  PBR,PC+1        | Stack Offset          |    R     |
    |           3   |  PBR,PC+1        | Internal Operation    |    R     |
    |           4   |  0,S+SO          | Absolute Address Low  |    R     |
    |           5   |  0,S+SO+1        | Absolute Address High |    R     |
    |           6   |  0,S+SO+1        | Internal Operation    |    R     |
    |           7   |  DBR,AA+Y        | Data Low              |   R/W    |
    |       (1) 7a  |  DBR,AA+Y+1      | Data High             |   R/W    |
    +---------------+------------------+-----------------------+----------+
    (1) Add 1 cycle for M=0 or X=0 (i.e. 16 bit data).

    See also: Abbreviations


 65816 Addressing mode: Block Move Positive (forward) -- xyc

 (MVP)
 (3 bytes)  (7 cycles)

    +---------------+------------------+-----------------------+----------+
    |     Cycle     |   Address Bus    |       Data Bus        |Read/Write|
    +---------------+------------------+-----------------------+----------+
    |        +- 1   |  PBR,PC          | Op Code               |    R     |
    |        |  2   |  PBR,PC+1        | Dest. Bank Address    |    R     |
    |        |  3   |  PBR,PC+2        | Source Bank Address   |    R     |
    |   N-2  |  4   |  SBA,X           | Source Data           |    R     |
    |   Byte |  5   |  DBA,Y           | Dest Data             |    W     |
    |   C=2  |  6   |  DBA,Y           | Internal Operation    |    R     |
    |        +- 7   |  DBA,Y           | Internal Operation    |    R     |
    |        +- 1   |  PBR,PC          | Op Code               |    R     |
    |        |  2   |  PBR,PC+1        | Dest. Bank Address    |    R     |
    |   N-1  |  3   |  PBR,PC+2        | Source Bank Address   |    R     |
    |   Byte |  4   |  SBA,X-1         | Source Data           |    R     |
    |   C=1  |  5   |  DBA,Y-1         | Dest Data             |    W     |
    |        |  6   |  DBA,Y-1         | Internal Operation    |    R     |
    |        +- 7   |  DBA,Y-1         | Internal Operation    |    R     |
    |        +- 1   |  PBR,PC          | Op Code               |    R     |
    |        |  2   |  PBR,PC+1        | Dest. Bank Address    |    R     |
    | N Byte |  3   |  PBR,PC+2        | Source Bank Address   |    R     |
    |   Last |  4   |  SBA,X-2         | Source Data           |    R     |
    |   C=0  |  5   |  DBA,Y-2         | Dest Data             |    W     |
    |        |  6   |  DBA,Y-2         | Internal Operation    |    R     |
    |        |  7   |  DBA,Y-2         | Internal Operation    |    R     |
    |        +- 1   |  PBR,PC+3        | New Op Code           |    R     |
    +---------------+------------------+-----------------------+----------+

    See also: Abbreviations

   x = Source Address
   y = Destination
   c = Number of Bytes to move -1
   x,y Decrement
   MVP is used when the destination start address is higher (more positive)
   than the source start address.

   FFFFFF
     ^  Dest Start
     |  Source Start
     |  Dest End
     |  Source End
   000000


 65816 Addressing mode: Block Move Negative (backward) -- xyc

 (MVN)
 (3 bytes)  (7 cycles)

    +---------------+------------------+-----------------------+----------+
    |     Cycle     |   Address Bus    |       Data Bus        |Read/Write|
    +---------------+------------------+-----------------------+----------+
    |        +- 1   |  PBR,PC          | Op Code               |    R     |
    |        |  2   |  PBR,PC+1        | Dest. Bank Address    |    R     |
    |        |  3   |  PBR,PC+2        | Source Bank Address   |    R     |
    |   N-2  |  4   |  SBA,X           | Source Data           |    R     |
    |   Byte |  5   |  DBA,Y           | Dest Data             |    W     |
    |   C=2  |  6   |  DBA,Y           | Internal Operation    |    R     |
    |        +- 7   |  DBA,Y           | Internal Operation    |    R     |
    |        +- 1   |  PBR,PC          | Op Code               |    R     |
    |        |  2   |  PBR,PC+1        | Dest. Bank Address    |    R     |
    |   N-1  |  3   |  PBR,PC+2        | Source Bank Address   |    R     |
    |   Byte |  4   |  SBA,X+1         | Source Data           |    R     |
    |   C=1  |  5   |  DBA,Y+1         | Dest Data             |    W     |
    |        |  6   |  DBA,Y+1         | Internal Operation    |    R     |
    |        +- 7   |  DBA,Y+1         | Internal Operation    |    R     |
    |        +- 1   |  PBR,PC          | Op Code               |    R     |
    |        |  2   |  PBR,PC+1        | Dest. Bank Address    |    R     |
    | N Byte |  3   |  PBR,PC+2        | Source Bank Address   |    R     |
    |   Last |  4   |  SBA,X+2         | Source Data           |    R     |
    |   C=0  |  5   |  DBA,Y+2         | Dest Data             |    W     |
    |        |  6   |  DBA,Y+2         | Internal Operation    |    R     |
    |        |  7   |  DBA,Y+2         | Internal Operation    |    R     |
    |        +- 1   |  PBR,PC+3        | New Op Code           |    R     |
    +---------------+------------------+-----------------------+----------+

    See also: Abbreviations

   x = Source Address
   y = Destination
   c = Number of Bytes to move -1
   x,y Increment
   MVN is used when the destination start address is lower (more negative)
   than the source start address.

   FFFFFF
     |  Source End
     |  Dest End
     |  Source Start
     v  Dest Start
   000000


 Address-Mode Abbreviations:

    AAB     Absolute Address Bank           IDL     Immediate Data Low
    AAH     Absolute Address High           IO      Internal Operation
    AAL     Absolute Address Low            P       Status Register
    AAVH    Abs.Addr. Vector High           PBR     Program Bank Register
    AAVL    Abs.Addr. Vector Low            PC      Program Counter
    C       Accumulator                     PCH     Program Counter High
    CAR     Carry                           PCL     Program Counter Low
    D       Direct Register                 R-M-W   Read-Modify-Write
    DBA     Dest. Bank Address              S       Stack Address
    DBR     Data Bank Register              SBA     Source Bank Address
    DO      Direct Offset                   SO      Stack Offset
    I       Index Registers                 VA      Vector Address
    IDH     Immediate Data High             X,Y     X,Y Registers


  Branch penalty cycles
 +---------------------+

 As you can see from the relative addressing mode, the relative offset is
 added to the PC after it has been fetched, of course. That means, that the
 penalty cycle for branching to a different page is given, if the branch
 is taken and the high byte of the PC after the branch instruction is
 different from the high byte of the destination address.

 The syntax of assemblers may lead to confusion here. For example, "BCS *+2"
 looks like an offset has to be added, but it will be coded as "B0 00", so
 this instruction will never branch to a different page.

 The 65816 applies this penalty cycle only in Emulation mode.


  Bugs and flaws of the 6510
 +--------------------------+

 Zeropage addressing modes & page wraps (fixed on 65816 in native mode):

   If you use an indexed-zeropage addressing mode, either direct or indirect,
   it is not able to leave the zeropage on page-wraps. Examples:

    LDX #$01
    LDA $FF,X

   will fetch from adress $0000 and not $0100.

    LDA ($FF),Y

    LDX #$00
    LDA ($FF,X)

    LDX #$FF
    LDA ($00,X)

   will all fetch the low-byte from $00FF and the high-byte from $0000.

 Indirect addressing mode & page wraps (fixed on 65816):

   If you use the indirect addressing mode, PCH will not be incremented on
   page wraps. Example:

    JMP ($C0FF)

   will fetch the low-byte from $C0FF and the high-byte from $C000.

 Decimal mode (flags fixed on 65816):

   In decimal mode, N and V are set after the high-order nibble is added or
   subtracted but before it is decimal-corrected, according to binary rules.
   Z is always set according to binary mode, not decimal.

   When decimal-correcting a nibble for addition, following rules apply:

    IF ((nibble >= $A) \/ C') THEN nibble += 6
    C'' = C' \/ (nibble + 6 >= $A)

   When decimal-correcting a nibble for subtraction,  following rules apply:

    IF (~C') THEN nibble -= 6
    C'' = C' \/ (nibble - 6 < 0)

   Thus, $F + $F in decimal mode is $14, not $24. Also, decimal correction
   can result in nibbles ranging from $A-$F. For example, $C + $D results
   in $19 before correction, $1F after.
   ```