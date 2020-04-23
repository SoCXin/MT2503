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
	FNCALL	_main,_twi_init
	FNCALL	_main,_twi_master_send
	FNCALL	_main,_twi_master_recv
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
	file	"E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_twi_master\src\main.c"
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
	global	_TWCR
_TWCR	set	21
	global	_TWDR
_TWDR	set	23
	global	_TXREG
_TXREG	set	25
	global	_GIE
_GIE	set	95
	global	_PEIE
_PEIE	set	94
	global	_RA0
_RA0	set	40
	global	_RA1
_RA1	set	41
	global	_RB5
_RB5	set	53
	global	_RB6
_RB6	set	54
	global	_TWIEN
_TWIEN	set	175
	global	_TWIF
_TWIF	set	104
	global	_TWIRACK
_TWIRACK	set	170
	global	_TWIRXD
_TWIRXD	set	177
	global	_TWIRXK
_TWIRXK	set	182
	global	_TWIRXP
_TWIRXP	set	179
	global	_TWIRXS
_TWIRXS	set	176
	global	_TWITACK
_TWITACK	set	171
	global	_TWITXD
_TWITXD	set	181
	global	_TWITXK
_TWITXK	set	178
	global	_TWITXP
_TWITXP	set	183
	global	_TWITXS
_TWITXS	set	180
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
	global	_TXSTA
_TXSTA	set	152
	global	_TRISA0
_TRISA0	set	1064
	global	_TRISA1
_TRISA1	set	1065
	global	_TRISB5
_TRISB5	set	1077
	global	_TRISB6
_TRISB6	set	1078
	global	_TRMT
_TRMT	set	1217
	global	_TXEN
_TXEN	set	1221
	global	_WPUA0
_WPUA0	set	1192
	global	_WPUA1
_WPUA1	set	1193
	global	_nRABPU
_nRABPU	set	1039
	global	_EEADR
_EEADR	set	269
	global	_EEADRH
_EEADRH	set	271
	global	_EEDAT
_EEDAT	set	268
	global	_EEDATH
_EEDATH	set	270
	global	_MCUCR
_MCUCR	set	415
	global	_EEPGD
_EEPGD	set	3175
	global	_EERD
_EERD	set	3168
	file	"twi_master.as"
	line	#
psect cinit,class=CODE,delta=2
global start_initialization
start_initialization:

psect	dataBANK0,class=BANK0,space=1
global __pdataBANK0
__pdataBANK0:
	file	"E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_twi_master\src\main.c"
	line	31
_recv_buf:
       ds      36

psect	dataBANK0
	file	"E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_twi_master\src\main.c"
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
	global	?_twi_init
?_twi_init:	; 0 bytes @ 0x0
	global	?_hisr
?_hisr:	; 0 bytes @ 0x0
	global	??_hisr
??_hisr:	; 0 bytes @ 0x0
	global	?_main
?_main:	; 2 bytes @ 0x0
	ds	2
	global	??_usart_init
??_usart_init:	; 0 bytes @ 0x2
	global	??_twi_init
??_twi_init:	; 0 bytes @ 0x2
	global	?_twi_master_send
?_twi_master_send:	; 0 bytes @ 0x2
	global	?_twi_master_recv
?_twi_master_recv:	; 0 bytes @ 0x2
	global	?_usart_send
?_usart_send:	; 0 bytes @ 0x2
	global	?_misc_eer
?_misc_eer:	; 2 bytes @ 0x2
	global	twi_master_send@send_adr
twi_master_send@send_adr:	; 1 bytes @ 0x2
	global	twi_master_recv@send_adr
twi_master_recv@send_adr:	; 1 bytes @ 0x2
	global	usart_send@txbuf
usart_send@txbuf:	; 1 bytes @ 0x2
	global	misc_eer@address
misc_eer@address:	; 2 bytes @ 0x2
	ds	1
	global	??_usart_send
??_usart_send:	; 0 bytes @ 0x3
	global	twi_master_send@send_buf
twi_master_send@send_buf:	; 1 bytes @ 0x3
	global	twi_master_recv@recv_buf
twi_master_recv@recv_buf:	; 1 bytes @ 0x3
	ds	1
	global	??_misc_eer
??_misc_eer:	; 0 bytes @ 0x4
	global	twi_master_send@length
twi_master_send@length:	; 1 bytes @ 0x4
	global	twi_master_recv@length
twi_master_recv@length:	; 1 bytes @ 0x4
	global	misc_eer@type
misc_eer@type:	; 1 bytes @ 0x4
	ds	1
	global	??_twi_master_send
??_twi_master_send:	; 0 bytes @ 0x5
	global	??_twi_master_recv
??_twi_master_recv:	; 0 bytes @ 0x5
	global	usart_send@length
usart_send@length:	; 1 bytes @ 0x5
	ds	1
	global	usart_send@i
usart_send@i:	; 1 bytes @ 0x6
	ds	1
	global	twi_master_send@sla_addr
twi_master_send@sla_addr:	; 1 bytes @ 0x7
	global	twi_master_recv@sla_addr
twi_master_recv@sla_addr:	; 1 bytes @ 0x7
	ds	1
	global	twi_master_send@i
twi_master_send@i:	; 1 bytes @ 0x8
	global	twi_master_recv@i
twi_master_recv@i:	; 1 bytes @ 0x8
	ds	1
	global	??_main
??_main:	; 0 bytes @ 0x9
	global	main@i
main@i:	; 2 bytes @ 0x9
	ds	2
	global	main@j
main@j:	; 1 bytes @ 0xB
	ds	1
;;Data sizes: Strings 0, constant 0, data 72, bss 0, persistent 0 stack 0
;;Auto spaces:   Size  Autos    Used
;; COMMON          14     12      12
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
;; twi_master_recv@recv_buf	PTR unsigned char  size(1) Largest target is 36
;;		 -> recv_buf(BANK0[36]), 
;;
;; twi_master_send@send_buf	PTR unsigned char  size(1) Largest target is 36
;;		 -> send_buf(BANK0[36]), 
;;


;;
;; Critical Paths under _main in COMMON
;;
;;   _main->_twi_master_send
;;   _main->_twi_master_recv
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
;; (0) _main                                                 3     3      0     465
;;                                              9 COMMON     3     3      0
;;                           _misc_eer
;;                         _usart_init
;;                           _twi_init
;;                    _twi_master_send
;;                    _twi_master_recv
;;                         _usart_send
;; ---------------------------------------------------------------------------------
;; (1) _usart_send                                           5     4      1      90
;;                                              2 COMMON     5     4      1
;; ---------------------------------------------------------------------------------
;; (1) _twi_master_recv                                      7     4      3     150
;;                                              2 COMMON     7     4      3
;; ---------------------------------------------------------------------------------
;; (1) _twi_master_send                                      7     4      3     120
;;                                              2 COMMON     7     4      3
;; ---------------------------------------------------------------------------------
;; (1) _twi_init                                             0     0      0       0
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
;;   _twi_init
;;   _twi_master_send
;;   _twi_master_recv
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
;;DATA                 0      0      55      10        0.0%
;;ABS                  0      0      54       8        0.0%
;;NULL                 0      0       0       0        0.0%
;;STACK                0      0       1       2        0.0%
;;BANK0               50      0      48       3       90.0%
;;BITBANK0            50      0       0       9        0.0%
;;SFR0                 0      0       0       1        0.0%
;;BITSFR0              0      0       0       1        0.0%
;;COMMON               E      C       C       1       85.7%
;;BITCOMMON            E      0       0       0        0.0%
;;EEDATA             100      0       0       0        0.0%

	global	_main
psect	maintext,global,class=CODE,delta=2
global __pmaintext
__pmaintext:

;; *************** function _main *****************
;; Defined at:
;;		line 51 in file "E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_twi_master\src\main.c"
;; Parameters:    Size  Location     Type
;;		None
;; Auto vars:     Size  Location     Type
;;  i               2    9[COMMON] unsigned int 
;;  j               1   11[COMMON] unsigned char 
;; Return value:  Size  Location     Type
;;                  2  1867[COMMON] int 
;; Registers used:
;;		wreg, fsr0l, fsr0h, status,2, status,0, pclath, cstack
;; Tracked objects:
;;		On entry : 17F/0
;;		On exit  : 60/0
;;		Unchanged: FFE00/0
;; Data sizes:     COMMON   BANK0   BANK1   BANK2
;;      Params:         0       0       0       0
;;      Locals:         3       0       0       0
;;      Temps:          0       0       0       0
;;      Totals:         3       0       0       0
;;Total ram usage:        3 bytes
;; Hardware stack levels required when called:    2
;; This function calls:
;;		_misc_eer
;;		_usart_init
;;		_twi_init
;;		_twi_master_send
;;		_twi_master_recv
;;		_usart_send
;; This function is called by:
;;		Startup code after reset
;; This function uses a non-reentrant model
;;
psect	maintext
	file	"E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_twi_master\src\main.c"
	line	51
	global	__size_of_main
	__size_of_main	equ	__end_of_main-_main
	
_main:	
	opt	stack 6
; Regs used in _main: [wreg-fsr0h+status,2+status,0+pclath+cstack]
	line	56
	
l8977:	
;main.c: 52: unsigned char j;
;main.c: 53: unsigned int i;
;main.c: 56: if((0x5a == misc_eer(1, 0x100f)))
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
	goto	u381
	goto	u380
u381:
	goto	l8981
u380:
	line	57
	
l8979:	
;main.c: 57: OSCTUNE = misc_eer(1, 0x1007);
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
	goto	l8983
	line	59
	
l8981:	
;main.c: 58: else
;main.c: 59: OSCTUNE = 0x92;
	movlw	(092h)
	bsf	status, 5	;RP0=1, select bank1
	bcf	status, 6	;RP1=0, select bank1
	movwf	(144)^080h	;volatile
	line	62
	
l8983:	
;main.c: 62: OSCCONbits.IRCF = 0x6;
	movf	(143)^080h,w	;volatile
	andlw	not (((1<<3)-1)<<4)
	iorlw	(06h & ((1<<3)-1))<<4
	movwf	(143)^080h	;volatile
	line	64
	
l8985:	
;main.c: 64: MCUCR = 0x55;
	movlw	(055h)
	bsf	status, 6	;RP1=1, select bank3
	movwf	(415)^0180h	;volatile
	line	65
	
l8987:	
;main.c: 65: MCUCR = 0xC6;
	movlw	(0C6h)
	movwf	(415)^0180h	;volatile
	line	67
	
l8989:	
;main.c: 67: usart_init();
	fcall	_usart_init
	line	69
	
l8991:	
;main.c: 69: twi_init();
	fcall	_twi_init
	line	73
	
l8993:	
;main.c: 73: twi_master_send(0x50, 0x30, send_buf, 0x8);
	movlw	(030h)
	movwf	(?_twi_master_send)
	movlw	(_send_buf)&0ffh
	movwf	0+(?_twi_master_send)+01h
	movlw	(08h)
	movwf	0+(?_twi_master_send)+02h
	movlw	(050h)
	fcall	_twi_master_send
	line	77
	
l8995:	
;main.c: 77: for(i=0; i< 0x7f0; i++)
	clrf	(main@i)
	clrf	(main@i+1)
	line	79
	
l9001:	
;main.c: 78: {
;main.c: 79: for(j=0; j < 0xf0; j++) { asm("nop"); }
	clrf	(main@j)
	
l1872:	
# 79 "E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_twi_master\src\main.c"
nop ;#
psect	maintext
	
l9007:	
	incf	(main@j),f
	
l9009:	
	movlw	(0F0h)
	subwf	(main@j),w
	skipc
	goto	u391
	goto	u390
u391:
	goto	l1872
u390:
	line	77
	
l9011:	
	incf	(main@i),f
	skipnz
	incf	(main@i+1),f
	
l9013:	
	movlw	high(07F0h)
	subwf	(main@i+1),w
	movlw	low(07F0h)
	skipnz
	subwf	(main@i),w
	skipc
	goto	u401
	goto	u400
u401:
	goto	l9001
u400:
	line	84
	
l9015:	
;main.c: 80: }
;main.c: 84: twi_master_recv(0x50, 0x30, recv_buf, 0x8);
	movlw	(030h)
	movwf	(?_twi_master_recv)
	movlw	(_recv_buf)&0ffh
	movwf	0+(?_twi_master_recv)+01h
	movlw	(08h)
	movwf	0+(?_twi_master_recv)+02h
	movlw	(050h)
	fcall	_twi_master_recv
	line	86
;main.c: 86: usart_send(8, recv_buf);
	movlw	(_recv_buf)&0ffh
	movwf	(?_usart_send)
	movlw	(08h)
	fcall	_usart_send
	line	88
	
l9017:	
;main.c: 88: TWIEN = 0x0;
	bcf	status, 5	;RP0=0, select bank0
	bcf	(175/8),(175)&7
	line	93
	
l9019:	
;main.c: 93: PEIE = 0x1;
	bsf	(94/8),(94)&7
	line	94
	
l9021:	
;main.c: 94: GIE = 0x0;
	bcf	(95/8),(95)&7
	line	96
	
l1875:	
	goto	l1875
	global	start
	ljmp	start
	opt stack 0
psect	maintext
	line	97
GLOBAL	__end_of_main
	__end_of_main:
;; =============== function _main ends ============

	signat	_main,90
	global	_usart_send
psect	text103,local,class=CODE,delta=2
global __ptext103
__ptext103:

;; *************** function _usart_send *****************
;; Defined at:
;;		line 58 in file "E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_twi_master\src\usart.c"
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
;;		On entry : 60/0
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
psect	text103
	file	"E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_twi_master\src\usart.c"
	line	58
	global	__size_of_usart_send
	__size_of_usart_send	equ	__end_of_usart_send-_usart_send
	
_usart_send:	
	opt	stack 6
; Regs used in _usart_send: [wreg-fsr0h+status,2+status,0]
;usart_send@length stored from wreg
	line	61
	movwf	(usart_send@length)
	
l8963:	
;usart.c: 59: unsigned char i;
;usart.c: 61: TXIF = 0x0;
	bcf	(100/8),(100)&7
	line	62
;usart.c: 62: TXEN = 0x1;
	bsf	status, 5	;RP0=1, select bank1
	bsf	(1221/8)^080h,(1221)&7
	line	64
	
l8965:	
;usart.c: 64: for (i=length; i>0; i--) {
	movf	(usart_send@length),w
	movwf	(usart_send@i)
	
l8967:	
	movf	(usart_send@i),f
	skipz
	goto	u351
	goto	u350
u351:
	goto	l5718
u350:
	goto	l5721
	line	65
	
l5718:	
	bsf	status, 5	;RP0=1, select bank1
	btfss	(1217/8)^080h,(1217)&7
	goto	u361
	goto	u360
u361:
	goto	l5718
u360:
	line	66
	
l8971:	
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
	
l8973:	
	decf	(usart_send@i),f
	goto	l8967
	line	69
	
l5721:	
	bcf	status, 5	;RP0=0, select bank0
	btfss	(100/8),(100)&7
	goto	u371
	goto	u370
u371:
	goto	l5721
u370:
	
l5723:	
	line	70
;usart.c: 70: TXIF = 0x0;
	bcf	(100/8),(100)&7
	line	72
;usart.c: 72: TXEN = 0x0;
	bsf	status, 5	;RP0=1, select bank1
	bcf	(1221/8)^080h,(1221)&7
	line	73
	
l5724:	
	return
	opt stack 0
GLOBAL	__end_of_usart_send
	__end_of_usart_send:
;; =============== function _usart_send ends ============

	signat	_usart_send,8312
	global	_twi_master_recv
psect	text104,local,class=CODE,delta=2
global __ptext104
__ptext104:

;; *************** function _twi_master_recv *****************
;; Defined at:
;;		line 278 in file "E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_twi_master\src\twi.c"
;; Parameters:    Size  Location     Type
;;  sla_addr        1    wreg     unsigned char 
;;  send_adr        1    2[COMMON] unsigned char 
;;  recv_buf        1    3[COMMON] PTR unsigned char 
;;		 -> recv_buf(36), 
;;  length          1    4[COMMON] unsigned char 
;; Auto vars:     Size  Location     Type
;;  sla_addr        1    7[COMMON] unsigned char 
;;  i               1    8[COMMON] unsigned char 
;; Return value:  Size  Location     Type
;;		None               void
;; Registers used:
;;		wreg, fsr0l, fsr0h, status,2, status,0
;; Tracked objects:
;;		On entry : 0/0
;;		On exit  : 60/0
;;		Unchanged: FFE9F/0
;; Data sizes:     COMMON   BANK0   BANK1   BANK2
;;      Params:         3       0       0       0
;;      Locals:         2       0       0       0
;;      Temps:          2       0       0       0
;;      Totals:         7       0       0       0
;;Total ram usage:        7 bytes
;; Hardware stack levels used:    1
;; Hardware stack levels required when called:    1
;; This function calls:
;;		Nothing
;; This function is called by:
;;		_main
;; This function uses a non-reentrant model
;;
psect	text104
	file	"E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_twi_master\src\twi.c"
	line	278
	global	__size_of_twi_master_recv
	__size_of_twi_master_recv	equ	__end_of_twi_master_recv-_twi_master_recv
	
_twi_master_recv:	
	opt	stack 6
; Regs used in _twi_master_recv: [wreg-fsr0h+status,2+status,0]
;twi_master_recv@sla_addr stored from wreg
	line	281
	movwf	(twi_master_recv@sla_addr)
	
l8911:	
;twi.c: 279: unsigned char i;
;twi.c: 281: TWIF = 0x0;
	bcf	status, 5	;RP0=0, select bank0
	bcf	status, 6	;RP1=0, select bank0
	bcf	(104/8),(104)&7
	line	285
;twi.c: 285: TWIRXS = 0x0;
	bcf	(176/8),(176)&7
	line	286
;twi.c: 286: TWITXS = 0x1;
	bsf	(180/8),(180)&7
	line	288
;twi.c: 288: while(TWIF==0x0);
	
l3815:	
	btfss	(104/8),(104)&7
	goto	u171
	goto	u170
u171:
	goto	l3815
u170:
	
l3817:	
	line	290
;twi.c: 290: if (TWIRXS==0x1) {
	btfss	(176/8),(176)&7
	goto	u181
	goto	u180
u181:
	goto	l8915
u180:
	line	292
	
l8913:	
;twi.c: 292: TWIRXS = 0x0;
	bcf	(176/8),(176)&7
	line	298
	
l8915:	
;twi.c: 293: }
;twi.c: 298: TWDR = (sla_addr << 1) + 0x0;
	clrc
	rlf	(twi_master_recv@sla_addr),w
	movwf	(23)	;volatile
	line	299
	
l8917:	
;twi.c: 299: TWITXD = 0x1;
	bsf	(181/8),(181)&7
	line	301
	
l8919:	
;twi.c: 301: TWIF = 0x0;
	bcf	(104/8),(104)&7
	line	303
;twi.c: 303: while(TWIF==0x0);
	
l3819:	
	btfss	(104/8),(104)&7
	goto	u191
	goto	u190
u191:
	goto	l3819
u190:
	
l3821:	
	line	305
;twi.c: 305: if (TWIRXK==0x1) {
	btfss	(182/8),(182)&7
	goto	u201
	goto	u200
u201:
	goto	l8925
u200:
	line	306
	
l8921:	
;twi.c: 306: if (TWIRACK == 0x1) {goto _not_addressed_;}
	btfss	(170/8),(170)&7
	goto	u211
	goto	u210
u211:
	goto	l3823
u210:
	goto	l3824
	
l3823:	
	line	307
;twi.c: 307: TWIRXK = 0x0;
	bcf	(182/8),(182)&7
	line	313
	
l8925:	
;twi.c: 308: }
;twi.c: 313: TWDR = send_adr;
	movf	(twi_master_recv@send_adr),w
	movwf	(23)	;volatile
	line	314
	
l8927:	
;twi.c: 314: TWITXD = 0x1;
	bsf	(181/8),(181)&7
	line	316
	
l8929:	
;twi.c: 316: TWIF = 0x0;
	bcf	(104/8),(104)&7
	line	318
;twi.c: 318: while(TWIF==0x0);
	
l3825:	
	btfss	(104/8),(104)&7
	goto	u221
	goto	u220
u221:
	goto	l3825
u220:
	
l3827:	
	line	320
;twi.c: 320: if (TWIRXK==0x1) {
	btfss	(182/8),(182)&7
	goto	u231
	goto	u230
u231:
	goto	l3828
u230:
	line	321
	
l8931:	
;twi.c: 321: if (TWIRACK == 0x1) {goto _not_addressed_;}
	btfss	(170/8),(170)&7
	goto	u241
	goto	u240
u241:
	goto	l3829
u240:
	goto	l3824
	
l3829:	
	line	322
;twi.c: 322: TWIRXK = 0x0;
	bcf	(182/8),(182)&7
	line	323
	
l3828:	
	line	327
;twi.c: 323: }
;twi.c: 327: TWIRXS = 0x0;
	bcf	(176/8),(176)&7
	line	328
;twi.c: 328: TWITXS = 0x1;
	bsf	(180/8),(180)&7
	line	330
;twi.c: 330: TWIF = 0x0;
	bcf	(104/8),(104)&7
	line	331
;twi.c: 331: while(TWIF==0x0);
	
l3830:	
	btfss	(104/8),(104)&7
	goto	u251
	goto	u250
u251:
	goto	l3830
u250:
	
l3832:	
	line	333
;twi.c: 333: if (TWIRXS==0x1) {
	btfss	(176/8),(176)&7
	goto	u261
	goto	u260
u261:
	goto	l8937
u260:
	line	335
	
l8935:	
;twi.c: 335: TWIRXS = 0x0;
	bcf	(176/8),(176)&7
	line	341
	
l8937:	
;twi.c: 336: }
;twi.c: 341: TWDR = (sla_addr << 1) + 0x1;
	setc
	rlf	(twi_master_recv@sla_addr),w
	movwf	(23)	;volatile
	line	342
	
l8939:	
;twi.c: 342: TWITXD = 0x1;
	bsf	(181/8),(181)&7
	line	344
	
l8941:	
;twi.c: 344: TWIF = 0x0;
	bcf	(104/8),(104)&7
	line	346
;twi.c: 346: while(TWIF==0x0);
	
l3834:	
	btfss	(104/8),(104)&7
	goto	u271
	goto	u270
u271:
	goto	l3834
u270:
	
l3836:	
	line	348
;twi.c: 348: if (TWIRXK==0x1) {
	btfss	(182/8),(182)&7
	goto	u281
	goto	u280
u281:
	goto	l8947
u280:
	line	349
	
l8943:	
;twi.c: 349: if (TWIRACK == 0x1) {goto _not_addressed_;}
	btfss	(170/8),(170)&7
	goto	u291
	goto	u290
u291:
	goto	l3838
u290:
	goto	l3824
	
l3838:	
	line	350
;twi.c: 350: TWIRXK = 0x0;
	bcf	(182/8),(182)&7
	line	356
	
l8947:	
;twi.c: 351: }
;twi.c: 356: for(i=length; i>0; i--)
	movf	(twi_master_recv@length),w
	movwf	(twi_master_recv@i)
	
l8949:	
	movf	(twi_master_recv@i),f
	skipz
	goto	u301
	goto	u300
u301:
	goto	l3839
u300:
	goto	l3824
	line	357
	
l3839:	
	line	358
;twi.c: 357: {
;twi.c: 358: TWITACK = 0x0;
	bcf	(171/8),(171)&7
	line	359
	
l8953:	
;twi.c: 359: if (i==0x1) { TWITACK = 0x1; }
	decf	(twi_master_recv@i),w
	skipz
	goto	u311
	goto	u310
u311:
	goto	l3841
u310:
	
l8955:	
	bsf	(171/8),(171)&7
	
l3841:	
	line	360
;twi.c: 360: TWITXK = 0x1;
	bsf	(178/8),(178)&7
	line	362
;twi.c: 362: TWIF = 0x0;
	bcf	(104/8),(104)&7
	line	364
;twi.c: 364: while(TWIF==0x0);
	
l3842:	
	btfss	(104/8),(104)&7
	goto	u321
	goto	u320
u321:
	goto	l3842
u320:
	
l3844:	
	line	366
;twi.c: 366: if (TWIRXD==0x1) {
	btfss	(177/8),(177)&7
	goto	u331
	goto	u330
u331:
	goto	l8959
u330:
	line	367
	
l8957:	
;twi.c: 367: recv_buf[length-i] = TWDR;
	movf	(twi_master_recv@recv_buf),w
	movwf	(??_twi_master_recv+0)+0
	movf	(twi_master_recv@i),w
	subwf	(??_twi_master_recv+0)+0,f
	movf	(twi_master_recv@length),w
	addwf	0+(??_twi_master_recv+0)+0,w
	movwf	(??_twi_master_recv+1)+0
	movf	0+(??_twi_master_recv+1)+0,w
	movwf	fsr0
	movf	(23),w	;volatile
	bcf	status, 7	;select IRP bank0
	movwf	indf
	line	356
	
l8959:	
	decf	(twi_master_recv@i),f
	goto	l8949
	line	374
;twi.c: 368: }
;twi.c: 369: }
;twi.c: 374: _not_addressed_:
	
l3824:	
	line	376
;twi.c: 376: TWIRXP = 0x0;
	bcf	(179/8),(179)&7
	line	377
;twi.c: 377: TWITXP = 0x1;
	bsf	(183/8),(183)&7
	line	378
;twi.c: 378: TWIF = 0x0;
	bcf	(104/8),(104)&7
	line	379
;twi.c: 379: while(TWITXP == 0x1);
	
l3846:	
	btfsc	(183/8),(183)&7
	goto	u341
	goto	u340
u341:
	goto	l3846
u340:
	line	380
	
l3849:	
	return
	opt stack 0
GLOBAL	__end_of_twi_master_recv
	__end_of_twi_master_recv:
;; =============== function _twi_master_recv ends ============

	signat	_twi_master_recv,16504
	global	_twi_master_send
psect	text105,local,class=CODE,delta=2
global __ptext105
__ptext105:

;; *************** function _twi_master_send *****************
;; Defined at:
;;		line 196 in file "E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_twi_master\src\twi.c"
;; Parameters:    Size  Location     Type
;;  sla_addr        1    wreg     unsigned char 
;;  send_adr        1    2[COMMON] unsigned char 
;;  send_buf        1    3[COMMON] PTR unsigned char 
;;		 -> send_buf(36), 
;;  length          1    4[COMMON] unsigned char 
;; Auto vars:     Size  Location     Type
;;  sla_addr        1    7[COMMON] unsigned char 
;;  i               1    8[COMMON] unsigned char 
;; Return value:  Size  Location     Type
;;		None               void
;; Registers used:
;;		wreg, fsr0l, fsr0h, status,2, status,0
;; Tracked objects:
;;		On entry : 60/0
;;		On exit  : 60/0
;;		Unchanged: FFE9F/0
;; Data sizes:     COMMON   BANK0   BANK1   BANK2
;;      Params:         3       0       0       0
;;      Locals:         2       0       0       0
;;      Temps:          2       0       0       0
;;      Totals:         7       0       0       0
;;Total ram usage:        7 bytes
;; Hardware stack levels used:    1
;; Hardware stack levels required when called:    1
;; This function calls:
;;		Nothing
;; This function is called by:
;;		_main
;; This function uses a non-reentrant model
;;
psect	text105
	file	"E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_twi_master\src\twi.c"
	line	196
	global	__size_of_twi_master_send
	__size_of_twi_master_send	equ	__end_of_twi_master_send-_twi_master_send
	
_twi_master_send:	
	opt	stack 6
; Regs used in _twi_master_send: [wreg-fsr0h+status,2+status,0]
;twi_master_send@sla_addr stored from wreg
	line	199
	movwf	(twi_master_send@sla_addr)
	
l8869:	
;twi.c: 197: unsigned char i;
;twi.c: 199: TWIF = 0x0;
	bcf	(104/8),(104)&7
	line	203
;twi.c: 203: TWIRXS = 0x0;
	bcf	(176/8),(176)&7
	line	204
;twi.c: 204: TWITXS = 0x1;
	bsf	(180/8),(180)&7
	line	206
;twi.c: 206: while(TWIF==0x0);
	
l3787:	
	btfss	(104/8),(104)&7
	goto	u31
	goto	u30
u31:
	goto	l3787
u30:
	
l3789:	
	line	208
;twi.c: 208: if (TWIRXS==0x1) {
	btfss	(176/8),(176)&7
	goto	u41
	goto	u40
u41:
	goto	l8873
u40:
	line	210
	
l8871:	
;twi.c: 210: TWIRXS = 0x0;
	bcf	(176/8),(176)&7
	line	216
	
l8873:	
;twi.c: 211: }
;twi.c: 216: TWDR = (sla_addr << 1) + 0x0;
	clrc
	rlf	(twi_master_send@sla_addr),w
	movwf	(23)	;volatile
	line	217
	
l8875:	
;twi.c: 217: TWITXD = 0x1;
	bsf	(181/8),(181)&7
	line	219
	
l8877:	
;twi.c: 219: TWIF = 0x0;
	bcf	(104/8),(104)&7
	line	221
;twi.c: 221: while(TWIF==0x0);
	
l3791:	
	btfss	(104/8),(104)&7
	goto	u51
	goto	u50
u51:
	goto	l3791
u50:
	
l3793:	
	line	223
;twi.c: 223: if (TWIRXK==0x1) {
	btfss	(182/8),(182)&7
	goto	u61
	goto	u60
u61:
	goto	l8883
u60:
	line	224
	
l8879:	
;twi.c: 224: if (TWIRACK == 0x1) {goto _not_addressed_;}
	btfss	(170/8),(170)&7
	goto	u71
	goto	u70
u71:
	goto	l3795
u70:
	goto	l3796
	
l3795:	
	line	225
;twi.c: 225: TWIRXK = 0x0;
	bcf	(182/8),(182)&7
	line	231
	
l8883:	
;twi.c: 226: }
;twi.c: 231: TWDR = send_adr;
	movf	(twi_master_send@send_adr),w
	movwf	(23)	;volatile
	line	232
	
l8885:	
;twi.c: 232: TWITXD = 0x1;
	bsf	(181/8),(181)&7
	line	234
	
l8887:	
;twi.c: 234: TWIF = 0x0;
	bcf	(104/8),(104)&7
	line	236
;twi.c: 236: while(TWIF==0x0);
	
l3797:	
	btfss	(104/8),(104)&7
	goto	u81
	goto	u80
u81:
	goto	l3797
u80:
	
l3799:	
	line	238
;twi.c: 238: if (TWIRXK==0x1) {
	btfss	(182/8),(182)&7
	goto	u91
	goto	u90
u91:
	goto	l8893
u90:
	line	239
	
l8889:	
;twi.c: 239: if (TWIRACK == 0x1) {goto _not_addressed_;}
	btfss	(170/8),(170)&7
	goto	u101
	goto	u100
u101:
	goto	l3801
u100:
	goto	l3796
	
l3801:	
	line	240
;twi.c: 240: TWIRXK = 0x0;
	bcf	(182/8),(182)&7
	line	247
	
l8893:	
;twi.c: 241: }
;twi.c: 247: for(i=length; i>0; i--)
	movf	(twi_master_send@length),w
	movwf	(twi_master_send@i)
	
l8895:	
	movf	(twi_master_send@i),f
	skipz
	goto	u111
	goto	u110
u111:
	goto	l8899
u110:
	goto	l3796
	line	250
	
l8899:	
;twi.c: 248: {
;twi.c: 250: TWDR = send_buf[length - i];
	movf	(twi_master_send@send_buf),w
	movwf	(??_twi_master_send+0)+0
	movf	(twi_master_send@i),w
	subwf	(??_twi_master_send+0)+0,f
	movf	(twi_master_send@length),w
	addwf	0+(??_twi_master_send+0)+0,w
	movwf	(??_twi_master_send+1)+0
	movf	0+(??_twi_master_send+1)+0,w
	movwf	fsr0
	bcf	status, 7	;select IRP bank0
	movf	indf,w
	movwf	(23)	;volatile
	line	252
	
l8901:	
;twi.c: 252: TWITXD = 0x1;
	bsf	(181/8),(181)&7
	line	254
	
l8903:	
;twi.c: 254: TWIF = 0x0;
	bcf	(104/8),(104)&7
	line	256
;twi.c: 256: while(TWIF==0x0);
	
l3804:	
	btfss	(104/8),(104)&7
	goto	u121
	goto	u120
u121:
	goto	l3804
u120:
	
l3806:	
	line	258
;twi.c: 258: if (TWIRXK==0x1) {
	btfss	(182/8),(182)&7
	goto	u131
	goto	u130
u131:
	goto	l8909
u130:
	line	259
	
l8905:	
;twi.c: 259: if (TWIRACK == 0x1) { goto _not_addressed_; }
	btfss	(170/8),(170)&7
	goto	u141
	goto	u140
u141:
	goto	l3808
u140:
	goto	l3796
	
l3808:	
	line	260
;twi.c: 260: TWIRXK = 0x0;
	bcf	(182/8),(182)&7
	line	247
	
l8909:	
	decf	(twi_master_send@i),f
	movf	(twi_master_send@i),f
	skipz
	goto	u151
	goto	u150
u151:
	goto	l8899
u150:
	line	267
;twi.c: 261: }
;twi.c: 262: }
;twi.c: 267: _not_addressed_:
	
l3796:	
	line	269
;twi.c: 269: TWIRXP = 0x0;
	bcf	(179/8),(179)&7
	line	270
;twi.c: 270: TWITXP = 0x1;
	bsf	(183/8),(183)&7
	line	271
;twi.c: 271: TWIF = 0x0;
	bcf	(104/8),(104)&7
	line	272
;twi.c: 272: while(TWITXP == 0x1);
	
l3809:	
	btfsc	(183/8),(183)&7
	goto	u161
	goto	u160
u161:
	goto	l3809
u160:
	line	273
	
l3812:	
	return
	opt stack 0
GLOBAL	__end_of_twi_master_send
	__end_of_twi_master_send:
;; =============== function _twi_master_send ends ============

	signat	_twi_master_send,16504
	global	_twi_init
psect	text106,local,class=CODE,delta=2
global __ptext106
__ptext106:

;; *************** function _twi_init *****************
;; Defined at:
;;		line 12 in file "E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_twi_master\src\twi.c"
;; Parameters:    Size  Location     Type
;;		None
;; Auto vars:     Size  Location     Type
;;		None
;; Return value:  Size  Location     Type
;;		None               void
;; Registers used:
;;		wreg
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
psect	text106
	file	"E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_twi_master\src\twi.c"
	line	12
	global	__size_of_twi_init
	__size_of_twi_init	equ	__end_of_twi_init-_twi_init
	
_twi_init:	
	opt	stack 6
; Regs used in _twi_init: [wreg]
	line	14
	
l8865:	
;twi.c: 14: RA0 = 0x1;
	bsf	(40/8),(40)&7
	line	15
;twi.c: 15: RA1 = 0x1;
	bsf	(41/8),(41)&7
	line	17
;twi.c: 17: TRISA0 = 0x1;
	bsf	status, 5	;RP0=1, select bank1
	bsf	(1064/8)^080h,(1064)&7
	line	18
;twi.c: 18: TRISA1 = 0x1;
	bsf	(1065/8)^080h,(1065)&7
	line	20
;twi.c: 20: WPUA0 = 0x1;
	bsf	(1192/8)^080h,(1192)&7
	line	21
;twi.c: 21: WPUA1 = 0x1;
	bsf	(1193/8)^080h,(1193)&7
	line	22
;twi.c: 22: nRABPU = 0x0;
	bcf	(1039/8)^080h,(1039)&7
	line	24
	
l8867:	
;twi.c: 24: TWCR = 0xc1;
	movlw	(0C1h)
	bcf	status, 5	;RP0=0, select bank0
	movwf	(21)	;volatile
	line	25
	
l3726:	
	return
	opt stack 0
GLOBAL	__end_of_twi_init
	__end_of_twi_init:
;; =============== function _twi_init ends ============

	signat	_twi_init,88
	global	_usart_init
psect	text107,local,class=CODE,delta=2
global __ptext107
__ptext107:

;; *************** function _usart_init *****************
;; Defined at:
;;		line 12 in file "E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_twi_master\src\usart.c"
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
psect	text107
	file	"E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_twi_master\src\usart.c"
	line	12
	global	__size_of_usart_init
	__size_of_usart_init	equ	__end_of_usart_init-_usart_init
	
_usart_init:	
	opt	stack 6
; Regs used in _usart_init: [wreg+status,2]
	line	13
	
l8855:	
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
	
l8857:	
;usart.c: 19: SPBRGH = 0x0;
	clrf	(154)^080h	;volatile
	line	20
	
l8859:	
;usart.c: 20: SPBRG = 0xc;
	movlw	(0Ch)
	movwf	(153)^080h	;volatile
	line	23
	
l8861:	
;usart.c: 23: TXSTA = 0x24;
	movlw	(024h)
	movwf	(152)^080h	;volatile
	line	24
	
l8863:	
;usart.c: 24: RCSTA = 0x80;
	movlw	(080h)
	bcf	status, 5	;RP0=0, select bank0
	movwf	(24)	;volatile
	line	25
	
l5698:	
	return
	opt stack 0
GLOBAL	__end_of_usart_init
	__end_of_usart_init:
;; =============== function _usart_init ends ============

	signat	_usart_init,88
	global	_misc_eer
psect	text108,local,class=CODE,delta=2
global __ptext108
__ptext108:

;; *************** function _misc_eer *****************
;; Defined at:
;;		line 6 in file "E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_twi_master\src\misc.c"
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
psect	text108
	file	"E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_twi_master\src\misc.c"
	line	6
	global	__size_of_misc_eer
	__size_of_misc_eer	equ	__end_of_misc_eer-_misc_eer
	
_misc_eer:	
	opt	stack 6
; Regs used in _misc_eer: [wreg]
;misc_eer@type stored from wreg
	movwf	(misc_eer@type)
	line	7
	
l8847:	
;misc.c: 7: EEADR = address & 0xff;
	movf	(misc_eer@address),w
	bsf	status, 6	;RP1=1, select bank2
	movwf	(269)^0100h	;volatile
	line	8
;misc.c: 8: EEADRH = (address >> 8) & 0xff;
	movf	(misc_eer@address+1),w
	movwf	(271)^0100h	;volatile
	line	10
	
l8849:	
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
	
l8851:	
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
	
l7593:	
	return
	opt stack 0
GLOBAL	__end_of_misc_eer
	__end_of_misc_eer:
;; =============== function _misc_eer ends ============

	signat	_misc_eer,8314
	global	_hisr
psect	text109,local,class=CODE,delta=2
global __ptext109
__ptext109:

;; *************** function _hisr *****************
;; Defined at:
;;		line 46 in file "E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_twi_master\src\main.c"
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
psect	text109
	file	"E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_twi_master\src\main.c"
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
psect	text109
	line	47
	
i1l8845:	
# 47 "E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_twi_master\src\main.c"
nop ;#
psect	text109
	line	48
	
i1l1865:	
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
psect	text110,local,class=CODE,delta=2
global __ptext110
__ptext110:
	global	btemp
	btemp set 07Eh

	DABS	1,126,2	;btemp
	global	wtemp0
	wtemp0 set btemp
	end
