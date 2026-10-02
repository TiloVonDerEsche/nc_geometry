STANDALONE_ID R1=R2+R1 ;unexpected VAR
VIT_TIR (1750)
STANDALONE_ID SE_FUNCTION(500) ; unexpected ID
;CYCLE832(0.05,_ROUGH,1) ;standalone function / function with side effect


;CALL "./data/nc_code/CONST.SPF" ;For _ROUGH

R1=-300 ; x correspondend
R2=-200 ; y correspondend
R3=0 ; z correspondend



;SUPA CYCLE832(0.05,_ROUGH,1) ;the standalone ID SUPA has its own Token

MSG "End of File, reached!"
