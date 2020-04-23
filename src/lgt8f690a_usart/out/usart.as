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
	FNCALL	_main,_usart_init
	FNCALL	_main,_usart_puts
	FNCALL	_main,_usart_getc
	FNCALL	_main,_usart_putc
	FNCALL	_cfw_valid,_misc_eer
	FNROOT	_main
	FNCALL	intlevel1,_hisr
	global	intlevel1
	FNROOT	intlevel1
	global	_RCREG
psect	text85,local,class=CODE,delta=2
global __ptext85
__ptext85:
_RCREG	set	26
	global	_RCSTA
_RCSTA	set	24
	global	_TXREG
_TXREG	set	25
	global	_CREN
_CREN	set	196
	global	_RB5
_RB5	set	53
	global	_RB6
_RB6	set	54
	global	_RCIF
_RCIF	set	101
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
	global	_EEPGD
_EEPGD	set	3175
	global	_EERD
_EERD	set	3168
psect	strings,class=STRING,delta=2
global __pstrings
__pstrings:
;	global	stringdir,stringtab,__stringbase
stringtab:
;	String table - string pointers are 1 byte each
stringcode:stringdir:
movlw high(stringdir)
movwf pclath
movf fsr,w
incf fsr
	addwf pc
__stringbase:
	retlw	0
psect	strings
	
STR_1:	
	retlw	104	;'h'
	retlw	101	;'e'
	retlw	108	;'l'
	retlw	108	;'l'
	retlw	111	;'o'
	retlw	32	;' '
	retlw	102	;'f'
	retlw	114	;'r'
	retlw	111	;'o'
	retlw	109	;'m'
	retlw	32	;' '
	retlw	108	;'l'
	retlw	103	;'g'
	retlw	116	;'t'
	retlw	56	;'8'
	retlw	102	;'f'
	retlw	54	;'6'
	retlw	57	;'9'
	retlw	48	;'0'
	retlw	33	;'!'
	retlw	13
	retlw	10
	retlw	0
psect	strings
	file	"usart.as"
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
	global	?_usart_puts
?_usart_puts:	; 0 bytes @ 0x0
	global	?_usart_putc
?_usart_putc:	; 0 bytes @ 0x0
	global	?_hisr
?_hisr:	; 0 bytes @ 0x0
	global	??_hisr
??_hisr:	; 0 bytes @ 0x0
	global	?_cfw_valid
?_cfw_valid:	; 1 bytes @ 0x0
	global	?_usart_getc
?_usart_getc:	; 1 bytes @ 0x0
	global	?_main
?_main:	; 2 bytes @ 0x0
	ds	2
	global	??_usart_init
??_usart_init:	; 0 bytes @ 0x2
	global	??_usart_puts
??_usart_puts:	; 0 bytes @ 0x2
	global	??_usart_putc
??_usart_putc:	; 0 bytes @ 0x2
	global	??_usart_getc
??_usart_getc:	; 0 bytes @ 0x2
	global	?_misc_eer
?_misc_eer:	; 2 bytes @ 0x2
	global	usart_putc@ch
usart_putc@ch:	; 1 bytes @ 0x2
	global	usart_puts@txbuf
usart_puts@txbuf:	; 1 bytes @ 0x2
	global	misc_eer@address
misc_eer@address:	; 2 bytes @ 0x2
	ds	2
	global	??_misc_eer
??_misc_eer:	; 0 bytes @ 0x4
	global	misc_eer@type
misc_eer@type:	; 1 bytes @ 0x4
	ds	1
	global	??_cfw_valid
??_cfw_valid:	; 0 bytes @ 0x5
	global	cfw_valid@btmp
cfw_valid@btmp:	; 1 bytes @ 0x5
	ds	1
	global	??_main
??_main:	; 0 bytes @ 0x6
;;Data sizes: Strings 23, constant 0, data 0, bss 0, persistent 0 stack 0
;;Auto spaces:   Size  Autos    Used
;; COMMON          14      6       6
;; BANK0           80      0       0
;; BANK1           80      0       0
;; BANK2           80      0       0

;;
;; Pointer list with targets:

;; ?_misc_eer	unsigned int  size(1) Largest target is 0
;;
;; sp__misc_utoa	PTR unsigned char  size(1) Largest target is 0
;;
;; usart_puts@txbuf	PTR unsigned char  size(1) Largest target is 23
;;		 -> STR_1(CODE[23]), 
;;


;;
;; Critical Paths under _main in COMMON
;;
;;   _main->_cfw_valid
;;   _cfw_valid->_misc_eer
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
;; (0) _main                                                 0     0      0     150
;;                          _cfw_valid
;;                           _misc_eer
;;                         _usart_init
;;                         _usart_puts
;;                         _usart_getc
;;                         _usart_putc
;; ---------------------------------------------------------------------------------
;; (1) _cfw_valid                                            1     1      0      60
;;                                              5 COMMON     1     1      0
;;                           _misc_eer
;; ---------------------------------------------------------------------------------
;; (1) _usart_putc                                           1     1      0      15
;;                                              2 COMMON     1     1      0
;; ---------------------------------------------------------------------------------
;; (1) _usart_getc                                           0     0      0       0
;; ---------------------------------------------------------------------------------
;; (2) _usart_puts                                           1     1      0      30
;;                                              2 COMMON     1     1      0
;; ---------------------------------------------------------------------------------
;; (1) _usart_init                                           0     0      0       0
;; ---------------------------------------------------------------------------------
;; (1) _misc_eer                                             3     1      2      45
;;                                              2 COMMON     3     1      2
;; ---------------------------------------------------------------------------------
;; Estimated maximum stack depth 2
;; ---------------------------------------------------------------------------------
;; (Depth) Function   	        Calls       Base Space   Used Autos Params    Refs
;; ---------------------------------------------------------------------------------
;; (3) _hisr                                                 2     2      0       0
;;                                              0 COMMON     2     2      0
;; ---------------------------------------------------------------------------------
;; Estimated maximum stack depth 3
;; ---------------------------------------------------------------------------------

;; Call Graph Graphs:

;; _main (ROOT)
;;   _cfw_valid
;;     _misc_eer
;;   _misc_eer
;;   _usart_init
;;   _usart_puts
;;   _usart_getc
;;   _usart_putc
;;
;; _hisr (ROOT)
;;

;; Address spaces:

;;Name               Size   Autos  Total    Cost      Usage
;;BITCOMMON            E      0       0       0        0.0%
;;EEDATA             100      0       0       0        0.0%
;;NULL                 0      0       0       0        0.0%
;;CODE                 0      0       0       0        0.0%
;;COMMON               E      6       6       1       42.9%
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
;;		line 20 in file "E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_usart\src\main.c"
;; Parameters:    Size  Location     Type
;;		None
;; Auto vars:     Size  Location     Type
;;		None
;; Return value:  Size  Location     Type
;;                  2  1863[COMMON] int 
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
;; Hardware stack levels required when called:    3
;; This function calls:
;;		_cfw_valid
;;		_misc_eer
;;		_usart_init
;;		_usart_puts
;;		_usart_getc
;;		_usart_putc
;; This function is called by:
;;		Startup code after reset
;; This function uses a non-reentrant model
;;
psect	maintext
	file	"E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_usart\src\main.c"
	line	20
	global	__size_of_main
	__size_of_main	equ	__end_of_main-_main
	
_main:	
	opt	stack 5
; Regs used in _main: [wreg-fsr0h+status,2+status,0+pclath+cstack]
	line	23
	
l6900:	
;main.c: 23: if(cfw_valid())
	fcall	_cfw_valid
	xorlw	0
	skipnz
	goto	u101
	goto	u100
u101:
	goto	l6904
u100:
	line	24
	
l6902:	
;main.c: 24: OSCTUNE = misc_eer(1, 0x1007);
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
	goto	l6906
	line	26
	
l6904:	
;main.c: 25: else
;main.c: 26: OSCTUNE = 0x92;
	movlw	(092h)
	bsf	status, 5	;RP0=1, select bank1
	bcf	status, 6	;RP1=0, select bank1
	movwf	(144)^080h	;volatile
	line	30
	
l6906:	
;main.c: 30: usart_init();
	fcall	_usart_init
	line	32
	
l6908:	
;main.c: 32: usart_puts((char *)"hello from lgt8f690!\r\n");
	movlw	((STR_1-__stringbase))&0ffh
	fcall	_usart_puts
	line	40
	
l6910:	
;main.c: 39: {
;main.c: 40: usart_putc(usart_getc());
	fcall	_usart_getc
	fcall	_usart_putc
	goto	l6910
	global	start
	ljmp	start
	opt stack 0
psect	maintext
	line	42
GLOBAL	__end_of_main
	__end_of_main:
;; =============== function _main ends ============

	signat	_main,90
	global	_cfw_valid
psect	text86,local,class=CODE,delta=2
global __ptext86
__ptext86:

;; *************** function _cfw_valid *****************
;; Defined at:
;;		line 85 in file "E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_usart\src\misc.c"
;; Parameters:    Size  Location     Type
;;		None
;; Auto vars:     Size  Location     Type
;;  btmp            1    5[COMMON] unsigned char 
;; Return value:  Size  Location     Type
;;                  1    wreg      unsigned char 
;; Registers used:
;;		wreg, pclath, cstack
;; Tracked objects:
;;		On entry : 17F/0
;;		On exit  : 60/40
;;		Unchanged: FFE00/0
;; Data sizes:     COMMON   BANK0   BANK1   BANK2
;;      Params:         0       0       0       0
;;      Locals:         1       0       0       0
;;      Temps:          0       0       0       0
;;      Totals:         1       0       0       0
;;Total ram usage:        1 bytes
;; Hardware stack levels used:    1
;; Hardware stack levels required when called:    2
;; This function calls:
;;		_misc_eer
;; This function is called by:
;;		_main
;; This function uses a non-reentrant model
;;
psect	text86
	file	"E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_usart\src\misc.c"
	line	85
	global	__size_of_cfw_valid
	__size_of_cfw_valid	equ	__end_of_cfw_valid-_cfw_valid
	
_cfw_valid:	
	opt	stack 5
; Regs used in _cfw_valid: [wreg+pclath+cstack]
	line	88
	
l6888:	
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
	
l6890:	
;misc.c: 90: if(btmp == 0x5a)
	movf	(cfw_valid@btmp),w
	xorlw	05Ah
	skipz
	goto	u91
	goto	u90
u91:
	goto	l6896
u90:
	line	91
	
l6892:	
;misc.c: 91: return 1;
	movlw	(01h)
	goto	l5627
	line	93
	
l6896:	
;misc.c: 93: return 0;
	movlw	(0)
	line	94
	
l5627:	
	return
	opt stack 0
GLOBAL	__end_of_cfw_valid
	__end_of_cfw_valid:
;; =============== function _cfw_valid ends ============

	signat	_cfw_valid,89
	global	_usart_putc
psect	text87,local,class=CODE,delta=2
global __ptext87
__ptext87:

;; *************** function _usart_putc *****************
;; Defined at:
;;		line 36 in file "E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_usart\src\usart.c"
;; Parameters:    Size  Location     Type
;;  ch              1    wreg     unsigned char 
;; Auto vars:     Size  Location     Type
;;  ch              1    2[COMMON] unsigned char 
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
;;      Locals:         1       0       0       0
;;      Temps:          0       0       0       0
;;      Totals:         1       0       0       0
;;Total ram usage:        1 bytes
;; Hardware stack levels used:    1
;; Hardware stack levels required when called:    1
;; This function calls:
;;		Nothing
;; This function is called by:
;;		_main
;; This function uses a non-reentrant model
;;
psect	text87
	file	"E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_usart\src\usart.c"
	line	36
	global	__size_of_usart_putc
	__size_of_usart_putc	equ	__end_of_usart_putc-_usart_putc
	
_usart_putc:	
	opt	stack 6
; Regs used in _usart_putc: [wreg]
;usart_putc@ch stored from wreg
	movwf	(usart_putc@ch)
	line	37
	
l6884:	
;usart.c: 37: TXIF = 0x0;
	bcf	(100/8),(100)&7
	line	39
;usart.c: 39: while ( TRMT == 0x0 );
	
l3721:	
	bsf	status, 5	;RP0=1, select bank1
	btfss	(1217/8)^080h,(1217)&7
	goto	u71
	goto	u70
u71:
	goto	l3721
u70:
	line	40
	
l6886:	
;usart.c: 40: TXREG = ch;
	movf	(usart_putc@ch),w
	bcf	status, 5	;RP0=0, select bank0
	movwf	(25)	;volatile
	line	42
;usart.c: 42: while ( TXIF == 0x0 );
	
l3724:	
	btfss	(100/8),(100)&7
	goto	u81
	goto	u80
u81:
	goto	l3724
u80:
	
l3726:	
	line	43
;usart.c: 43: TXIF = 0x0;
	bcf	(100/8),(100)&7
	line	44
	
l3727:	
	return
	opt stack 0
GLOBAL	__end_of_usart_putc
	__end_of_usart_putc:
;; =============== function _usart_putc ends ============

	signat	_usart_putc,4216
	global	_usart_getc
psect	text88,local,class=CODE,delta=2
global __ptext88
__ptext88:

;; *************** function _usart_getc *****************
;; Defined at:
;;		line 47 in file "E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_usart\src\usart.c"
;; Parameters:    Size  Location     Type
;;		None
;; Auto vars:     Size  Location     Type
;;		None
;; Return value:  Size  Location     Type
;;                  1    wreg      unsigned char 
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
psect	text88
	file	"E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_usart\src\usart.c"
	line	47
	global	__size_of_usart_getc
	__size_of_usart_getc	equ	__end_of_usart_getc-_usart_getc
	
_usart_getc:	
	opt	stack 6
; Regs used in _usart_getc: [wreg]
	line	49
	
l6878:	
;usart.c: 49: RCIF = 0x0;
	bcf	(101/8),(101)&7
	line	51
;usart.c: 51: CREN = 0x1;
	bsf	(196/8),(196)&7
	line	52
;usart.c: 52: while ( RCIF == 0x0 );
	
l3730:	
	btfss	(101/8),(101)&7
	goto	u61
	goto	u60
u61:
	goto	l3730
u60:
	
l3732:	
	line	53
;usart.c: 53: RCIF = 0x0;
	bcf	(101/8),(101)&7
	line	55
	
l6880:	
;usart.c: 55: return RCREG;
	movf	(26),w	;volatile
	line	56
	
l3733:	
	return
	opt stack 0
GLOBAL	__end_of_usart_getc
	__end_of_usart_getc:
;; =============== function _usart_getc ends ============

	signat	_usart_getc,89
	global	_usart_puts
psect	text89,local,class=CODE,delta=2
global __ptext89
__ptext89:

;; *************** function _usart_puts *****************
;; Defined at:
;;		line 59 in file "E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_usart\src\usart.c"
;; Parameters:    Size  Location     Type
;;  txbuf           1    wreg     PTR unsigned char 
;;		 -> STR_1(23), 
;; Auto vars:     Size  Location     Type
;;  txbuf           1    2[COMMON] PTR unsigned char 
;;		 -> STR_1(23), 
;; Return value:  Size  Location     Type
;;		None               void
;; Registers used:
;;		wreg, fsr0l, fsr0h, status,2, status,0, pclath
;; Tracked objects:
;;		On entry : 60/0
;;		On exit  : 60/0
;;		Unchanged: FFF9F/0
;; Data sizes:     COMMON   BANK0   BANK1   BANK2
;;      Params:         0       0       0       0
;;      Locals:         1       0       0       0
;;      Temps:          0       0       0       0
;;      Totals:         1       0       0       0
;;Total ram usage:        1 bytes
;; Hardware stack levels used:    2
;; Hardware stack levels required when called:    1
;; This function calls:
;;		Nothing
;; This function is called by:
;;		_main
;; This function uses a non-reentrant model
;;
psect	text89
	file	"E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_usart\src\usart.c"
	line	59
	global	__size_of_usart_puts
	__size_of_usart_puts	equ	__end_of_usart_puts-_usart_puts
	
_usart_puts:	
	opt	stack 5
; Regs used in _usart_puts: [wreg-fsr0h+status,2+status,0+pclath]
;usart_puts@txbuf stored from wreg
	movwf	(usart_puts@txbuf)
	line	60
	
l6870:	
;usart.c: 60: TXIF = 0x0;
	bcf	(100/8),(100)&7
	line	62
;usart.c: 62: while(*txbuf != '\0') {
	goto	l6876
	line	63
	
l3738:	
	bsf	status, 5	;RP0=1, select bank1
	btfss	(1217/8)^080h,(1217)&7
	goto	u31
	goto	u30
u31:
	goto	l3738
u30:
	line	64
	
l6872:	
;usart.c: 64: TXREG = *txbuf++;
	movf	(usart_puts@txbuf),w
	movwf	fsr0
	fcall	stringdir
	bcf	status, 5	;RP0=0, select bank0
	movwf	(25)	;volatile
	
l6874:	
	incf	(usart_puts@txbuf),f
	line	62
	
l6876:	
	movf	(usart_puts@txbuf),w
	movwf	fsr0
	fcall	stringdir
	iorlw	0
	skipz
	goto	u41
	goto	u40
u41:
	goto	l3738
u40:
	line	67
	
l3742:	
	btfss	(100/8),(100)&7
	goto	u51
	goto	u50
u51:
	goto	l3742
u50:
	
l3744:	
	line	68
;usart.c: 68: TXIF = 0x0;
	bcf	(100/8),(100)&7
	line	69
	
l3745:	
	return
	opt stack 0
GLOBAL	__end_of_usart_puts
	__end_of_usart_puts:
;; =============== function _usart_puts ends ============

	signat	_usart_puts,4216
	global	_usart_init
psect	text90,local,class=CODE,delta=2
global __ptext90
__ptext90:

;; *************** function _usart_init *****************
;; Defined at:
;;		line 14 in file "E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_usart\src\usart.c"
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
;; Hardware stack levels required when called:    1
;; This function calls:
;;		Nothing
;; This function is called by:
;;		_main
;; This function uses a non-reentrant model
;;
psect	text90
	file	"E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_usart\src\usart.c"
	line	14
	global	__size_of_usart_init
	__size_of_usart_init	equ	__end_of_usart_init-_usart_init
	
_usart_init:	
	opt	stack 6
; Regs used in _usart_init: [wreg+status,2]
	line	15
	
l6860:	
;usart.c: 15: RB5 = 0x1;
	bcf	status, 5	;RP0=0, select bank0
	bsf	(53/8),(53)&7
	line	16
;usart.c: 16: RB6 = 0x1;
	bsf	(54/8),(54)&7
	line	18
;usart.c: 18: TRISB5 = 0x0;
	bsf	status, 5	;RP0=1, select bank1
	bcf	(1077/8)^080h,(1077)&7
	line	19
;usart.c: 19: TRISB6 = 0x1;
	bsf	(1078/8)^080h,(1078)&7
	line	25
	
l6862:	
;usart.c: 25: SPBRGH = 0x0;
	clrf	(154)^080h	;volatile
	line	26
	
l6864:	
;usart.c: 26: SPBRG = 0xc;
	movlw	(0Ch)
	movwf	(153)^080h	;volatile
	line	29
	
l6866:	
;usart.c: 29: TXSTA = 0x26;
	movlw	(026h)
	movwf	(152)^080h	;volatile
	line	30
	
l6868:	
;usart.c: 30: RCSTA = 0x90;
	movlw	(090h)
	bcf	status, 5	;RP0=0, select bank0
	movwf	(24)	;volatile
	line	33
	
l3718:	
	return
	opt stack 0
GLOBAL	__end_of_usart_init
	__end_of_usart_init:
;; =============== function _usart_init ends ============

	signat	_usart_init,88
	global	_misc_eer
psect	text91,local,class=CODE,delta=2
global __ptext91
__ptext91:

;; *************** function _misc_eer *****************
;; Defined at:
;;		line 6 in file "E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_usart\src\misc.c"
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
;; Hardware stack levels required when called:    1
;; This function calls:
;;		Nothing
;; This function is called by:
;;		_main
;;		_cfw_valid
;; This function uses a non-reentrant model
;;
psect	text91
	file	"E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_usart\src\misc.c"
	line	6
	global	__size_of_misc_eer
	__size_of_misc_eer	equ	__end_of_misc_eer-_misc_eer
	
_misc_eer:	
	opt	stack 6
; Regs used in _misc_eer: [wreg]
;misc_eer@type stored from wreg
	movwf	(misc_eer@type)
	line	7
	
l6852:	
;misc.c: 7: EEADR = address & 0xff;
	movf	(misc_eer@address),w
	bsf	status, 6	;RP1=1, select bank2
	movwf	(269)^0100h	;volatile
	line	8
;misc.c: 8: EEADRH = (address >> 8) & 0xff;
	movf	(misc_eer@address+1),w
	movwf	(271)^0100h	;volatile
	line	10
	
l6854:	
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
	
l6856:	
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
	global	_hisr
psect	text92,local,class=CODE,delta=2
global __ptext92
__ptext92:

;; *************** function _hisr *****************
;; Defined at:
;;		line 15 in file "E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_usart\src\main.c"
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
psect	text92
	file	"E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_usart\src\main.c"
	global	__size_of_hisr
	__size_of_hisr	equ	__end_of_hisr-_hisr
	
_hisr:	
	opt	stack 5
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
psect	text92
	line	16
	
i1l6850:	
# 16 "E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_usart\src\main.c"
nop ;#
psect	text92
	line	17
	
i1l1861:	
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
psect	text93,local,class=CODE,delta=2
global __ptext93
__ptext93:
	global	btemp
	btemp set 07Eh

	DABS	1,126,2	;btemp
	global	wtemp0
	wtemp0 set btemp
	end
