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
	FNCALL	_main,_eccp2_pwm_init
	FNROOT	_main
	FNCALL	intlevel1,_hisr
	global	intlevel1
	FNROOT	intlevel1
	global	_t0cnt
	global	_T2CON
psect	text48,local,class=CODE,delta=2
global __ptext48
__ptext48:
_T2CON	set	18
	global	_ECP2IF
_ECP2IF	set	103
	global	_GIE
_GIE	set	95
	global	_PEIE
_PEIE	set	94
	global	_T0IE
_T0IE	set	93
	global	_T0IF
_T0IF	set	90
	global	_TMR2IF
_TMR2IF	set	97
	global	_OPTION_REG
_OPTION_REG	set	129
	global	_OSCCONbits
_OSCCONbits	set	143
	global	_OSCTUNE
_OSCTUNE	set	144
	global	_PR2H
_PR2H	set	146
	global	_PR2L
_PR2L	set	145
	global	_TRISA
_TRISA	set	133
	global	_TRISB
_TRISB	set	134
	global	_ECP2IE
_ECP2IE	set	1127
	global	_TMR2IE
_TMR2IE	set	1121
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
	global	_ECP2CR4
_ECP2CR4	set	436
	global	_ECP2CR5
_ECP2CR5	set	437
	global	_ECP2DTN
_ECP2DTN	set	439
	global	_ECP2DTP
_ECP2DTP	set	438
	global	_ECP2IR0
_ECP2IR0	set	431
	global	_ECP2PR0
_ECP2PR0	set	430
	global	_ECP2R1H
_ECP2R1H	set	441
	global	_ECP2R1L
_ECP2R1L	set	440
	global	_ECP2R2H
_ECP2R2H	set	443
	global	_ECP2R2L
_ECP2R2L	set	442
	global	_ECP2R3H
_ECP2R3H	set	445
	global	_ECP2R3L
_ECP2R3L	set	444
	global	_ECP2R4H
_ECP2R4H	set	447
	global	_ECP2R4L
_ECP2R4L	set	446
	global	_MCUCR
_MCUCR	set	415
	global	_TCCR
_TCCR	set	414
	global	_EEPGD
_EEPGD	set	3175
	global	_EERD
_EERD	set	3168
	file	"eccp2_pwm.as"
	line	#
psect cinit,class=CODE,delta=2
global start_initialization
start_initialization:

psect	bssCOMMON,class=COMMON,space=1
global __pbssCOMMON
__pbssCOMMON:
_t0cnt:
       ds      1

; Clear objects allocated to COMMON
psect cinit,class=CODE,delta=2
	clrf	((__pbssCOMMON)+0)&07Fh
psect cinit,class=CODE,delta=2
global end_of_initialization

;End of C runtime variable initialization code

end_of_initialization:
clrf status
ljmp _main	;jump to C main() function
psect	cstackCOMMON,class=COMMON,space=1
global __pcstackCOMMON
__pcstackCOMMON:
	global	?_eccp2_pwm_init
?_eccp2_pwm_init:	; 0 bytes @ 0x0
	global	?_hisr
?_hisr:	; 0 bytes @ 0x0
	global	??_hisr
??_hisr:	; 0 bytes @ 0x0
	global	?_main
?_main:	; 2 bytes @ 0x0
	ds	2
	global	??_eccp2_pwm_init
??_eccp2_pwm_init:	; 0 bytes @ 0x2
	global	?_misc_eer
?_misc_eer:	; 2 bytes @ 0x2
	global	misc_eer@address
misc_eer@address:	; 2 bytes @ 0x2
	ds	2
	global	??_misc_eer
??_misc_eer:	; 0 bytes @ 0x4
	global	misc_eer@type
misc_eer@type:	; 1 bytes @ 0x4
	ds	1
	global	??_main
??_main:	; 0 bytes @ 0x5
;;Data sizes: Strings 0, constant 0, data 0, bss 1, persistent 0 stack 0
;;Auto spaces:   Size  Autos    Used
;; COMMON          14      5       6
;; BANK0           80      0       0
;; BANK1           80      0       0
;; BANK2           80      0       0

;;
;; Pointer list with targets:

;; ?_misc_eer	unsigned int  size(1) Largest target is 0
;;
;; sp__misc_utoa	PTR unsigned char  size(1) Largest target is 0
;;


;;
;; Critical Paths under _main in COMMON
;;
;;   _main->_misc_eer
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
;; (0) _main                                                 0     0      0      45
;;                           _misc_eer
;;                     _eccp2_pwm_init
;; ---------------------------------------------------------------------------------
;; (1) _misc_eer                                             3     1      2      45
;;                                              2 COMMON     3     1      2
;; ---------------------------------------------------------------------------------
;; (1) _eccp2_pwm_init                                       0     0      0       0
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
;;   _eccp2_pwm_init
;;
;; _hisr (ROOT)
;;

;; Address spaces:

;;Name               Size   Autos  Total    Cost      Usage
;;BITCOMMON            E      0       0       0        0.0%
;;EEDATA             100      0       0       0        0.0%
;;NULL                 0      0       0       0        0.0%
;;CODE                 0      0       0       0        0.0%
;;COMMON               E      5       6       1       42.9%
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
;;ABS                  0      0       6       8        0.0%
;;BITBANK0            50      0       0       9        0.0%
;;DATA                 0      0       7      10        0.0%

	global	_main
psect	maintext,global,class=CODE,delta=2
global __pmaintext
__pmaintext:

;; *************** function _main *****************
;; Defined at:
;;		line 62 in file "E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_eccp2_pwm\src\main.c"
;; Parameters:    Size  Location     Type
;;		None
;; Auto vars:     Size  Location     Type
;;		None
;; Return value:  Size  Location     Type
;;                  2  3731[COMMON] int 
;; Registers used:
;;		wreg, status,2, status,0, pclath, cstack
;; Tracked objects:
;;		On entry : 17F/0
;;		On exit  : 40/0
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
;;		_eccp2_pwm_init
;; This function is called by:
;;		Startup code after reset
;; This function uses a non-reentrant model
;;
psect	maintext
	file	"E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_eccp2_pwm\src\main.c"
	line	62
	global	__size_of_main
	__size_of_main	equ	__end_of_main-_main
	
_main:	
	opt	stack 6
; Regs used in _main: [wreg+status,2+status,0+pclath+cstack]
	line	64
	
l6924:	
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
	goto	u61
	goto	u60
u61:
	goto	l6928
u60:
	line	65
	
l6926:	
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
	goto	l6930
	line	67
	
l6928:	
;main.c: 66: else
;main.c: 67: OSCTUNE = 0x92;
	movlw	(092h)
	bsf	status, 5	;RP0=1, select bank1
	bcf	status, 6	;RP1=0, select bank1
	movwf	(144)^080h	;volatile
	line	70
	
l6930:	
;main.c: 70: OSCCONbits.IRCF = 0x6;
	movf	(143)^080h,w	;volatile
	andlw	not (((1<<3)-1)<<4)
	iorlw	(06h & ((1<<3)-1))<<4
	movwf	(143)^080h	;volatile
	line	72
	
l6932:	
;main.c: 72: MCUCR = 0x55;
	movlw	(055h)
	bsf	status, 6	;RP1=1, select bank3
	movwf	(415)^0180h	;volatile
	line	73
	
l6934:	
;main.c: 73: MCUCR = 0xC6;
	movlw	(0C6h)
	movwf	(415)^0180h	;volatile
	line	75
	
l6936:	
;main.c: 75: eccp2_pwm_init();
	fcall	_eccp2_pwm_init
	line	77
	
l6938:	
;main.c: 77: OPTION_REG = 0x0f;
	movlw	(0Fh)
	bsf	status, 5	;RP0=1, select bank1
	movwf	(129)^080h	;volatile
	line	78
	
l6940:	
;main.c: 78: t0cnt = 0x0;
	clrf	(_t0cnt)
	line	79
	
l6942:	
;main.c: 79: T0IE = 0x1;
	bsf	(93/8),(93)&7
	line	82
	
l6944:	
;main.c: 82: ECP2IE = 0x0;
	bcf	(1127/8)^080h,(1127)&7
	line	83
	
l6946:	
;main.c: 83: PEIE = 0x1;
	bsf	(94/8),(94)&7
	line	84
	
l6948:	
;main.c: 84: GIE = 0x1;
	bsf	(95/8),(95)&7
	line	86
	
l3735:	
	goto	l3735
	global	start
	ljmp	start
	opt stack 0
psect	maintext
	line	87
GLOBAL	__end_of_main
	__end_of_main:
;; =============== function _main ends ============

	signat	_main,90
	global	_misc_eer
psect	text49,local,class=CODE,delta=2
global __ptext49
__ptext49:

;; *************** function _misc_eer *****************
;; Defined at:
;;		line 6 in file "E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_eccp2_pwm\src\misc.c"
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
psect	text49
	file	"E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_eccp2_pwm\src\misc.c"
	line	6
	global	__size_of_misc_eer
	__size_of_misc_eer	equ	__end_of_misc_eer-_misc_eer
	
_misc_eer:	
	opt	stack 6
; Regs used in _misc_eer: [wreg]
;misc_eer@type stored from wreg
	movwf	(misc_eer@type)
	line	7
	
l6916:	
;misc.c: 7: EEADR = address & 0xff;
	movf	(misc_eer@address),w
	bsf	status, 6	;RP1=1, select bank2
	movwf	(269)^0100h	;volatile
	line	8
;misc.c: 8: EEADRH = (address >> 8) & 0xff;
	movf	(misc_eer@address+1),w
	movwf	(271)^0100h	;volatile
	line	10
	
l6918:	
;misc.c: 10: EEPGD = type & 1;
	btfsc	(misc_eer@type),0
	goto	u41
	goto	u40
	
u41:
	bsf	status, 5	;RP0=1, select bank3
	bsf	status, 6	;RP1=1, select bank3
	bsf	(3175/8)^0180h,(3175)&7
	goto	u54
u40:
	bsf	status, 5	;RP0=1, select bank3
	bsf	status, 6	;RP1=1, select bank3
	bcf	(3175/8)^0180h,(3175)&7
u54:
	line	12
	
l6920:	
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
	
l5594:	
	return
	opt stack 0
GLOBAL	__end_of_misc_eer
	__end_of_misc_eer:
;; =============== function _misc_eer ends ============

	signat	_misc_eer,8314
	global	_eccp2_pwm_init
psect	text50,local,class=CODE,delta=2
global __ptext50
__ptext50:

;; *************** function _eccp2_pwm_init *****************
;; Defined at:
;;		line 60 in file "E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_eccp2_pwm\src\eccp2.c"
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
psect	text50
	file	"E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_eccp2_pwm\src\eccp2.c"
	line	60
	global	__size_of_eccp2_pwm_init
	__size_of_eccp2_pwm_init	equ	__end_of_eccp2_pwm_init-_eccp2_pwm_init
	
_eccp2_pwm_init:	
	opt	stack 6
; Regs used in _eccp2_pwm_init: [wreg+status,2]
	line	61
	
l6846:	
;eccp2.c: 61: TCCR = 0x80;
	movlw	(080h)
	movwf	(414)^0180h	;volatile
	line	63
	
l6848:	
;eccp2.c: 63: TRISB &= 0xfe;
	bcf	status, 6	;RP1=0, select bank1
	bcf	(134)^080h+(0/8),(0)&7	;volatile
	line	64
;eccp2.c: 64: TRISA = 0x80;
	movlw	(080h)
	movwf	(133)^080h	;volatile
	line	67
	
l6850:	
;eccp2.c: 67: PR2H = 0x00;
	clrf	(146)^080h	;volatile
	line	69
	
l6852:	
;eccp2.c: 69: PR2L = 0xff;
	movlw	(0FFh)
	movwf	(145)^080h	;volatile
	line	71
;eccp2.c: 71: ECP2CR0 = 0x00;
	bsf	status, 6	;RP1=1, select bank3
	clrf	(432)^0180h	;volatile
	line	72
;eccp2.c: 72: ECP2CR1 = 0x00;
	clrf	(433)^0180h	;volatile
	line	73
;eccp2.c: 73: ECP2CR2 = 0x00;
	clrf	(434)^0180h	;volatile
	line	74
;eccp2.c: 74: ECP2CR3 = 0x00;
	clrf	(435)^0180h	;volatile
	line	75
	
l6854:	
;eccp2.c: 75: ECP2CR4 = 0x80;
	movlw	(080h)
	movwf	(436)^0180h	;volatile
	line	76
	
l6856:	
;eccp2.c: 76: ECP2CR5 = 0x00;
	clrf	(437)^0180h	;volatile
	line	78
;eccp2.c: 78: ECP2DTP = 0x3;
	movlw	(03h)
	movwf	(438)^0180h	;volatile
	line	79
;eccp2.c: 79: ECP2DTN = 0x5;
	movlw	(05h)
	movwf	(439)^0180h	;volatile
	line	81
;eccp2.c: 81: ECP2PR0 = 0x1c;
	movlw	(01Ch)
	movwf	(430)^0180h	;volatile
	line	83
	
l6858:	
;eccp2.c: 83: ECP2R1H = 0x00;
	clrf	(441)^0180h	;volatile
	line	84
	
l6860:	
;eccp2.c: 84: ECP2R1L = 0xf0;
	movlw	(0F0h)
	movwf	(440)^0180h	;volatile
	line	86
;eccp2.c: 86: ECP2R2H = 0x00;
	clrf	(443)^0180h	;volatile
	line	87
	
l6862:	
;eccp2.c: 87: ECP2R2L = 0xc0;
	movlw	(0C0h)
	movwf	(442)^0180h	;volatile
	line	89
	
l6864:	
;eccp2.c: 89: ECP2R3H = 0x00;
	clrf	(445)^0180h	;volatile
	line	90
;eccp2.c: 90: ECP2R3L = 0xa0;
	movlw	(0A0h)
	movwf	(444)^0180h	;volatile
	line	92
	
l6866:	
;eccp2.c: 92: ECP2R4H = 0x00;
	clrf	(447)^0180h	;volatile
	line	93
	
l6868:	
;eccp2.c: 93: ECP2R4L = 0x70;
	movlw	(070h)
	movwf	(446)^0180h	;volatile
	line	95
	
l6870:	
;eccp2.c: 95: ECP2CR2 = 0x55;
	movlw	(055h)
	movwf	(434)^0180h	;volatile
	line	96
	
l6872:	
;eccp2.c: 96: ECP2CR3 = 0x15;
	movlw	(015h)
	movwf	(435)^0180h	;volatile
	line	98
	
l6874:	
;eccp2.c: 98: ECP2IR0 = 0xf0;
	movlw	(0F0h)
	movwf	(431)^0180h	;volatile
	line	100
	
l6876:	
;eccp2.c: 100: ECP2CR1 = 0x01;
	movlw	(01h)
	movwf	(433)^0180h	;volatile
	line	101
	
l6878:	
;eccp2.c: 101: ECP2CR0 = 0xaa;
	movlw	(0AAh)
	movwf	(432)^0180h	;volatile
	line	103
	
l6880:	
;eccp2.c: 103: ECP2IF = 0x0;
	bcf	status, 5	;RP0=0, select bank0
	bcf	status, 6	;RP1=0, select bank0
	bcf	(103/8),(103)&7
	line	104
	
l6882:	
;eccp2.c: 104: ECP2IE = 0x0;
	bsf	status, 5	;RP0=1, select bank1
	bcf	(1127/8)^080h,(1127)&7
	line	106
	
l6884:	
;eccp2.c: 106: TMR2IF = 0x0;
	bcf	status, 5	;RP0=0, select bank0
	bcf	(97/8),(97)&7
	line	107
	
l6886:	
;eccp2.c: 107: TMR2IE = 0x0;
	bsf	status, 5	;RP0=1, select bank1
	bcf	(1121/8)^080h,(1121)&7
	line	108
	
l6888:	
;eccp2.c: 108: TCCR = 0x90;
	movlw	(090h)
	bsf	status, 6	;RP1=1, select bank3
	movwf	(414)^0180h	;volatile
	line	110
	
l6890:	
;eccp2.c: 110: T2CON = 0x84;
	movlw	(084h)
	bcf	status, 5	;RP0=0, select bank0
	bcf	status, 6	;RP1=0, select bank0
	movwf	(18)	;volatile
	line	111
	
l1860:	
	return
	opt stack 0
GLOBAL	__end_of_eccp2_pwm_init
	__end_of_eccp2_pwm_init:
;; =============== function _eccp2_pwm_init ends ============

	signat	_eccp2_pwm_init,88
	global	_hisr
psect	text51,local,class=CODE,delta=2
global __ptext51
__ptext51:

;; *************** function _hisr *****************
;; Defined at:
;;		line 47 in file "E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_eccp2_pwm\src\main.c"
;; Parameters:    Size  Location     Type
;;		None
;; Auto vars:     Size  Location     Type
;;		None
;; Return value:  Size  Location     Type
;;		None               void
;; Registers used:
;;		wreg, status,2, status,0
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
psect	text51
	file	"E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_eccp2_pwm\src\main.c"
	line	47
	global	__size_of_hisr
	__size_of_hisr	equ	__end_of_hisr-_hisr
	
_hisr:	
	opt	stack 6
; Regs used in _hisr: [wreg+status,2+status,0]
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
psect	text51
	line	48
	
i1l6892:	
# 48 "E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_eccp2_pwm\src\main.c"
nop ;#
psect	text51
	line	49
;main.c: 49: if (T0IE && T0IF ) {t0cnt++; T0IF = 0x0;}
	btfss	(93/8),(93)&7
	goto	u1_21
	goto	u1_20
u1_21:
	goto	i1l6900
u1_20:
	
i1l6894:	
	btfss	(90/8),(90)&7
	goto	u2_21
	goto	u2_20
u2_21:
	goto	i1l6900
u2_20:
	
i1l6896:	
	incf	(_t0cnt),f
	
i1l6898:	
	bcf	(90/8),(90)&7
	line	50
	
i1l6900:	
;main.c: 50: if (t0cnt == 0xf0)
	movf	(_t0cnt),w
	xorlw	0F0h
	skipz
	goto	u3_21
	goto	u3_20
u3_21:
	goto	i1l3729
u3_20:
	line	52
	
i1l6902:	
;main.c: 51: {
;main.c: 52: ECP2R1H = 0x0; ECP2R1L ++;
	bsf	status, 5	;RP0=1, select bank3
	bsf	status, 6	;RP1=1, select bank3
	clrf	(441)^0180h	;volatile
	
i1l6904:	
	incf	(440)^0180h,f	;volatile
	line	53
	
i1l6906:	
;main.c: 53: ECP2R2H = 0x0; ECP2R2L ++;
	clrf	(443)^0180h	;volatile
	incf	(442)^0180h,f	;volatile
	line	54
	
i1l6908:	
;main.c: 54: ECP2R3H = 0x0; ECP2R3L ++;
	clrf	(445)^0180h	;volatile
	
i1l6910:	
	incf	(444)^0180h,f	;volatile
	line	55
;main.c: 55: ECP2R4H = 0x0; ECP2R4L ++;
	clrf	(447)^0180h	;volatile
	
i1l6912:	
	incf	(446)^0180h,f	;volatile
	line	57
	
i1l6914:	
;main.c: 57: t0cnt = 0x0;
	clrf	(_t0cnt)
	line	59
	
i1l3729:	
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
psect	text52,local,class=CODE,delta=2
global __ptext52
__ptext52:
	global	btemp
	btemp set 07Eh

	DABS	1,126,2	;btemp
	global	wtemp0
	wtemp0 set btemp
	end
