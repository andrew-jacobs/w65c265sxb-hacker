;===============================================================================
; __        ____  ____   ____ ____   __  ____
; \ \      / / /_| ___| / ___|___ \ / /_| ___|
;  \ \ /\ / / '_ \___ \| |     __) | '_ \___ \
;   \ V  V /| (_) |__) | |___ / __/| (_) |__) |
;    \_/\_/  \___/____/ \____|_____|\___/____/
;
; Western Design Center W65C265 device definitions
;-------------------------------------------------------------------------------
; Copyright (C)2015 HandCoded Software Ltd.
; All rights reserved.
;
; This work is made available under the terms of the Creative Commons
; Attribution-NonCommercial-ShareAlike 4.0 International license. Open the
; following URL to see the details.
;
; http://creativecommons.org/licenses/by-nc-sa/4.0/
;
;===============================================================================
; Notes:
;
; Various macros and definitions for the W65C265 microcontroller.
;
;===============================================================================
; Revision History:
;
; 2015-12-18 AJ Initial version
;-------------------------------------------------------------------------------
; $Id$
;-------------------------------------------------------------------------------

;===============================================================================
; Hardware Registers
;-------------------------------------------------------------------------------

;00DF00-1F CS0 Port Replacement & Expansion uninitialized

PD0             equ     $00DF00         ; Port 0 Data Register
PD1             equ     $00DF01         ; Port 1 Data Register
PD2             equ     $00DF02         ; Port 2 Data Register
PD3             equ     $00DF03         ; Port 3 Data Register
PDD0            equ     $00DF04         ; Port 0 Data Direction Register
PDD1            equ     $00DF05         ; Port 1 Data Direction Register
PDD2            equ     $00DF06         ; Port 2 Data Direction Register
PDD3            equ     $00DF07         ; Port 3 Data Direction Register

PD4             equ     $00DF20         ; Port 4 Data Register
PD5             equ     $00DF21         ; Port 5 Data Register
PD6             equ     $00DF22         ; Port 6 Data Register
PD7             equ     $00DF23         ; Port 7 Data Register
PDD4            equ     $00DF24         ; Port 4 Data Direction Register
PDD5            equ     $00DF25         ; Port 5 Data Direction Register
PDD6            equ     $00DF26         ; Port 6 Data Direction Register
PCS7            equ     $00DF27         ; Port 7 Chip Select

;00DF28-3F --- Reserved uninitialized

BCR             equ     $00DF40         ; Bus Control Register
SSCR            equ     $00DF41         ; System Speed Control Register
TCR             equ     $00DF42         ; Timer Control Register
TER             equ     $00DF43         ; Timer Enable Register
TIFR            equ     $00DF44         ; Timer Interrupt Flag Register
EIFR            equ     $00DF45         ; Edge Interrupt Flag Register
TIER            equ     $00DF46         ; Timer Interrupt Enable Register
EIER            equ     $00DF47         ; Edge Interrupt Enable Register
UIFR            equ     $00DF48         ; UART Interrupt Flag Register
UIER            equ     $00DF49         ; UART Interrupt Enable Register

T0LL            equ     $00DF50         ; Timer 0 Latch Low
T0LH            equ     $00DF51         ; Timer 0 Latch High
T1LL            equ     $00DF52         ; Timer 1 Latch Low
T1LH            equ     $00DF53         ; Timer 1 Latch High
T2LL            equ     $00DF54         ; Timer 2 Latch Low
T2LH            equ     $00DF55         ; Timer 2 Latch High
T3LL            equ     $00DF56         ; Timer 3 Latch Low
T3LH            equ     $00DF57         ; Timer 3 Latch High
T4LL            equ     $00DF58         ; Timer 4 Latch Low
T4LH            equ     $00DF59         ; Timer 4 Latch High
T5LL            equ     $00DF5A         ; Timer 5 Latch Low
T5LH            equ     $00DF5B         ; Timer 5 Latch High
T6LL            equ     $00DF5C         ; Timer 6 Latch Low
T6LH            equ     $00DF5D         ; Timer 6 Latch High
T7LL            equ     $00DF5E         ; Timer 7 Latch Low
T7LH            equ     $00DF5F         ; Timer 7 Latch High
T0CL            equ     $00DF60         ; Timer 0 Counter Low
T0CH            equ     $00DF61         ; Timer 0 Counter High
T1CL            equ     $00DF62         ; Timer 1 Counter Low
T1CH            equ     $00DF63         ; Timer 1 Counter High
T2CL            equ     $00DF64         ; Timer 2 Counter Low
T2CH            equ     $00DF65         ; Timer 2 Counter High
T3CL            equ     $00DF66         ; Timer 3 Counter Low
T3CH            equ     $00DF67         ; Timer 3 Counter High
T4CL            equ     $00DF68         ; Timer 4 Counter Low
T4CH            equ     $00DF69         ; Timer 4 Counter High
T5CL            equ     $00DF6A         ; Timer 5 Counter Low
T5CH            equ     $00DF6B         ; Timer 5 Counter High
T6CL            equ     $00DF6C         ; Timer 6 Counter Low
T6CH            equ     $00DF6D         ; Timer 6 Counter High
T7CL            equ     $00DF6E         ; Timer 7 Counter Low
T7CH            equ     $00DF6F         ; Timer 7 Counter High

;00DFC0-FF CS1 COProcessor Expansion uninitialized

ACSR0           equ     $00DF70         ; UART 0 Control/Status Register
ARTD0           equ     $00DF71         ; UART 0 Data Register
ACSR1           equ     $00DF72         ; UART 1 Control/Status Register
ARTD1           equ     $00DF73         ; UART 1 Data Register
ACSR2           equ     $00DF74         ; UART 2 Control/Status Register
ARTD2           equ     $00DF75         ; UART 2 Data Register
ACSR3           equ     $00DF76         ; UART 3 Control/Status Register
ARTD3           equ     $00DF77         ; UART 3 Data Register
PIBFR           equ     $00DF78         ; Parallel Interface Flag Register
PIBER           equ     $00DF79         ; Parallel Interface Enable Register
PIR2            equ     $00DF7A         ; Parallel Interface Register 2
PIR3            equ     $00DF7B         ; Parallel Interface Register 3
PIR4            equ     $00DF7C         ; Parallel Interface Register 4
PIR5            equ     $00DF7D         ; Parallel Interface Register 5
PIR6            equ     $00DF7E         ; Parallel Interface Register 6
PIR7            equ     $00DF7F         ; Parallel Interface Register 7

;00DF80-BF RAM RAM Registers uninitialized

                include "w65c816.inc"