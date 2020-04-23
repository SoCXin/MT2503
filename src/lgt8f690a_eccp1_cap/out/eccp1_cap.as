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
	FNCALL	_main,_eccp1_cap_init
	FNCALL	_main,_usart_send
	FNROOT	_main
	FNCALL	intlevel1,_hisr
	global	intlevel1
	FNROOT	intlevel1
	global	_recv_buf
psect	idataBANK0,class=CODE,space=0,delta=2
global __pidataBANK0
__pidataBANK0:
	file	"E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_eccp1_cap\src\main.c"
	line	27

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
	global	_ecp1_f1
	global	_PIR1
_PIR1	set	12
	global	_PR1H
_PR1H	set	29
	global	_PR1L
_PR1L	set	28
	global	_RCSTA
_RCSTA	set	24
	global	_T1CON
_T1CON	set	16
	global	_TXREG
_TXREG	set	25
	global	_CCP1IF
_CCP1IF	set	98
	global	_GIE
_GIE	set	95
	global	_PEIE
_PEIE	set	94
	global	_RB5
_RB5	set	53
	global	_RB6
_RB6	set	54
	global	_TMR1IF
_TMR1IF	set	96
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
	global	_CCP1IE
_CCP1IE	set	1122
	global	_TMR1IE
_TMR1IE	set	1120
	global	_TRISB4
_TRISB4	set	1076
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
	global	_ECP1CR0
_ECP1CR0	set	416
	global	_ECP1CR1
_ECP1CR1	set	417
	global	_ECP1CR2
_ECP1CR2	set	418
	global	_ECP1IR0
_ECP1IR0	set	423
	global	_ECP1PR0
_ECP1PR0	set	422
	global	_ECP1R1H
_ECP1R1H	set	425
	global	_ECP1R1L
_ECP1R1L	set	424
	global	_MCUCR
_MCUCR	set	415
	global	_ECP1_CE1
_ECP1_CE1	set	3328
	global	_ECP1_IF1
_ECP1_IF1	set	3384
	global	_EEPGD
_EEPGD	set	3175
	global	_EERD
_EERD	set	3168
	file	"eccp1_cap.as"
	line	#
psect cinit,class=CODE,delta=2
global start_initialization
start_initialization:

psect	bssBANK0,class=BANK0,space=1
global __pbssBANK0
__pbssBANK0:
_ecp1_f1:
       ds      1

psect	dataBANK0,class=BANK0,space=1
global __pdataBANK0
__pdataBANK0:
	file	"E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_eccp1_cap\src\main.c"
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
	global	?_eccp1_cap_init
?_eccp1_cap_init:	; 0 bytes @ 0x0
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
	global	??_eccp1_cap_init
??_eccp1_cap_init:	; 0 bytes @ 0x2
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
	global	main@ecp1_fr
main@ecp1_fr:	; 1 bytes @ 0x6
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
;;                     _eccp1_cap_init
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
;; (1) _eccp1_cap_init                                       0     0      0       0
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
;;   _eccp1_cap_init
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
;;		line 59 in file "E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_eccp1_cap\src\main.c"
;; Parameters:    Size  Location     Type
;;		None
;; Auto vars:     Size  Location     Type
;;  u16tmp          2    4[BANK0 ] unsigned int 
;;  u16tmp0         2    2[BANK0 ] unsigned int 
;;  u16tmp1         2    0[BANK0 ] unsigned int 
;;  ecp1_fr         1    6[BANK0 ] unsigned char 
;; Return value:  Size  Location     Type
;;                  2  3721[COMMON] int 
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
;;		_eccp1_cap_init
;;		_usart_send
;; This function is called by:
;;		Startup code after reset
;; This function uses a non-reentrant model
;;
psect	maintext
	file	"E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_eccp1_cap\src\main.c"
	line	59
	global	__size_of_main
	__size_of_main	equ	__end_of_main-_main
	
_main:	
	opt	stack 6
; Regs used in _main: [wreg-fsr0h+status,2+status,0+pclath+cstack]
	line	66
	
l8930:	
;main.c: 60: unsigned char ecp1_fr;
;main.c: 61: unsigned int u16tmp0;
;main.c: 62: unsigned int u16tmp1;
;main.c: 63: unsigned int u16tmp;
;main.c: 66: if((0x5a == misc_eer(1, 0x100f)))
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
	goto	l8934
u210:
	line	67
	
l8932:	
;main.c: 67: OSCTUNE = misc_eer(1, 0x1007);
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
	goto	l8936
	line	69
	
l8934:	
;main.c: 68: else
;main.c: 69: OSCTUNE = 0x92;
	movlw	(092h)
	bsf	status, 5	;RP0=1, select bank1
	bcf	status, 6	;RP1=0, select bank1
	movwf	(144)^080h	;volatile
	line	72
	
l8936:	
;main.c: 72: OSCCONbits.IRCF = 0x6;
	movf	(143)^080h,w	;volatile
	andlw	not (((1<<3)-1)<<4)
	iorlw	(06h & ((1<<3)-1))<<4
	movwf	(143)^080h	;volatile
	line	74
	
l8938:	
;main.c: 74: MCUCR = 0x55;
	movlw	(055h)
	bsf	status, 6	;RP1=1, select bank3
	movwf	(415)^0180h	;volatile
	line	75
	
l8940:	
;main.c: 75: MCUCR = 0xC0;
	movlw	(0C0h)
	movwf	(415)^0180h	;volatile
	line	77
	
l8942:	
;main.c: 77: usart_init();
	fcall	_usart_init
	line	79
	
l8944:	
;main.c: 79: ecp1_f1 = 0x0;
	clrf	(_ecp1_f1)
	line	80
	
l8946:	
;main.c: 80: ecp1_fr = 0x0;
	clrf	(main@ecp1_fr)
	line	81
	
l8948:	
;main.c: 81: u16tmp0 = 0x0;
	clrf	(main@u16tmp0)
	clrf	(main@u16tmp0+1)
	line	82
	
l8950:	
;main.c: 82: u16tmp1 = 0x0;
	clrf	(main@u16tmp1)
	clrf	(main@u16tmp1+1)
	line	84
	
l8952:	
;main.c: 84: eccp1_cap_init();
	fcall	_eccp1_cap_init
	line	87
	
l8954:	
;main.c: 87: CCP1IE = 0x1;
	bsf	status, 5	;RP0=1, select bank1
	bsf	(1122/8)^080h,(1122)&7
	line	88
	
l8956:	
;main.c: 88: PEIE = 0x1;
	bsf	(94/8),(94)&7
	line	89
	
l8958:	
;main.c: 89: GIE = 0x1;
	bsf	(95/8),(95)&7
	line	93
	
l8960:	
;main.c: 92: {
;main.c: 93: if (ecp1_f1 == 0x1)
	bcf	status, 5	;RP0=0, select bank0
	bcf	status, 6	;RP1=0, select bank0
	decf	(_ecp1_f1),w
	skipz
	goto	u221
	goto	u220
u221:
	goto	l8972
u220:
	line	95
	
l8962:	
;main.c: 94: {
;main.c: 95: ecp1_fr ++;
	incf	(main@ecp1_fr),f
	line	96
	
l8964:	
;main.c: 96: if (ecp1_fr == 0x2) CCP1IE = 0x0;
	movf	(main@ecp1_fr),w
	xorlw	02h
	skipz
	goto	u231
	goto	u230
u231:
	goto	l8968
u230:
	
l8966:	
	bsf	status, 5	;RP0=1, select bank1
	bcf	(1122/8)^080h,(1122)&7
	line	97
	
l8968:	
;main.c: 97: u16tmp1 = u16tmp0;
	bcf	status, 5	;RP0=0, select bank0
	movf	(main@u16tmp0+1),w
	movwf	(main@u16tmp1+1)
	movf	(main@u16tmp0),w
	movwf	(main@u16tmp1)
	line	98
;main.c: 98: u16tmp0 = (recv_buf[6] << 8) + recv_buf[7];
	movf	0+(_recv_buf)+06h,w
	movwf	(main@u16tmp0+1)
	movf	0+(_recv_buf)+07h,w
	movwf	(main@u16tmp0)
	line	99
	
l8970:	
;main.c: 99: ecp1_f1 = 0x0;
	clrf	(_ecp1_f1)
	line	101
	
l8972:	
;main.c: 100: }
;main.c: 101: if (ecp1_fr == 0x2)
	movf	(main@ecp1_fr),w
	xorlw	02h
	skipz
	goto	u241
	goto	u240
u241:
	goto	l8960
u240:
	line	103
	
l8974:	
;main.c: 102: {
;main.c: 103: CCP1IE = 0x0;
	bsf	status, 5	;RP0=1, select bank1
	bcf	(1122/8)^080h,(1122)&7
	line	104
;main.c: 104: ECP1_CE1 = 0x0;
	bsf	status, 6	;RP1=1, select bank3
	bcf	(3328/8)^0180h,(3328)&7
	line	105
	
l8976:	
;main.c: 105: ecp1_fr = 0x0;
	bcf	status, 5	;RP0=0, select bank0
	bcf	status, 6	;RP1=0, select bank0
	clrf	(main@ecp1_fr)
	line	106
	
l8978:	
;main.c: 106: u16tmp = u16tmp0 - u16tmp1;
	movf	(main@u16tmp0+1),w
	movwf	(main@u16tmp+1)
	movf	(main@u16tmp0),w
	movwf	(main@u16tmp)
	
l8980:	
	movf	(main@u16tmp1),w
	subwf	(main@u16tmp),f
	movf	(main@u16tmp1+1),w
	skipc
	decf	(main@u16tmp+1),f
	subwf	(main@u16tmp+1),f
	line	107
	
l8982:	
;main.c: 107: recv_buf[0] = u16tmp >> 8;
	movf	(main@u16tmp+1),w
	movwf	(_recv_buf)
	line	108
	
l8984:	
;main.c: 108: recv_buf[1] = u16tmp & 0xff;
	movf	(main@u16tmp),w
	movwf	0+(_recv_buf)+01h
	line	109
	
l8986:	
;main.c: 109: usart_send(3, recv_buf);
	movlw	(_recv_buf)&0ffh
	movwf	(?_usart_send)
	movlw	(03h)
	fcall	_usart_send
	line	110
	
l8988:	
;main.c: 110: CCP1IF = 0x0;
	bcf	status, 5	;RP0=0, select bank0
	bcf	(98/8),(98)&7
	line	111
	
l8990:	
;main.c: 111: CCP1IE = 0x1;
	bsf	status, 5	;RP0=1, select bank1
	bsf	(1122/8)^080h,(1122)&7
	line	112
	
l8992:	
;main.c: 112: ECP1_CE1 = 0x1;
	bsf	status, 6	;RP1=1, select bank3
	bsf	(3328/8)^0180h,(3328)&7
	goto	l8960
	global	start
	ljmp	start
	opt stack 0
psect	maintext
	line	115
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
;;		line 58 in file "E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_eccp1_cap\src\usart.c"
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
	file	"E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_eccp1_cap\src\usart.c"
	line	58
	global	__size_of_usart_send
	__size_of_usart_send	equ	__end_of_usart_send-_usart_send
	
_usart_send:	
	opt	stack 6
; Regs used in _usart_send: [wreg-fsr0h+status,2+status,0]
;usart_send@length stored from wreg
	line	61
	movwf	(usart_send@length)
	
l8916:	
;usart.c: 59: unsigned char i;
;usart.c: 61: TXIF = 0x0;
	bcf	(100/8),(100)&7
	line	62
;usart.c: 62: TXEN = 0x1;
	bsf	status, 5	;RP0=1, select bank1
	bsf	(1221/8)^080h,(1221)&7
	line	64
	
l8918:	
;usart.c: 64: for (i=length; i>0; i--) {
	movf	(usart_send@length),w
	movwf	(usart_send@i)
	
l8920:	
	movf	(usart_send@i),f
	skipz
	goto	u181
	goto	u180
u181:
	goto	l5599
u180:
	goto	l5602
	line	65
	
l5599:	
	bsf	status, 5	;RP0=1, select bank1
	btfss	(1217/8)^080h,(1217)&7
	goto	u191
	goto	u190
u191:
	goto	l5599
u190:
	line	66
	
l8924:	
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
	
l8926:	
	decf	(usart_send@i),f
	goto	l8920
	line	69
	
l5602:	
	bcf	status, 5	;RP0=0, select bank0
	btfss	(100/8),(100)&7
	goto	u201
	goto	u200
u201:
	goto	l5602
u200:
	
l5604:	
	line	70
;usart.c: 70: TXIF = 0x0;
	bcf	(100/8),(100)&7
	line	72
;usart.c: 72: TXEN = 0x0;
	bsf	status, 5	;RP0=1, select bank1
	bcf	(1221/8)^080h,(1221)&7
	line	73
	
l5605:	
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
;;		line 12 in file "E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_eccp1_cap\src\usart.c"
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
	file	"E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_eccp1_cap\src\usart.c"
	line	12
	global	__size_of_usart_init
	__size_of_usart_init	equ	__end_of_usart_init-_usart_init
	
_usart_init:	
	opt	stack 6
; Regs used in _usart_init: [wreg+status,2]
	line	13
	
l8906:	
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
	
l8908:	
;usart.c: 19: SPBRGH = 0x0;
	clrf	(154)^080h	;volatile
	line	20
	
l8910:	
;usart.c: 20: SPBRG = 0xc;
	movlw	(0Ch)
	movwf	(153)^080h	;volatile
	line	23
	
l8912:	
;usart.c: 23: TXSTA = 0x24;
	movlw	(024h)
	movwf	(152)^080h	;volatile
	line	24
	
l8914:	
;usart.c: 24: RCSTA = 0x80;
	movlw	(080h)
	bcf	status, 5	;RP0=0, select bank0
	movwf	(24)	;volatile
	line	25
	
l5579:	
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
;;		line 6 in file "E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_eccp1_cap\src\misc.c"
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
	file	"E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_eccp1_cap\src\misc.c"
	line	6
	global	__size_of_misc_eer
	__size_of_misc_eer	equ	__end_of_misc_eer-_misc_eer
	
_misc_eer:	
	opt	stack 6
; Regs used in _misc_eer: [wreg]
;misc_eer@type stored from wreg
	movwf	(misc_eer@type)
	line	7
	
l8898:	
;misc.c: 7: EEADR = address & 0xff;
	movf	(misc_eer@address),w
	bsf	status, 6	;RP1=1, select bank2
	movwf	(269)^0100h	;volatile
	line	8
;misc.c: 8: EEADRH = (address >> 8) & 0xff;
	movf	(misc_eer@address+1),w
	movwf	(271)^0100h	;volatile
	line	10
	
l8900:	
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
	
l8902:	
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
	
l7474:	
	return
	opt stack 0
GLOBAL	__end_of_misc_eer
	__end_of_misc_eer:
;; =============== function _misc_eer ends ============

	signat	_misc_eer,8314
	global	_eccp1_cap_init
psect	text154,local,class=CODE,delta=2
global __ptext154
__ptext154:

;; *************** function _eccp1_cap_init *****************
;; Defined at:
;;		line 14 in file "E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_eccp1_cap\src\eccp1.c"
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
	file	"E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_eccp1_cap\src\eccp1.c"
	line	14
	global	__size_of_eccp1_cap_init
	__size_of_eccp1_cap_init	equ	__end_of_eccp1_cap_init-_eccp1_cap_init
	
_eccp1_cap_init:	
	opt	stack 6
; Regs used in _eccp1_cap_init: [wreg+status,2]
	line	15
	
l8860:	
;eccp1.c: 15: TRISB4 = 0x1;
	bsf	status, 5	;RP0=1, select bank1
	bsf	(1076/8)^080h,(1076)&7
	line	18
	
l8862:	
;eccp1.c: 18: PR1H = 0xff;
	movlw	(0FFh)
	bcf	status, 5	;RP0=0, select bank0
	movwf	(29)	;volatile
	line	20
;eccp1.c: 20: PR1L = 0xff;
	movlw	(0FFh)
	movwf	(28)	;volatile
	line	22
	
l8864:	
;eccp1.c: 22: ECP1CR0 = 0x00;
	bsf	status, 5	;RP0=1, select bank3
	bsf	status, 6	;RP1=1, select bank3
	clrf	(416)^0180h	;volatile
	line	23
	
l8866:	
;eccp1.c: 23: ECP1CR1 = 0x00;
	clrf	(417)^0180h	;volatile
	line	24
	
l8868:	
;eccp1.c: 24: ECP1CR2 = 0x00;
	clrf	(418)^0180h	;volatile
	line	26
	
l8870:	
;eccp1.c: 26: ECP1PR0 = 0x02;
	movlw	(02h)
	movwf	(422)^0180h	;volatile
	line	28
	
l8872:	
;eccp1.c: 28: ECP1IR0 = 0x10;
	movlw	(010h)
	movwf	(423)^0180h	;volatile
	line	30
	
l8874:	
;eccp1.c: 30: ECP1CR1 = 0x30;
	movlw	(030h)
	movwf	(417)^0180h	;volatile
	line	31
	
l8876:	
;eccp1.c: 31: ECP1CR0 = 0x01;
	movlw	(01h)
	movwf	(416)^0180h	;volatile
	line	33
	
l8878:	
;eccp1.c: 33: CCP1IF = 0x0;
	bcf	status, 5	;RP0=0, select bank0
	bcf	status, 6	;RP1=0, select bank0
	bcf	(98/8),(98)&7
	line	34
	
l8880:	
;eccp1.c: 34: CCP1IE = 0x0;
	bsf	status, 5	;RP0=1, select bank1
	bcf	(1122/8)^080h,(1122)&7
	line	36
	
l8882:	
;eccp1.c: 36: TMR1IF = 0x0;
	bcf	status, 5	;RP0=0, select bank0
	bcf	(96/8),(96)&7
	line	37
	
l8884:	
;eccp1.c: 37: TMR1IE = 0x0;
	bsf	status, 5	;RP0=1, select bank1
	bcf	(1120/8)^080h,(1120)&7
	line	39
	
l8886:	
;eccp1.c: 39: T1CON = 0x01;
	movlw	(01h)
	bcf	status, 5	;RP0=0, select bank0
	movwf	(16)	;volatile
	line	40
	
l1849:	
	return
	opt stack 0
GLOBAL	__end_of_eccp1_cap_init
	__end_of_eccp1_cap_init:
;; =============== function _eccp1_cap_init ends ============

	signat	_eccp1_cap_init,88
	global	_hisr
psect	text155,local,class=CODE,delta=2
global __ptext155
__ptext155:

;; *************** function _hisr *****************
;; Defined at:
;;		line 44 in file "E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_eccp1_cap\src\main.c"
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
;;		Unchanged: 0/0
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
	file	"E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_eccp1_cap\src\main.c"
	line	44
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
	line	45
	
i1l8888:	
;main.c: 45: _nop();
	nop
	line	47
;main.c: 47: if(CCP1IE && CCP1IF) {
	bsf	status, 5	;RP0=1, select bank1
	bcf	status, 6	;RP1=0, select bank1
	btfss	(1122/8)^080h,(1122)&7
	goto	u13_21
	goto	u13_20
u13_21:
	goto	i1l3719
u13_20:
	
i1l8890:	
	bcf	status, 5	;RP0=0, select bank0
	btfss	(98/8),(98)&7
	goto	u14_21
	goto	u14_20
u14_21:
	goto	i1l3719
u14_20:
	line	48
	
i1l8892:	
;main.c: 48: if(ECP1_IF1)
	bsf	status, 5	;RP0=1, select bank3
	bsf	status, 6	;RP1=1, select bank3
	btfss	(3384/8)^0180h,(3384)&7
	goto	u15_21
	goto	u15_20
u15_21:
	goto	i1l3718
u15_20:
	line	50
	
i1l8894:	
;main.c: 49: {
;main.c: 50: recv_buf[6] = ECP1R1H;
	movf	(425)^0180h,w	;volatile
	bcf	status, 5	;RP0=0, select bank0
	bcf	status, 6	;RP1=0, select bank0
	movwf	0+(_recv_buf)+06h
	line	51
;main.c: 51: recv_buf[7] = ECP1R1L;
	bsf	status, 5	;RP0=1, select bank3
	bsf	status, 6	;RP1=1, select bank3
	movf	(424)^0180h,w	;volatile
	bcf	status, 5	;RP0=0, select bank0
	bcf	status, 6	;RP1=0, select bank0
	movwf	0+(_recv_buf)+07h
	line	52
	
i1l8896:	
;main.c: 52: ecp1_f1 = 0x1;
	clrf	(_ecp1_f1)
	incf	(_ecp1_f1),f
	line	53
	
i1l3718:	
	line	54
;main.c: 53: }
;main.c: 54: PIR1 = 0xfb;
	movlw	(0FBh)
	bcf	status, 5	;RP0=0, select bank0
	bcf	status, 6	;RP1=0, select bank0
	movwf	(12)	;volatile
	line	56
	
i1l3719:	
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
