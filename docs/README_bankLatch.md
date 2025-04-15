# Bank Address Latch

In order to extend the Minimal 65816 memory beyond 64kb, a proper Bank Latching mechanism needs to be implemented. The 65816 outputs the bank address through the Bus Data Bus during PHI2 low phase. Bank Address has to be latched while PHI2 is low.

This is the most basic latching circuit, copied over from the WDC Documentation

```
────────┐                   ┌───────────────┐          
        │       ┌─────┐     │               │          
 PHI2   ┼───────│ INV ┼─┬───┼ E             │          
        │       └─────┘ │   │               │          
        │               │   │             Q ┼────────► 
        │               │   │            __ │          
        │           ┌───┼───┼ D          OE │          
        │           │   │   └───────────────┘          
 65c816 │           │   │         74373                     
        │           │   │   ┌───────────────┐          
        │           │   │   │ __            │          
        │           │   └───┼ CE            │          
        │           │       │               │          
BA0-BA7 ┼───────────┼───────┼ B           A ┼─────────►
        │                   │               │          
    RWB ┼───────────────────┼ DIR           │          
────────┘                   └───────────────┘          
```                               74245                        
                                                       
First choice to make is, should I implement this as a single chip, or reuse the code from other 74xx ICs? Reusing should be easier, but more limited in the future. So a single chip is the way. A simpler approach is to add this into the M65816 glue logic. 

## Step 1. Extend memory device

Extend the memory device to 16 MB. Create a memory image with 16MB of data with a recognizable pattern. For each bank, alternate the bank number with 00. 
It seems to work

## Step 2. Extend the memory viewer. 

Extend the memory viewer so it can display the whole 16MB of memory. Limited to 3 first banks, as it affects performances. Whole memory is loaded?
It seems to work, but more work is needed.

## Step 3. Try the emulator with 64kb of data

Try a small test program with only 64kb of data. Behaviour should not change
Seems OK

## Step 4. Implement long jump

It would not work correctly, as I am not capturing the bank address yet. However, PBR should be modified.
Done. I see PBR changing. Feels right, but I am not sure about it. Something seems off after jump. PC seems to jump a few bytes ahead, but it could be a return from interrupt handled wrongly. Retest after step 5. 

## Step 5. Implement BA Latch

Capture the bank address and try with long jump!

## Step 6. Logic Analyzer

Extend the logic analyzer to capture and display bank address. First, as a separate trace, then combined with the address bush.

## Step 7. Sit back and reevaluate

During all execution cycles something must be outputted during first half cycle. I am not paying attention to that. However it should have some sense built into it. During fetch, program bank is outputted. During operand fecth, program bank is outputted. I haven't looked into DBR yet. When is it used? How is it used? I know for a fact I am outputting PBR during fetch. But I don't know what happens on other cycles. 


                                                       
                                                       
                                                       
                                                       
                                                       
                                                       
                                                       
                                                       
                                                       
                                                       
                                                       
                                                       
