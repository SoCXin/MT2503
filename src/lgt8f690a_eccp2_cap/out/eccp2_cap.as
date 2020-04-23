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
	FNCALL	_main,_eccp2_cap_init
	FNCALL	_main,_usart_send
	FNROOT	_main
	FNCALL	intlevel1,_hisr
	global	intlevel1
	FNROOT	intlevel1
	global	_recv_buf
psect	idataBANK0,class=CODE,space=0,delta=2
global __pidataBANK0
__pidataBANK0:
	file	"E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_eccp2_cap\src\main.c"
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
	global	_ecp2_f4
	global	_PIR1
_PIR1	set	12
	global	_RCSTA
_RCSTA	set	24
	global	_T2CON
_T2CON	set	18
	global	_TXREG
_TXREG	set	25
	global	_ECP2IF
_ECP2IF	set	103
	global	_GIE
_GIE	set	95
	global	_PEIE
_PEIE	set	94
	global	_RB5
_RB5	set	53
	global	_RB6
_RB6	set	54
	global	_TMR2IF
_TMR2IF	set	97
	global	_TXIF
_TXIF	set	100
	global	_OSCCONbits
_OSCCONbits	set	143
	global	_OSCTUNE
_OSCTUNE	set	144
	global	_PR2H
_PR2H	set	146
	global	_PR2L
_PR2L	set	145
	global	_SPBRG
_SPBRG	set	153
	global	_SPBRGH
_SPBRGH	set	154
	global	_TXSTA
_TXSTA	set	152
	global	_ECP2IE
_ECP2IE	set	1127
	global	_TMR2IE
_TMR2IE	set	1121
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
	global	_EEADR
_EEADR	set	269
	global	_EEADRH
_EEADRH	set	271
	global	_EEDAT
_EEDAT	set	268
	global	_EEDATH
_EEDATH	set	270
	global	_ECP2CR0
_ECP2CR0	set	432
	global	_ECP2CR1
_ECP2CR1	set	433
	global	_ECP2CR2
_ECP2CR2	set	434
	global	_ECP2CR3
_ECP2CR3	set	435
	global	_ECP2IR0
_ECP2IR0	set	431
	global	_ECP2PR0
_ECP2PR0	set	430
	global	_ECP2R4H
_ECP2R4H	set	447
	global	_ECP2R4L
_ECP2R4L	set	446
	global	_MCUCR
_MCUCR	set	415
	global	_TCCR
_TCCR	set	414
	global	_ECP2_CE4
_ECP2_CE4	set	3462
	global	_ECP2_IF4
_ECP2_IF4	set	3451
	global	_EEPGD
_EEPGD	set	3175
	global	_EERD
_EERD	set	3168
	file	"eccp2_cap.as"
	line	#
psect cinit,class=CODE,delta=2
global start_initialization
start_initialization:

psect	bssBANK0,class=BANK0,space=1
global __pbssBANK0
__pbssBANK0:
_ecp2_f4:
       ds      1

psect	dataBANK0,class=BANK0,space=1
global __pdataBANK0
__pdataBANK0:
	file	"E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_eccp2_cap\src\main.c"
_recv_buf:
       ds      36

; Clear objects allocated to BANK0
psect cinit,class=CODE,delta=2
	clrf	((__pbssBANK0)+0)&07Fh
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
	movlw low(__pdataBANK0+36)
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
	global	?_eccp2_cap_init
?_eccp2_cap_init:	; 0 bytes @ 0x0
	global	?_hisr
?_hisr:	; 0 bytes @ 0x0
	global	??_hisr
??_hisr:	; 0 bytes @ 0x0
	global	?_main
?_main:	; 2 bytes @ 0x0
	ds	2
	global	??_usart_init
??_usart_init:	; 0 bytes @ 0x2
	global	?_usart_send
?_usart_send:	; 0 bytes @ 0x2
	global	??_eccp2_cap_init
??_eccp2_cap_init:	; 0 bytes @ 0x2
	global	?_misc_eer
?_misc_eer:	; 2 bytes @ 0x2
	global	usart_send@txbuf
usart_send@txbuf:	; 1 bytes @ 0x2
	global	misc_eer@address
misc_eer@address:	; 2 bytes @ 0x2
	ds	1
	global	??_usart_send
??_usart_send:	; 0 bytes @ 0x3
	ds	1
	global	??_misc_eer
??_misc_eer:	; 0 bytes @ 0x4
	global	misc_eer@type
misc_eer@type:	; 1 bytes @ 0x4
	ds	1
	global	usart_send@length
usart_send@length:	; 1 bytes @ 0x5
	ds	1
	global	usart_send@i
usart_send@i:	; 1 bytes @ 0x6
	ds	1
	global	??_main
??_main:	; 0 bytes @ 0x7
psect	cstackBANK0,class=BANK0,space=1
global __pcstackBANK0
__pcstackBANK0:
	global	main@u16tmp1
main@u16tmp1:	; 2 bytes @ 0x0
	ds	2
	global	main@u16tmp0
main@u16tmp0:	; 2 bytes @ 0x2
	ds	2
	global	main@u16tmp
main@u16tmp:	; 2 bytes @ 0x4
	ds	2
	global	main@ecp2_fr
main@ecp2_fr:	; 1 bytes @ 0x6
	ds	1
;;Data sizes: Strings 0, constant 0, data 36, bss 1, persistent 0 stack 0
;;Auto spaces:   Size  Autos    Used
;; COMMON          14      7       7
;; BANK0           80      7      44
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
;; (0) _main                                                 7     7      0     363
;;                                              0 BANK0      7     7      0
;;                           _misc_eer
;;                         _usart_init
;;                     _eccp2_cap_init
;;                         _usart_send
;; ---------------------------------------------------------------------------------
;; (1) _usart_send                                           5     4      1     134
;;                                              2 COMMON     5     4      1
;; ---------------------------------------------------------------------------------
;; (1) _usart_init                                           0     0      0       0
;; ---------------------------------------------------------------------------------
;; (1) _misc_eer                                             3     1      2      66
;;                                              2 COMMON     3     1      2
;; ---------------------------------------------------------------------------------
;; (1) _eccp2_cap_init                                       0     0      0       0
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
;;   _eccp2_cap_init
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
;;DATA                 0      0      34      10        0.0%
;;ABS                  0      0      33       8        0.0%
;;NULL                 0      0       0       0        0.0%
;;STACK                0      0       1       2        0.0%
;;BANK0               50      7      2C       3       55.0%
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
;;		line 57 in file "E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_eccp2_cap\src\main.c"
;; Parameters:    Size  Location     Type
;;		None
;; Auto vars:     Size  Location     Type
;;  u16tmp          2    4[BANK0 ] unsigned int 
;;  u16tmp0         2    2[BANK0 ] unsigned int 
;;  u16tmp1         2    0[BANK0 ] unsigned int 
;;  ecp2_fr         1    6[BANK0 ] unsigned char 
;; Return value:  Size  Location     Type
;;                  2  3735[COMMON] int 
;; Registers used:
;;		wreg, fsr0l, fsr0h, status,2, status,0, pclath, cstack
;; Tracked objects:
;;		On entry : 17F/0
;;		On exit  : 60/60
;;		Unchanged: FFE00/0
;; Data sizes:     COMMON   BANK0   BANK1   BANK2
;;      Params:         0       0       0       0
;;      Locals:         0       7       0       0
;;      Temps:          0       0       0       0
;;      Totals:         0       7       0       0
;;Total ram usage:        7 bytes
;; Hardware stack levels required when called:    2
;; This function calls:
;;		_misc_eer
;;		_usart_init
;;		_eccp2_cap_init
;;		_usart_send
;; This function is called by:
;;		Startup code after reset
;; This function uses a non-reentrant model
;;
psect	maintext
	file	"E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_eccp2_cap\src\main.c"
	line	57
	global	__size_of_main
	__size_of_main	equ	__end_of_main-_main
	
_main:	
	opt	stack 6
; Regs used in _main: [wreg-fsr0h+status,2+status,0+pclath+cstack]
	line	64
	
l8944:	
;main.c: 58: unsigned char ecp2_fr;
;main.c: 59: unsigned int u16tmp0;
;main.c: 60: unsigned int u16tmp1;
;main.c: 61: unsigned int u16tmp;
;main.c: 64: if((0x5a == misc_eer(1, 0x100f)))
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
	goto	u211
	goto	u210
u211:
	goto	l8948
u210:
	line	65
	
l8946:	
;main.c: 65: OSCTUNE = misc_eer(1, 0x1007);
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
	goto	l8950
	line	67
	
l8948:	
;main.c: 66: else
;main.c: 67: OSCTUNE = 0x92;
	movlw	(092h)
	bsf	status, 5	;RP0=1, select bank1
	bcf	status, 6	;RP1=0, select bank1
	movwf	(144)^080h	;volatile
	line	70
	
l8950:	
;main.c: 70: OSCCONbits.IRCF = 0x6;
	movf	(143)^080h,w	;volatile
	andlw	not (((1<<3)-1)<<4)
	iorlw	(06h & ((1<<3)-1))<<4
	movwf	(143)^080h	;volatile
	line	72
	
l8952:	
;main.c: 72: MCUCR = 0x55;
	movlw	(055h)
	bsf	status, 6	;RP1=1, select bank3
	movwf	(415)^0180h	;volatile
	line	73
	
l8954:	
;main.c: 73: MCUCR = 0xC0;
	movlw	(0C0h)
	movwf	(415)^0180h	;volatile
	line	75
	
l8956:	
;main.c: 75: usart_init();
	fcall	_usart_init
	line	77
	
l8958:	
;main.c: 77: ecp2_f4 = 0x0;
	clrf	(_ecp2_f4)
	line	78
	
l8960:	
;main.c: 78: ecp2_fr = 0x0;
	clrf	(main@ecp2_fr)
	line	79
	
l8962:	
;main.c: 79: u16tmp0 = 0x0;
	clrf	(main@u16tmp0)
	clrf	(main@u16tmp0+1)
	line	80
	
l8964:	
;main.c: 80: u16tmp1 = 0x0;
	clrf	(main@u16tmp1)
	clrf	(main@u16tmp1+1)
	line	82
	
l8966:	
;main.c: 82: eccp2_cap_init();
	fcall	_eccp2_cap_init
	line	85
	
l8968:	
;main.c: 85: ECP2IE = 0x1;
	bsf	status, 5	;RP0=1, select bank1
	bsf	(1127/8)^080h,(1127)&7
	line	86
	
l8970:	
;main.c: 86: PEIE = 0x1;
	bsf	(94/8),(94)&7
	line	87
	
l8972:	
;main.c: 87: GIE = 0x1;
	bsf	(95/8),(95)&7
	line	91
	
l8974:	
;main.c: 90: {
;main.c: 91: if (ecp2_f4 == 0x1)
	bcf	status, 5	;RP0=0, select bank0
	bcf	status, 6	;RP1=0, select bank0
	decf	(_ecp2_f4),w
	skipz
	goto	u221
	goto	u220
u221:
	goto	l8986
u220:
	line	93
	
l8976:	
;main.c: 92: {
;main.c: 93: ecp2_fr ++;
	incf	(main@ecp2_fr),f
	line	94
	
l8978:	
;main.c: 94: if (ecp2_fr == 0x2) ECP2IE = 0x0;
	movf	(main@ecp2_fr),w
	xorlw	02h
	skipz
	goto	u231
	goto	u230
u231:
	goto	l8982
u230:
	
l8980:	
	bsf	status, 5	;RP0=1, select bank1
	bcf	(1127/8)^080h,(1127)&7
	line	95
	
l8982:	
;main.c: 95: u16tmp1 = u16tmp0;
	bcf	status, 5	;RP0=0, select bank0
	movf	(main@u16tmp0+1),w
	movwf	(main@u16tmp1+1)
	movf	(main@u16tmp0),w
	movwf	(main@u16tmp1)
	line	96
;main.c: 96: u16tmp0 = (recv_buf[6] << 8) + recv_buf[7];
	movf	0+(_recv_buf)+06h,w
	movwf	(main@u16tmp0+1)
	movf	0+(_recv_buf)+07h,w
	movwf	(main@u16tmp0)
	line	97
	
l8984:	
;main.c: 97: ecp2_f4 = 0x0;
	clrf	(_ecp2_f4)
	line	99
	
l8986:	
;main.c: 98: }
;main.c: 99: if (ecp2_fr == 0x2)
	movf	(main@ecp2_fr),w
	xorlw	02h
	skipz
	goto	u241
	goto	u240
u241:
	goto	l8974
u240:
	line	101
	
l8988:	
;main.c: 100: {
;main.c: 101: ECP2IE = 0x0;
	bsf	status, 5	;RP0=1, select bank1
	bcf	(1127/8)^080h,(1127)&7
	line	102
;main.c: 102: ECP2_CE4 = 0x0;
	bsf	status, 6	;RP1=1, select bank3
	bcf	(3462/8)^0180h,(3462)&7
	line	103
	
l8990:	
;main.c: 103: ecp2_fr = 0x0;
	bcf	status, 5	;RP0=0, select bank0
	bcf	status, 6	;RP1=0, select bank0
	clrf	(main@ecp2_fr)
	line	104
	
l8992:	
;main.c: 104: u16tmp = u16tmp0 - u16tmp1;
	movf	(main@u16tmp0+1),w
	movwf	(main@u16tmp+1)
	movf	(main@u16tmp0),w
	movwf	(main@u16tmp)
	
l8994:	
	movf	(main@u16tmp1),w
	subwf	(main@u16tmp),f
	movf	(main@u16tmp1+1),w
	skipc
	decf	(main@u16tmp+1),f
	subwf	(main@u16tmp+1),f
	line	105
	
l8996:	
;main.c: 105: recv_buf[0] = u16tmp >> 8;
	movf	(main@u16tmp+1),w
	movwf	(_recv_buf)
	line	106
	
l8998:	
;main.c: 106: recv_buf[1] = u16tmp & 0xff;
	movf	(main@u16tmp),w
	movwf	0+(_recv_buf)+01h
	line	107
	
l9000:	
;main.c: 107: usart_send(3, recv_buf);
	movlw	(_recv_buf)&0ffh
	movwf	(?_usart_send)
	movlw	(03h)
	fcall	_usart_send
	line	108
	
l9002:	
;main.c: 108: ECP2IF = 0x0;
	bcf	status, 5	;RP0=0, select bank0
	bcf	(103/8),(103)&7
	line	109
	
l9004:	
;main.c: 109: ECP2IE = 0x1;
	bsf	status, 5	;RP0=1, select bank1
	bsf	(1127/8)^080h,(1127)&7
	line	110
	
l9006:	
;main.c: 110: ECP2_CE4 = 0x1;
	bsf	status, 6	;RP1=1, select bank3
	bsf	(3462/8)^0180h,(3462)&7
	goto	l8974
	global	start
	ljmp	start
	opt stack 0
psect	maintext
	line	113
GLOBAL	__end_of_main
	__end_of_main:
;; =============== function _main ends ============

	signat	_main,90
	global	_usart_send
psect	text151,local,class=CODE,delta=2
global __ptext151
__ptext151:

;; *************** function _usart_send *****************
;; Defined at:
;;		line 58 in file "E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_eccp2_cap\src\usart.c"
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
psect	text151
	file	"E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_eccp2_cap\src\usart.c"
	line	58
	global	__size_of_usart_send
	__size_of_usart_send	equ	__end_of_usart_send-_usart_send
	
_usart_send:	
	opt	stack 6
; Regs used in _usart_send: [wreg-fsr0h+status,2+status,0]
;usart_send@length stored from wreg
	line	61
	movwf	(usart_send@length)
	
l8930:	
;usart.c: 59: unsigned char i;
;usart.c: 61: TXIF = 0x0;
	bcf	(100/8),(100)&7
	line	62
;usart.c: 62: TXEN = 0x1;
	bsf	status, 5	;RP0=1, select bank1
	bsf	(1221/8)^080h,(1221)&7
	line	64
	
l8932:	
;usart.c: 64: for (i=length; i>0; i--) {
	movf	(usart_send@length),w
	movwf	(usart_send@i)
	
l8934:	
	movf	(usart_send@i),f
	skipz
	goto	u181
	goto	u180
u181:
	goto	l5621
u180:
	goto	l5624
	line	65
	
l5621:	
	bsf	status, 5	;RP0=1, select bank1
	btfss	(1217/8)^080h,(1217)&7
	goto	u191
	goto	u190
u191:
	goto	l5621
u190:
	line	66
	
l8938:	
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
	
l8940:	
	decf	(usart_send@i),f
	goto	l8934
	line	69
	
l5624:	
	bcf	status, 5	;RP0=0, select bank0
	btfss	(100/8),(100)&7
	goto	u201
	goto	u200
u201:
	goto	l5624
u200:
	
l5626:	
	line	70
;usart.c: 70: TXIF = 0x0;
	bcf	(100/8),(100)&7
	line	72
;usart.c: 72: TXEN = 0x0;
	bsf	status, 5	;RP0=1, select bank1
	bcf	(1221/8)^080h,(1221)&7
	line	73
	
l5627:	
	return
	opt stack 0
GLOBAL	__end_of_usart_send
	__end_of_usart_send:
;; =============== function _usart_send ends ============

	signat	_usart_send,8312
	global	_usart_init
psect	text152,local,class=CODE,delta=2
global __ptext152
__ptext152:

;; *************** function _usart_init *****************
;; Defined at:
;;		line 12 in file "E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_eccp2_cap\src\usart.c"
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
psect	text152
	file	"E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_eccp2_cap\src\usart.c"
	line	12
	global	__size_of_usart_init
	__size_of_usart_init	equ	__end_of_usart_init-_usart_init
	
_usart_init:	
	opt	stack 6
; Regs used in _usart_init: [wreg+status,2]
	line	13
	
l8920:	
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
	
l8922:	
;usart.c: 19: SPBRGH = 0x0;
	clrf	(154)^080h	;volatile
	line	20
	
l8924:	
;usart.c: 20: SPBRG = 0xc;
	movlw	(0Ch)
	movwf	(153)^080h	;volatile
	line	23
	
l8926:	
;usart.c: 23: TXSTA = 0x24;
	movlw	(024h)
	movwf	(152)^080h	;volatile
	line	24
	
l8928:	
;usart.c: 24: RCSTA = 0x80;
	movlw	(080h)
	bcf	status, 5	;RP0=0, select bank0
	movwf	(24)	;volatile
	line	25
	
l5601:	
	return
	opt stack 0
GLOBAL	__end_of_usart_init
	__end_of_usart_init:
;; =============== function _usart_init ends ============

	signat	_usart_init,88
	global	_misc_eer
psect	text153,local,class=CODE,delta=2
global __ptext153
__ptext153:

;; *************** function _misc_eer *****************
;; Defined at:
;;		line 6 in file "E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_eccp2_cap\src\misc.c"
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
psect	text153
	file	"E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_eccp2_cap\src\misc.c"
	line	6
	global	__size_of_misc_eer
	__size_of_misc_eer	equ	__end_of_misc_eer-_misc_eer
	
_misc_eer:	
	opt	stack 6
; Regs used in _misc_eer: [wreg]
;misc_eer@type stored from wreg
	movwf	(misc_eer@type)
	line	7
	
l8912:	
;misc.c: 7: EEADR = address & 0xff;
	movf	(misc_eer@address),w
	bsf	status, 6	;RP1=1, select bank2
	movwf	(269)^0100h	;volatile
	line	8
;misc.c: 8: EEADRH = (address >> 8) & 0xff;
	movf	(misc_eer@address+1),w
	movwf	(271)^0100h	;volatile
	line	10
	
l8914:	
;misc.c: 10: EEPGD = type & 1;
	btfsc	(misc_eer@type),0
	goto	u161
	goto	u160
	
u161:
	bsf	status, 5	;RP0=1, select bank3
	bsf	status, 6	;RP1=1, select bank3
	bsf	(3175/8)^0180h,(3175)&7
	goto	u174
u160:
	bsf	status, 5	;RP0=1, select bank3
	bsf	status, 6	;RP1=1, select bank3
	bcf	(3175/8)^0180h,(3175)&7
u174:
	line	12
	
l8916:	
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
	
l7504:	
	return
	opt stack 0
GLOBAL	__end_of_misc_eer
	__end_of_misc_eer:
;; =============== function _misc_eer ends ============

	signat	_misc_eer,8314
	global	_eccp2_cap_init
psect	text154,local,class=CODE,delta=2
global __ptext154
__ptext154:

;; *************** function _eccp2_cap_init *****************
;; Defined at:
;;		line 14 in file "E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_eccp2_cap\src\eccp2.c"
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
psect	text154
	file	"E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_eccp2_cap\src\eccp2.c"
	line	14
	global	__size_of_eccp2_cap_init
	__size_of_eccp2_cap_init	equ	__end_of_eccp2_cap_init-_eccp2_cap_init
	
_eccp2_cap_init:	
	opt	stack 6
; Regs used in _eccp2_cap_init: [wreg+status,2]
	line	15
	
l8882:	
;eccp2.c: 15: TCCR = 0x80;
	movlw	(080h)
	bsf	status, 5	;RP0=1, select bank3
	bsf	status, 6	;RP1=1, select bank3
	movwf	(414)^0180h	;volatile
	line	17
	
l8884:	
;eccp2.c: 17: TRISA1 = 0x1;
	bcf	status, 6	;RP1=0, select bank1
	bsf	(1065/8)^080h,(1065)&7
	line	20
;eccp2.c: 20: PR2H = 0xff;
	movlw	(0FFh)
	movwf	(146)^080h	;volatile
	line	22
;eccp2.c: 22: PR2L = 0xff;
	movlw	(0FFh)
	movwf	(145)^080h	;volatile
	line	24
	
l8886:	
;eccp2.c: 24: ECP2CR0 = 0x00;
	bsf	status, 6	;RP1=1, select bank3
	clrf	(432)^0180h	;volatile
	line	25
	
l8888:	
;eccp2.c: 25: ECP2CR1 = 0x00;
	clrf	(433)^0180h	;volatile
	line	26
	
l8890:	
;eccp2.c: 26: ECP2CR2 = 0x00;
	clrf	(434)^0180h	;volatile
	line	27
	
l8892:	
;eccp2.c: 27: ECP2CR3 = 0x00;
	clrf	(435)^0180h	;volatile
	line	29
;eccp2.c: 29: ECP2PR0 = 0x10;
	movlw	(010h)
	movwf	(430)^0180h	;volatile
	line	31
;eccp2.c: 31: ECP2IR0 = 0x80;
	movlw	(080h)
	movwf	(431)^0180h	;volatile
	line	33
;eccp2.c: 33: ECP2CR1 = 0xf0;
	movlw	(0F0h)
	movwf	(433)^0180h	;volatile
	line	34
;eccp2.c: 34: ECP2CR0 = 0x40;
	movlw	(040h)
	movwf	(432)^0180h	;volatile
	line	36
	
l8894:	
;eccp2.c: 36: ECP2IF = 0x0;
	bcf	status, 5	;RP0=0, select bank0
	bcf	status, 6	;RP1=0, select bank0
	bcf	(103/8),(103)&7
	line	37
	
l8896:	
;eccp2.c: 37: ECP2IE = 0x0;
	bsf	status, 5	;RP0=1, select bank1
	bcf	(1127/8)^080h,(1127)&7
	line	39
	
l8898:	
;eccp2.c: 39: TMR2IF = 0x0;
	bcf	status, 5	;RP0=0, select bank0
	bcf	(97/8),(97)&7
	line	40
	
l8900:	
;eccp2.c: 40: TMR2IE = 0x0;
	bsf	status, 5	;RP0=1, select bank1
	bcf	(1121/8)^080h,(1121)&7
	line	42
;eccp2.c: 42: TCCR = 0x90;
	movlw	(090h)
	bsf	status, 6	;RP1=1, select bank3
	movwf	(414)^0180h	;volatile
	line	44
;eccp2.c: 44: T2CON = 0x84;
	movlw	(084h)
	bcf	status, 5	;RP0=0, select bank0
	bcf	status, 6	;RP1=0, select bank0
	movwf	(18)	;volatile
	line	45
	
l1857:	
	return
	opt stack 0
GLOBAL	__end_of_eccp2_cap_init
	__end_of_eccp2_cap_init:
;; =============== function _eccp2_cap_init ends ============

	signat	_eccp2_cap_init,88
	global	_hisr
psect	text155,local,class=CODE,delta=2
global __ptext155
__ptext155:

;; *************** function _hisr *****************
;; Defined at:
;;		line 48 in file "E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_eccp2_cap\src\main.c"
;; Parameters:    Size  Location     Type
;;		None
;; Auto vars:     Size  Location     Type
;;		None
;; Return value:  Size  Location     Type
;;		None               void
;; Registers used:
;;		wreg
;; Tracked objects:
;;		On entry : 0/0
;;		On exit  : 40/0
;;		Unchanged: FFF9F/0
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
psect	text155
	file	"E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_eccp2_cap\src\main.c"
	line	48
	global	__size_of_hisr
	__size_of_hisr	equ	__end_of_hisr-_hisr
	
_hisr:	
	opt	stack 6
; Regs used in _hisr: [wreg]
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
psect	text155
	line	50
	
i1l8902:	
;main.c: 50: if(ECP2IE && ECP2IF) {
	bsf	status, 5	;RP0=1, select bank1
	bcf	status, 6	;RP1=0, select bank1
	btfss	(1127/8)^080h,(1127)&7
	goto	u13_21
	goto	u13_20
u13_21:
	goto	i1l3733
u13_20:
	
i1l8904:	
	bcf	status, 5	;RP0=0, select bank0
	btfss	(103/8),(103)&7
	goto	u14_21
	goto	u14_20
u14_21:
	goto	i1l3733
u14_20:
	line	51
	
i1l8906:	
;main.c: 51: if(ECP2_IF4) {recv_buf[6] = ECP2R4H; recv_buf[7] = ECP2R4L; ecp2_f4 = 0x1;}
	bsf	status, 5	;RP0=1, select bank3
	bsf	status, 6	;RP1=1, select bank3
	btfss	(3451/8)^0180h,(3451)&7
	goto	u15_21
	goto	u15_20
u15_21:
	goto	i1l3732
u15_20:
	
i1l8908:	
	movf	(447)^0180h,w	;volatile
	bcf	status, 5	;RP0=0, select bank0
	bcf	status, 6	;RP1=0, select bank0
	movwf	0+(_recv_buf)+06h
	bsf	status, 5	;RP0=1, select bank3
	bsf	status, 6	;RP1=1, select bank3
	movf	(446)^0180h,w	;volatile
	bcf	status, 5	;RP0=0, select bank0
	bcf	status, 6	;RP1=0, select bank0
	movwf	0+(_recv_buf)+07h
	
i1l8910:	
	clrf	(_ecp2_f4)
	incf	(_ecp2_f4),f
	
i1l3732:	
	line	52
;main.c: 52: PIR1 = 0x7f;
	movlw	(07Fh)
	bcf	status, 5	;RP0=0, select bank0
	bcf	status, 6	;RP1=0, select bank0
	movwf	(12)	;volatile
	line	54
	
i1l3733:	
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
psect	text156,local,class=CODE,delta=2
global __ptext156
__ptext156:
	global	btemp
	btemp set 07Eh

	DABS	1,126,2	;btemp
	global	wtemp0
	wtemp0 set btemp
	end
