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
	FNCALL	_main,_twi_slave_recv
	FNCALL	_main,_twi_slave_send
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
	file	"E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_twi_slave\src\main.c"
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
	global	_TWSR
_TWSR	set	22
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
	file	"twi_slave.as"
	line	#
psect cinit,class=CODE,delta=2
global start_initialization
start_initialization:

psect	dataBANK0,class=BANK0,space=1
global __pdataBANK0
__pdataBANK0:
	file	"E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_twi_slave\src\main.c"
	line	31
_recv_buf:
       ds      36

psect	dataBANK0
	file	"E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_twi_slave\src\main.c"
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
	global	?_twi_slave_send
?_twi_slave_send:	; 0 bytes @ 0x2
	global	?_usart_send
?_usart_send:	; 0 bytes @ 0x2
	global	?_twi_slave_recv
?_twi_slave_recv:	; 1 bytes @ 0x2
	global	?_misc_eer
?_misc_eer:	; 2 bytes @ 0x2
	global	twi_slave_recv@recv_buf
twi_slave_recv@recv_buf:	; 1 bytes @ 0x2
	global	twi_slave_send@send_adr
twi_slave_send@send_adr:	; 1 bytes @ 0x2
	global	usart_send@txbuf
usart_send@txbuf:	; 1 bytes @ 0x2
	global	misc_eer@address
misc_eer@address:	; 2 bytes @ 0x2
	ds	1
	global	??_usart_send
??_usart_send:	; 0 bytes @ 0x3
	global	twi_slave_recv@length
twi_slave_recv@length:	; 1 bytes @ 0x3
	global	twi_slave_send@send_buf
twi_slave_send@send_buf:	; 1 bytes @ 0x3
	ds	1
	global	??_misc_eer
??_misc_eer:	; 0 bytes @ 0x4
	global	??_twi_slave_recv
??_twi_slave_recv:	; 0 bytes @ 0x4
	global	twi_slave_send@length
twi_slave_send@length:	; 1 bytes @ 0x4
	global	misc_eer@type
misc_eer@type:	; 1 bytes @ 0x4
	ds	1
	global	??_twi_slave_send
??_twi_slave_send:	; 0 bytes @ 0x5
	global	usart_send@length
usart_send@length:	; 1 bytes @ 0x5
	ds	1
	global	usart_send@i
usart_send@i:	; 1 bytes @ 0x6
	ds	2
	global	twi_slave_recv@sla_addr
twi_slave_recv@sla_addr:	; 1 bytes @ 0x8
	ds	1
	global	twi_slave_recv@recv_adr
twi_slave_recv@recv_adr:	; 1 bytes @ 0x9
	global	twi_slave_send@sla_addr
twi_slave_send@sla_addr:	; 1 bytes @ 0x9
	ds	1
	global	twi_slave_recv@i
twi_slave_recv@i:	; 1 bytes @ 0xA
	global	twi_slave_send@i
twi_slave_send@i:	; 1 bytes @ 0xA
	ds	1
psect	cstackBANK0,class=BANK0,space=1
global __pcstackBANK0
__pcstackBANK0:
	global	??_main
??_main:	; 0 bytes @ 0x0
	ds	3
	global	main@reg_addr
main@reg_addr:	; 1 bytes @ 0x3
	ds	1
	global	main@i
main@i:	; 1 bytes @ 0x4
	ds	1
;;Data sizes: Strings 0, constant 0, data 72, bss 0, persistent 0 stack 0
;;Auto spaces:   Size  Autos    Used
;; COMMON          14     11      11
;; BANK0           80      5      77
;; BANK1           80      0       0
;; BANK2           80      0       0

;;
;; Pointer list with targets:

;; ?_misc_eer	unsigned int  size(1) Largest target is 0
;;
;; sp__misc_utoa	PTR unsigned char  size(1) Largest target is 0
;;
;; usart_send@txbuf	PTR unsigned char  size(1) Largest target is 36
;;		 -> send_buf(BANK0[36]), 
;;
;; twi_slave_send@send_buf	PTR unsigned char  size(1) Largest target is 36
;;		 -> send_buf(BANK0[36]), 
;;
;; twi_slave_recv@recv_buf	PTR unsigned char  size(1) Largest target is 36
;;		 -> recv_buf(BANK0[36]), 
;;


;;
;; Critical Paths under _main in COMMON
;;
;;   _main->_twi_slave_recv
;;   _main->_twi_slave_send
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
;;Main: autosize = 0, tempsize = 3, incstack = 0, save=0
;;

;;
;;Call Graph Tables:
;;
;; ---------------------------------------------------------------------------------
;; (Depth) Function   	        Calls       Base Space   Used Autos Params    Refs
;; ---------------------------------------------------------------------------------
;; (0) _main                                                 5     5      0     716
;;                                              0 BANK0      5     5      0
;;                           _misc_eer
;;                         _usart_init
;;                           _twi_init
;;                     _twi_slave_recv
;;                     _twi_slave_send
;;                         _usart_send
;; ---------------------------------------------------------------------------------
;; (1) _usart_send                                           5     4      1     134
;;                                              2 COMMON     5     4      1
;; ---------------------------------------------------------------------------------
;; (1) _twi_slave_send                                       9     6      3     178
;;                                              2 COMMON     9     6      3
;; ---------------------------------------------------------------------------------
;; (1) _twi_slave_recv                                       9     7      2     202
;;                                              2 COMMON     9     7      2
;; ---------------------------------------------------------------------------------
;; (1) _twi_init                                             0     0      0       0
;; ---------------------------------------------------------------------------------
;; (1) _usart_init                                           0     0      0       0
;; ---------------------------------------------------------------------------------
;; (1) _misc_eer                                             3     1      2      66
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
;;   _twi_slave_recv
;;   _twi_slave_send
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
;;DATA                 0      0      59      10        0.0%
;;ABS                  0      0      58       8        0.0%
;;NULL                 0      0       0       0        0.0%
;;STACK                0      0       1       2        0.0%
;;BANK0               50      5      4D       3       96.3%
;;BITBANK0            50      0       0       9        0.0%
;;SFR0                 0      0       0       1        0.0%
;;BITSFR0              0      0       0       1        0.0%
;;COMMON               E      B       B       1       78.6%
;;BITCOMMON            E      0       0       0        0.0%
;;EEDATA             100      0       0       0        0.0%

	global	_main
psect	maintext,global,class=CODE,delta=2
global __pmaintext
__pmaintext:

;; *************** function _main *****************
;; Defined at:
;;		line 51 in file "E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_twi_slave\src\main.c"
;; Parameters:    Size  Location     Type
;;		None
;; Auto vars:     Size  Location     Type
;;  i               1    4[BANK0 ] unsigned char 
;;  reg_addr        1    3[BANK0 ] unsigned char 
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
;;      Locals:         0       2       0       0
;;      Temps:          0       3       0       0
;;      Totals:         0       5       0       0
;;Total ram usage:        5 bytes
;; Hardware stack levels required when called:    2
;; This function calls:
;;		_misc_eer
;;		_usart_init
;;		_twi_init
;;		_twi_slave_recv
;;		_twi_slave_send
;;		_usart_send
;; This function is called by:
;;		Startup code after reset
;; This function uses a non-reentrant model
;;
psect	maintext
	file	"E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_twi_slave\src\main.c"
	line	51
	global	__size_of_main
	__size_of_main	equ	__end_of_main-_main
	
_main:	
	opt	stack 6
; Regs used in _main: [wreg-fsr0h+status,2+status,0+pclath+cstack]
	line	56
	
l9103:	
;main.c: 52: unsigned char i;
;main.c: 53: unsigned char reg_addr;
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
	goto	u611
	goto	u610
u611:
	goto	l9107
u610:
	line	57
	
l9105:	
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
	goto	l9109
	line	59
	
l9107:	
;main.c: 58: else
;main.c: 59: OSCTUNE = 0x92;
	movlw	(092h)
	bsf	status, 5	;RP0=1, select bank1
	bcf	status, 6	;RP1=0, select bank1
	movwf	(144)^080h	;volatile
	line	62
	
l9109:	
;main.c: 62: OSCCONbits.IRCF = 0x6;
	movf	(143)^080h,w	;volatile
	andlw	not (((1<<3)-1)<<4)
	iorlw	(06h & ((1<<3)-1))<<4
	movwf	(143)^080h	;volatile
	line	64
	
l9111:	
;main.c: 64: MCUCR = 0x55;
	movlw	(055h)
	bsf	status, 6	;RP1=1, select bank3
	movwf	(415)^0180h	;volatile
	line	65
	
l9113:	
;main.c: 65: MCUCR = 0xC6;
	movlw	(0C6h)
	movwf	(415)^0180h	;volatile
	line	67
	
l9115:	
;main.c: 67: usart_init();
	fcall	_usart_init
	line	69
	
l9117:	
;main.c: 69: twi_init();
	fcall	_twi_init
	line	73
	
l9119:	
;main.c: 73: reg_addr = twi_slave_recv(0x50, recv_buf, 0x8);
	movlw	(_recv_buf)&0ffh
	movwf	(?_twi_slave_recv)
	movlw	(08h)
	movwf	0+(?_twi_slave_recv)+01h
	movlw	(050h)
	fcall	_twi_slave_recv
	movwf	(main@reg_addr)
	line	75
	
l9121:	
;main.c: 75: for(i=0; i < 8; i++) { send_buf[i+reg_addr] = recv_buf[i]; }
	clrf	(main@i)
	
l9127:	
	movf	(main@i),w
	addlw	_recv_buf&0ffh
	movwf	fsr0
	bcf	status, 7	;select IRP bank0
	movf	indf,w
	movwf	(??_main+0)+0
	movlw	(_send_buf)&0ffh
	addwf	(main@reg_addr),w
	movwf	(??_main+1)+0
	movf	(main@i),w
	addwf	0+(??_main+1)+0,w
	movwf	(??_main+2)+0
	movf	0+(??_main+2)+0,w
	movwf	fsr0
	movf	(??_main+0)+0,w
	movwf	indf
	
l9129:	
	incf	(main@i),f
	
l9131:	
	movlw	(08h)
	subwf	(main@i),w
	skipc
	goto	u621
	goto	u620
u621:
	goto	l9127
u620:
	line	79
	
l9133:	
;main.c: 79: reg_addr = twi_slave_recv(0x50, recv_buf, 0x0);
	movlw	(_recv_buf)&0ffh
	movwf	(?_twi_slave_recv)
	clrf	0+(?_twi_slave_recv)+01h
	movlw	(050h)
	fcall	_twi_slave_recv
	movwf	(main@reg_addr)
	line	80
;main.c: 80: twi_slave_send(0x50, reg_addr, send_buf, 0x8);
	movf	(main@reg_addr),w
	movwf	(?_twi_slave_send)
	movlw	(_send_buf)&0ffh
	movwf	0+(?_twi_slave_send)+01h
	movlw	(08h)
	movwf	0+(?_twi_slave_send)+02h
	movlw	(050h)
	fcall	_twi_slave_send
	line	82
;main.c: 82: usart_send(8, send_buf);
	movlw	(_send_buf)&0ffh
	movwf	(?_usart_send)
	movlw	(08h)
	fcall	_usart_send
	line	84
	
l9135:	
;main.c: 84: TWIEN = 0x0;
	bcf	status, 5	;RP0=0, select bank0
	bcf	(175/8),(175)&7
	line	89
	
l9137:	
;main.c: 89: PEIE = 0x1;
	bsf	(94/8),(94)&7
	line	90
	
l9139:	
;main.c: 90: GIE = 0x0;
	bcf	(95/8),(95)&7
	line	92
	
l1873:	
	goto	l1873
	global	start
	ljmp	start
	opt stack 0
psect	maintext
	line	93
GLOBAL	__end_of_main
	__end_of_main:
;; =============== function _main ends ============

	signat	_main,90
	global	_usart_send
psect	text184,local,class=CODE,delta=2
global __ptext184
__ptext184:

;; *************** function _usart_send *****************
;; Defined at:
;;		line 58 in file "E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_twi_slave\src\usart.c"
;; Parameters:    Size  Location     Type
;;  length          1    wreg     unsigned char 
;;  txbuf           1    2[COMMON] PTR unsigned char 
;;		 -> send_buf(36), 
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
psect	text184
	file	"E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_twi_slave\src\usart.c"
	line	58
	global	__size_of_usart_send
	__size_of_usart_send	equ	__end_of_usart_send-_usart_send
	
_usart_send:	
	opt	stack 6
; Regs used in _usart_send: [wreg-fsr0h+status,2+status,0]
;usart_send@length stored from wreg
	line	61
	movwf	(usart_send@length)
	
l9089:	
;usart.c: 59: unsigned char i;
;usart.c: 61: TXIF = 0x0;
	bcf	(100/8),(100)&7
	line	62
;usart.c: 62: TXEN = 0x1;
	bsf	status, 5	;RP0=1, select bank1
	bsf	(1221/8)^080h,(1221)&7
	line	64
	
l9091:	
;usart.c: 64: for (i=length; i>0; i--) {
	movf	(usart_send@length),w
	movwf	(usart_send@i)
	
l9093:	
	movf	(usart_send@i),f
	skipz
	goto	u581
	goto	u580
u581:
	goto	l5716
u580:
	goto	l5719
	line	65
	
l5716:	
	bsf	status, 5	;RP0=1, select bank1
	btfss	(1217/8)^080h,(1217)&7
	goto	u591
	goto	u590
u591:
	goto	l5716
u590:
	line	66
	
l9097:	
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
	
l9099:	
	decf	(usart_send@i),f
	goto	l9093
	line	69
	
l5719:	
	bcf	status, 5	;RP0=0, select bank0
	btfss	(100/8),(100)&7
	goto	u601
	goto	u600
u601:
	goto	l5719
u600:
	
l5721:	
	line	70
;usart.c: 70: TXIF = 0x0;
	bcf	(100/8),(100)&7
	line	72
;usart.c: 72: TXEN = 0x0;
	bsf	status, 5	;RP0=1, select bank1
	bcf	(1221/8)^080h,(1221)&7
	line	73
	
l5722:	
	return
	opt stack 0
GLOBAL	__end_of_usart_send
	__end_of_usart_send:
;; =============== function _usart_send ends ============

	signat	_usart_send,8312
	global	_twi_slave_send
psect	text185,local,class=CODE,delta=2
global __ptext185
__ptext185:

;; *************** function _twi_slave_send *****************
;; Defined at:
;;		line 119 in file "E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_twi_slave\src\twi.c"
;; Parameters:    Size  Location     Type
;;  sla_addr        1    wreg     unsigned char 
;;  send_adr        1    2[COMMON] unsigned char 
;;  send_buf        1    3[COMMON] PTR unsigned char 
;;		 -> send_buf(36), 
;;  length          1    4[COMMON] unsigned char 
;; Auto vars:     Size  Location     Type
;;  sla_addr        1    9[COMMON] unsigned char 
;;  i               1   10[COMMON] unsigned char 
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
;;      Temps:          4       0       0       0
;;      Totals:         9       0       0       0
;;Total ram usage:        9 bytes
;; Hardware stack levels used:    1
;; Hardware stack levels required when called:    1
;; This function calls:
;;		Nothing
;; This function is called by:
;;		_main
;; This function uses a non-reentrant model
;;
psect	text185
	file	"E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_twi_slave\src\twi.c"
	line	119
	global	__size_of_twi_slave_send
	__size_of_twi_slave_send	equ	__end_of_twi_slave_send-_twi_slave_send
	
_twi_slave_send:	
	opt	stack 6
; Regs used in _twi_slave_send: [wreg-fsr0h+status,2+status,0]
;twi_slave_send@sla_addr stored from wreg
	line	122
	movwf	(twi_slave_send@sla_addr)
	
l9055:	
;twi.c: 120: unsigned char i;
;twi.c: 122: TWSR = 0x0;
	clrf	(22)	;volatile
	line	123
	
l9057:	
;twi.c: 123: TWIF = 0x0;
	bcf	(104/8),(104)&7
	line	124
	
l9059:	
;twi.c: 124: TWIEN = 0x1;
	bsf	(175/8),(175)&7
	line	129
;twi.c: 129: while(TWIF==0x0);
	
l3758:	
	btfss	(104/8),(104)&7
	goto	u471
	goto	u470
u471:
	goto	l3758
u470:
	
l3760:	
	line	131
;twi.c: 131: if (TWIRXS==0x1) {
	btfss	(176/8),(176)&7
	goto	u481
	goto	u480
u481:
	goto	l3761
u480:
	line	132
	
l9061:	
;twi.c: 132: TWIRXP = 0x0;
	bcf	(179/8),(179)&7
	line	133
;twi.c: 133: TWIRXS = 0x0;
	bcf	(176/8),(176)&7
	line	134
	
l3761:	
	line	136
;twi.c: 134: }
;twi.c: 136: TWITACK = 0x1;
	bsf	(171/8),(171)&7
	line	137
;twi.c: 137: TWIF = 0x0;
	bcf	(104/8),(104)&7
	line	143
;twi.c: 143: while(TWIF==0x0);
	
l3762:	
	btfss	(104/8),(104)&7
	goto	u491
	goto	u490
u491:
	goto	l3762
u490:
	
l3764:	
	line	145
;twi.c: 145: if (TWIRXD==0x1) {
	btfss	(177/8),(177)&7
	goto	u501
	goto	u500
u501:
	goto	l3768
u500:
	line	146
	
l9063:	
;twi.c: 146: if (TWDR != ((sla_addr << 1) + 0x1)) {
	movf	(twi_slave_send@sla_addr),w
	movwf	(??_twi_slave_send+0)+0
	clrf	(??_twi_slave_send+0)+0+1
	clrc
	rlf	(??_twi_slave_send+0)+0,f
	rlf	(??_twi_slave_send+0)+1,f
	movf	0+(??_twi_slave_send+0)+0,w
	addlw	low(01h)
	movwf	(??_twi_slave_send+2)+0
	movf	1+(??_twi_slave_send+0)+0,w
	skipnc
	addlw	1
	addlw	high(01h)
	movwf	1+(??_twi_slave_send+2)+0
	movf	(23),w	;volatile
	xorwf	0+(??_twi_slave_send+2)+0,w
	iorwf	1+(??_twi_slave_send+2)+0,w
	skipnz
	goto	u511
	goto	u510
u511:
	goto	l3766
u510:
	goto	l3767
	line	148
	
l3766:	
	line	150
;twi.c: 148: }
;twi.c: 150: TWIRXD = 0x0;
	bcf	(177/8),(177)&7
	line	152
;twi.c: 152: TWITACK = 0x0;
	bcf	(171/8),(171)&7
	line	153
;twi.c: 153: TWITXK = 0x1;
	bsf	(178/8),(178)&7
	line	155
;twi.c: 155: TWIF = 0x0;
	bcf	(104/8),(104)&7
	line	161
	
l3768:	
	btfss	(104/8),(104)&7
	goto	u521
	goto	u520
u521:
	goto	l3768
u520:
	line	163
	
l9067:	
;twi.c: 163: for(i=length; i>0; )
	movf	(twi_slave_send@length),w
	movwf	(twi_slave_send@i)
	
l9069:	
	movf	(twi_slave_send@i),f
	skipz
	goto	u531
	goto	u530
u531:
	goto	l9073
u530:
	goto	l3779
	line	166
	
l9073:	
;twi.c: 164: {
;twi.c: 166: TWDR = send_buf[send_adr + length - i];
	movf	(twi_slave_send@send_buf),w
	movwf	(??_twi_slave_send+0)+0
	movf	(twi_slave_send@i),w
	subwf	(??_twi_slave_send+0)+0,f
	movf	(twi_slave_send@length),w
	addwf	0+(??_twi_slave_send+0)+0,w
	movwf	(??_twi_slave_send+1)+0
	movf	(twi_slave_send@send_adr),w
	addwf	0+(??_twi_slave_send+1)+0,w
	movwf	(??_twi_slave_send+2)+0
	movf	0+(??_twi_slave_send+2)+0,w
	movwf	fsr0
	bcf	status, 7	;select IRP bank0
	movf	indf,w
	movwf	(23)	;volatile
	line	168
	
l9075:	
;twi.c: 168: TWITXD = 0x1;
	bsf	(181/8),(181)&7
	line	170
	
l9077:	
;twi.c: 170: TWIF = 0x0;
	bcf	(104/8),(104)&7
	line	172
;twi.c: 172: while(TWIF==0x0);
	
l3773:	
	btfss	(104/8),(104)&7
	goto	u541
	goto	u540
u541:
	goto	l3773
u540:
	
l3775:	
	line	174
;twi.c: 174: if (TWIRXK==0x1) {
	btfss	(182/8),(182)&7
	goto	u551
	goto	u550
u551:
	goto	l9069
u550:
	line	175
	
l9079:	
;twi.c: 175: if (TWIRACK == 0x1) { goto _not_addressed_; }
	btfss	(170/8),(170)&7
	goto	u561
	goto	u560
u561:
	goto	l9085
u560:
	goto	l3767
	line	176
	
l9085:	
;twi.c: 176: else {i--;}
	decf	(twi_slave_send@i),f
	line	177
	
l9087:	
;twi.c: 177: TWIRXK = 0x0;
	bcf	(182/8),(182)&7
	goto	l9069
	line	181
	
l3779:	
	btfss	(104/8),(104)&7
	goto	u571
	goto	u570
u571:
	goto	l3779
u570:
	line	186
;twi.c: 186: _not_addressed_:
	
l3767:	
	line	188
;twi.c: 188: TWIF = 0x0;
	bcf	(104/8),(104)&7
	line	189
;twi.c: 189: TWIEN = 0x0;
	bcf	(175/8),(175)&7
	line	191
	
l3782:	
	return
	opt stack 0
GLOBAL	__end_of_twi_slave_send
	__end_of_twi_slave_send:
;; =============== function _twi_slave_send ends ============

	signat	_twi_slave_send,16504
	global	_twi_slave_recv
psect	text186,local,class=CODE,delta=2
global __ptext186
__ptext186:

;; *************** function _twi_slave_recv *****************
;; Defined at:
;;		line 28 in file "E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_twi_slave\src\twi.c"
;; Parameters:    Size  Location     Type
;;  sla_addr        1    wreg     unsigned char 
;;  recv_buf        1    2[COMMON] PTR unsigned char 
;;		 -> recv_buf(36), 
;;  length          1    3[COMMON] unsigned char 
;; Auto vars:     Size  Location     Type
;;  sla_addr        1    8[COMMON] unsigned char 
;;  i               1   10[COMMON] unsigned char 
;;  recv_adr        1    9[COMMON] unsigned char 
;; Return value:  Size  Location     Type
;;                  1    wreg      unsigned char 
;; Registers used:
;;		wreg, fsr0l, fsr0h, status,2, status,0
;; Tracked objects:
;;		On entry : 60/0
;;		On exit  : 60/0
;;		Unchanged: FFE9F/0
;; Data sizes:     COMMON   BANK0   BANK1   BANK2
;;      Params:         2       0       0       0
;;      Locals:         3       0       0       0
;;      Temps:          4       0       0       0
;;      Totals:         9       0       0       0
;;Total ram usage:        9 bytes
;; Hardware stack levels used:    1
;; Hardware stack levels required when called:    1
;; This function calls:
;;		Nothing
;; This function is called by:
;;		_main
;; This function uses a non-reentrant model
;;
psect	text186
	file	"E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_twi_slave\src\twi.c"
	line	28
	global	__size_of_twi_slave_recv
	__size_of_twi_slave_recv	equ	__end_of_twi_slave_recv-_twi_slave_recv
	
_twi_slave_recv:	
	opt	stack 6
; Regs used in _twi_slave_recv: [wreg-fsr0h+status,2+status,0]
;twi_slave_recv@sla_addr stored from wreg
	line	32
	movwf	(twi_slave_recv@sla_addr)
	
l9015:	
;twi.c: 29: unsigned char i;
;twi.c: 30: unsigned char recv_adr;
;twi.c: 32: recv_adr = 0x0;
	clrf	(twi_slave_recv@recv_adr)
	line	34
;twi.c: 34: TWSR = 0x0;
	clrf	(22)	;volatile
	line	35
	
l9017:	
;twi.c: 35: TWIF = 0x0;
	bcf	(104/8),(104)&7
	line	36
	
l9019:	
;twi.c: 36: TWIEN = 0x1;
	bsf	(175/8),(175)&7
	line	40
	
l9021:	
;twi.c: 40: TWIRXP = 0x0;
	bcf	(179/8),(179)&7
	line	41
;twi.c: 41: while(TWIF==0x0);
	
l3727:	
	btfss	(104/8),(104)&7
	goto	u341
	goto	u340
u341:
	goto	l3727
u340:
	
l3729:	
	line	43
;twi.c: 43: if (TWIRXS==0x1) {
	btfss	(176/8),(176)&7
	goto	u351
	goto	u350
u351:
	goto	l3730
u350:
	line	44
	
l9023:	
;twi.c: 44: TWIRXP = 0x0;
	bcf	(179/8),(179)&7
	line	45
;twi.c: 45: TWIRXS = 0x0;
	bcf	(176/8),(176)&7
	line	46
	
l3730:	
	line	51
;twi.c: 46: }
;twi.c: 51: TWITACK = 0x1;
	bsf	(171/8),(171)&7
	line	52
;twi.c: 52: TWIF = 0x0;
	bcf	(104/8),(104)&7
	line	54
;twi.c: 54: while(TWIF==0x0);
	
l3731:	
	btfss	(104/8),(104)&7
	goto	u361
	goto	u360
u361:
	goto	l3731
u360:
	
l3733:	
	line	56
;twi.c: 56: if (TWIRXD==0x1) {
	btfss	(177/8),(177)&7
	goto	u371
	goto	u370
u371:
	goto	l3734
u370:
	line	57
	
l9025:	
;twi.c: 57: if (TWDR != ((sla_addr << 1) + 0x0)) {
	movf	(twi_slave_recv@sla_addr),w
	movwf	(??_twi_slave_recv+0)+0
	clrf	(??_twi_slave_recv+0)+0+1
	clrc
	rlf	(??_twi_slave_recv+0)+0,f
	rlf	(??_twi_slave_recv+0)+1,f
	movf	(23),w	;volatile
	movwf	(??_twi_slave_recv+2)+0
	clrf	(??_twi_slave_recv+2)+0+1
	movf	1+(??_twi_slave_recv+0)+0,w
	xorwf	1+(??_twi_slave_recv+2)+0,w
	skipz
	goto	u385
	movf	0+(??_twi_slave_recv+0)+0,w
	xorwf	0+(??_twi_slave_recv+2)+0,w
u385:

	skipnz
	goto	u381
	goto	u380
u381:
	goto	l3735
u380:
	goto	l3736
	line	59
	
l3735:	
	line	60
;twi.c: 59: }
;twi.c: 60: TWITACK = 0x0;
	bcf	(171/8),(171)&7
	line	61
;twi.c: 61: TWIRXD = 0x0;
	bcf	(177/8),(177)&7
	line	62
;twi.c: 62: TWITXK = 0x1;
	bsf	(178/8),(178)&7
	line	63
	
l3734:	
	line	68
;twi.c: 63: }
;twi.c: 68: TWIF = 0x0;
	bcf	(104/8),(104)&7
	line	70
;twi.c: 70: while(TWIF==0x0);
	
l3737:	
	btfss	(104/8),(104)&7
	goto	u391
	goto	u390
u391:
	goto	l3737
u390:
	
l3739:	
	line	75
;twi.c: 75: TWIF = 0x0;
	bcf	(104/8),(104)&7
	line	77
;twi.c: 77: while(TWIF==0x0);
	
l3740:	
	btfss	(104/8),(104)&7
	goto	u401
	goto	u400
u401:
	goto	l3740
u400:
	
l3742:	
	line	79
;twi.c: 79: if (TWIRXD==0x1) {
	btfss	(177/8),(177)&7
	goto	u411
	goto	u410
u411:
	goto	l3743
u410:
	line	80
	
l9029:	
;twi.c: 80: recv_adr = TWDR;
	movf	(23),w	;volatile
	movwf	(twi_slave_recv@recv_adr)
	line	81
	
l9031:	
;twi.c: 81: TWITACK = 0x0;
	bcf	(171/8),(171)&7
	line	82
	
l9033:	
;twi.c: 82: TWIRXD = 0x0;
	bcf	(177/8),(177)&7
	line	83
	
l9035:	
;twi.c: 83: TWITXK = 0x1;
	bsf	(178/8),(178)&7
	line	84
	
l3743:	
	line	89
;twi.c: 84: }
;twi.c: 89: for(i=length; i>0; )
	movf	(twi_slave_recv@length),w
	movwf	(twi_slave_recv@i)
	
l9037:	
	movf	(twi_slave_recv@i),f
	skipz
	goto	u421
	goto	u420
u421:
	goto	l3744
u420:
	goto	l3752
	line	90
	
l3744:	
	line	91
;twi.c: 90: {
;twi.c: 91: TWIF = 0x0;
	bcf	(104/8),(104)&7
	line	93
;twi.c: 93: while(TWIF==0x0);
	
l3746:	
	btfss	(104/8),(104)&7
	goto	u431
	goto	u430
u431:
	goto	l3746
u430:
	
l3748:	
	line	95
;twi.c: 95: if (TWIRXD==0x1) {
	btfss	(177/8),(177)&7
	goto	u441
	goto	u440
u441:
	goto	l9037
u440:
	line	96
	
l9041:	
;twi.c: 96: recv_buf[length-i] = TWDR;
	movf	(twi_slave_recv@recv_buf),w
	movwf	(??_twi_slave_recv+0)+0
	movf	(twi_slave_recv@i),w
	subwf	(??_twi_slave_recv+0)+0,f
	movf	(twi_slave_recv@length),w
	addwf	0+(??_twi_slave_recv+0)+0,w
	movwf	(??_twi_slave_recv+1)+0
	movf	0+(??_twi_slave_recv+1)+0,w
	movwf	fsr0
	movf	(23),w	;volatile
	bcf	status, 7	;select IRP bank0
	movwf	indf
	line	97
	
l9043:	
;twi.c: 97: if (i==1) { TWITACK = 0x1; }
	decf	(twi_slave_recv@i),w
	skipz
	goto	u451
	goto	u450
u451:
	goto	l3750
u450:
	
l9045:	
	bsf	(171/8),(171)&7
	goto	l3751
	line	98
	
l3750:	
;twi.c: 98: else { TWITACK = 0x0; }
	bcf	(171/8),(171)&7
	
l3751:	
	line	99
;twi.c: 99: TWIRXD = 0x0;
	bcf	(177/8),(177)&7
	line	100
	
l9047:	
;twi.c: 100: i--;
	decf	(twi_slave_recv@i),f
	line	101
	
l9049:	
;twi.c: 101: TWITXK = 0x1;
	bsf	(178/8),(178)&7
	goto	l9037
	line	105
	
l3752:	
	btfss	(104/8),(104)&7
	goto	u461
	goto	u460
u461:
	goto	l3752
u460:
	line	110
;twi.c: 110: _not_addressed_:
	
l3736:	
	line	112
;twi.c: 112: TWIF = 0x0;
	bcf	(104/8),(104)&7
	line	113
;twi.c: 113: TWIEN = 0x0;
	bcf	(175/8),(175)&7
	line	115
	
l9051:	
;twi.c: 115: return recv_adr;
	movf	(twi_slave_recv@recv_adr),w
	line	116
	
l3755:	
	return
	opt stack 0
GLOBAL	__end_of_twi_slave_recv
	__end_of_twi_slave_recv:
;; =============== function _twi_slave_recv ends ============

	signat	_twi_slave_recv,12409
	global	_twi_init
psect	text187,local,class=CODE,delta=2
global __ptext187
__ptext187:

;; *************** function _twi_init *****************
;; Defined at:
;;		line 12 in file "E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_twi_slave\src\twi.c"
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
psect	text187
	file	"E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_twi_slave\src\twi.c"
	line	12
	global	__size_of_twi_init
	__size_of_twi_init	equ	__end_of_twi_init-_twi_init
	
_twi_init:	
	opt	stack 6
; Regs used in _twi_init: [wreg]
	line	14
	
l9011:	
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
	
l9013:	
;twi.c: 24: TWCR = 0xc1;
	movlw	(0C1h)
	bcf	status, 5	;RP0=0, select bank0
	movwf	(21)	;volatile
	line	25
	
l3724:	
	return
	opt stack 0
GLOBAL	__end_of_twi_init
	__end_of_twi_init:
;; =============== function _twi_init ends ============

	signat	_twi_init,88
	global	_usart_init
psect	text188,local,class=CODE,delta=2
global __ptext188
__ptext188:

;; *************** function _usart_init *****************
;; Defined at:
;;		line 12 in file "E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_twi_slave\src\usart.c"
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
psect	text188
	file	"E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_twi_slave\src\usart.c"
	line	12
	global	__size_of_usart_init
	__size_of_usart_init	equ	__end_of_usart_init-_usart_init
	
_usart_init:	
	opt	stack 6
; Regs used in _usart_init: [wreg+status,2]
	line	13
	
l9001:	
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
	
l9003:	
;usart.c: 19: SPBRGH = 0x0;
	clrf	(154)^080h	;volatile
	line	20
	
l9005:	
;usart.c: 20: SPBRG = 0xc;
	movlw	(0Ch)
	movwf	(153)^080h	;volatile
	line	23
	
l9007:	
;usart.c: 23: TXSTA = 0x24;
	movlw	(024h)
	movwf	(152)^080h	;volatile
	line	24
	
l9009:	
;usart.c: 24: RCSTA = 0x80;
	movlw	(080h)
	bcf	status, 5	;RP0=0, select bank0
	movwf	(24)	;volatile
	line	25
	
l5696:	
	return
	opt stack 0
GLOBAL	__end_of_usart_init
	__end_of_usart_init:
;; =============== function _usart_init ends ============

	signat	_usart_init,88
	global	_misc_eer
psect	text189,local,class=CODE,delta=2
global __ptext189
__ptext189:

;; *************** function _misc_eer *****************
;; Defined at:
;;		line 6 in file "E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_twi_slave\src\misc.c"
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
psect	text189
	file	"E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_twi_slave\src\misc.c"
	line	6
	global	__size_of_misc_eer
	__size_of_misc_eer	equ	__end_of_misc_eer-_misc_eer
	
_misc_eer:	
	opt	stack 6
; Regs used in _misc_eer: [wreg]
;misc_eer@type stored from wreg
	movwf	(misc_eer@type)
	line	7
	
l8993:	
;misc.c: 7: EEADR = address & 0xff;
	movf	(misc_eer@address),w
	bsf	status, 6	;RP1=1, select bank2
	movwf	(269)^0100h	;volatile
	line	8
;misc.c: 8: EEADRH = (address >> 8) & 0xff;
	movf	(misc_eer@address+1),w
	movwf	(271)^0100h	;volatile
	line	10
	
l8995:	
;misc.c: 10: EEPGD = type & 1;
	btfsc	(misc_eer@type),0
	goto	u321
	goto	u320
	
u321:
	bsf	status, 5	;RP0=1, select bank3
	bsf	status, 6	;RP1=1, select bank3
	bsf	(3175/8)^0180h,(3175)&7
	goto	u334
u320:
	bsf	status, 5	;RP0=1, select bank3
	bsf	status, 6	;RP1=1, select bank3
	bcf	(3175/8)^0180h,(3175)&7
u334:
	line	12
	
l8997:	
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
	
l7591:	
	return
	opt stack 0
GLOBAL	__end_of_misc_eer
	__end_of_misc_eer:
;; =============== function _misc_eer ends ============

	signat	_misc_eer,8314
	global	_hisr
psect	text190,local,class=CODE,delta=2
global __ptext190
__ptext190:

;; *************** function _hisr *****************
;; Defined at:
;;		line 46 in file "E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_twi_slave\src\main.c"
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
psect	text190
	file	"E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_twi_slave\src\main.c"
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
psect	text190
	line	47
	
i1l8843:	
# 47 "E:\Projects\PDN1609\lgt8f690a_demo_v1.0\lgt8f690a_twi_slave\src\main.c"
nop ;#
psect	text190
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
psect	text191,local,class=CODE,delta=2
global __ptext191
__ptext191:
	global	btemp
	btemp set 07Eh

	DABS	1,126,2	;btemp
	global	wtemp0
	wtemp0 set btemp
	end
