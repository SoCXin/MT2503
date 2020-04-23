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
	FNROOT	_main
	global	_RB0
psect	maintext,global,class=CODE,delta=2
global __pmaintext
__pmaintext:
_RB0	set	48
	global	_OSCTUNE
_OSCTUNE	set	144
	global	_WPUA
_WPUA	set	149
	global	_DPSM1
_DPSM1	set	1139
	global	_IOCA2
_IOCA2	set	1202
	global	_LVRE
_LVRE	set	1140
	global	_TRISA0
_TRISA0	set	1064
	global	_nRABPU
_nRABPU	set	1039
	global	_WPUB
_WPUB	set	277
	global	_WPUC
_WPUC	set	275
	global	_LATA0
_LATA0	set	2088
	file	"sleep_s3.as"
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
	global	??_main
??_main:	; 0 bytes @ 0x0
	global	?_main
?_main:	; 2 bytes @ 0x0
;;Data sizes: Strings 0, constant 0, data 0, bss 0, persistent 0 stack 0
;;Auto spaces:   Size  Autos    Used
;; COMMON          14      0       0
;; BANK0           80      0       0
;; BANK1           80      0       0
;; BANK2           80      0       0

;;
;; Pointer list with targets:



;;
;; Critical Paths under _main in COMMON
;;
;;   None.
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
;;Main: autosize = 0, tempsize = 0, incstack = 0, save=0
;;

;;
;;Call Graph Tables:
;;
;; ---------------------------------------------------------------------------------
;; (Depth) Function   	        Calls       Base Space   Used Autos Params    Refs
;; ---------------------------------------------------------------------------------
;; (0) _main                                                 0     0      0       0
;; ---------------------------------------------------------------------------------
;; Estimated maximum stack depth 0
;; ---------------------------------------------------------------------------------

;; Call Graph Graphs:

;; _main (ROOT)
;;

;; Address spaces:

;;Name               Size   Autos  Total    Cost      Usage
;;BITCOMMON            E      0       0       0        0.0%
;;EEDATA             100      0       0       0        0.0%
;;NULL                 0      0       0       0        0.0%
;;CODE                 0      0       0       0        0.0%
;;COMMON               E      0       0       1        0.0%
;;BITSFR0              0      0       0       1        0.0%
;;SFR0                 0      0       0       1        0.0%
;;BITSFR1              0      0       0       2        0.0%
;;SFR1                 0      0       0       2        0.0%
;;STACK                0      0       0       2        0.0%
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
psect	maintext

;; *************** function _main *****************
;; Defined at:
;;		line 14 in file "E:\Projects\PDN1609\lgt8f690a_sdk\lgt8f690a_sleep\dpsm_s3.c"
;; Parameters:    Size  Location     Type
;;		None
;; Auto vars:     Size  Location     Type
;;		None
;; Return value:  Size  Location     Type
;;                  2  1868[COMMON] int 
;; Registers used:
;;		wreg
;; Tracked objects:
;;		On entry : 17F/0
;;		On exit  : 0/0
;;		Unchanged: 0/0
;; Data sizes:     COMMON   BANK0   BANK1   BANK2
;;      Params:         0       0       0       0
;;      Locals:         0       0       0       0
;;      Temps:          0       0       0       0
;;      Totals:         0       0       0       0
;;Total ram usage:        0 bytes
;; This function calls:
;;		Nothing
;; This function is called by:
;;		Startup code after reset
;; This function uses a non-reentrant model
;;
psect	maintext
	file	"E:\Projects\PDN1609\lgt8f690a_sdk\lgt8f690a_sleep\dpsm_s3.c"
	line	14
	global	__size_of_main
	__size_of_main	equ	__end_of_main-_main
	
_main:	
	opt	stack 8
; Regs used in _main: [wreg]
	line	21
	
l3096:	
;dpsm_s3.c: 21: OSCTUNE = 0x91;
	movlw	(091h)
	bsf	status, 5	;RP0=1, select bank1
	movwf	(144)^080h	;volatile
	line	24
;dpsm_s3.c: 24: WPUC = 0xff;
	movlw	(0FFh)
	bcf	status, 5	;RP0=0, select bank2
	bsf	status, 6	;RP1=1, select bank2
	movwf	(275)^0100h	;volatile
	line	25
;dpsm_s3.c: 25: WPUA = 0xfe;
	movlw	(0FEh)
	bsf	status, 5	;RP0=1, select bank1
	bcf	status, 6	;RP1=0, select bank1
	movwf	(149)^080h	;volatile
	line	26
;dpsm_s3.c: 26: WPUB = 0xff;
	movlw	(0FFh)
	bcf	status, 5	;RP0=0, select bank2
	bsf	status, 6	;RP1=1, select bank2
	movwf	(277)^0100h	;volatile
	line	29
	
l3098:	
;dpsm_s3.c: 29: nRABPU = 0;
	bsf	status, 5	;RP0=1, select bank1
	bcf	status, 6	;RP1=0, select bank1
	bcf	(1039/8)^080h,(1039)&7
	line	32
	
l3100:	
;dpsm_s3.c: 32: DPSM1 = 1;
	bsf	(1139/8)^080h,(1139)&7
	line	34
	
l3102:	
;dpsm_s3.c: 34: LVRE = 0;
	bcf	(1140/8)^080h,(1140)&7
	line	37
	
l3104:	
;dpsm_s3.c: 37: TRISA0 = 0;
	bcf	(1064/8)^080h,(1064)&7
	line	41
	
l3106:	
;dpsm_s3.c: 41: IOCA2 = 1;
	bsf	(1202/8)^080h,(1202)&7
	line	43
;dpsm_s3.c: 43: while(1)
	
l1869:	
	line	45
;dpsm_s3.c: 44: {
;dpsm_s3.c: 45: LATA0 = ~LATA0;
	movlw	1<<((2088)&7)
	bcf	status, 5	;RP0=0, select bank2
	bsf	status, 6	;RP1=1, select bank2
	xorwf	((2088)/8)^0100h,f
	line	46
	
l3108:	
;dpsm_s3.c: 46: _nop(); _nop();
	nop
	
l3110:	
	nop
	line	48
	
l3112:	
;dpsm_s3.c: 48: if(RB0 == 0)
	bcf	status, 5	;RP0=0, select bank0
	bcf	status, 6	;RP1=0, select bank0
	btfsc	(48/8),(48)&7
	goto	u11
	goto	u10
u11:
	goto	l1870
u10:
	line	50
	
l3114:	
# 50 "E:\Projects\PDN1609\lgt8f690a_sdk\lgt8f690a_sleep\dpsm_s3.c"
sleep ;#
psect	maintext
	line	51
	
l1870:	
	line	53
;dpsm_s3.c: 51: }
;dpsm_s3.c: 53: _nop(); _nop();
	nop
	nop
	goto	l1869
	global	start
	ljmp	start
	opt stack 0
psect	maintext
	line	55
GLOBAL	__end_of_main
	__end_of_main:
;; =============== function _main ends ============

	signat	_main,90
	global	btemp
	btemp set 07Eh

	DABS	1,126,2	;btemp
	global	wtemp0
	wtemp0 set btemp
	end
