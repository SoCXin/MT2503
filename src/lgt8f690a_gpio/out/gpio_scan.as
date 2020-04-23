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
	FNCALL	_main,_cfw_valid
	FNCALL	_main,_misc_eer
	FNCALL	_cfw_valid,_misc_eer
	FNROOT	_main
	global	_L_ADCON0bits
psect	text36,local,class=CODE,delta=2
global __ptext36
__ptext36:
_L_ADCON0bits	set	31
	global	_OSCTUNE
_OSCTUNE	set	144
	global	_TRISA
_TRISA	set	133
	global	_VCFG0
_VCFG0	set	1279
	global	_EEADR
_EEADR	set	269
	global	_EEADRH
_EEADRH	set	271
	global	_EEDAT
_EEDAT	set	268
	global	_EEDATH
_EEDATH	set	270
	global	_LATA
_LATA	set	261
	global	_VRTUNE
_VRTUNE	set	281
	global	_ANTM
_ANTM	set	2296
	global	_EEPGD
_EEPGD	set	3175
	global	_EERD
_EERD	set	3168
	file	"gpio_scan.as"
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
	global	?_cfw_valid
?_cfw_valid:	; 1 bytes @ 0x0
	global	?_misc_eer
?_misc_eer:	; 2 bytes @ 0x0
	global	?_main
?_main:	; 2 bytes @ 0x0
	global	misc_eer@address
misc_eer@address:	; 2 bytes @ 0x0
	ds	2
	global	??_misc_eer
??_misc_eer:	; 0 bytes @ 0x2
	global	misc_eer@type
misc_eer@type:	; 1 bytes @ 0x2
	ds	1
	global	??_cfw_valid
??_cfw_valid:	; 0 bytes @ 0x3
	global	cfw_valid@btmp
cfw_valid@btmp:	; 1 bytes @ 0x3
	ds	1
	global	??_main
??_main:	; 0 bytes @ 0x4
	ds	1
;;Data sizes: Strings 0, constant 0, data 0, bss 0, persistent 0 stack 0
;;Auto spaces:   Size  Autos    Used
;; COMMON          14      5       5
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
;;   _main->_cfw_valid
;;   _cfw_valid->_misc_eer
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
;;Main: autosize = 0, tempsize = 1, incstack = 0, save=0
;;

;;
;;Call Graph Tables:
;;
;; ---------------------------------------------------------------------------------
;; (Depth) Function   	        Calls       Base Space   Used Autos Params    Refs
;; ---------------------------------------------------------------------------------
;; (0) _main                                                 1     1      0     105
;;                                              4 COMMON     1     1      0
;;                          _cfw_valid
;;                           _misc_eer
;; ---------------------------------------------------------------------------------
;; (1) _cfw_valid                                            1     1      0      60
;;                                              3 COMMON     1     1      0
;;                           _misc_eer
;; ---------------------------------------------------------------------------------
;; (1) _misc_eer                                             3     1      2      45
;;                                              0 COMMON     3     1      2
;; ---------------------------------------------------------------------------------
;; Estimated maximum stack depth 1
;; ---------------------------------------------------------------------------------

;; Call Graph Graphs:

;; _main (ROOT)
;;   _cfw_valid
;;     _misc_eer
;;   _misc_eer
;;

;; Address spaces:

;;Name               Size   Autos  Total    Cost      Usage
;;BITCOMMON            E      0       0       0        0.0%
;;EEDATA             100      0       0       0        0.0%
;;NULL                 0      0       0       0        0.0%
;;CODE                 0      0       0       0        0.0%
;;COMMON               E      5       5       1       35.7%
;;BITSFR0              0      0       0       1        0.0%
;;SFR0                 0      0       0       1        0.0%
;;BITSFR1              0      0       0       2        0.0%
;;SFR1                 0      0       0       2        0.0%
;;STACK                0      0       2       2        0.0%
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
;;		line 10 in file "E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_gpio\gpio_scan.c"
;; Parameters:    Size  Location     Type
;;		None
;; Auto vars:     Size  Location     Type
;;		None
;; Return value:  Size  Location     Type
;;                  2  1856[COMMON] int 
;; Registers used:
;;		wreg, status,2, status,0, pclath, cstack
;; Tracked objects:
;;		On entry : 17F/0
;;		On exit  : 0/0
;;		Unchanged: 0/0
;; Data sizes:     COMMON   BANK0   BANK1   BANK2
;;      Params:         0       0       0       0
;;      Locals:         0       0       0       0
;;      Temps:          1       0       0       0
;;      Totals:         1       0       0       0
;;Total ram usage:        1 bytes
;; Hardware stack levels required when called:    2
;; This function calls:
;;		_cfw_valid
;;		_misc_eer
;; This function is called by:
;;		Startup code after reset
;; This function uses a non-reentrant model
;;
psect	maintext
	file	"E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_gpio\gpio_scan.c"
	line	10
	global	__size_of_main
	__size_of_main	equ	__end_of_main-_main
	
_main:	
	opt	stack 6
; Regs used in _main: [wreg+status,2+status,0+pclath+cstack]
	line	12
	
l4989:	
;gpio_scan.c: 12: if(cfw_valid())
	fcall	_cfw_valid
	xorlw	0
	skipnz
	goto	u41
	goto	u40
u41:
	goto	l4993
u40:
	line	13
	
l4991:	
;gpio_scan.c: 13: OSCTUNE = misc_eer(1, 0x1007);
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
	goto	l4995
	line	15
	
l4993:	
;gpio_scan.c: 14: else
;gpio_scan.c: 15: OSCTUNE = 0x92;
	movlw	(092h)
	bsf	status, 5	;RP0=1, select bank1
	bcf	status, 6	;RP1=0, select bank1
	movwf	(144)^080h	;volatile
	line	21
	
l4995:	
;gpio_scan.c: 21: VCFG0 = 1;
	bsf	(1279/8)^080h,(1279)&7
	line	22
	
l4997:	
;gpio_scan.c: 22: ANTM = 1;
	bcf	status, 5	;RP0=0, select bank2
	bsf	status, 6	;RP1=1, select bank2
	bsf	(2296/8)^0100h,(2296)&7
	line	23
	
l4999:	
;gpio_scan.c: 23: L_ADCON0bits.CHS = 0x0a;
	bcf	status, 6	;RP1=0, select bank0
	movf	(31),w	;volatile
	andlw	not (((1<<4)-1)<<2)
	iorlw	(0Ah & ((1<<4)-1))<<2
	movwf	(31)	;volatile
	line	26
	
l5001:	
;gpio_scan.c: 26: if(cfw_valid())
	fcall	_cfw_valid
	xorlw	0
	skipnz
	goto	u51
	goto	u50
u51:
	goto	l5005
u50:
	line	27
	
l5003:	
;gpio_scan.c: 27: VRTUNE = misc_eer(1, 0x100d);
	movlw	low(0100Dh)
	movwf	(?_misc_eer)
	movlw	high(0100Dh)
	movwf	((?_misc_eer))+1
	movlw	(01h)
	fcall	_misc_eer
	movf	(0+(?_misc_eer)),w
	movwf	(281)^0100h	;volatile
	goto	l1860
	line	29
	
l5005:	
;gpio_scan.c: 28: else
;gpio_scan.c: 29: VRTUNE = 0x2a;
	movlw	(02Ah)
	movwf	(281)^0100h	;volatile
	
l1860:	
	line	32
;gpio_scan.c: 32: _delay((unsigned long)((1)*(4000000/4000.0)));
	opt asmopt_off
movlw	249
movwf	(??_main+0)+0,f
u67:
	clrwdt
decfsz	(??_main+0)+0,f
	goto	u67
	nop2	;nop
	clrwdt
opt asmopt_on

	line	34
	
l5007:	
;gpio_scan.c: 34: _nop();
	nop
	line	35
	
l5009:	
;gpio_scan.c: 35: _nop();
	nop
	line	38
;gpio_scan.c: 38: TRISA = 0x80;
	movlw	(080h)
	bsf	status, 5	;RP0=1, select bank1
	bcf	status, 6	;RP1=0, select bank1
	movwf	(133)^080h	;volatile
	line	41
	
l5011:	
;gpio_scan.c: 41: LATA = ~LATA;
	bcf	status, 5	;RP0=0, select bank2
	bsf	status, 6	;RP1=1, select bank2
	comf	(261)^0100h,f	;volatile
	line	43
	
l5013:	
;gpio_scan.c: 43: _delay((unsigned long)((2)*(4000000/4000000.0)));
		opt asmopt_off
	nop2	;2 cycle nop
	opt asmopt_on

	goto	l5011
	global	start
	ljmp	start
	opt stack 0
psect	maintext
	line	46
GLOBAL	__end_of_main
	__end_of_main:
;; =============== function _main ends ============

	signat	_main,90
	global	_cfw_valid
psect	text37,local,class=CODE,delta=2
global __ptext37
__ptext37:

;; *************** function _cfw_valid *****************
;; Defined at:
;;		line 85 in file "E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_gpio\misc.c"
;; Parameters:    Size  Location     Type
;;		None
;; Auto vars:     Size  Location     Type
;;  btmp            1    3[COMMON] unsigned char 
;; Return value:  Size  Location     Type
;;                  1    wreg      unsigned char 
;; Registers used:
;;		wreg, pclath, cstack
;; Tracked objects:
;;		On entry : 60/0
;;		On exit  : 60/40
;;		Unchanged: FFE00/0
;; Data sizes:     COMMON   BANK0   BANK1   BANK2
;;      Params:         0       0       0       0
;;      Locals:         1       0       0       0
;;      Temps:          0       0       0       0
;;      Totals:         1       0       0       0
;;Total ram usage:        1 bytes
;; Hardware stack levels used:    1
;; Hardware stack levels required when called:    1
;; This function calls:
;;		_misc_eer
;; This function is called by:
;;		_main
;; This function uses a non-reentrant model
;;
psect	text37
	file	"E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_gpio\misc.c"
	line	85
	global	__size_of_cfw_valid
	__size_of_cfw_valid	equ	__end_of_cfw_valid-_cfw_valid
	
_cfw_valid:	
	opt	stack 6
; Regs used in _cfw_valid: [wreg+pclath+cstack]
	line	88
	
l4977:	
;misc.c: 86: uint8_t btmp;
;misc.c: 88: btmp = misc_eer(1, 0x100f);
	movlw	low(0100Fh)
	movwf	(?_misc_eer)
	movlw	high(0100Fh)
	movwf	((?_misc_eer))+1
	movlw	(01h)
	fcall	_misc_eer
	movf	(0+(?_misc_eer)),w
	movwf	(cfw_valid@btmp)
	line	90
	
l4979:	
;misc.c: 90: if(btmp == 0x5a)
	movf	(cfw_valid@btmp),w
	xorlw	05Ah
	skipz
	goto	u31
	goto	u30
u31:
	goto	l4985
u30:
	line	91
	
l4981:	
;misc.c: 91: return 1;
	movlw	(01h)
	goto	l3746
	line	93
	
l4985:	
;misc.c: 93: return 0;
	movlw	(0)
	line	94
	
l3746:	
	return
	opt stack 0
GLOBAL	__end_of_cfw_valid
	__end_of_cfw_valid:
;; =============== function _cfw_valid ends ============

	signat	_cfw_valid,89
	global	_misc_eer
psect	text38,local,class=CODE,delta=2
global __ptext38
__ptext38:

;; *************** function _misc_eer *****************
;; Defined at:
;;		line 6 in file "E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_gpio\misc.c"
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
;;		On entry : 20/40
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
;;		_cfw_valid
;; This function uses a non-reentrant model
;;
psect	text38
	file	"E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_gpio\misc.c"
	line	6
	global	__size_of_misc_eer
	__size_of_misc_eer	equ	__end_of_misc_eer-_misc_eer
	
_misc_eer:	
	opt	stack 7
; Regs used in _misc_eer: [wreg]
;misc_eer@type stored from wreg
	movwf	(misc_eer@type)
	line	7
	
l4969:	
;misc.c: 7: EEADR = address & 0xff;
	movf	(misc_eer@address),w
	bsf	status, 6	;RP1=1, select bank2
	movwf	(269)^0100h	;volatile
	line	8
;misc.c: 8: EEADRH = (address >> 8) & 0xff;
	movf	(misc_eer@address+1),w
	movwf	(271)^0100h	;volatile
	line	10
	
l4971:	
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
	
l4973:	
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
	
l3713:	
	return
	opt stack 0
GLOBAL	__end_of_misc_eer
	__end_of_misc_eer:
;; =============== function _misc_eer ends ============

	signat	_misc_eer,8314
psect	text39,local,class=CODE,delta=2
global __ptext39
__ptext39:
	global	btemp
	btemp set 07Eh

	DABS	1,126,2	;btemp
	global	wtemp0
	wtemp0 set btemp
	end
