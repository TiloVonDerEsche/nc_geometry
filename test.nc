;++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
;+                                                                              +
;+         	MANUFACTURING CUBE MM CuAl10/316L 10Vx - XY DIRECTION   	        +
;+                                     HOPPER 2/3                               +
;+                /!\  BECAREFUL WITH WORKPIECE COORDINATE  /!\                 +
;+                                      G55                                     +
;++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
;AENDERUNGSDATUM: 08.09.2025
;NAME: THORE GERICKE
;MATERIAL: MM CUAL VOESTALPINE 316L GTV 0-100 %
;PF2: REZEPT 3
;PF3: REZEPT 6
;FOERDERRATE   0%: 0.00G/MIN CUAL / 6.50G/MIN 316L
;FOERDERRATE  20%: 1.24G/MIN CUAL / 4.96G/MIN 316L
;FOERDERRATE  50%: 2.88G/MIN CUAL / 2.88G/MIN 316L
;FOERDERRATE  60%: 3.36G/MIN CUAL / 2.24G/MIN 316L
;FOERDERRATE  80%: 4.24G/MIN CUAL / 1.06G/MIN 316L
;FOERDERRATE 100%: 5.00G/MIN CUAL / 0.00G/MIN 316L
;Z-INKREMENT: 0.18MM
;NULLPUNKTE: G55 X-15, Y-15

;Amplituden 316L:
R40=79 ;   0% CuAl
R41=63 ;  20% CuAl
R42=42 ;  50% CuAl
R43=35 ;  60% CuAl
R44=18 ;  80% CuAl
R45=1  ; 100% CuAl
R46=13 ;  80% CuAl
R47=25 ;  60% CuAl
R48=31 ;  50% CuAl
R49=53 ;  20% CuAl
R50=77 ;   0% CuAl

;Amplituden CuAl:
R60=1  ;   0% CuAl
R61=21 ;  20% CuAl
R62=47 ;  50% CuAl
R63=54 ;  60% CuAl
R64=67 ;  80% CuAl
R65=77 ; 100% CuAl
R66=67 ;  80% CuAl
R67=54 ;  60% CuAl
R68=48 ;  50% CuAl
R69=23 ;  20% CuAl
R70=1  ;   0% CuAl

;Verfahrgeschwindigkeiten
R80=1750 ;   0% CuAl
R81=1800 ;  20% CuAl
R82=1875 ;  50% CuAl
R83=1900 ;  60% CuAl
R84=1950 ;  80% CuAl
R85=2000 ; 100% CuAl

;Laserleistung
R90=311 ;   0% CuAl
R91=270 ;  20% CuAl
R92=326 ;  50% CuAl
R93=332 ;  60% CuAl
R94=366 ;  80% CuAl
R95=414 ; 100% CuAl

CALL "//LOCAL_DRIVE:/SPANMAT/00_HILFSPROGRAMME/00_PROGRAMMLAUFZEIT/ZEIT_START_SPANMAT.MPF"

N10 G90 G17 G40
N20 TRAFOOF
N30 TRANS
N40 G00 SUPA D0 Z459.
N50 G00 SUPA D0 X69. Y249.
N60 G00 B0. C=DC(0.)
N70 G55
N80 G00 SUPA D0 Z459.
N90 T1
N100 G00 SUPA D0 Z459.
N110 D1

N120 MODE_LASER 1    			;Speed / Power Indexed.
N130 PUIS_LASER R90   		   ;Laser Power
N140 VIT_TIR=R80  			  ;Specifies the feed rate nozzle/laser (mm/mn)
N150 COMMANDE_LASER  			;Laser Activation

;++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
;+                                                                              +
;+                SHIELD & FORMING & CARRIER GAS & POWDER FLOW					+
;+                                                                              +
;++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

N160 H61=3      				;Central gas flow (1 = 1l/min) (Shield)
N170 H62=6      				;Secondary gas flow (1 = 1l/min) (Forming)

N180 Flow_gas_2 1.5      		;Channel 2 carrier gas  1 = 1l/min
N190 Feeder_Speed_2 R60 			;Channel 2 number of hopper 2 controlled (0.00/MIN) CUAL
N200 Flow_gas_3 1.5      		;Channel 3 carrier gas  1 = 1l/min
N210 Feeder_Speed_3 R40  		;Channel 3 number of hopper 3 controled (6.50G/MIN) 316L

;++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
;+                                                                              +
;+                  SCHIELD GAS & FORMING GAS 									+
;+                                                                              +
;++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

/N230 M180     					;Central gas ON (Shield)
/N240 M182     					;Secondary gas ON (Forming)

;++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
;+                                                                              +
;+                  TURNTABLE & STIRRER & GAS 									+
;+                                                                              +
;++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

/N250 M162                 		;Channel 2 ON (Hopper 2 ON)
/N260 M164                 		;Channel 3 ON (Hopper 3 ON)

;++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
;+                                                                              +
;+                  CONSTRUCTION											+
;+                                                                              +
;++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
N280 R1=27.85       	;Length
N290 R2=32         		;Width/Delta XY (even number)-No. of Stepover
N300 R3=7         		;Height/Delta Z (even number)-No. of Layers per Material
N310 R7=32				;Length/Delta XY (even number)-No. of Stepover
N320 R33=27.85          ;Width
     R4=0.45        	;Delta XY
     R5=0.18        	;Delta Z
     R6=0.25			;Contour
	 R8=300				;Dwell time in seconds

N330 R21=0                		;var indice Boucle1
N340 R22=0                		;var indice Boucle2
N341 R23=0			         	;var indice Material Changes

N350 STOPRE 					;Preprocessing stop until all prepared R(x) as a parameter will be read or execute

N360 G55                  		;Select workpiece coordinate system 1, with feed rate define
N370 G90                  		;Specifies Absolute
N380 G00 X0 Y0
N390 G00 Z10
N400 G01 Z0 F=VIT_TIR     		;Linear Interpolation with feed rate define
N410 G91                  		;Specifies Incremental (Relative)

;++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
;+                                                                              +
;+                  SWAP CONSTRUCTION	X-DIRECTION								+
;+                                                                              +
;++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
CONSTRUCTION1:
/N420 G04 F=R8              		;Dwell time is specified after F (in seconds)

N430 BOUCLE1:					;LOOP 1
	N440 R22=0
		N450 BOUCLE2: 			;LOOP 2
			/N460 LASER_ON			;Fire with LASER
				N470 X=R1 F=VIT_TIR		;Length
			N480 LASER_OFF			;Stop LASER Fire
				N490 Y=R4		;Delta XY
			/N500 LASER_ON			;Fire with LASER
				N510 X=-R1		;-Length
			N520 LASER_OFF			;Stop LASER Fire
				N530 Y=R4		;Delta XY
			N540 R22=R22+1		;var indice Boucle2
			N550 IF R22<R2 GOTOB BOUCLE2
		N560 ENDIF
;++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
;+                                                                              +
;+                  POSITION and OUTLINE										+
;+                                                                              +
;++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

N560	G90 						;Specifies Absolute
N570	G01 X0 Y0 F=VIT_TIR  		;Linear Interpolation with feed rate define
N580	G91
N590	X=-R6						;Outline started
	/N600 LASER_ON					;Fire with LASER
N610	Y=R33+R6					;width+contour
N620	X=R1+2*R6					;length+2contours
N630	Y=-R33-2*R6					;width+2contours
N640	X=-R1-2*R6					;length+2contours
N650	Y=R6						;Outline closed
N660	LASER_OFF					;Stop LASER Fire

;++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
;+                                                                              +
;+                  INCREMENT OFFSET											+
;+                                                                              +
;++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	N670 Z=R5					;Delta Z
	N680 G90 					;Specifies Absolute
	N690 G01 X0 Y0 F=VIT_TIR	;Linear Interpolation with feed rate define
	N700 G91					;Specifies Incremental (Relative)


;+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
;+                                                                              +
;+                  SWAP CONSTRUCTION	Y-DIRECTION								+
;+                                                                              +
;++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	N710 R22=0
		N720 BOUCLE3: 				;LOOP 3
			/N730 LASER_ON			;Fire with LASER
				N740 Y=R33			;+Length
			N750 LASER_OFF			;Stop LASER Fire
				N760 X=R4			;Delta XY
			/N770 LASER_ON			;Fire with LASER
				N780 Y=-R33			;+-Length
			N790 LASER_OFF			;Stop LASER Fire
				N800 X=R4			;Delta X
			N810 R22=R22+1			;var indice Boucle3
			N820 IF R22<R7 GOTOB BOUCLE3
		N830 ENDIF


;++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
;+                                                                              +
;+                  POSITION and OUTLINE										+
;+                                                                              +
;++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

N840	G90 						;Specifies Absolute
N850	G01 X0 Y0 F=VIT_TIR  		;Linear Interpolation with feed rate define
N860	G91
N870	Y=-R6						;Outline started
/N880	LASER_ON					;Fire with LASER
N890	X=R1+R6						;length+2contours
N900	Y=R33+2*R6					;width+2contourS
N910	X=-R1-2*R6					;length+2contours
N920	Y=-R33-2*R6					;width+2contours
N930	X=R6						;Outline closed
N940	LASER_OFF					;Stop LASER Fire

;++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
;+                                                                              +
;+                  INCREMENT OFFSET											+
;+                                                                              +
;++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	N580 Z=R5					;Delta Z
	N590 G90 					;Specifies Absolute
	N600 G01 X0 Y0 F=VIT_TIR	;Linear Interpolation with feed rate define
	N610 G91					;Specifies Incremental (Relative)

	N620 R21=R21+1				;var indice Boucle1
	N630 IF R21<R3 GOTOB BOUCLE1
N640 ENDIF

R23=R23+1
CONSTRUCTION0:

PARAMETER1:
PUIS_LASER R91   		   ;Laser Power
VIT_TIR=R81  			  ;Specifies the feed rate nozzle/laser (mm/min)
Feeder_Speed_2 R61
Feeder_Speed_3 R41
REPEAT CONSTRUCTION1 PARAMETER1

PARAMETER2:
PUIS_LASER R92   		   ;Laser Power
VIT_TIR=R82  			  ;Specifies the feed rate nozzle/laser (mm/min)
Feeder_Speed_2 R62
Feeder_Speed_3 R42
REPEAT CONSTRUCTION1 PARAMETER2 ;Never reaches PARAMETER2, since it jumps away with the previous REPEAT
				;TODO Look REPEAT Manual
