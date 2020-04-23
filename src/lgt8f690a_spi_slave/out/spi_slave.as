opt subtitle "HI-TECH Software Omniscient Code Generator (PRO mode) build 10920"

opt pagewidth 120

	opt pm

	processor	16F690
clrc	macro
	bcf	3,0
	endm
clrz	macro
	bcf	3,2
	endm
setc	macro
	bsf	3,0
	endm
setz	macro
	bsf	3,2
	endm
skipc	macro
	btfss	3,0
	endm
skipz	macro
	btfss	3,2
	endm
skipnc	macro
	btfsc	3,0
	endm
skipnz	macro
	btfsc	3,2
	endm
indf	equ	0
indf0	equ	0
pc	equ	2
pcl	equ	2
status	equ	3
fsr	equ	4
fsr0	equ	4
c	equ	1
z	equ	0
pclath	equ	10
	FNCALL	_main,_misc_eer
	FNCALL	_main,_usart_init
	FNCALL	_main,_spi_init_sla
	FNCALL	_main,_spi_exch
	FNCALL	_main,_usart_send
	FNROOT	_main
	FNCALL	intlevel1,_hisr
	global	intlevel1
	FNROOT	intlevel1
	global	_recv_buf
	global	_send_buf
psect	idataBANK0,class=CODE,space=0,delta=2
global __pidataBANK0
__pidataBANK0:
	file	"E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_spi_slave\src\main.c"
	line	31

;initializer for _recv_buf
	retlw	0FFh
	retlw	0FFh
	retlw	0FFh
	retlw	0FFh
	retlw	0FFh
	retlw	0FFh
	retlw	0FFh
	retlw	0FFh
	retlw	0
	retlw	0FFh
	retlw	0FFh
	retlw	0FFh
	retlw	0FFh
	retlw	0FFh
	retlw	0FFh
	retlw	0FFh
	retlw	0FFh
	retlw	0FFh
	retlw	0FFh
	retlw	0FFh
	retlw	0FFh
	retlw	0FFh
	retlw	0FFh
	retlw	0FFh
	retlw	0FFh
	retlw	0FFh
	retlw	0FFh
	retlw	0FFh
	retlw	0FFh
	retlw	0FFh
	retlw	0FFh
	retlw	0AAh
	retlw	0FFh
	retlw	0FFh
	retlw	0FFh
	retlw	0FFh
	line	18

;initializer for _send_buf
	retlw	0Fh
	retlw	01Eh
	retlw	02Dh
	retlw	03Ch
	retlw	04Bh
	retlw	05Ah
	retlw	069h
	retlw	078h
	retlw	087h
	retlw	096h
	retlw	0A5h
	retlw	0B4h
	retlw	0C3h
	retlw	0D2h
	retlw	0E1h
	retlw	0F0h
	retlw	0FFh
	retlw	0FFh
	retlw	0FFh
	retlw	0FFh
	retlw	0FFh
	retlw	0FFh
	retlw	0FFh
	retlw	0FFh
	retlw	0FFh
	retlw	0FFh
	retlw	0FFh
	retlw	0FFh
	retlw	0FFh
	retlw	0FFh
	retlw	0FFh
	retlw	055h
	retlw	0FFh
	retlw	0FFh
	retlw	0FFh
	retlw	0FFh
	global	_RCSTA
_RCSTA	set	24
	global	_SPCR
_SPCR	set	20
	global	_SPDR
_SPDR	set	19
	global	_TXREG
_TXREG	set	25
	global	_GIE
_GIE	set	95
	global	_PEIE
_PEIE	set	94
	global	_RA4
_RA4	set	44
	global	_RA5
_RA5	set	45
	global	_RA6
_RA6	set	46
	global	_RB0
_RB0	set	48
	global	_RB5
_RB5	set	53
	global	_RB6
_RB6	set	54
	global	_SPIEN
_SPIEN	set	166
	global	_TXIF
_TXIF	set	100
	global	_OSCCONbits
_OSCCONbits	set	143
	global	_OSCTUNE
_OSCTUNE	set	144
	global	_SPBRG
_SPBRG	set	153
	global	_SPBRGH
_SPBRGH	set	154
	global	_SPFR
_SPFR	set	147
	global	_SPSR
_SPSR	set	148
	global	_TXSTA
_TXSTA	set	152
	global	_TRISA4
_TRISA4	set	1068
	global	_TRISA5
_TRISA5	set	1069
	global	_TRISA6
_TRISA6	set	1070
	global	_TRISB0
_TRISB0	set	1072
	global	_TRISB5
_TRISB5	set	1077
	global	_TRISB6
_TRISB6	set	1078
	global	_TRMT
_TRMT	set	1217
	global	_TXEN
_TXEN	set	1221
	global	_EEADR
_EEADR	set	269
	global	_EEADRH
_EEADRH	set	271
	global	_EEDAT
_EEDAT	set	268
	global	_EEDATH
_EEDATH	set	270
	global	_L_MCUCRbits
_L_MCUCRbits	set	415
	global	_MCUCR
_MCUCR	set	415
	global	_EEPGD
_EEPGD	set	3175
	global	_EERD
_EERD	set	3168
	file	"spi_slave.as"
	line	#
psect cinit,class=CODE,delta=2
global start_initialization
start_initialization:

psect	dataBANK0,class=BANK0,space=1
global __pdataBANK0
__pdataBANK0:
	file	"E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_spi_slave\src\main.c"
	line	31
_recv_buf:
       ds      36

psect	dataBANK0
	file	"E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_spi_slave\src\main.c"
	line	18
_send_buf:
       ds      36

global btemp
psect inittext,class=CODE,delta=2
global init_fetch,btemp
;	Called with low address in FSR and high address in W
init_fetch:
	movf btemp,w
	movwf pclath
	movf btemp+1,w
	movwf pc
global init_ram
;Called with:
;	high address of idata address in btemp 
;	low address of idata address in btemp+1 
;	low address of data in FSR
;	high address + 1 of data in btemp-1
init_ram:
	fcall init_fetch
	movwf indf,f
	incf fsr,f
	movf fsr,w
	xorwf btemp-1,w
	btfsc status,2
	retlw 0
	incf btemp+1,f
	btfsc status,2
	incf btemp,f
	goto init_ram
; Initialize objects allocated to BANK0
psect cinit,class=CODE,delta=2
global init_ram, __pidataBANK0
	bcf	status, 7	;select IRP bank0
	movlw low(__pdataBANK0+72)
	movwf btemp-1,f
	movlw high(__pidataBANK0)
	movwf btemp,f
	movlw low(__pidataBANK0)
	movwf btemp+1,f
	movlw low(__pdataBANK0)
	movwf fsr,f
	fcall init_ram
psect cinit,class=CODE,delta=2
global end_of_initialization

;End of C runtime variable initialization code

end_of_initialization:
clrf status
ljmp _main	;jump to C main() function
psect	cstackCOMMON,class=COMMON,space=1
global __pcstackCOMMON
__pcstackCOMMON:
	global	?_usart_init
?_usart_init:	; 0 bytes @ 0x0
	global	?_spi_init_sla
?_spi_init_sla:	; 0 bytes @ 0x0
	global	?_hisr
?_hisr:	; 0 bytes @ 0x0
	global	??_hisr
??_hisr:	; 0 bytes @ 0x0
	global	?_main
?_main:	; 2 bytes @ 0x0
	ds	2
	global	??_usart_init
??_usart_init:	; 0 bytes @ 0x2
	global	??_spi_init_sla
??_spi_init_sla:	; 0 bytes @ 0x2
	global	?_spi_exch
?_spi_exch:	; 0 bytes @ 0x2
	global	?_usart_send
?_usart_send:	; 0 bytes @ 0x2
	global	?_misc_eer
?_misc_eer:	; 2 bytes @ 0x2
	global	spi_exch@rxbuf
spi_exch@rxbuf:	; 1 bytes @ 0x2
	global	usart_send@txbuf
usart_send@txbuf:	; 1 bytes @ 0x2
	global	misc_eer@address
misc_eer@address:	; 2 bytes @ 0x2
	ds	1
	global	??_usart_send
??_usart_send:	; 0 bytes @ 0x3
	global	spi_exch@txbuf
spi_exch@txbuf:	; 1 bytes @ 0x3
	ds	1
	global	??_misc_eer
??_misc_eer:	; 0 bytes @ 0x4
	global	??_spi_exch
??_spi_exch:	; 0 bytes @ 0x4
	global	spi_exch@i
spi_exch@i:	; 1 bytes @ 0x4
	global	misc_eer@type
misc_eer@type:	; 1 bytes @ 0x4
	ds	1
	global	spi_exch@length
spi_exch@length:	; 1 bytes @ 0x5
	global	usart_send@length
usart_send@length:	; 1 bytes @ 0x5
	ds	1
	global	usart_send@i
usart_send@i:	; 1 bytes @ 0x6
	ds	1
	global	??_main
??_main:	; 0 bytes @ 0x7
;;Data sizes: Strings 0, constant 0, data 72, bss 0, persistent 0 stack 0
;;Auto spaces:   Size  Autos    Used
;; COMMON          14      7       7
;; BANK0           80      0      72
;; BANK1           80      0       0
;; BANK2           80      0       0

;;
;; Pointer list with targets:

;; ?_misc_eer	unsigned int  size(1) Largest target is 0
;;
;; sp__misc_utoa	PTR unsigned char  size(1) Largest target is 0
;;
;; usart_send@txbuf	PTR unsigned char  size(1) Largest target is 36
;;		 -> recv_buf(BANK0[36]), 
;;
;; spi_exch@rxbuf	PTR unsigned char  size(1) Largest target is 36
;;		 -> recv_buf(BANK0[36]), 
;;
;; spi_exch@txbuf	PTR unsigned char  size(1) Largest target is 36
;;		 -> send_buf(BANK0[36]), 
;;


;;
;; Critical Paths under _main in COMMON
;;
;;   _main->_usart_send
;;
;; Critical Paths under _hisr in COMMON
;;
;;   None.
;;
;; Critical Paths under _main in BANK0
;;
;;   None.
;;
;; Critical Paths under _hisr in BANK0
;;
;;   None.
;;
;; Critical Paths under _main in BANK1
;;
;;   None.
;;
;; Critical Paths under _hisr in BANK1
;;
;;   None.
;;
;; Critical Paths under _main in BANK2
;;
;;   None.
;;
;; Critical Paths under _hisr in BANK2
;;
;;   None.

;;
;;Main: autosize = 0, tempsize = 0, incstack = 0, save=0
;;

;;
;;Call Graph Tables:
;;
;; ---------------------------------------------------------------------------------
;; (Depth) Function   	        Calls       Base Space   Used Autos Params    Refs
;; ---------------------------------------------------------------------------------
;; (0) _main                                                 0     0      0     270
;;                           _misc_eer
;;                         _usart_init
;;                       _spi_init_sla
;;                           _spi_exch
;;                         _usart_send
;; ---------------------------------------------------------------------------------
;; (1) _usart_send                                           5     4      1      90
;;                                              2 COMMON     5     4      1
;; ---------------------------------------------------------------------------------
;; (1) _spi_exch                                             5     3      2     135
;;                                              2 COMMON     4     2      2
;; ---------------------------------------------------------------------------------
;; (1) _spi_init_sla                                         0     0      0       0
;; ---------------------------------------------------------------------------------
;; (1) _usart_init                                           0     0      0       0
;; ---------------------------------------------------------------------------------
;; (1) _misc_eer                                             3     1      2      45
;;                                              2 COMMON     3     1      2
;; ---------------------------------------------------------------------------------
;; Estimated maximum stack depth 1
;; ---------------------------------------------------------------------------------
;; (Depth) Function   	        Calls       Base Space   Used Autos Params    Refs
;; ---------------------------------------------------------------------------------
;; (2) _hisr                                                 2     2      0       0
;;                                              0 COMMON     2     2      0
;; ---------------------------------------------------------------------------------
;; Estimated maximum stack depth 2
;; ---------------------------------------------------------------------------------

;; Call Graph Graphs:

;; _main (ROOT)
;;   _misc_eer
;;   _usart_init
;;   _spi_init_sla
;;   _spi_exch
;;   _usart_send
;;
;; _hisr (ROOT)
;;

;; Address spaces:

;;Name               Size   Autos  Total    Cost      Usage
;;SFR3                 0      0       0       4        0.0%
;;BITSFR3              0      0       0       4        0.0%
;;BANK2               50      0       0       7        0.0%
;;BITBANK2            50      0       0       6        0.0%
;;SFR2                 0      0       0       5        0.0%
;;BITSFR2              0      0       0       5        0.0%
;;SFR1                 0      0       0       2        0.0%
;;BITSFR1              0      0       0       2        0.0%
;;BANK1               50      0       0       5        0.0%
;;BITBANK1            50      0       0       4        0.0%
;;CODE                 0      0       0       0        0.0%
;;DATA                 0      0      50      10        0.0%
;;ABS                  0      0      4F       8        0.0%
;;NULL                 0      0       0       0        0.0%
;;STACK                0      0       1       2        0.0%
;;BANK0               50      0      48       3       90.0%
;;BITBANK0            50      0       0       9        0.0%
;;SFR0                 0      0       0       1        0.0%
;;BITSFR0              0      0       0       1        0.0%
;;COMMON               E      7       7       1       50.0%
;;BITCOMMON            E      0       0       0        0.0%
;;EEDATA             100      0       0       0        0.0%

	global	_main
psect	maintext,global,class=CODE,delta=2
global __pmaintext
__pmaintext:

;; *************** function _main *****************
;; Defined at:
;;		line 51 in file "E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_spi_slave\src\main.c"
;; Parameters:    Size  Location     Type
;;		None
;; Auto vars:     Size  Location     Type
;;		None
;; Return value:  Size  Location     Type
;;                  2  1865[COMMON] int 
;; Registers used:
;;		wreg, fsr0l, fsr0h, status,2, status,0, pclath, cstack
;; Tracked objects:
;;		On entry : 17F/0
;;		On exit  : 60/0
;;		Unchanged: FFE00/0
;; Data sizes:     COMMON   BANK0   BANK1   BANK2
;;      Params:         0       0       0       0
;;      Locals:         0       0       0       0
;;      Temps:          0       0       0       0
;;      Totals:         0       0       0       0
;;Total ram usage:        0 bytes
;; Hardware stack levels required when called:    2
;; This function calls:
;;		_misc_eer
;;		_usart_init
;;		_spi_init_sla
;;		_spi_exch
;;		_usart_send
;; This function is called by:
;;		Startup code after reset
;; This function uses a non-reentrant model
;;
psect	maintext
	file	"E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_spi_slave\src\main.c"
	line	51
	global	__size_of_main
	__size_of_main	equ	__end_of_main-_main
	
_main:	
	opt	stack 6
; Regs used in _main: [wreg-fsr0h+status,2+status,0+pclath+cstack]
	line	53
	
l8811:	
;main.c: 53: if((0x5a == misc_eer(1, 0x100f)))
	movlw	low(0100Fh)
	movwf	(?_misc_eer)
	movlw	high(0100Fh)
	movwf	((?_misc_eer))+1
	movlw	(01h)
	fcall	_misc_eer
		movf	(0+(?_misc_eer)),w
	xorlw	90
	iorwf	(1+(?_misc_eer)),w

	skipz
	goto	u131
	goto	u130
u131:
	goto	l8815
u130:
	line	54
	
l8813:	
;main.c: 54: OSCTUNE = misc_eer(1, 0x1007);
	movlw	low(01007h)
	movwf	(?_misc_eer)
	movlw	high(01007h)
	movwf	((?_misc_eer))+1
	movlw	(01h)
	fcall	_misc_eer
	movf	(0+(?_misc_eer)),w
	bsf	status, 5	;RP0=1, select bank1
	bcf	status, 6	;RP1=0, select bank1
	movwf	(144)^080h	;volatile
	goto	l8817
	line	56
	
l8815:	
;main.c: 55: else
;main.c: 56: OSCTUNE = 0x92;
	movlw	(092h)
	bsf	status, 5	;RP0=1, select bank1
	bcf	status, 6	;RP1=0, select bank1
	movwf	(144)^080h	;volatile
	line	59
	
l8817:	
;main.c: 59: OSCCONbits.IRCF = 0x6;
	movf	(143)^080h,w	;volatile
	andlw	not (((1<<3)-1)<<4)
	iorlw	(06h & ((1<<3)-1))<<4
	movwf	(143)^080h	;volatile
	line	61
	
l8819:	
;main.c: 61: MCUCR = 0x55;
	movlw	(055h)
	bsf	status, 6	;RP1=1, select bank3
	movwf	(415)^0180h	;volatile
	line	62
	
l8821:	
;main.c: 62: MCUCR = 0xC6;
	movlw	(0C6h)
	movwf	(415)^0180h	;volatile
	line	64
	
l8823:	
;main.c: 64: usart_init();
	fcall	_usart_init
	line	66
	
l8825:	
;main.c: 66: spi_init_sla();
	fcall	_spi_init_sla
	line	68
	
l8827:	
;main.c: 68: SPIEN = 0x1;
	bsf	(166/8),(166)&7
	line	72
	
l8829:	
;main.c: 72: spi_exch(1, recv_buf, send_buf);
	movlw	(_recv_buf)&0ffh
	movwf	(?_spi_exch)
	movlw	(_send_buf)&0ffh
	movwf	0+(?_spi_exch)+01h
	movlw	(01h)
	fcall	_spi_exch
	line	76
	
l8831:	
# 76 "E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_spi_slave\src\main.c"
nop ;#
psect	maintext
	line	80
	
l8833:	
;main.c: 80: spi_exch(2, recv_buf, send_buf);
	movlw	(_recv_buf)&0ffh
	movwf	(?_spi_exch)
	movlw	(_send_buf)&0ffh
	movwf	0+(?_spi_exch)+01h
	movlw	(02h)
	fcall	_spi_exch
	line	84
	
l8835:	
# 84 "E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_spi_slave\src\main.c"
nop ;#
psect	maintext
	line	88
	
l8837:	
;main.c: 88: spi_exch(3, recv_buf, send_buf);
	movlw	(_recv_buf)&0ffh
	movwf	(?_spi_exch)
	movlw	(_send_buf)&0ffh
	movwf	0+(?_spi_exch)+01h
	movlw	(03h)
	fcall	_spi_exch
	line	92
	
l8839:	
# 92 "E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_spi_slave\src\main.c"
nop ;#
psect	maintext
	line	96
	
l8841:	
;main.c: 96: spi_exch(4, recv_buf, send_buf);
	movlw	(_recv_buf)&0ffh
	movwf	(?_spi_exch)
	movlw	(_send_buf)&0ffh
	movwf	0+(?_spi_exch)+01h
	movlw	(04h)
	fcall	_spi_exch
	line	100
	
l8843:	
# 100 "E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_spi_slave\src\main.c"
nop ;#
psect	maintext
	line	104
	
l8845:	
;main.c: 104: spi_exch(32, recv_buf, send_buf);
	movlw	(_recv_buf)&0ffh
	movwf	(?_spi_exch)
	movlw	(_send_buf)&0ffh
	movwf	0+(?_spi_exch)+01h
	movlw	(020h)
	fcall	_spi_exch
	line	106
	
l8847:	
;main.c: 106: usart_send(32, recv_buf);
	movlw	(_recv_buf)&0ffh
	movwf	(?_usart_send)
	movlw	(020h)
	fcall	_usart_send
	line	108
	
l8849:	
;main.c: 108: SPIEN = 0x0;
	bcf	status, 5	;RP0=0, select bank0
	bcf	(166/8),(166)&7
	line	115
	
l8851:	
;main.c: 115: PEIE = 0x1;
	bsf	(94/8),(94)&7
	line	116
	
l8853:	
;main.c: 116: GIE = 0x0;
	bcf	(95/8),(95)&7
	line	118
	
l1869:	
	goto	l1869
	global	start
	ljmp	start
	opt stack 0
psect	maintext
	line	119
GLOBAL	__end_of_main
	__end_of_main:
;; =============== function _main ends ============

	signat	_main,90
	global	_usart_send
psect	text85,local,class=CODE,delta=2
global __ptext85
__ptext85:

;; *************** function _usart_send *****************
;; Defined at:
;;		line 58 in file "E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_spi_slave\src\usart.c"
;; Parameters:    Size  Location     Type
;;  length          1    wreg     unsigned char 
;;  txbuf           1    2[COMMON] PTR unsigned char 
;;		 -> recv_buf(36), 
;; Auto vars:     Size  Location     Type
;;  length          1    5[COMMON] unsigned char 
;;  i               1    6[COMMON] unsigned char 
;; Return value:  Size  Location     Type
;;		None               void
;; Registers used:
;;		wreg, fsr0l, fsr0h, status,2, status,0
;; Tracked objects:
;;		On entry : 0/0
;;		On exit  : 60/20
;;		Unchanged: FFE9F/0
;; Data sizes:     COMMON   BANK0   BANK1   BANK2
;;      Params:         1       0       0       0
;;      Locals:         2       0       0       0
;;      Temps:          2       0       0       0
;;      Totals:         5       0       0       0
;;Total ram usage:        5 bytes
;; Hardware stack levels used:    1
;; Hardware stack levels required when called:    1
;; This function calls:
;;		Nothing
;; This function is called by:
;;		_main
;; This function uses a non-reentrant model
;;
psect	text85
	file	"E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_spi_slave\src\usart.c"
	line	58
	global	__size_of_usart_send
	__size_of_usart_send	equ	__end_of_usart_send-_usart_send
	
_usart_send:	
	opt	stack 6
; Regs used in _usart_send: [wreg-fsr0h+status,2+status,0]
;usart_send@length stored from wreg
	line	61
	movwf	(usart_send@length)
	
l8797:	
;usart.c: 59: unsigned char i;
;usart.c: 61: TXIF = 0x0;
	bcf	status, 5	;RP0=0, select bank0
	bcf	status, 6	;RP1=0, select bank0
	bcf	(100/8),(100)&7
	line	62
;usart.c: 62: TXEN = 0x1;
	bsf	status, 5	;RP0=1, select bank1
	bsf	(1221/8)^080h,(1221)&7
	line	64
	
l8799:	
;usart.c: 64: for (i=length; i>0; i--) {
	movf	(usart_send@length),w
	movwf	(usart_send@i)
	
l8801:	
	movf	(usart_send@i),f
	skipz
	goto	u101
	goto	u100
u101:
	goto	l5606
u100:
	goto	l5609
	line	65
	
l5606:	
	bsf	status, 5	;RP0=1, select bank1
	btfss	(1217/8)^080h,(1217)&7
	goto	u111
	goto	u110
u111:
	goto	l5606
u110:
	line	66
	
l8805:	
;usart.c: 66: TXREG = txbuf[length-i];
	movf	(usart_send@txbuf),w
	movwf	(??_usart_send+0)+0
	movf	(usart_send@i),w
	subwf	(??_usart_send+0)+0,f
	movf	(usart_send@length),w
	addwf	0+(??_usart_send+0)+0,w
	movwf	(??_usart_send+1)+0
	movf	0+(??_usart_send+1)+0,w
	movwf	fsr0
	bcf	status, 7	;select IRP bank0
	movf	indf,w
	bcf	status, 5	;RP0=0, select bank0
	movwf	(25)	;volatile
	line	64
	
l8807:	
	decf	(usart_send@i),f
	goto	l8801
	line	69
	
l5609:	
	bcf	status, 5	;RP0=0, select bank0
	btfss	(100/8),(100)&7
	goto	u121
	goto	u120
u121:
	goto	l5609
u120:
	
l5611:	
	line	70
;usart.c: 70: TXIF = 0x0;
	bcf	(100/8),(100)&7
	line	72
;usart.c: 72: TXEN = 0x0;
	bsf	status, 5	;RP0=1, select bank1
	bcf	(1221/8)^080h,(1221)&7
	line	73
	
l5612:	
	return
	opt stack 0
GLOBAL	__end_of_usart_send
	__end_of_usart_send:
;; =============== function _usart_send ends ============

	signat	_usart_send,8312
	global	_spi_exch
psect	text86,local,class=CODE,delta=2
global __ptext86
__ptext86:

;; *************** function _spi_exch *****************
;; Defined at:
;;		line 29 in file "E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_spi_slave\src\spi.c"
;; Parameters:    Size  Location     Type
;;  length          1    wreg     unsigned char 
;;  rxbuf           1    2[COMMON] PTR unsigned char 
;;		 -> recv_buf(36), 
;;  txbuf           1    3[COMMON] PTR unsigned char 
;;		 -> send_buf(36), 
;; Auto vars:     Size  Location     Type
;;  length          1    5[COMMON] unsigned char 
;;  i               1    4[COMMON] unsigned char 
;;  tmp             1    0        unsigned char 
;; Return value:  Size  Location     Type
;;		None               void
;; Registers used:
;;		wreg, fsr0l, fsr0h, status,2, status,0
;; Tracked objects:
;;		On entry : 0/0
;;		On exit  : 0/0
;;		Unchanged: FFE9F/0
;; Data sizes:     COMMON   BANK0   BANK1   BANK2
;;      Params:         2       0       0       0
;;      Locals:         2       0       0       0
;;      Temps:          0       0       0       0
;;      Totals:         4       0       0       0
;;Total ram usage:        4 bytes
;; Hardware stack levels used:    1
;; Hardware stack levels required when called:    1
;; This function calls:
;;		Nothing
;; This function is called by:
;;		_main
;; This function uses a non-reentrant model
;;
psect	text86
	file	"E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_spi_slave\src\spi.c"
	line	29
	global	__size_of_spi_exch
	__size_of_spi_exch	equ	__end_of_spi_exch-_spi_exch
	
_spi_exch:	
	opt	stack 6
; Regs used in _spi_exch: [wreg-fsr0h+status,2+status,0]
;spi_exch@length stored from wreg
	line	34
	movwf	(spi_exch@length)
	
l8761:	
;spi.c: 30: unsigned char i;
;spi.c: 31: unsigned char tmp;
;spi.c: 34: SPFR = 0x44;
	movlw	(044h)
	bsf	status, 5	;RP0=1, select bank1
	bcf	status, 6	;RP1=0, select bank1
	movwf	(147)^080h	;volatile
	line	38
	
l8763:	
;spi.c: 38: if(L_MCUCRbits.TCYC > 0x0)
	bsf	status, 6	;RP1=1, select bank3
	rrf	(415)^0180h,w	;volatile
	andlw	(1<<2)-1
	xorlw	0
	skipnz
	goto	u31
	goto	u30
u31:
	goto	l8767
u30:
	line	40
	
l8765:	
;spi.c: 39: {
;spi.c: 40: tmp = SPDR;tmp = SPDR;tmp = SPDR;
	bcf	status, 5	;RP0=0, select bank0
	bcf	status, 6	;RP1=0, select bank0
	movf	(19),w	;volatile
	movf	(19),w	;volatile
	movf	(19),w	;volatile
	line	45
	
l8767:	
;spi.c: 41: }
;spi.c: 45: for(i=0; ((i<length) && (i < 4)); i++) { SPDR = *txbuf++; }
	clrf	(spi_exch@i)
	goto	l8775
	
l8769:	
	movf	(spi_exch@txbuf),w
	movwf	fsr0
	bcf	status, 7	;select IRP bank0
	movf	indf,w
	bcf	status, 5	;RP0=0, select bank0
	bcf	status, 6	;RP1=0, select bank0
	movwf	(19)	;volatile
	
l8771:	
	incf	(spi_exch@txbuf),f
	
l8773:	
	incf	(spi_exch@i),f
	
l8775:	
	movf	(spi_exch@length),w
	subwf	(spi_exch@i),w
	skipnc
	goto	u41
	goto	u40
u41:
	goto	l8787
u40:
	
l8777:	
	movlw	(04h)
	subwf	(spi_exch@i),w
	skipc
	goto	u51
	goto	u50
u51:
	goto	l8769
u50:
	goto	l8787
	line	47
	
l3730:	
	line	51
;spi.c: 48: {
;spi.c: 51: if (!(SPFR & (1 << 3))) {*rxbuf++ = SPDR; length--; SPDR = *txbuf++;}
	bsf	status, 5	;RP0=1, select bank1
	bcf	status, 6	;RP1=0, select bank1
	btfsc	(147)^080h,(3)&7
	goto	u61
	goto	u60
u61:
	goto	l8787
u60:
	
l8779:	
	movf	(spi_exch@rxbuf),w
	movwf	fsr0
	bcf	status, 5	;RP0=0, select bank0
	movf	(19),w	;volatile
	bcf	status, 7	;select IRP bank0
	movwf	indf
	
l8781:	
	incf	(spi_exch@rxbuf),f
	
l8783:	
	decf	(spi_exch@length),f
	movf	(spi_exch@txbuf),w
	movwf	fsr0
	movf	indf,w
	movwf	(19)	;volatile
	
l8785:	
	incf	(spi_exch@txbuf),f
	line	47
	
l8787:	
	movlw	(05h)
	subwf	(spi_exch@length),w
	skipnc
	goto	u71
	goto	u70
u71:
	goto	l3730
u70:
	goto	l8795
	line	54
	
l3734:	
	line	58
;spi.c: 55: {
;spi.c: 58: if ((SPFR & (1 << 2))) {*rxbuf++ = SPDR; length--;}
	bsf	status, 5	;RP0=1, select bank1
	bcf	status, 6	;RP1=0, select bank1
	btfss	(147)^080h,(2)&7
	goto	u81
	goto	u80
u81:
	goto	l8795
u80:
	
l8789:	
	movf	(spi_exch@rxbuf),w
	movwf	fsr0
	bcf	status, 5	;RP0=0, select bank0
	movf	(19),w	;volatile
	bcf	status, 7	;select IRP bank0
	movwf	indf
	
l8791:	
	incf	(spi_exch@rxbuf),f
	
l8793:	
	decf	(spi_exch@length),f
	line	54
	
l8795:	
	movf	(spi_exch@length),f
	skipz
	goto	u91
	goto	u90
u91:
	goto	l3734
u90:
	line	60
	
l3737:	
	return
	opt stack 0
GLOBAL	__end_of_spi_exch
	__end_of_spi_exch:
;; =============== function _spi_exch ends ============

	signat	_spi_exch,12408
	global	_spi_init_sla
psect	text87,local,class=CODE,delta=2
global __ptext87
__ptext87:

;; *************** function _spi_init_sla *****************
;; Defined at:
;;		line 12 in file "E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_spi_slave\src\spi.c"
;; Parameters:    Size  Location     Type
;;		None
;; Auto vars:     Size  Location     Type
;;		None
;; Return value:  Size  Location     Type
;;		None               void
;; Registers used:
;;		wreg, status,2
;; Tracked objects:
;;		On entry : 60/0
;;		On exit  : 60/0
;;		Unchanged: FFF9F/0
;; Data sizes:     COMMON   BANK0   BANK1   BANK2
;;      Params:         0       0       0       0
;;      Locals:         0       0       0       0
;;      Temps:          0       0       0       0
;;      Totals:         0       0       0       0
;;Total ram usage:        0 bytes
;; Hardware stack levels used:    1
;; Hardware stack levels required when called:    1
;; This function calls:
;;		Nothing
;; This function is called by:
;;		_main
;; This function uses a non-reentrant model
;;
psect	text87
	file	"E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_spi_slave\src\spi.c"
	line	12
	global	__size_of_spi_init_sla
	__size_of_spi_init_sla	equ	__end_of_spi_init_sla-_spi_init_sla
	
_spi_init_sla:	
	opt	stack 6
; Regs used in _spi_init_sla: [wreg+status,2]
	line	13
	
l8753:	
;spi.c: 13: RA6 = 0x1;
	bsf	(46/8),(46)&7
	line	14
;spi.c: 14: RA4 = 0x1;
	bsf	(44/8),(44)&7
	line	15
;spi.c: 15: RA5 = 0x1;
	bsf	(45/8),(45)&7
	line	16
;spi.c: 16: RB0 = 0x1;
	bsf	(48/8),(48)&7
	line	18
;spi.c: 18: TRISA4 = 0x1;
	bsf	status, 5	;RP0=1, select bank1
	bsf	(1068/8)^080h,(1068)&7
	line	19
;spi.c: 19: TRISA5 = 0x1;
	bsf	(1069/8)^080h,(1069)&7
	line	20
;spi.c: 20: TRISA6 = 0x1;
	bsf	(1070/8)^080h,(1070)&7
	line	21
;spi.c: 21: TRISB0 = 0x0;
	bcf	(1072/8)^080h,(1072)&7
	line	23
	
l8755:	
;spi.c: 23: SPSR = 0x00;
	clrf	(148)^080h	;volatile
	line	24
	
l8757:	
;spi.c: 24: SPCR = 0x01;
	movlw	(01h)
	bcf	status, 5	;RP0=0, select bank0
	movwf	(20)	;volatile
	line	25
	
l8759:	
;spi.c: 25: SPCR = 0x41;
	movlw	(041h)
	movwf	(20)	;volatile
	line	26
	
l3720:	
	return
	opt stack 0
GLOBAL	__end_of_spi_init_sla
	__end_of_spi_init_sla:
;; =============== function _spi_init_sla ends ============

	signat	_spi_init_sla,88
	global	_usart_init
psect	text88,local,class=CODE,delta=2
global __ptext88
__ptext88:

;; *************** function _usart_init *****************
;; Defined at:
;;		line 12 in file "E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_spi_slave\src\usart.c"
;; Parameters:    Size  Location     Type
;;		None
;; Auto vars:     Size  Location     Type
;;		None
;; Return value:  Size  Location     Type
;;		None               void
;; Registers used:
;;		wreg, status,2
;; Tracked objects:
;;		On entry : 60/60
;;		On exit  : 60/0
;;		Unchanged: FFF9F/0
;; Data sizes:     COMMON   BANK0   BANK1   BANK2
;;      Params:         0       0       0       0
;;      Locals:         0       0       0       0
;;      Temps:          0       0       0       0
;;      Totals:         0       0       0       0
;;Total ram usage:        0 bytes
;; Hardware stack levels used:    1
;; Hardware stack levels required when called:    1
;; This function calls:
;;		Nothing
;; This function is called by:
;;		_main
;; This function uses a non-reentrant model
;;
psect	text88
	file	"E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_spi_slave\src\usart.c"
	line	12
	global	__size_of_usart_init
	__size_of_usart_init	equ	__end_of_usart_init-_usart_init
	
_usart_init:	
	opt	stack 6
; Regs used in _usart_init: [wreg+status,2]
	line	13
	
l8743:	
;usart.c: 13: RB5 = 0x1;
	bcf	status, 5	;RP0=0, select bank0
	bcf	status, 6	;RP1=0, select bank0
	bsf	(53/8),(53)&7
	line	14
;usart.c: 14: RB6 = 0x1;
	bsf	(54/8),(54)&7
	line	16
;usart.c: 16: TRISB5 = 0x0;
	bsf	status, 5	;RP0=1, select bank1
	bcf	(1077/8)^080h,(1077)&7
	line	17
;usart.c: 17: TRISB6 = 0x1;
	bsf	(1078/8)^080h,(1078)&7
	line	19
	
l8745:	
;usart.c: 19: SPBRGH = 0x0;
	clrf	(154)^080h	;volatile
	line	20
	
l8747:	
;usart.c: 20: SPBRG = 0xc;
	movlw	(0Ch)
	movwf	(153)^080h	;volatile
	line	23
	
l8749:	
;usart.c: 23: TXSTA = 0x24;
	movlw	(024h)
	movwf	(152)^080h	;volatile
	line	24
	
l8751:	
;usart.c: 24: RCSTA = 0x80;
	movlw	(080h)
	bcf	status, 5	;RP0=0, select bank0
	movwf	(24)	;volatile
	line	25
	
l5586:	
	return
	opt stack 0
GLOBAL	__end_of_usart_init
	__end_of_usart_init:
;; =============== function _usart_init ends ============

	signat	_usart_init,88
	global	_misc_eer
psect	text89,local,class=CODE,delta=2
global __ptext89
__ptext89:

;; *************** function _misc_eer *****************
;; Defined at:
;;		line 6 in file "E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_spi_slave\src\misc.c"
;; Parameters:    Size  Location     Type
;;  type            1    wreg     unsigned char 
;;  address         2    2[COMMON] unsigned int 
;; Auto vars:     Size  Location     Type
;;  type            1    4[COMMON] unsigned char 
;; Return value:  Size  Location     Type
;;                  2    2[COMMON] unsigned int 
;; Registers used:
;;		wreg
;; Tracked objects:
;;		On entry : 20/0
;;		On exit  : 60/40
;;		Unchanged: FFE00/0
;; Data sizes:     COMMON   BANK0   BANK1   BANK2
;;      Params:         2       0       0       0
;;      Locals:         1       0       0       0
;;      Temps:          0       0       0       0
;;      Totals:         3       0       0       0
;;Total ram usage:        3 bytes
;; Hardware stack levels used:    1
;; Hardware stack levels required when called:    1
;; This function calls:
;;		Nothing
;; This function is called by:
;;		_main
;; This function uses a non-reentrant model
;;
psect	text89
	file	"E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_spi_slave\src\misc.c"
	line	6
	global	__size_of_misc_eer
	__size_of_misc_eer	equ	__end_of_misc_eer-_misc_eer
	
_misc_eer:	
	opt	stack 6
; Regs used in _misc_eer: [wreg]
;misc_eer@type stored from wreg
	movwf	(misc_eer@type)
	line	7
	
l8735:	
;misc.c: 7: EEADR = address & 0xff;
	movf	(misc_eer@address),w
	bsf	status, 6	;RP1=1, select bank2
	movwf	(269)^0100h	;volatile
	line	8
;misc.c: 8: EEADRH = (address >> 8) & 0xff;
	movf	(misc_eer@address+1),w
	movwf	(271)^0100h	;volatile
	line	10
	
l8737:	
;misc.c: 10: EEPGD = type & 1;
	btfsc	(misc_eer@type),0
	goto	u11
	goto	u10
	
u11:
	bsf	status, 5	;RP0=1, select bank3
	bsf	status, 6	;RP1=1, select bank3
	bsf	(3175/8)^0180h,(3175)&7
	goto	u24
u10:
	bsf	status, 5	;RP0=1, select bank3
	bsf	status, 6	;RP1=1, select bank3
	bcf	(3175/8)^0180h,(3175)&7
u24:
	line	12
	
l8739:	
;misc.c: 12: EERD = 1;
	bsf	(3168/8)^0180h,(3168)&7
	line	14
;misc.c: 14: return (EEDATH << 8) | (EEDAT);
	bcf	status, 5	;RP0=0, select bank2
	movf	(270)^0100h,w	;volatile
	movwf	(?_misc_eer+1)
	clrf	(?_misc_eer)
	movf	(268)^0100h,w	;volatile
	iorwf	(?_misc_eer),f
	line	15
	
l7481:	
	return
	opt stack 0
GLOBAL	__end_of_misc_eer
	__end_of_misc_eer:
;; =============== function _misc_eer ends ============

	signat	_misc_eer,8314
	global	_hisr
psect	text90,local,class=CODE,delta=2
global __ptext90
__ptext90:

;; *************** function _hisr *****************
;; Defined at:
;;		line 46 in file "E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_spi_slave\src\main.c"
;; Parameters:    Size  Location     Type
;;		None
;; Auto vars:     Size  Location     Type
;;		None
;; Return value:  Size  Location     Type
;;		None               void
;; Registers used:
;;		None
;; Tracked objects:
;;		On entry : 0/0
;;		On exit  : 0/0
;;		Unchanged: FFE00/0
;; Data sizes:     COMMON   BANK0   BANK1   BANK2
;;      Params:         0       0       0       0
;;      Locals:         0       0       0       0
;;      Temps:          2       0       0       0
;;      Totals:         2       0       0       0
;;Total ram usage:        2 bytes
;; Hardware stack levels used:    1
;; This function calls:
;;		Nothing
;; This function is called by:
;;		Interrupt level 1
;; This function uses a non-reentrant model
;;
psect	text90
	file	"E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_spi_slave\src\main.c"
	line	46
	global	__size_of_hisr
	__size_of_hisr	equ	__end_of_hisr-_hisr
	
_hisr:	
	opt	stack 6
; Regs used in _hisr: []
psect	intentry,class=CODE,delta=2
global __pintentry
__pintentry:
global interrupt_function
interrupt_function:
	global saved_w
	saved_w	set	btemp+0
	movwf	saved_w
	swapf	status,w
	movwf	(??_hisr+0)
	movf	pclath,w
	movwf	(??_hisr+1)
	ljmp	_hisr
psect	text90
	line	47
	
i1l8733:	
# 47 "E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_spi_slave\src\main.c"
nop ;#
psect	text90
	line	48
	
i1l1863:	
	movf	(??_hisr+1),w
	movwf	pclath
	swapf	(??_hisr+0)^0FFFFFF80h,w
	movwf	status
	swapf	saved_w,f
	swapf	saved_w,w
	retfie
	opt stack 0
GLOBAL	__end_of_hisr
	__end_of_hisr:
;; =============== function _hisr ends ============

	signat	_hisr,88
psect	text91,local,class=CODE,delta=2
global __ptext91
__ptext91:
	global	btemp
	btemp set 07Eh

	DABS	1,126,2	;btemp
	global	wtemp0
	wtemp0 set btemp
	end
