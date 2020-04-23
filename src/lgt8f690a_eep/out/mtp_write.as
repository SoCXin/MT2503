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
	FNCALL	_main,_EEW
	FNCALL	_main,_EER
	FNCALL	_main,_usart_putc
	FNROOT	_main
	global	_INTCON
psect	text77,local,class=CODE,delta=2
global __ptext77
__ptext77:
_INTCON	set	11
	global	_RCSTA
_RCSTA	set	24
	global	_TXREG
_TXREG	set	25
	global	_GIE
_GIE	set	95
	global	_RB5
_RB5	set	53
	global	_RB6
_RB6	set	54
	global	_TXIF
_TXIF	set	100
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
	global	_TRISB5
_TRISB5	set	1077
	global	_TRISB6
_TRISB6	set	1078
	global	_TRMT
_TRMT	set	1217
	global	_EEADR
_EEADR	set	269
	global	_EEADRH
_EEADRH	set	271
	global	_EEDAT
_EEDAT	set	268
	global	_EEDATH
_EEDATH	set	270
	global	_LATA0
_LATA0	set	2088
	global	_EECON2
_EECON2	set	397
	global	_EEPGD
_EEPGD	set	3175
	global	_EERD
_EERD	set	3168
	global	_EEWR
_EEWR	set	3169
	global	_WREN
_WREN	set	3170
	file	"mtp_write.as"
	line	#
psect cinit,class=CODE,delta=2
global start_initialization
start_initialization:

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
	global	??_usart_init
??_usart_init:	; 0 bytes @ 0x0
	global	?_usart_putc
?_usart_putc:	; 0 bytes @ 0x0
	global	??_usart_putc
??_usart_putc:	; 0 bytes @ 0x0
	global	?_EEW
?_EEW:	; 0 bytes @ 0x0
	global	?_misc_eer
?_misc_eer:	; 2 bytes @ 0x0
	global	?_EER
?_EER:	; 2 bytes @ 0x0
	global	?_main
?_main:	; 2 bytes @ 0x0
	global	usart_putc@ch
usart_putc@ch:	; 1 bytes @ 0x0
	global	EEW@address
EEW@address:	; 2 bytes @ 0x0
	global	EER@address
EER@address:	; 2 bytes @ 0x0
	global	misc_eer@address
misc_eer@address:	; 2 bytes @ 0x0
	ds	2
	global	??_misc_eer
??_misc_eer:	; 0 bytes @ 0x2
	global	??_EER
??_EER:	; 0 bytes @ 0x2
	global	EEW@data
EEW@data:	; 1 bytes @ 0x2
	global	EER@type
EER@type:	; 1 bytes @ 0x2
	global	misc_eer@type
misc_eer@type:	; 1 bytes @ 0x2
	ds	1
	global	??_EEW
??_EEW:	; 0 bytes @ 0x3
	global	EEW@type
EEW@type:	; 1 bytes @ 0x3
	ds	1
	global	EEW@btmp
EEW@btmp:	; 1 bytes @ 0x4
	ds	1
	global	??_main
??_main:	; 0 bytes @ 0x5
	ds	2
	global	main@bval
main@bval:	; 1 bytes @ 0x7
	ds	1
	global	main@btmp
main@btmp:	; 1 bytes @ 0x8
	ds	1
	global	main@i
main@i:	; 1 bytes @ 0x9
	ds	1
;;Data sizes: Strings 0, constant 0, data 0, bss 0, persistent 0 stack 0
;;Auto spaces:   Size  Autos    Used
;; COMMON          14     10      10
;; BANK0           80      0       0
;; BANK1           80      0       0
;; BANK2           80      0       0

;;
;; Pointer list with targets:

;; ?_misc_eer	unsigned int  size(1) Largest target is 0
;;
;; ?_EER	unsigned int  size(1) Largest target is 0
;;
;; sp__misc_utoa	PTR unsigned char  size(1) Largest target is 0
;;


;;
;; Critical Paths under _main in COMMON
;;
;;   _main->_EEW
;;
;; Critical Paths under _main in BANK0
;;
;;   None.
;;
;; Critical Paths under _main in BANK1
;;
;;   None.
;;
;; Critical Paths under _main in BANK2
;;
;;   None.

;;
;;Main: autosize = 0, tempsize = 2, incstack = 0, save=0
;;

;;
;;Call Graph Tables:
;;
;; ---------------------------------------------------------------------------------
;; (Depth) Function   	        Calls       Base Space   Used Autos Params    Refs
;; ---------------------------------------------------------------------------------
;; (0) _main                                                 5     5      0     300
;;                                              5 COMMON     5     5      0
;;                           _misc_eer
;;                         _usart_init
;;                                _EEW
;;                                _EER
;;                         _usart_putc
;; ---------------------------------------------------------------------------------
;; (1) _usart_putc                                           1     1      0      15
;;                                              0 COMMON     1     1      0
;; ---------------------------------------------------------------------------------
;; (1) _usart_init                                           0     0      0       0
;; ---------------------------------------------------------------------------------
;; (1) _misc_eer                                             3     1      2      45
;;                                              0 COMMON     3     1      2
;; ---------------------------------------------------------------------------------
;; (1) _EER                                                  3     1      2      45
;;                                              0 COMMON     3     1      2
;; ---------------------------------------------------------------------------------
;; (1) _EEW                                                  5     2      3      75
;;                                              0 COMMON     5     2      3
;; ---------------------------------------------------------------------------------
;; Estimated maximum stack depth 1
;; ---------------------------------------------------------------------------------

;; Call Graph Graphs:

;; _main (ROOT)
;;   _misc_eer
;;   _usart_init
;;   _EEW
;;   _EER
;;   _usart_putc
;;

;; Address spaces:

;;Name               Size   Autos  Total    Cost      Usage
;;BITCOMMON            E      0       0       0        0.0%
;;EEDATA             100      0       0       0        0.0%
;;NULL                 0      0       0       0        0.0%
;;CODE                 0      0       0       0        0.0%
;;COMMON               E      A       A       1       71.4%
;;BITSFR0              0      0       0       1        0.0%
;;SFR0                 0      0       0       1        0.0%
;;BITSFR1              0      0       0       2        0.0%
;;SFR1                 0      0       0       2        0.0%
;;STACK                0      0       1       2        0.0%
;;BANK0               50      0       0       3        0.0%
;;BITBANK1            50      0       0       4        0.0%
;;BITSFR3              0      0       0       4        0.0%
;;SFR3                 0      0       0       4        0.0%
;;BANK1               50      0       0       5        0.0%
;;BITSFR2              0      0       0       5        0.0%
;;SFR2                 0      0       0       5        0.0%
;;BITBANK2            50      0       0       6        0.0%
;;BANK2               50      0       0       7        0.0%
;;ABS                  0      0       0       8        0.0%
;;BITBANK0            50      0       0       9        0.0%
;;DATA                 0      0       0      10        0.0%

	global	_main
psect	maintext,global,class=CODE,delta=2
global __pmaintext
__pmaintext:

;; *************** function _main *****************
;; Defined at:
;;		line 45 in file "E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_eep\src\mtp_write.c"
;; Parameters:    Size  Location     Type
;;		None
;; Auto vars:     Size  Location     Type
;;  i               1    9[COMMON] unsigned char 
;;  btmp            1    8[COMMON] unsigned char 
;;  bval            1    7[COMMON] unsigned char 
;; Return value:  Size  Location     Type
;;                  2  1862[COMMON] int 
;; Registers used:
;;		wreg, status,2, status,0, pclath, cstack
;; Tracked objects:
;;		On entry : 17F/0
;;		On exit  : 0/0
;;		Unchanged: 0/0
;; Data sizes:     COMMON   BANK0   BANK1   BANK2
;;      Params:         0       0       0       0
;;      Locals:         3       0       0       0
;;      Temps:          2       0       0       0
;;      Totals:         5       0       0       0
;;Total ram usage:        5 bytes
;; Hardware stack levels required when called:    1
;; This function calls:
;;		_misc_eer
;;		_usart_init
;;		_EEW
;;		_EER
;;		_usart_putc
;; This function is called by:
;;		Startup code after reset
;; This function uses a non-reentrant model
;;
psect	maintext
	file	"E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_eep\src\mtp_write.c"
	line	45
	global	__size_of_main
	__size_of_main	equ	__end_of_main-_main
	
_main:	
	opt	stack 7
; Regs used in _main: [wreg+status,2+status,0+pclath+cstack]
	line	50
	
l6896:	
;mtp_write.c: 46: uint8_t i;
;mtp_write.c: 47: uint8_t btmp, bval;
;mtp_write.c: 50: if((0x5a == misc_eer(1, 0x100f)))
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
	goto	u91
	goto	u90
u91:
	goto	l6900
u90:
	line	52
	
l6898:	
;mtp_write.c: 51: {
;mtp_write.c: 52: OSCTUNE = misc_eer(1, 0x1007);
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
	line	53
;mtp_write.c: 53: }
	goto	l6902
	line	56
	
l6900:	
;mtp_write.c: 54: else
;mtp_write.c: 55: {
;mtp_write.c: 56: OSCTUNE = 0x92;
	movlw	(092h)
	bsf	status, 5	;RP0=1, select bank1
	bcf	status, 6	;RP1=0, select bank1
	movwf	(144)^080h	;volatile
	line	61
	
l6902:	
;mtp_write.c: 57: }
;mtp_write.c: 61: usart_init();
	fcall	_usart_init
	line	68
	
l6904:	
;mtp_write.c: 68: TRISA0 = 0;
	bsf	status, 5	;RP0=1, select bank1
	bcf	(1064/8)^080h,(1064)&7
	line	69
	
l6906:	
;mtp_write.c: 69: LATA0 = 0;
	bcf	status, 5	;RP0=0, select bank2
	bsf	status, 6	;RP1=1, select bank2
	bcf	(2088/8)^0100h,(2088)&7
	line	71
	
l6908:	
;mtp_write.c: 71: btmp = 0;
	clrf	(main@btmp)
	line	74
	
l6910:	
;mtp_write.c: 74: for(i = 0; i < 128; i++) {
	clrf	(main@i)
	line	75
	
l6916:	
;mtp_write.c: 75: EEW(0, i, btmp);
	movf	(main@i),w
	movwf	(?_EEW)
	clrf	(?_EEW+1)
	movf	(main@btmp),w
	movwf	0+(?_EEW)+02h
	movlw	(0)
	fcall	_EEW
	line	76
	
l6918:	
;mtp_write.c: 76: _delay((unsigned long)((10)*(4000000/4000000.0)));
	opt asmopt_off
movlw	3
movwf	(??_main+0)+0,f
u127:
decfsz	(??_main+0)+0,f
	goto	u127
opt asmopt_on

	line	77
	
l6920:	
;mtp_write.c: 77: LATA0 = ~LATA0;
	movlw	1<<((2088)&7)
	bcf	status, 5	;RP0=0, select bank2
	bsf	status, 6	;RP1=1, select bank2
	xorwf	((2088)/8)^0100h,f
	line	74
	
l6922:	
	incf	(main@i),f
	
l6924:	
	movlw	(080h)
	subwf	(main@i),w
	skipc
	goto	u101
	goto	u100
u101:
	goto	l6916
u100:
	line	80
	
l6926:	
;mtp_write.c: 78: }
;mtp_write.c: 80: btmp++;
	incf	(main@btmp),f
	line	81
	
l6928:	
;mtp_write.c: 81: LATA0 = 1;
	bsf	(2088/8)^0100h,(2088)&7
	line	83
	
l6930:	
;mtp_write.c: 83: for(i = 0; i < 128; i++)
	clrf	(main@i)
	line	85
	
l6936:	
;mtp_write.c: 84: {
;mtp_write.c: 85: bval = EER(0, i);
	movf	(main@i),w
	movwf	(?_EER)
	clrf	(?_EER+1)
	movlw	(0)
	fcall	_EER
	movf	(0+(?_EER)),w
	movwf	(main@bval)
	line	86
	
l6938:	
;mtp_write.c: 86: usart_putc(bval);
	movf	(main@bval),w
	fcall	_usart_putc
	line	88
	
l6940:	
;mtp_write.c: 88: _delay((unsigned long)((100)*(4000000/4000.0)));
	opt asmopt_off
movlw	130
movwf	((??_main+0)+0+1),f
	movlw	221
movwf	((??_main+0)+0),f
u137:
	decfsz	((??_main+0)+0),f
	goto	u137
	decfsz	((??_main+0)+0+1),f
	goto	u137
	nop2
opt asmopt_on

	line	83
	
l6942:	
	incf	(main@i),f
	movlw	(080h)
	subwf	(main@i),w
	skipc
	goto	u111
	goto	u110
u111:
	goto	l6936
u110:
	goto	l6910
	line	106
	
l1873:	
	line	94
	goto	l1873
	global	start
	ljmp	start
	opt stack 0
psect	maintext
	line	107
GLOBAL	__end_of_main
	__end_of_main:
;; =============== function _main ends ============

	signat	_main,90
	global	_usart_putc
psect	text78,local,class=CODE,delta=2
global __ptext78
__ptext78:

;; *************** function _usart_putc *****************
;; Defined at:
;;		line 33 in file "E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_eep\src\usart.c"
;; Parameters:    Size  Location     Type
;;  ch              1    wreg     unsigned char 
;; Auto vars:     Size  Location     Type
;;  ch              1    0[COMMON] unsigned char 
;; Return value:  Size  Location     Type
;;		None               void
;; Registers used:
;;		wreg
;; Tracked objects:
;;		On entry : 60/40
;;		On exit  : 60/0
;;		Unchanged: FFF9F/0
;; Data sizes:     COMMON   BANK0   BANK1   BANK2
;;      Params:         0       0       0       0
;;      Locals:         1       0       0       0
;;      Temps:          0       0       0       0
;;      Totals:         1       0       0       0
;;Total ram usage:        1 bytes
;; Hardware stack levels used:    1
;; This function calls:
;;		Nothing
;; This function is called by:
;;		_main
;; This function uses a non-reentrant model
;;
psect	text78
	file	"E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_eep\src\usart.c"
	line	33
	global	__size_of_usart_putc
	__size_of_usart_putc	equ	__end_of_usart_putc-_usart_putc
	
_usart_putc:	
	opt	stack 7
; Regs used in _usart_putc: [wreg]
;usart_putc@ch stored from wreg
	movwf	(usart_putc@ch)
	line	34
	
l6892:	
;usart.c: 34: TXIF = 0x0;
	bcf	status, 6	;RP1=0, select bank0
	bcf	(100/8),(100)&7
	line	36
;usart.c: 36: while ( TRMT == 0x0 );
	
l3727:	
	bsf	status, 5	;RP0=1, select bank1
	btfss	(1217/8)^080h,(1217)&7
	goto	u71
	goto	u70
u71:
	goto	l3727
u70:
	line	37
	
l6894:	
;usart.c: 37: TXREG = ch;
	movf	(usart_putc@ch),w
	bcf	status, 5	;RP0=0, select bank0
	movwf	(25)	;volatile
	line	39
;usart.c: 39: while ( TXIF == 0x0 );
	
l3730:	
	btfss	(100/8),(100)&7
	goto	u81
	goto	u80
u81:
	goto	l3730
u80:
	
l3732:	
	line	40
;usart.c: 40: TXIF = 0x0;
	bcf	(100/8),(100)&7
	line	41
	
l3733:	
	return
	opt stack 0
GLOBAL	__end_of_usart_putc
	__end_of_usart_putc:
;; =============== function _usart_putc ends ============

	signat	_usart_putc,4216
	global	_usart_init
psect	text79,local,class=CODE,delta=2
global __ptext79
__ptext79:

;; *************** function _usart_init *****************
;; Defined at:
;;		line 15 in file "E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_eep\src\usart.c"
;; Parameters:    Size  Location     Type
;;		None
;; Auto vars:     Size  Location     Type
;;		None
;; Return value:  Size  Location     Type
;;		None               void
;; Registers used:
;;		wreg, status,2
;; Tracked objects:
;;		On entry : 60/20
;;		On exit  : 60/0
;;		Unchanged: FFF9F/0
;; Data sizes:     COMMON   BANK0   BANK1   BANK2
;;      Params:         0       0       0       0
;;      Locals:         0       0       0       0
;;      Temps:          0       0       0       0
;;      Totals:         0       0       0       0
;;Total ram usage:        0 bytes
;; Hardware stack levels used:    1
;; This function calls:
;;		Nothing
;; This function is called by:
;;		_main
;; This function uses a non-reentrant model
;;
psect	text79
	file	"E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_eep\src\usart.c"
	line	15
	global	__size_of_usart_init
	__size_of_usart_init	equ	__end_of_usart_init-_usart_init
	
_usart_init:	
	opt	stack 7
; Regs used in _usart_init: [wreg+status,2]
	line	16
	
l6882:	
;usart.c: 16: RB5 = 0x1;
	bcf	status, 5	;RP0=0, select bank0
	bsf	(53/8),(53)&7
	line	17
;usart.c: 17: RB6 = 0x1;
	bsf	(54/8),(54)&7
	line	19
;usart.c: 19: TRISB5 = 0x0;
	bsf	status, 5	;RP0=1, select bank1
	bcf	(1077/8)^080h,(1077)&7
	line	20
;usart.c: 20: TRISB6 = 0x1;
	bsf	(1078/8)^080h,(1078)&7
	line	22
	
l6884:	
;usart.c: 22: SPBRGH = 0x0;
	clrf	(154)^080h	;volatile
	line	23
	
l6886:	
;usart.c: 23: SPBRG = 0xc;
	movlw	(0Ch)
	movwf	(153)^080h	;volatile
	line	26
	
l6888:	
;usart.c: 26: TXSTA = 0x26;
	movlw	(026h)
	movwf	(152)^080h	;volatile
	line	27
	
l6890:	
;usart.c: 27: RCSTA = 0x90;
	movlw	(090h)
	bcf	status, 5	;RP0=0, select bank0
	movwf	(24)	;volatile
	line	30
	
l3724:	
	return
	opt stack 0
GLOBAL	__end_of_usart_init
	__end_of_usart_init:
;; =============== function _usart_init ends ============

	signat	_usart_init,88
	global	_misc_eer
psect	text80,local,class=CODE,delta=2
global __ptext80
__ptext80:

;; *************** function _misc_eer *****************
;; Defined at:
;;		line 6 in file "E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_eep\src\misc.c"
;; Parameters:    Size  Location     Type
;;  type            1    wreg     unsigned char 
;;  address         2    0[COMMON] unsigned int 
;; Auto vars:     Size  Location     Type
;;  type            1    2[COMMON] unsigned char 
;; Return value:  Size  Location     Type
;;                  2    0[COMMON] unsigned int 
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
;; This function calls:
;;		Nothing
;; This function is called by:
;;		_main
;; This function uses a non-reentrant model
;;
psect	text80
	file	"E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_eep\src\misc.c"
	line	6
	global	__size_of_misc_eer
	__size_of_misc_eer	equ	__end_of_misc_eer-_misc_eer
	
_misc_eer:	
	opt	stack 7
; Regs used in _misc_eer: [wreg]
;misc_eer@type stored from wreg
	movwf	(misc_eer@type)
	line	7
	
l6874:	
;misc.c: 7: EEADR = address & 0xff;
	movf	(misc_eer@address),w
	bsf	status, 6	;RP1=1, select bank2
	movwf	(269)^0100h	;volatile
	line	8
;misc.c: 8: EEADRH = (address >> 8) & 0xff;
	movf	(misc_eer@address+1),w
	movwf	(271)^0100h	;volatile
	line	10
	
l6876:	
;misc.c: 10: EEPGD = type & 1;
	btfsc	(misc_eer@type),0
	goto	u51
	goto	u50
	
u51:
	bsf	status, 5	;RP0=1, select bank3
	bsf	status, 6	;RP1=1, select bank3
	bsf	(3175/8)^0180h,(3175)&7
	goto	u64
u50:
	bsf	status, 5	;RP0=1, select bank3
	bsf	status, 6	;RP1=1, select bank3
	bcf	(3175/8)^0180h,(3175)&7
u64:
	line	12
	
l6878:	
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
	
l5602:	
	return
	opt stack 0
GLOBAL	__end_of_misc_eer
	__end_of_misc_eer:
;; =============== function _misc_eer ends ============

	signat	_misc_eer,8314
	global	_EER
psect	text81,local,class=CODE,delta=2
global __ptext81
__ptext81:

;; *************** function _EER *****************
;; Defined at:
;;		line 34 in file "E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_eep\src\mtp_write.c"
;; Parameters:    Size  Location     Type
;;  type            1    wreg     unsigned char 
;;  address         2    0[COMMON] unsigned int 
;; Auto vars:     Size  Location     Type
;;  type            1    2[COMMON] unsigned char 
;; Return value:  Size  Location     Type
;;                  2    0[COMMON] unsigned int 
;; Registers used:
;;		wreg
;; Tracked objects:
;;		On entry : 0/40
;;		On exit  : 60/40
;;		Unchanged: FFE00/0
;; Data sizes:     COMMON   BANK0   BANK1   BANK2
;;      Params:         2       0       0       0
;;      Locals:         1       0       0       0
;;      Temps:          0       0       0       0
;;      Totals:         3       0       0       0
;;Total ram usage:        3 bytes
;; Hardware stack levels used:    1
;; This function calls:
;;		Nothing
;; This function is called by:
;;		_main
;; This function uses a non-reentrant model
;;
psect	text81
	file	"E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_eep\src\mtp_write.c"
	line	34
	global	__size_of_EER
	__size_of_EER	equ	__end_of_EER-_EER
	
_EER:	
	opt	stack 7
; Regs used in _EER: [wreg]
;EER@type stored from wreg
	movwf	(EER@type)
	line	35
	
l6866:	
;mtp_write.c: 35: EEADR = address & 0xff;
	movf	(EER@address),w
	bcf	status, 5	;RP0=0, select bank2
	bsf	status, 6	;RP1=1, select bank2
	movwf	(269)^0100h	;volatile
	line	36
;mtp_write.c: 36: EEADRH = (address >> 8) & 0xff;
	movf	(EER@address+1),w
	movwf	(271)^0100h	;volatile
	line	38
	
l6868:	
;mtp_write.c: 38: EEPGD = type;
	btfsc	(EER@type),0
	goto	u31
	goto	u30
	
u31:
	bsf	status, 5	;RP0=1, select bank3
	bsf	status, 6	;RP1=1, select bank3
	bsf	(3175/8)^0180h,(3175)&7
	goto	u44
u30:
	bsf	status, 5	;RP0=1, select bank3
	bsf	status, 6	;RP1=1, select bank3
	bcf	(3175/8)^0180h,(3175)&7
u44:
	line	39
	
l6870:	
;mtp_write.c: 39: EERD = 1;
	bsf	(3168/8)^0180h,(3168)&7
	line	41
;mtp_write.c: 41: return (EEDATH << 8) | EEDAT;
	bcf	status, 5	;RP0=0, select bank2
	movf	(270)^0100h,w	;volatile
	movwf	(?_EER+1)
	clrf	(?_EER)
	movf	(268)^0100h,w	;volatile
	iorwf	(?_EER),f
	line	42
	
l1860:	
	return
	opt stack 0
GLOBAL	__end_of_EER
	__end_of_EER:
;; =============== function _EER ends ============

	signat	_EER,8314
	global	_EEW
psect	text82,local,class=CODE,delta=2
global __ptext82
__ptext82:

;; *************** function _EEW *****************
;; Defined at:
;;		line 9 in file "E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_eep\src\mtp_write.c"
;; Parameters:    Size  Location     Type
;;  type            1    wreg     unsigned char 
;;  address         2    0[COMMON] unsigned int 
;;  data            1    2[COMMON] unsigned char 
;; Auto vars:     Size  Location     Type
;;  type            1    3[COMMON] unsigned char 
;;  btmp            1    4[COMMON] volatile unsigned char 
;; Return value:  Size  Location     Type
;;		None               void
;; Registers used:
;;		wreg
;; Tracked objects:
;;		On entry : 0/40
;;		On exit  : 60/60
;;		Unchanged: FFE00/0
;; Data sizes:     COMMON   BANK0   BANK1   BANK2
;;      Params:         3       0       0       0
;;      Locals:         2       0       0       0
;;      Temps:          0       0       0       0
;;      Totals:         5       0       0       0
;;Total ram usage:        5 bytes
;; Hardware stack levels used:    1
;; This function calls:
;;		Nothing
;; This function is called by:
;;		_main
;; This function uses a non-reentrant model
;;
psect	text82
	file	"E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_eep\src\mtp_write.c"
	line	9
	global	__size_of_EEW
	__size_of_EEW	equ	__end_of_EEW-_EEW
	
_EEW:	
	opt	stack 7
; Regs used in _EEW: [wreg]
;EEW@type stored from wreg
	line	12
	movwf	(EEW@type)
	
l6854:	
;mtp_write.c: 10: volatile uint8_t btmp;
;mtp_write.c: 12: EEPGD = type;
	btfsc	(EEW@type),0
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
	line	13
	
l6856:	
;mtp_write.c: 13: EEADR = address & 0xff;
	movf	(EEW@address),w
	bcf	status, 5	;RP0=0, select bank2
	movwf	(269)^0100h	;volatile
	line	14
;mtp_write.c: 14: EEADRH = (address >> 8) & 0xff;
	movf	(EEW@address+1),w
	movwf	(271)^0100h	;volatile
	line	15
;mtp_write.c: 15: EEDAT = data;
	movf	(EEW@data),w
	movwf	(268)^0100h	;volatile
	line	18
	
l6858:	
;mtp_write.c: 18: WREN = 1;
	bsf	status, 5	;RP0=1, select bank3
	bsf	(3170/8)^0180h,(3170)&7
	line	20
;mtp_write.c: 20: btmp = INTCON;
	movf	(11),w	;volatile
	movwf	(EEW@btmp)	;volatile
	line	21
	
l6860:	
;mtp_write.c: 21: GIE = 0;
	bcf	(95/8),(95)&7
	line	24
;mtp_write.c: 24: EECON2 = 0x55;
	movlw	(055h)
	movwf	(397)^0180h	;volatile
	line	25
;mtp_write.c: 25: EECON2 = 0xaa;
	movlw	(0AAh)
	movwf	(397)^0180h	;volatile
	line	26
	
l6862:	
;mtp_write.c: 26: EEWR = 1;
	bsf	(3169/8)^0180h,(3169)&7
	line	28
	
l6864:	
;mtp_write.c: 28: WREN = 0;
	bcf	(3170/8)^0180h,(3170)&7
	line	29
;mtp_write.c: 29: INTCON = btmp;
	movf	(EEW@btmp),w	;volatile
	movwf	(11)	;volatile
	line	30
	
l1857:	
	return
	opt stack 0
GLOBAL	__end_of_EEW
	__end_of_EEW:
;; =============== function _EEW ends ============

	signat	_EEW,12408
psect	text83,local,class=CODE,delta=2
global __ptext83
__ptext83:
	global	btemp
	btemp set 07Eh

	DABS	1,126,2	;btemp
	global	wtemp0
	wtemp0 set btemp
	end
