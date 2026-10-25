R1=0 ;Length
R2=0
R3=0
R4=0 ;Delta XY

G91 ;Incremental Mode
N1020 R22=0
N1030 BOUCLE4:				;LOOP 2
	/N1040 LASER_ON			;Fire with LASER
		N1050 X=R1 F=VIT_TIR			;Length
		N1060 LASER_OFF			;Stop LASER Fire
			N1070 Y=R4			;Delta XY
		/N1080 LASER_ON			;Fire with LASER
			N1090 X=-R1			;-Length
		N1100 LASER_OFF			;Stop LASER Fire
			N1110 Y=R4			;Delta XY
		N1120 R22=R22+1			;var indice Boucle2
		N1130 IF R22<R2 GOTOB BOUCLE4
	N1140 ENDIF
MSG "End of File!!"
