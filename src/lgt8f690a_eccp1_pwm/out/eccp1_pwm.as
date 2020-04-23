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
	FNCALL	_main,_eccp1_pwm_init
	FNROOT	_main
	FNCALL	intlevel1,_hisr
	global	intlevel1
	FNROOT	intlevel1
	global	_t0cnt
	global	_PR1H
psect	text48,local,class=CODE,delta=2
global __ptext48
__ptext48:
_PR1H	set	29
	global	_PR1L
_PR1L	set	28
	global	_T1CON
_T1CON	set	16
	global	_ECP1IF
_ECP1IF	set	98
	global	_GIE
_GIE	set	95
	global	_PEIE
_PEIE	set	94
	global	_T0IE
_T0IE	set	93
	global	_T0IF
_T0IF	set	90
	global	_TMR1IF
_TMR1IF	set	96
	global	_OPTION_REG
_OPTION_REG	set	129
	global	_OSCCONbits
_OSCCONbits	set	143
	global	_OSCTUNE
_OSCTUNE	set	144
	global	_ECP1IE
_ECP1IE	set	1122
	global	_TMR1IE
_TMR1IE	set	1120
	global	_TRISA6
_TRISA6	set	1070
	global	_TRISB3
_TRISB3	set	1075
	global	_TRISB4
_TRISB4	set	1076
	global	_EEADR
_EEADR	set	269
	global	_EEADRH
_EEADRH	set	271
	global	_EEDAT
_EEDAT	set	268
	global	_EEDATH
_EEDATH	set	270
	global	_T1ACS
_T1ACS	set	2267
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
	global	_ECP1R2H
_ECP1R2H	set	427
	global	_ECP1R2L
_ECP1R2L	set	426
	global	_ECP1R3H
_ECP1R3H	set	429
	global	_ECP1R3L
_ECP1R3L	set	428
	global	_MCUCR
_MCUCR	set	415
	global	_EEPGD
_EEPGD	set	3175
	global	_EERD
_EERD	set	3168
	file	"eccp1_pwm.as"
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
	global	?_eccp1_pwm_init
?_eccp1_pwm_init:	; 0 bytes @ 0x0
	global	?_hisr
?_hisr:	; 0 bytes @ 0x0
	global	??_hisr
??_hisr:	; 0 bytes @ 0x0
	global	?_main
?_main:	; 2 bytes @ 0x0
	ds	2
	global	??_eccp1_pwm_init
??_eccp1_pwm_init:	; 0 bytes @ 0x2
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
;;                     _eccp1_pwm_init
;; ---------------------------------------------------------------------------------
;; (1) _misc_eer                                             3     1      2      45
;;                                              2 COMMON     3     1      2
;; ---------------------------------------------------------------------------------
;; (1) _eccp1_pwm_init                                       0     0      0       0
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
;;   _eccp1_pwm_init
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
;;		line 63 in file "E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_eccp1_pwm\src\main.c"
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
;;		_eccp1_pwm_init
;; This function is called by:
;;		Startup code after reset
;; This function uses a non-reentrant model
;;
psect	maintext
	file	"E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_eccp1_pwm\src\main.c"
	line	63
	global	__size_of_main
	__size_of_main	equ	__end_of_main-_main
	
_main:	
	opt	stack 6
; Regs used in _main: [wreg+status,2+status,0+pclath+cstack]
	line	66
	
l6910:	
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
	goto	u61
	goto	u60
u61:
	goto	l6914
u60:
	line	67
	
l6912:	
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
	goto	l6916
	line	69
	
l6914:	
;main.c: 68: else
;main.c: 69: OSCTUNE = 0x92;
	movlw	(092h)
	bsf	status, 5	;RP0=1, select bank1
	bcf	status, 6	;RP1=0, select bank1
	movwf	(144)^080h	;volatile
	line	72
	
l6916:	
;main.c: 72: OSCCONbits.IRCF = 0x6;
	movf	(143)^080h,w	;volatile
	andlw	not (((1<<3)-1)<<4)
	iorlw	(06h & ((1<<3)-1))<<4
	movwf	(143)^080h	;volatile
	line	74
	
l6918:	
;main.c: 74: MCUCR = 0x55;
	movlw	(055h)
	bsf	status, 6	;RP1=1, select bank3
	movwf	(415)^0180h	;volatile
	line	75
	
l6920:	
;main.c: 75: MCUCR = 0xC6;
	movlw	(0C6h)
	movwf	(415)^0180h	;volatile
	line	77
	
l6922:	
;main.c: 77: eccp1_pwm_init();
	fcall	_eccp1_pwm_init
	line	79
	
l6924:	
;main.c: 79: OPTION_REG = 0x0f;
	movlw	(0Fh)
	bsf	status, 5	;RP0=1, select bank1
	movwf	(129)^080h	;volatile
	line	80
	
l6926:	
;main.c: 80: t0cnt = 0x0;
	clrf	(_t0cnt)
	line	81
	
l6928:	
;main.c: 81: T0IE = 0x1;
	bsf	(93/8),(93)&7
	line	84
	
l6930:	
;main.c: 84: ECP1IE = 0x0;
	bcf	(1122/8)^080h,(1122)&7
	line	85
	
l6932:	
;main.c: 85: PEIE = 0x1;
	bsf	(94/8),(94)&7
	line	86
	
l6934:	
;main.c: 86: GIE = 0x1;
	bsf	(95/8),(95)&7
	line	88
	
l3735:	
	goto	l3735
	global	start
	ljmp	start
	opt stack 0
psect	maintext
	line	89
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
;;		line 6 in file "E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_eccp1_pwm\src\misc.c"
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
	file	"E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_eccp1_pwm\src\misc.c"
	line	6
	global	__size_of_misc_eer
	__size_of_misc_eer	equ	__end_of_misc_eer-_misc_eer
	
_misc_eer:	
	opt	stack 6
; Regs used in _misc_eer: [wreg]
;misc_eer@type stored from wreg
	movwf	(misc_eer@type)
	line	7
	
l6902:	
;misc.c: 7: EEADR = address & 0xff;
	movf	(misc_eer@address),w
	bsf	status, 6	;RP1=1, select bank2
	movwf	(269)^0100h	;volatile
	line	8
;misc.c: 8: EEADRH = (address >> 8) & 0xff;
	movf	(misc_eer@address+1),w
	movwf	(271)^0100h	;volatile
	line	10
	
l6904:	
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
	
l6906:	
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
	global	_eccp1_pwm_init
psect	text50,local,class=CODE,delta=2
global __ptext50
__ptext50:

;; *************** function _eccp1_pwm_init *****************
;; Defined at:
;;		line 54 in file "E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_eccp1_pwm\src\eccp1.c"
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
	file	"E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_eccp1_pwm\src\eccp1.c"
	line	54
	global	__size_of_eccp1_pwm_init
	__size_of_eccp1_pwm_init	equ	__end_of_eccp1_pwm_init-_eccp1_pwm_init
	
_eccp1_pwm_init:	
	opt	stack 6
; Regs used in _eccp1_pwm_init: [wreg+status,2]
	line	55
	
l6846:	
;eccp1.c: 55: TRISB4 = 0x0;
	bcf	status, 6	;RP1=0, select bank1
	bcf	(1076/8)^080h,(1076)&7
	line	56
;eccp1.c: 56: TRISB3 = 0x0;
	bcf	(1075/8)^080h,(1075)&7
	line	57
;eccp1.c: 57: TRISA6 = 0x0;
	bcf	(1070/8)^080h,(1070)&7
	line	60
	
l6848:	
;eccp1.c: 60: PR1H = 0x00;
	bcf	status, 5	;RP0=0, select bank0
	clrf	(29)	;volatile
	line	62
	
l6850:	
;eccp1.c: 62: PR1L = 0xff;
	movlw	(0FFh)
	movwf	(28)	;volatile
	line	64
	
l6852:	
;eccp1.c: 64: ECP1CR0 = 0x00;
	bsf	status, 5	;RP0=1, select bank3
	bsf	status, 6	;RP1=1, select bank3
	clrf	(416)^0180h	;volatile
	line	65
	
l6854:	
;eccp1.c: 65: ECP1CR1 = 0x00;
	clrf	(417)^0180h	;volatile
	line	66
	
l6856:	
;eccp1.c: 66: ECP1CR2 = 0x00;
	clrf	(418)^0180h	;volatile
	line	68
;eccp1.c: 68: ECP1PR0 = 0x06;
	movlw	(06h)
	movwf	(422)^0180h	;volatile
	line	70
	
l6858:	
;eccp1.c: 70: ECP1R1H = 0x00;
	clrf	(425)^0180h	;volatile
	line	71
	
l6860:	
;eccp1.c: 71: ECP1R1L = 0xf0;
	movlw	(0F0h)
	movwf	(424)^0180h	;volatile
	line	73
;eccp1.c: 73: ECP1R2H = 0x00;
	clrf	(427)^0180h	;volatile
	line	74
	
l6862:	
;eccp1.c: 74: ECP1R2L = 0xc0;
	movlw	(0C0h)
	movwf	(426)^0180h	;volatile
	line	76
	
l6864:	
;eccp1.c: 76: ECP1R3H = 0x00;
	clrf	(429)^0180h	;volatile
	line	77
;eccp1.c: 77: ECP1R3L = 0xa0;
	movlw	(0A0h)
	movwf	(428)^0180h	;volatile
	line	79
;eccp1.c: 79: ECP1CR2 = 0x15;
	movlw	(015h)
	movwf	(418)^0180h	;volatile
	line	81
	
l6866:	
;eccp1.c: 81: ECP1IR0 = 0x00;
	clrf	(423)^0180h	;volatile
	line	83
	
l6868:	
;eccp1.c: 83: ECP1CR0 = 0xaa;
	movlw	(0AAh)
	movwf	(416)^0180h	;volatile
	line	85
	
l6870:	
;eccp1.c: 85: ECP1IF = 0x0;
	bcf	status, 5	;RP0=0, select bank0
	bcf	status, 6	;RP1=0, select bank0
	bcf	(98/8),(98)&7
	line	86
	
l6872:	
;eccp1.c: 86: ECP1IE = 0x0;
	bsf	status, 5	;RP0=1, select bank1
	bcf	(1122/8)^080h,(1122)&7
	line	88
	
l6874:	
;eccp1.c: 88: TMR1IF = 0x0;
	bcf	status, 5	;RP0=0, select bank0
	bcf	(96/8),(96)&7
	line	89
	
l6876:	
;eccp1.c: 89: TMR1IE = 0x0;
	bsf	status, 5	;RP0=1, select bank1
	bcf	(1120/8)^080h,(1120)&7
	line	91
	
l6878:	
;eccp1.c: 91: T1ACS = 0x1;
	bcf	status, 5	;RP0=0, select bank2
	bsf	status, 6	;RP1=1, select bank2
	bsf	(2267/8)^0100h,(2267)&7
	line	92
	
l6880:	
;eccp1.c: 92: T1CON = 0x01;
	movlw	(01h)
	bcf	status, 6	;RP1=0, select bank0
	movwf	(16)	;volatile
	line	93
	
l1860:	
	return
	opt stack 0
GLOBAL	__end_of_eccp1_pwm_init
	__end_of_eccp1_pwm_init:
;; =============== function _eccp1_pwm_init ends ============

	signat	_eccp1_pwm_init,88
	global	_hisr
psect	text51,local,class=CODE,delta=2
global __ptext51
__ptext51:

;; *************** function _hisr *****************
;; Defined at:
;;		line 48 in file "E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_eccp1_pwm\src\main.c"
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
	file	"E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_eccp1_pwm\src\main.c"
	line	48
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
	line	49
	
i1l6882:	
# 49 "E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_eccp1_pwm\src\main.c"
nop ;#
psect	text51
	line	50
;main.c: 50: if (T0IE && T0IF ) {t0cnt++; T0IF = 0x0;}
	btfss	(93/8),(93)&7
	goto	u1_21
	goto	u1_20
u1_21:
	goto	i1l6890
u1_20:
	
i1l6884:	
	btfss	(90/8),(90)&7
	goto	u2_21
	goto	u2_20
u2_21:
	goto	i1l6890
u2_20:
	
i1l6886:	
	incf	(_t0cnt),f
	
i1l6888:	
	bcf	(90/8),(90)&7
	line	51
	
i1l6890:	
;main.c: 51: if (t0cnt == 0xf0)
	movf	(_t0cnt),w
	xorlw	0F0h
	skipz
	goto	u3_21
	goto	u3_20
u3_21:
	goto	i1l3729
u3_20:
	line	53
	
i1l6892:	
;main.c: 52: {
;main.c: 53: ECP1R1H = 0x0; ECP1R1L ++;
	bsf	status, 5	;RP0=1, select bank3
	bsf	status, 6	;RP1=1, select bank3
	clrf	(425)^0180h	;volatile
	
i1l6894:	
	incf	(424)^0180h,f	;volatile
	line	54
	
i1l6896:	
;main.c: 54: ECP1R2H = 0x0; ECP1R2L ++;
	clrf	(427)^0180h	;volatile
	incf	(426)^0180h,f	;volatile
	line	55
	
i1l6898:	
;main.c: 55: ECP1R3H = 0x0; ECP1R3L ++;
	clrf	(429)^0180h	;volatile
	
i1l6900:	
	incf	(428)^0180h,f	;volatile
	line	57
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
