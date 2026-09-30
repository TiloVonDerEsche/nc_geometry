R1=-300 ; x correspondend
R2=R1/4 ; y correspondend
R3=R1/4 ; z correspondend
Loop:
  X=R1 Y=R2 Z=R3
  R1=R1+2
  R2=R1/2  ;+upper y | -lower y
  LASER_ON
    X=R1 Y=R2 Z=R3
  LASER_OFF

  X=R1 Y=R2 Z=R3
  R3=R3+10
  LASER_ON
    X=R1 Y=R2 Z=R3
  LASER_OFF

  X=R1 Y=R2 Z=R3
  R2=R1/4 ;-upper y | +lower y
  R1=R1+1 ;X increment
  LASER_ON
    X=R1 Y=R2 Z=R3
  LASER_OFF


IF (R1<=300) THEN GOTO Loop ENDIF

MSG "End of File, reached!"
