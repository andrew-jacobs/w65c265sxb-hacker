;==============================================================================
; __        ____  ____   ____ ____   __  ____ ______  ______
; \ \      / / /_| ___| / ___|___ \ / /_| ___/ ___\ \/ / __ )
;  \ \ /\ / / '_ \___ \| |     __) | '_ \___ \___ \\  /|  _ \
;   \ V  V /| (_) |__) | |___ / __/| (_) |__) |__) /  \| |_) |
;    \_/\_/  \___/____/ \____|_____|\___/____/____/_/\_\____/
;
; Basic Vector Handling for the W65C265SXB Development Board
;------------------------------------------------------------------------------
; Copyright (C)2015 HandCoded Software Ltd.
; All rights reserved.
;
; This work is made available under the terms of the Creative Commons
; Attribution-NonCommercial-ShareAlike 4.0 International license. Open the
; following URL to see the details.
;
; http://creativecommons.org/licenses/by-nc-sa/4.0/
;
;==============================================================================
; Notes:
;
;------------------------------------------------------------------------------

                pw      132
                inclist on

                chip    65816

                include "w65c265.inc"
                include "w65c265sxb.inc"

;==============================================================================
; Configuration
;------------------------------------------------------------------------------

BAUD_RATE       equ     9600                   	; ACIA baud rate

BRG_VALUE       equ     OSC_FREQ/(16*BAUD_RATE)-1

                if      BRG_VALUE&$ffff0000
                messg   "BRG_VALUE does not fit in 16-bits"
                endif

UART            equ     3

;==============================================================================
; Power On Reset
;------------------------------------------------------------------------------

                code
                extern  Start
                longi   off
                longa   off
RESET:
                sei                             ; Disable interrupts
                native                          ; Switch to native mode
                long_i
                ldx     #$01ff                  ; Reset the stack
                txs

                ; Ensure no via interrupts
                stz     UIER

                lda     #$c0                    ; Ensure A15/AMS are output
                sta     PDD4
                stz     PD4                     ; And select bank 0

                lda     #($10<<UART)            ; Set UART to use timer 3
                trb     TCR
                lda     #<BRG_VALUE             ; And set baud rate
                sta     T3CL
                lda     #>BRG_VALUE
                sta     T3CH

                lda     #1<<3                   ; Enable timer 3
                tsb     TER

                lda     #%00100101              ; Set UART for 8-N-1
                sta     ACSR0+2*UART

                jmp     Start                   ; Jump to the application start

;==============================================================================
; ACIA Interface
;------------------------------------------------------------------------------

; Wait until the last transmission has been completed then send the character
; in A.

                public  UartTx
UartTx:
                pha                             ; Save the character
                php                             ; Save register sizes
                short_a                         ; Make A 8-bits
                pha
                lda     #(1<<1)<<(2*UART)
TxWait:         bit     UIFR                    ; Has the timer finished?
                beq     TxWait
                pla
                sta     ARTD0+2*UART            ; Transmit the character
                plp                             ; Restore register sizes
                pla                             ; And callers A
                rts                             ; Done

; Fetch the next character from the receive buffer waiting for some to arrive
; if the buffer is empty.

                public  UartRx
UartRx:
                php                             ; Save register sizes
                short_a                         ; Make A 8-bits
                lda     #(1<<0)<<(2*UART)
RxWait:         bit     UIFR                    ; Any data in RX buffer?
                beq     RxWait                  ; No
                lda     ARTD0+2*UART            ; Yes, read it
                plp                             ; Restore register sizes
                rts                             ; Done

; Check if the receive buffer contains any data and return C=1 if there is
; some.

                public  UartRxTest
UartRxTest:
                pha                             ; Save callers A
                php
                short_a
                lda     UIFR                    ; Read the status register
                and     #(1<<0)<<(2*UART)
                beq     RxDone
                inc     a
RxDone:         plp
                ror     a                       ; Shift UART0R bit into carry
                pla                             ; Restore A
                rts                             ; Done

;===============================================================================
; ROM Bank Selection
;-------------------------------------------------------------------------------

; Select the flash ROM bank indicated by the two low order bits of A. The pins
; should be set to inputs when a hi bit is needed and a low output for a lo bit.

                public RomSelect
RomSelect:
                php
                short_a
                and     #$03                    ; Strip out bank number
                asl     a                       ; And rotate into bits
                asl     a
                asl     a
                pha                             ; Save bit pattern
                eor     #$18                    ; Invert to get directions
                eor     PDD4                    ; Work out change
                and     #$18
                eor     PDD4                    ; And apply to direction reg
                sta     PDD4
                pla
                eor     PD4                     ; Then adjust data register
                and     #$18
                eor     PD4
                sta     PD4
                plp                             ; Restore register sizes
                rts                             ; Done

; Check if the select ROM bank contains WDC firmware. If it does return with
; the Z flag set.

                public RomCheck
RomCheck:
                rep     #Z_FLAG                 ; No firmware in the ROM
                rts

                end