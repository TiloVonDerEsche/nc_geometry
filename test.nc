R1=-300 ; x correspondend
R2=-200 ; y correspondend
R3=0 ; z correspondend
Loop:
  X=R1 Y=R2
  LASER_ON
    R1=R1+4
    R2=R2+2
    X=R1 Y=R2
  LASER_OFF
  ;R1=R1+1
  ;R2=R2+1
IF (R1 < 300) THEN GOTO Loop ENDIF

MSG "End of File, reached!"
