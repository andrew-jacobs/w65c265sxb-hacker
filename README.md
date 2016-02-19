# w65c265sxb-hacker
A tool for modifying the firmware on your WDC W65C265SXB Development Board

The SXB development board has a 128K Flash ROM that can be updated under
software control. The design of the SXB board divides the ROM into four 32K
banks and maps one these into the memory area between $00:8000 and $00:FFFF.

On reset the choice of default bank is controlled by two pull up resistors
on the FAMS and FA15 signal tracks. Once your SXB is running you can change
the bank by configuring one or both of PD4<3> and PD4<4> as low outputs or
high impedence inputs using the PD4 and PDD4 registers.

The WDC boot ROM is part of W65C265 processor so it can not be erased (but it
can be disabled). The mask ROM tests for the character sequence 'WDC' at various
locations in memory and will execute code following the string if found. If you
are developing a flash ROM image the only add the 'WDC' string when you are sure
the ROM functions properly (or you will have to remove the flash ROM to erase
it).

## UART Connections

You will need two USB connections to the board to access two of the four
built-in UARTs of the W65C265 microcontroller. 

The built-in WDC monitor uprogram uses UART3 at 9600 baud with automatic line
control, which is the connector to J6 at the bottom of the board. You use this
to provide power to the boards and to upload the hacking tool via the WDC
debugger (or other methods to upload S28 files). 

Second, the hacking tool itself is configured to use UART0 via J5 - the jack on
the bottom right of the board. Use a USB serial adapter (like one of the cheap
PL2303 modules with jumper wires sold on eBay) to establish a connection. You
must connect the TXD, RXD and GND lines. On J5, GND is pin 2, TXD is pin 4 (just
above GND), and RXD is pin 3 (just above pin 1, which is marked with a dot and
left open).

> It would be possible to configure SXB-Hacker to use UART3 but then either
> it would have to use the slower 9600 baud settings used by the Mensch monitor
> or you would have to change the terminal settings after it started. I decided
> to use a second UART to keep things simpler.

You also need a terminal program like AlphaCom or Tera Term on your PC that
supports XMODEM file transfers. Configure it to work at 19200 baud, 8 data
bits, no parity and 1 stop bit (8N1). Disable hardware flow control. 

> Depending on your operating system, you might have had to add a Line Feed (LF)
> to the output of the Mensch Monitor. This is not necessary for the hacking
> tool.

## Using W65C265SXB-Hacker

Use the WDC debugger to download the compiled S28 image of the hacking tool
(sxb-hacker.s28) to the SXB board using your terminal (the Mensch Monitor
will load the S28 records into memory) and start execution with 'G 00:0300'.
The tool will respond with a message in the terminal software.

```
W65C265SXB-Hacker [16.01]
.
```
The 'M' command allows you to display memory, for example 'M FFE0 FFFF' will
display the vectors at the top of the memory
```
.M FFE0 FFFF
00:FFE0 FF FF FF FF 1C 81 08 81 19 81 0C 81 24 81 16 81 |............$...|
00:FFF0 1F 81 1F 81 10 81 1F 81 13 81 04 81 2B 81 00 81 |............+...|
```
Using the 'R' command you can pick another ROM memory bank, like 0
```
.R 0
.M FFE0 FFFF
00:FFE0 FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF |................|
00:FFF0 FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF |................|
```
The 'E' command will erase a ROM bank from 8000 to FFFF ready for a new
image to be loaded. After erasing the entire region should be set to FF
values.

The 'X' command starts an XMODEM download to the specified address. You can
download into any memory area including the ROM but you must disable the
mask ROM to write to the whole ROM area.
```
.X A000

Waiting for XMODEM transfer to start
```
You can start execution of downloaded code using the 'G' command. You can
either specify the address to start at or omit it to tell the monitor to
use the reset vector as the target address.
```
.G A000
or
.G
```
You can write a single byte of data  into memory manually using the 'W' command.
The program will prompt you to enter another byte at the next address.
```
.W 4000 FF
.W 4001
```
You can leave byte entry mode by pressing escape, deleting the command with
backspace or pressing ENTER.

The 'B' command sets the memory bank (e.g. bits 23 to 16 of the address bus)
when displaying or altering memory. I've added this command to support a
hardware project I'm working on to add additional RAM via the XBUS connector.
On a standard SXB board you can only access bank 00.

The 'F' command allows you to disable the mask ROM and expose the whole of
the flash ROM. An argument of '0' turns the ROM off while a '1' turns it back
on.
```
.f 1
.m e000 e020
00:E000 4C 96 EB 4C EA F1 4C 06 E0 4C 23 F4 4C 61 E6 4C |L..L..L..L#.La.L|
00:E010 84 E7 4C 9A EC 4C 73 EC 4C 93 EC 4C 6B EC 4C 5C |..L..Ls.L..Lk.L\|
.f 0
.m e000 e020
00:E000 FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF |................|
00:E010 FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF |................|
```

The 'H' command performs a search of the entire memory space (in 4K increments)
to locate RAM areas. On a standard board all it will find is the 32K area on
bank $00. If you have added RAM then it may find it in other locations, for
example my 1M SRAM board displays this.
```
.h
00:0000-00:7FFF
C0:0000-FF:FFFF
```

The 'D' command disassembles the instructions in the specified memory range,
for example:
```
.d eb96 ebb0
00:EB96 22 73 EC 00 JSL $00:EC73
00:EB9A 90 03       BCC $EB9F
00:EB9C 82 A8 00    BRL $EC47
00:EB9F A5 4E       LDA $4E
00:EBA1 89 01       BIT #$01
00:EBA3 F0 04       BEQ $EBA9
00:EBA5 A9 08       LDA #$08
00:EBA7 80 02       BRA $EBAB
00:EBA9 A9 10       LDA #$10
00:EBAB 85 57       STA $57
00:EBAD A5 62       LDA $62
00:EBAF 22 70 F3 00 JSL $00:F370
```

## Building SXB-Hacker
To build the hacking tool from the sources you will need a computer running the
Microsoft Windows operating system (XP,Vista,7,8 or 10) and an installed copy of
the tools provided by WDC at
[http://wdc65xx.com/Products/WDCTools/](http://wdc65xx.com/Products/WDCTools/).

A copy of Microsoft NMAKE is included with the source along with some batch files
that invoke the makefile for various targets. 'build' will run the assembler and
linker to update the S28 output file (if any of the sources have changed). 'clean'
erases all the files generated by the assembler.
