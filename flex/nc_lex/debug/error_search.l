flex -l test.l
bison -dv grammar.y
grammar.y: warning: 2 nonterminals useless in grammar [-Wother]
grammar.y: warning: 10 rules useless in grammar [-Wother]
grammar.y:117.1-6: warning: nonterminal useless in grammar: params [-Wother]
  117 | params:
      | ^~~~~~
grammar.y:134.1-9: warning: nonterminal useless in grammar: bool_expr [-Wother]
  134 | bool_expr:
      | ^~~~~~~~~
gcc -o test main.c
tilo@tilo-Laptop:~/work/nc_geometry/flex/nc_lex$ ./test < 100_DXY_LA 
{"laser":"0.000000","line":"1.000000","laser_power":"0.000000",},
A comment: ; created by hyperMILL 2025, Datum: 21.8.2025, Laufzeit 0:16:52
Set line to 2.000000
{"laser":"0.000000","laser_power":"0.000000","line":"2.000000",},

A comment: ;Programmlaufzeit Start
Set line to 3.000000
{"laser":"0.000000","laser_power":"0.000000","line":"3.000000",},

A string: "//LOCAL_DRIVE:/EffiDED/PROGRAMMLAUFZEIT/ZEIT_START.MPF"
Set line to 4.000000
{"laser":"0.000000","laser_power":"0.000000","line":"4.000000",},

A comment: ; Program begin
Set line to 5.000000
{"laser":"0.000000","laser_power":"0.000000","line":"5.000000",},

A command: G
An integer: 90
INT=90
Set G to 90.000000
A command: G
An integer: 17
INT=17
Set G to 17.000000
A command: G
An integer: 40
INT=40
Set G to 40.000000
Set line to 6.000000
{"laser":"0.000000","laser_power":"0.000000","line":"6.000000","G":"40.000000",},

Set line to 7.000000
{"laser":"0.000000","laser_power":"0.000000","line":"7.000000","G":"40.000000",},

Set line to 8.000000
{"laser":"0.000000","laser_power":"0.000000","line":"8.000000","G":"40.000000",},

A comment: ; Safe pos. program begin
Set line to 9.000000
{"laser":"0.000000","laser_power":"0.000000","line":"9.000000","G":"40.000000",},

A command: G
An integer: 0
INT=0
Set G to 0.000000
A command: D
An integer: 0
INT=0
Set D to 0.000000
A command: Z
A float: 459.000000
FLOAT=459.000000
Set Z to 459.000000
Set line to 10.000000
{"laser":"0.000000","laser_power":"0.000000","line":"10.000000","G":"0.000000","Z":"459.000000","D":"0.000000",},

A command: G
An integer: 0
INT=0
Set G to 0.000000
A command: D
An integer: 0
INT=0
Set D to 0.000000
A command: X
A float: 69.000000
FLOAT=69.000000
Set X to 69.000000
A command: Y
A float: 249.000000
FLOAT=249.000000
Set Y to 249.000000
Set line to 11.000000
{"X":"69.000000","Y":"249.000000","laser":"0.000000","laser_power":"0.000000","line":"11.000000","G":"0.000000","Z":"459.000000","D":"0.000000",},

A command: G
An integer: 0
INT=0
Set G to 0.000000
A command: B
A float: 0.000000
FLOAT=0.000000
Set B to 0.000000
A command: C
SET
A command: D
Error: syntax error, in line: 11
Set line to 12.000000
{"X":"69.000000","Y":"249.000000","laser":"0.000000","laser_power":"0.000000","line":"12.000000","G":"0.000000","Z":"459.000000","B":"0.000000","D":"0.000000",},

A command: C
Set line to 13.000000
{"X":"69.000000","Y":"249.000000","laser":"0.000000","laser_power":"0.000000","line":"13.000000","G":"0.000000","Z":"459.000000","B":"0.000000","D":"0.000000",},

Set line to 14.000000
{"X":"69.000000","Y":"249.000000","laser":"0.000000","laser_power":"0.000000","line":"14.000000","G":"0.000000","Z":"459.000000","B":"0.000000","D":"0.000000",},

A float: 0.000000
Set line to 15.000000
{"X":"69.000000","Y":"249.000000","laser":"0.000000","laser_power":"0.000000","line":"15.000000","G":"0.000000","Z":"459.000000","B":"0.000000","D":"0.000000",},

Set line to 16.000000
{"X":"69.000000","Y":"249.000000","laser":"0.000000","laser_power":"0.000000","line":"16.000000","G":"0.000000","Z":"459.000000","B":"0.000000","D":"0.000000",},

Set line to 17.000000
{"X":"69.000000","Y":"249.000000","laser":"0.000000","laser_power":"0.000000","line":"17.000000","G":"0.000000","Z":"459.000000","B":"0.000000","D":"0.000000",},

A comment: ; ---
Set line to 18.000000
{"X":"69.000000","Y":"249.000000","laser":"0.000000","laser_power":"0.000000","line":"18.000000","G":"0.000000","Z":"459.000000","B":"0.000000","D":"0.000000",},

A command: G
An integer: 54
INT=54
Set G to 54.000000
Set line to 19.000000
{"X":"69.000000","Y":"249.000000","laser":"0.000000","laser_power":"0.000000","line":"19.000000","G":"54.000000","Z":"459.000000","B":"0.000000","D":"0.000000",},

A comment: ; TOOL 1: 10Vx_Gravur
Set line to 20.000000
{"X":"69.000000","Y":"249.000000","laser":"0.000000","laser_power":"0.000000","line":"20.000000","G":"54.000000","Z":"459.000000","B":"0.000000","D":"0.000000",},

A comment: ; Safe pos. tool change begin
Set line to 21.000000
{"X":"69.000000","Y":"249.000000","laser":"0.000000","laser_power":"0.000000","line":"21.000000","G":"54.000000","Z":"459.000000","B":"0.000000","D":"0.000000",},

A command: G
An integer: 0
INT=0
Set G to 0.000000
A command: D
An integer: 0
INT=0
Set D to 0.000000
A command: Z
A float: 459.000000
FLOAT=459.000000
Set Z to 459.000000
Set line to 22.000000
{"X":"69.000000","Y":"249.000000","laser":"0.000000","laser_power":"0.000000","line":"22.000000","G":"0.000000","Z":"459.000000","B":"0.000000","D":"0.000000",},

A comment: ; ---
Set line to 23.000000
{"X":"69.000000","Y":"249.000000","laser":"0.000000","laser_power":"0.000000","line":"23.000000","G":"0.000000","Z":"459.000000","B":"0.000000","D":"0.000000",},

A comment: ; Before tool change
Set line to 24.000000
{"X":"69.000000","Y":"249.000000","laser":"0.000000","laser_power":"0.000000","line":"24.000000","G":"0.000000","Z":"459.000000","B":"0.000000","D":"0.000000",},

A command: T
An integer: 1
INT=1
Set T to 1.000000
Set line to 25.000000
{"X":"69.000000","Y":"249.000000","laser":"0.000000","laser_power":"0.000000","T":"1.000000","line":"25.000000","G":"0.000000","Z":"459.000000","B":"0.000000","D":"0.000000",},

A comment: ; After tool change
Set line to 26.000000
{"X":"69.000000","Y":"249.000000","laser":"0.000000","laser_power":"0.000000","T":"1.000000","line":"26.000000","G":"0.000000","Z":"459.000000","B":"0.000000","D":"0.000000",},

A comment: ; Safe pos. tool change end
Set line to 27.000000
{"X":"69.000000","Y":"249.000000","laser":"0.000000","laser_power":"0.000000","T":"1.000000","line":"27.000000","G":"0.000000","Z":"459.000000","B":"0.000000","D":"0.000000",},

A command: G
An integer: 0
INT=0
Set G to 0.000000
A command: D
An integer: 0
INT=0
Set D to 0.000000
A command: Z
A float: 459.000000
FLOAT=459.000000
Set Z to 459.000000
Set line to 28.000000
{"X":"69.000000","Y":"249.000000","laser":"0.000000","laser_power":"0.000000","T":"1.000000","line":"28.000000","G":"0.000000","Z":"459.000000","B":"0.000000","D":"0.000000",},

A comment: ; ---
Set line to 29.000000
{"X":"69.000000","Y":"249.000000","laser":"0.000000","laser_power":"0.000000","T":"1.000000","line":"29.000000","G":"0.000000","Z":"459.000000","B":"0.000000","D":"0.000000",},

A comment: ; OPERATION Beschrifftung_316L_Laserleistung
Set line to 30.000000
{"X":"69.000000","Y":"249.000000","laser":"0.000000","laser_power":"0.000000","T":"1.000000","line":"30.000000","G":"0.000000","Z":"459.000000","B":"0.000000","D":"0.000000",},

A command: D
An integer: 1
INT=1
Set D to 1.000000
Set line to 31.000000
{"X":"69.000000","Y":"249.000000","laser":"0.000000","laser_power":"0.000000","T":"1.000000","line":"31.000000","G":"0.000000","Z":"459.000000","B":"0.000000","D":"1.000000",},

A custom Variable: VIT_TIR
SET
A float: 1750.000000
FLOAT=1750.000000
Set VIT_TIR to 1750.000000
A comment: ; Vorschub XY
Set line to 32.000000
{"X":"69.000000","Y":"249.000000","laser":"0.000000","VIT_TIR":"1750.000000","laser_power":"0.000000","T":"1.000000","line":"32.000000","G":"0.000000","Z":"459.000000","B":"0.000000","D":"1.000000",},

A comment: ; ---- start approach movement ----
Set line to 33.000000
{"X":"69.000000","Y":"249.000000","laser":"0.000000","VIT_TIR":"1750.000000","laser_power":"0.000000","T":"1.000000","line":"33.000000","G":"0.000000","Z":"459.000000","B":"0.000000","D":"1.000000",},

A command: G
An integer: 0
INT=0
Set G to 0.000000
A command: B
A float: 0.000000
FLOAT=0.000000
Set B to 0.000000
A command: C
SET
A command: D
Error: syntax error, in line: 33
Set line to 34.000000
{"X":"69.000000","Y":"249.000000","laser":"0.000000","VIT_TIR":"1750.000000","laser_power":"0.000000","T":"1.000000","line":"34.000000","G":"0.000000","Z":"459.000000","B":"0.000000","D":"1.000000",},

A command: C
Set line to 35.000000
{"X":"69.000000","Y":"249.000000","laser":"0.000000","VIT_TIR":"1750.000000","laser_power":"0.000000","T":"1.000000","line":"35.000000","G":"0.000000","Z":"459.000000","B":"0.000000","D":"1.000000",},

Set line to 36.000000
{"X":"69.000000","Y":"249.000000","laser":"0.000000","VIT_TIR":"1750.000000","laser_power":"0.000000","T":"1.000000","line":"36.000000","G":"0.000000","Z":"459.000000","B":"0.000000","D":"1.000000",},

A float: 0.000000
Set line to 37.000000
{"X":"69.000000","Y":"249.000000","laser":"0.000000","VIT_TIR":"1750.000000","laser_power":"0.000000","T":"1.000000","line":"37.000000","G":"0.000000","Z":"459.000000","B":"0.000000","D":"1.000000",},

Set line to 38.000000
{"X":"69.000000","Y":"249.000000","laser":"0.000000","VIT_TIR":"1750.000000","laser_power":"0.000000","T":"1.000000","line":"38.000000","G":"0.000000","Z":"459.000000","B":"0.000000","D":"1.000000",},

Set line to 39.000000
{"X":"69.000000","Y":"249.000000","laser":"0.000000","VIT_TIR":"1750.000000","laser_power":"0.000000","T":"1.000000","line":"39.000000","G":"0.000000","Z":"459.000000","B":"0.000000","D":"1.000000",},

An integer: 1
INT=1
Set line to 40.000000
{"X":"69.000000","Y":"249.000000","laser":"0.000000","VIT_TIR":"1750.000000","laser_power":"0.000000","T":"1.000000","line":"40.000000","G":"0.000000","Z":"459.000000","B":"0.000000","D":"1.000000",},

A command: G
An integer: 54
INT=54
Set G to 54.000000
Set line to 41.000000
{"X":"69.000000","Y":"249.000000","laser":"0.000000","VIT_TIR":"1750.000000","laser_power":"0.000000","T":"1.000000","line":"41.000000","G":"54.000000","Z":"459.000000","B":"0.000000","D":"1.000000",},

A command: G
An integer: 0
INT=0
Set G to 0.000000
A command: X
An arithmetic operator: -
A float: 20.420000
FLOAT=20.420000
Set X to -20.420000
A command: Y
An arithmetic operator: -
A float: 27.597000
FLOAT=27.597000
Set Y to -27.597000
A command: Z
A float: 363.561005
FLOAT=363.561005
Set Z to 363.561005
Set line to 42.000000
{"X":"-20.420000","Y":"-27.597000","laser":"0.000000","VIT_TIR":"1750.000000","laser_power":"0.000000","T":"1.000000","line":"42.000000","G":"0.000000","Z":"363.561005","B":"0.000000","D":"1.000000",},

A command: G
An integer: 0
INT=0
Set G to 0.000000
A command: Z
A float: 50.000000
FLOAT=50.000000
Set Z to 50.000000
Set line to 43.000000
{"X":"-20.420000","Y":"-27.597000","laser":"0.000000","VIT_TIR":"1750.000000","laser_power":"0.000000","T":"1.000000","line":"43.000000","G":"0.000000","Z":"50.000000","B":"0.000000","D":"1.000000",},

A comment: ; ---- end approach movement ----
Set line to 44.000000
{"X":"-20.420000","Y":"-27.597000","laser":"0.000000","VIT_TIR":"1750.000000","laser_power":"0.000000","T":"1.000000","line":"44.000000","G":"0.000000","Z":"50.000000","B":"0.000000","D":"1.000000",},

A comment: ; 5X Job begin
Set line to 45.000000
{"X":"-20.420000","Y":"-27.597000","laser":"0.000000","VIT_TIR":"1750.000000","laser_power":"0.000000","T":"1.000000","line":"45.000000","G":"0.000000","Z":"50.000000","B":"0.000000","D":"1.000000",},

A float: 0.050000
FLOAT=0.050000
Error: syntax error, in line: 45
Set line to 46.000000
{"X":"-20.420000","Y":"-27.597000","laser":"0.000000","VIT_TIR":"1750.000000","laser_power":"0.000000","T":"1.000000","line":"46.000000","G":"0.000000","Z":"50.000000","B":"0.000000","D":"1.000000",},

Set line to 47.000000
{"X":"-20.420000","Y":"-27.597000","laser":"0.000000","VIT_TIR":"1750.000000","laser_power":"0.000000","T":"1.000000","line":"47.000000","G":"0.000000","Z":"50.000000","B":"0.000000","D":"1.000000",},

Set line to 48.000000
{"X":"-20.420000","Y":"-27.597000","laser":"0.000000","VIT_TIR":"1750.000000","laser_power":"0.000000","T":"1.000000","line":"48.000000","G":"0.000000","Z":"50.000000","B":"0.000000","D":"1.000000",},

An integer: 1
Set line to 49.000000
{"X":"-20.420000","Y":"-27.597000","laser":"0.000000","VIT_TIR":"1750.000000","laser_power":"0.000000","T":"1.000000","line":"49.000000","G":"0.000000","Z":"50.000000","B":"0.000000","D":"1.000000",},

Set line to 50.000000
{"X":"-20.420000","Y":"-27.597000","laser":"0.000000","VIT_TIR":"1750.000000","laser_power":"0.000000","T":"1.000000","line":"50.000000","G":"0.000000","Z":"50.000000","B":"0.000000","D":"1.000000",},

Set line to 51.000000
{"X":"-20.420000","Y":"-27.597000","laser":"0.000000","VIT_TIR":"1750.000000","laser_power":"0.000000","T":"1.000000","line":"51.000000","G":"0.000000","Z":"50.000000","B":"0.000000","D":"1.000000",},

Set line to 52.000000
{"X":"-20.420000","Y":"-27.597000","laser":"0.000000","VIT_TIR":"1750.000000","laser_power":"0.000000","T":"1.000000","line":"52.000000","G":"0.000000","Z":"50.000000","B":"0.000000","D":"1.000000",},

A comment: ; Technology peripheral
Set line to 53.000000
{"X":"-20.420000","Y":"-27.597000","laser":"0.000000","VIT_TIR":"1750.000000","laser_power":"0.000000","T":"1.000000","line":"53.000000","G":"0.000000","Z":"50.000000","B":"0.000000","D":"1.000000",},

A command: G
An integer: 645
INT=645
Set G to 645.000000
Set line to 54.000000
{"X":"-20.420000","Y":"-27.597000","laser":"0.000000","VIT_TIR":"1750.000000","laser_power":"0.000000","T":"1.000000","line":"54.000000","G":"645.000000","Z":"50.000000","B":"0.000000","D":"1.000000",},

A custom Variable: PUIS_LASER
An integer: 250
INT=250
Set laser_power to 250.000000
Set line to 55.000000
{"X":"-20.420000","Y":"-27.597000","laser":"0.000000","VIT_TIR":"1750.000000","laser_power":"250.000000","T":"1.000000","line":"55.000000","G":"645.000000","Z":"50.000000","B":"0.000000","D":"1.000000",},

An integer: 1
Error: syntax error, in line: 55
Set line to 56.000000
{"X":"-20.420000","Y":"-27.597000","laser":"0.000000","VIT_TIR":"1750.000000","laser_power":"250.000000","T":"1.000000","line":"56.000000","G":"645.000000","Z":"50.000000","B":"0.000000","D":"1.000000",},

Set line to 57.000000
{"X":"-20.420000","Y":"-27.597000","laser":"0.000000","VIT_TIR":"1750.000000","laser_power":"250.000000","T":"1.000000","line":"57.000000","G":"645.000000","Z":"50.000000","B":"0.000000","D":"1.000000",},

Set line to 58.000000
{"X":"-20.420000","Y":"-27.597000","laser":"0.000000","VIT_TIR":"1750.000000","laser_power":"250.000000","T":"1.000000","line":"58.000000","G":"645.000000","Z":"50.000000","B":"0.000000","D":"1.000000",},

A command: H
An integer: 61
INT=61
SET
Set H to 61.000000
Set line to 59.000000
{"X":"-20.420000","VIT_TIR":"1750.000000","Y":"-27.597000","laser":"0.000000","H":"61.000000","laser_power":"250.000000","T":"1.000000","Z":"50.000000","line":"59.000000","G":"645.000000","B":"0.000000","D":"1.000000",},

Error: syntax error, in line: 59
Set line to 60.000000
{"X":"-20.420000","VIT_TIR":"1750.000000","Y":"-27.597000","laser":"0.000000","H":"61.000000","laser_power":"250.000000","T":"1.000000","Z":"50.000000","line":"60.000000","G":"645.000000","B":"0.000000","D":"1.000000",},

An integer: 3
Set line to 61.000000
{"X":"-20.420000","VIT_TIR":"1750.000000","Y":"-27.597000","laser":"0.000000","H":"61.000000","laser_power":"250.000000","T":"1.000000","Z":"50.000000","line":"61.000000","G":"645.000000","B":"0.000000","D":"1.000000",},

Set line to 62.000000
{"X":"-20.420000","VIT_TIR":"1750.000000","Y":"-27.597000","laser":"0.000000","H":"61.000000","laser_power":"250.000000","T":"1.000000","Z":"50.000000","line":"62.000000","G":"645.000000","B":"0.000000","D":"1.000000",},

A command: H
An integer: 62
INT=62
SET
Set H to 62.000000
Set line to 63.000000
{"X":"-20.420000","VIT_TIR":"1750.000000","Y":"-27.597000","laser":"0.000000","H":"62.000000","laser_power":"250.000000","T":"1.000000","Z":"50.000000","line":"63.000000","G":"645.000000","B":"0.000000","D":"1.000000",},

Error: syntax error, in line: 63
Set line to 64.000000
{"X":"-20.420000","VIT_TIR":"1750.000000","Y":"-27.597000","laser":"0.000000","H":"62.000000","laser_power":"250.000000","T":"1.000000","Z":"50.000000","line":"64.000000","G":"645.000000","B":"0.000000","D":"1.000000",},

An integer: 6
Set line to 65.000000
{"X":"-20.420000","VIT_TIR":"1750.000000","Y":"-27.597000","laser":"0.000000","H":"62.000000","laser_power":"250.000000","T":"1.000000","Z":"50.000000","line":"65.000000","G":"645.000000","B":"0.000000","D":"1.000000",},

Set line to 66.000000
{"X":"-20.420000","VIT_TIR":"1750.000000","Y":"-27.597000","laser":"0.000000","H":"62.000000","laser_power":"250.000000","T":"1.000000","Z":"50.000000","line":"66.000000","G":"645.000000","B":"0.000000","D":"1.000000",},

A special command: /M180
Set line to 67.000000
{"X":"-20.420000","VIT_TIR":"1750.000000","Y":"-27.597000","laser":"0.000000","H":"62.000000","laser_power":"250.000000","T":"1.000000","Z":"50.000000","line":"67.000000","G":"645.000000","B":"0.000000","D":"1.000000",},

A special command: /M182
Set line to 68.000000
{"X":"-20.420000","VIT_TIR":"1750.000000","Y":"-27.597000","laser":"0.000000","H":"62.000000","laser_power":"250.000000","T":"1.000000","Z":"50.000000","line":"68.000000","G":"645.000000","B":"0.000000","D":"1.000000",},

A special command: /G4
A command: F
An integer: 10
INT=10
Set F to 10.000000
Set line to 69.000000
{"X":"-20.420000","VIT_TIR":"1750.000000","Y":"-27.597000","laser":"0.000000","H":"62.000000","F":"10.000000","laser_power":"250.000000","T":"1.000000","Z":"50.000000","line":"69.000000","G":"645.000000","B":"0.000000","D":"1.000000",},

A comment: ; ---
Set line to 70.000000
{"X":"-20.420000","VIT_TIR":"1750.000000","Y":"-27.597000","laser":"0.000000","H":"62.000000","F":"10.000000","laser_power":"250.000000","T":"1.000000","Z":"50.000000","line":"70.000000","G":"645.000000","B":"0.000000","D":"1.000000",},

A comment: ; Layer: 1
Set line to 71.000000
{"X":"-20.420000","VIT_TIR":"1750.000000","Y":"-27.597000","laser":"0.000000","H":"62.000000","F":"10.000000","laser_power":"250.000000","T":"1.000000","Z":"50.000000","line":"71.000000","G":"645.000000","B":"0.000000","D":"1.000000",},

A comment: ; Pocket: 1
Set line to 72.000000
{"X":"-20.420000","VIT_TIR":"1750.000000","Y":"-27.597000","laser":"0.000000","H":"62.000000","F":"10.000000","laser_power":"250.000000","T":"1.000000","Z":"50.000000","line":"72.000000","G":"645.000000","B":"0.000000","D":"1.000000",},

A command: G
An integer: 0
INT=0
Set G to 0.000000
A command: Z
A float: 0.000000
FLOAT=0.000000
Set Z to 0.000000
Set line to 73.000000
{"X":"-20.420000","VIT_TIR":"1750.000000","Y":"-27.597000","laser":"0.000000","H":"62.000000","F":"10.000000","laser_power":"250.000000","T":"1.000000","Z":"0.000000","line":"73.000000","G":"0.000000","B":"0.000000","D":"1.000000",},

A comment: ; Laser ON
Set line to 74.000000
{"X":"-20.420000","VIT_TIR":"1750.000000","Y":"-27.597000","laser":"0.000000","H":"62.000000","F":"10.000000","laser_power":"250.000000","T":"1.000000","Z":"0.000000","line":"74.000000","G":"0.000000","B":"0.000000","D":"1.000000",},

A special command: /LASER_ON
Set laser to 1.000000
Set line to 75.000000
{"X":"-20.420000","VIT_TIR":"1750.000000","Y":"-27.597000","laser":"1.000000","H":"62.000000","F":"10.000000","laser_power":"250.000000","T":"1.000000","Z":"0.000000","line":"75.000000","G":"0.000000","B":"0.000000","D":"1.000000",},

A command: G
An integer: 1
INT=1
Set G to 1.000000
A command: X
An arithmetic operator: -
A float: 20.150000
FLOAT=20.150000
Set X to -20.150000
A command: Y
An arithmetic operator: -
A float: 27.246000
FLOAT=27.246000
Set Y to -27.246000
A command: F
SET
A custom Variable: VIT_TIR
Error: syntax error, in line: 75
Set line to 76.000000
{"X":"-20.150000","VIT_TIR":"1750.000000","Y":"-27.246000","laser":"1.000000","H":"62.000000","F":"10.000000","laser_power":"250.000000","T":"1.000000","Z":"0.000000","line":"76.000000","G":"1.000000","B":"0.000000","D":"1.000000",},

Set line to 77.000000
{"X":"-20.150000","VIT_TIR":"1750.000000","Y":"-27.246000","laser":"1.000000","H":"62.000000","F":"10.000000","laser_power":"250.000000","T":"1.000000","Z":"0.000000","line":"77.000000","G":"1.000000","B":"0.000000","D":"1.000000",},

A command: G
An integer: 1
INT=1
Set G to 1.000000
A command: X
An arithmetic operator: -
A float: 20.000000
FLOAT=20.000000
Set X to -20.000000
A command: Y
An arithmetic operator: -
A float: 26.691000
FLOAT=26.691000
Set Y to -26.691000
Set line to 78.000000
{"X":"-20.000000","VIT_TIR":"1750.000000","Y":"-26.691000","laser":"1.000000","H":"62.000000","F":"10.000000","laser_power":"250.000000","T":"1.000000","Z":"0.000000","line":"78.000000","G":"1.000000","B":"0.000000","D":"1.000000",},

A command: G
An integer: 1
INT=1
Set G to 1.000000
A command: Y
An arithmetic operator: -
A float: 25.986000
FLOAT=25.986000
Set Y to -25.986000
Set line to 79.000000
{"X":"-20.000000","VIT_TIR":"1750.000000","Y":"-25.986000","laser":"1.000000","H":"62.000000","F":"10.000000","laser_power":"250.000000","T":"1.000000","Z":"0.000000","line":"79.000000","G":"1.000000","B":"0.000000","D":"1.000000",},

A command: G
An integer: 1
INT=1
Set G to 1.000000
A command: X
An arithmetic operator: -
A float: 20.150000
FLOAT=20.150000
Set X to -20.150000
A command: Y
An arithmetic operator: -
A float: 25.431000
FLOAT=25.431000
Set Y to -25.431000
Set line to 80.000000
{"X":"-20.150000","VIT_TIR":"1750.000000","Y":"-25.431000","laser":"1.000000","H":"62.000000","F":"10.000000","laser_power":"250.000000","T":"1.000000","Z":"0.000000","line":"80.000000","G":"1.000000","B":"0.000000","D":"1.000000",},

A command: G
An integer: 1
INT=1
Set G to 1.000000
A command: X
An arithmetic operator: -
A float: 20.420000
FLOAT=20.420000
Set X to -20.420000
A command: Y
An arithmetic operator: -
A float: 25.077000
FLOAT=25.077000
Set Y to -25.077000
Set line to 81.000000
{"X":"-20.420000","VIT_TIR":"1750.000000","Y":"-25.077000","laser":"1.000000","H":"62.000000","F":"10.000000","laser_power":"250.000000","T":"1.000000","Z":"0.000000","line":"81.000000","G":"1.000000","B":"0.000000","D":"1.000000",},

A command: G
An integer: 1
INT=1
Set G to 1.000000
A command: X
An arithmetic operator: -
A float: 20.870001
FLOAT=20.870001
Set X to -20.870001
Set line to 82.000000
{"X":"-20.870001","VIT_TIR":"1750.000000","Y":"-25.077000","laser":"1.000000","H":"62.000000","F":"10.000000","laser_power":"250.000000","T":"1.000000","Z":"0.000000","line":"82.000000","G":"1.000000","B":"0.000000","D":"1.000000",},

A command: G
An integer: 1
INT=1
Set G to 1.000000
A command: X
An arithmetic operator: -
A float: 21.139999
FLOAT=21.139999
Set X to -21.139999
A command: Y
An arithmetic operator: -
A float: 25.254000
FLOAT=25.254000
Set Y to -25.254000
Set line to 83.000000
{"X":"-21.139999","VIT_TIR":"1750.000000","Y":"-25.254000","laser":"1.000000","H":"62.000000","F":"10.000000","laser_power":"250.000000","T":"1.000000","Z":"0.000000","line":"83.000000","G":"1.000000","B":"0.000000","D":"1.000000",},

A command: G
An integer: 1
INT=1
Set G to 1.000000
A command: X
An arithmetic operator: -
A float: 21.290001
FLOAT=21.290001
Set X to -21.290001
A command: Y
An arithmetic operator: -
A float: 25.431000
FLOAT=25.431000
Set Y to -25.431000
Set line to 84.000000
{"X":"-21.290001","VIT_TIR":"1750.000000","Y":"-25.431000","laser":"1.000000","H":"62.000000","F":"10.000000","laser_power":"250.000000","T":"1.000000","Z":"0.000000","line":"84.000000","G":"1.000000","B":"0.000000","D":"1.000000",},

A command: G
An integer: 1
INT=1
Set G to 1.000000
A command: X
An arithmetic operator: -
A float: 21.440001
FLOAT=21.440001
Set X to -21.440001
A command: Y
An arithmetic operator: -
A float: 25.809000
FLOAT=25.809000
Set Y to -25.809000
Set line to 85.000000
{"X":"-21.440001","VIT_TIR":"1750.000000","Y":"-25.809000","laser":"1.000000","H":"62.000000","F":"10.000000","laser_power":"250.000000","T":"1.000000","Z":"0.000000","line":"85.000000","G":"1.000000","B":"0.000000","D":"1.000000",},

A command: G
An integer: 1
INT=1
Set G to 1.000000
A command: X
An arithmetic operator: -
A float: 21.575001
FLOAT=21.575001
Set X to -21.575001
A command: Y
An arithmetic operator: -
A float: 26.338499
FLOAT=26.338499
Set Y to -26.338499
Set line to 86.000000
{"X":"-21.575001","VIT_TIR":"1750.000000","Y":"-26.338499","laser":"1.000000","H":"62.000000","F":"10.000000","laser_power":"250.000000","T":"1.000000","Z":"0.000000","line":"86.000000","G":"1.000000","B":"0.000000","D":"1.000000",},

A command: G
An integer: 1
INT=1
Set G to 1.000000
A command: X
An arithmetic operator: -
A float: 21.709999
FLOAT=21.709999
Set X to -21.709999
A command: Y
An arithmetic operator: -
A float: 26.868000
FLOAT=26.868000
Set Y to -26.868000
Set line to 87.000000
{"X":"-21.709999","VIT_TIR":"1750.000000","Y":"-26.868000","laser":"1.000000","H":"62.000000","F":"10.000000","laser_power":"250.000000","T":"1.000000","Z":"0.000000","line":"87.000000","G":"1.000000","B":"0.000000","D":"1.000000",},

A command: G
An integer: 1
INT=1
Set G to 1.000000
A command: X
An arithmetic operator: -
A float: 21.860001
FLOAT=21.860001
Set X to -21.860001
A command: Y
An arithmetic operator: -
A float: 27.246000
FLOAT=27.246000
Set Y to -27.246000
Set line to 88.000000
{"X":"-21.860001","VIT_TIR":"1750.000000","Y":"-27.246000","laser":"1.000000","H":"62.000000","F":"10.000000","laser_power":"250.000000","T":"1.000000","Z":"0.000000","line":"88.000000","G":"1.000000","B":"0.000000","D":"1.000000",},

A command: G
An integer: 1
INT=1
Set G to 1.000000
A command: X
An arithmetic operator: -
A float: 22.010000
FLOAT=22.010000
Set X to -22.010000
A command: Y
An arithmetic operator: -
A float: 27.423000
FLOAT=27.423000
Set Y to -27.423000
Set line to 89.000000
{"X":"-22.010000","VIT_TIR":"1750.000000","Y":"-27.423000","laser":"1.000000","H":"62.000000","F":"10.000000","laser_power":"250.000000","T":"1.000000","Z":"0.000000","line":"89.000000","G":"1.000000","B":"0.000000","D":"1.000000",},

A command: G
An integer: 1
INT=1
Set G to 1.000000
A command: X
An arithmetic operator: -
A float: 22.280800
FLOAT=22.280800
Set X to -22.280800
A command: Y
An arithmetic operator: -
A float: 27.597000
FLOAT=27.597000
Set Y to -27.597000
Set line to 90.000000
{"X":"-22.280800","VIT_TIR":"1750.000000","Y":"-27.597000","laser":"1.000000","H":"62.000000","F":"10.000000","laser_power":"250.000000","T":"1.000000","Z":"0.000000","line":"90.000000","G":"1.000000","B":"0.000000","D":"1.000000",},

A command: G
An integer: 1
INT=1
Set G to 1.000000
A command: X
An arithmetic operator: -
A float: 22.580000
FLOAT=22.580000
Set X to -22.580000
Set line to 91.000000
{"X":"-22.580000","VIT_TIR":"1750.000000","Y":"-27.597000","laser":"1.000000","H":"62.000000","F":"10.000000","laser_power":"250.000000","T":"1.000000","Z":"0.000000","line":"91.000000","G":"1.000000","B":"0.000000","D":"1.000000",},

A command: G
An integer: 1
INT=1
Set G to 1.000000
A command: X
An arithmetic operator: -
A float: 22.850000
FLOAT=22.850000
Set X to -22.850000
A command: Y
An arithmetic operator: -
A float: 27.246000
FLOAT=27.246000
Set Y to -27.246000
Set line to 92.000000
{"X":"-22.850000","VIT_TIR":"1750.000000","Y":"-27.246000","laser":"1.000000","H":"62.000000","F":"10.000000","laser_power":"250.000000","T":"1.000000","Z":"0.000000","line":"92.000000","G":"1.000000","B":"0.000000","D":"1.000000",},

A command: G
An integer: 1
INT=1
Set G to 1.000000
A command: X
An arithmetic operator: -
A float: 23.000000
FLOAT=23.000000
Set X to -23.000000
A command: Y
An arithmetic operator: -
A float: 26.691000
FLOAT=26.691000
Set Y to -26.691000
Set line to 93.000000
{"X":"-23.000000","VIT_TIR":"1750.000000","Y":"-26.691000","laser":"1.000000","H":"62.000000","F":"10.000000","laser_power":"250.000000","T":"1.000000","Z":"0.000000","line":"93.000000","G":"1.000000","B":"0.000000","D":"1.000000",},

A command: G
An integer: 1
INT=1
Set G to 1.000000
A command: Y
An arithmetic operator: -
A float: 25.986000
FLOAT=25.986000
Set Y to -25.986000
Set line to 94.000000
{"X":"-23.000000","VIT_TIR":"1750.000000","Y":"-25.986000","laser":"1.000000","H":"62.000000","F":"10.000000","laser_power":"250.000000","T":"1.000000","Z":"0.000000","line":"94.000000","G":"1.000000","B":"0.000000","D":"1.000000",},

A command: G
An integer: 1
INT=1
Set G to 1.000000
A command: X
An arithmetic operator: -
A float: 22.850800
FLOAT=22.850800
Set X to -22.850800
A command: Y
An arithmetic operator: -
A float: 25.434000
FLOAT=25.434000
Set Y to -25.434000
Set line to 95.000000
{"X":"-22.850800","VIT_TIR":"1750.000000","Y":"-25.434000","laser":"1.000000","H":"62.000000","F":"10.000000","laser_power":"250.000000","T":"1.000000","Z":"0.000000","line":"95.000000","G":"1.000000","B":"0.000000","D":"1.000000",},

A command: G
An integer: 1
INT=1
Set G to 1.000000
A command: X
An arithmetic operator: -
A float: 22.580000
FLOAT=22.580000
Set X to -22.580000
A command: Y
An arithmetic operator: -
A float: 25.077000
FLOAT=25.077000
Set Y to -25.077000
Set line to 96.000000
{"X":"-22.580000","VIT_TIR":"1750.000000","Y":"-25.077000","laser":"1.000000","H":"62.000000","F":"10.000000","laser_power":"250.000000","T":"1.000000","Z":"0.000000","line":"96.000000","G":"1.000000","B":"0.000000","D":"1.000000",},

A comment: ; Laser OFF
Set line to 97.000000
{"X":"-22.580000","VIT_TIR":"1750.000000","Y":"-25.077000","laser":"1.000000","H":"62.000000","F":"10.000000","laser_power":"250.000000","T":"1.000000","Z":"0.000000","line":"97.000000","G":"1.000000","B":"0.000000","D":"1.000000",},

Set line to 98.000000
{"X":"-22.580000","VIT_TIR":"1750.000000","Y":"-25.077000","laser":"1.000000","H":"62.000000","F":"10.000000","laser_power":"250.000000","T":"1.000000","Z":"0.000000","line":"98.000000","G":"1.000000","B":"0.000000","D":"1.000000",},

Set line to 99.000000
{"X":"-22.580000","VIT_TIR":"1750.000000","Y":"-25.077000","laser":"1.000000","H":"62.000000","F":"10.000000","laser_power":"250.000000","T":"1.000000","Z":"0.000000","line":"99.000000","G":"1.000000","B":"0.000000","D":"1.000000",},

A comment: ; Layer: 1
Set line to 100.000000
{"X":"-22.580000","VIT_TIR":"1750.000000","Y":"-25.077000","laser":"1.000000","H":"62.000000","F":"10.000000","laser_power":"250.000000","T":"1.000000","Z":"0.000000","line":"100.000000","G":"1.000000","B":"0.000000","D":"1.000000",},

A comment: ; Pocket: 2
Set line to 101.000000
{"X":"-22.580000","VIT_TIR":"1750.000000","Y":"-25.077000","laser":"1.000000","H":"62.000000","F":"10.000000","laser_power":"250.000000","T":"1.000000","Z":"0.000000","line":"101.000000","G":"1.000000","B":"0.000000","D":"1.000000",},

A command: G
An integer: 0
INT=0
Set G to 0.000000
A command: X
An arithmetic operator: -
A float: 19.040001
FLOAT=19.040001
Set X to -19.040001
A command: Y
An arithmetic operator: -
A float: 24.476999
FLOAT=24.476999
Set Y to -24.476999
Set line to 102.000000
{"X":"-19.040001","VIT_TIR":"1750.000000","Y":"-24.476999","laser":"1.000000","H":"62.000000","F":"10.000000","laser_power":"250.000000","T":"1.000000","Z":"0.000000","line":"102.000000","G":"0.000000","B":"0.000000","D":"1.000000",},

A comment: ; Laser ON
Set line to 103.000000
{"X":"-19.040001","VIT_TIR":"1750.000000","Y":"-24.476999","laser":"1.000000","H":"62.000000","F":"10.000000","laser_power":"250.000000","T":"1.000000","Z":"0.000000","line":"103.000000","G":"0.000000","B":"0.000000","D":"1.000000",},

A special command: /LASER_ON
Set laser to 1.000000
Set line to 104.000000
{"X":"-19.040001","VIT_TIR":"1750.000000","Y":"-24.476999","laser":"1.000000","H":"62.000000","F":"10.000000","laser_power":"250.000000","T":"1.000000","Z":"0.000000","line":"104.000000","G":"0.000000","B":"0.000000","D":"1.000000",},

A command: G
An integer: 1
INT=1
Set G to 1.000000
A command: X
An arithmetic operator: -
A float: 20.030001
FLOAT=20.030001
Set X to -20.030001
A command: F
SET
A custom Variable: VIT_TIR
Error: syntax error, in line: 104
Set line to 105.000000
{"X":"-20.030001","VIT_TIR":"1750.000000","Y":"-24.476999","laser":"1.000000","H":"62.000000","F":"10.000000","laser_power":"250.000000","T":"1.000000","Z":"0.000000","line":"105.000000","G":"1.000000","B":"0.000000","D":"1.000000",},

Set line to 106.000000
{"X":"-20.030001","VIT_TIR":"1750.000000","Y":"-24.476999","laser":"1.000000","H":"62.000000","F":"10.000000","laser_power":"250.000000","T":"1.000000","Z":"0.000000","line":"106.000000","G":"1.000000","B":"0.000000","D":"1.000000",},

A command: G
An integer: 1
INT=1
Set G to 1.000000
A command: X
An arithmetic operator: -
A float: 21.020000
FLOAT=21.020000
Set X to -21.020000
Set line to 107.000000
{"X":"-21.020000","VIT_TIR":"1750.000000","Y":"-24.476999","laser":"1.000000","H":"62.000000","F":"10.000000","laser_power":"250.000000","T":"1.000000","Z":"0.000000","line":"107.000000","G":"1.000000","B":"0.000000","D":"1.000000",},

A command: G
An integer: 1
INT=1
Set G to 1.000000
A command: X
An arithmetic operator: -
A float: 22.010000
FLOAT=22.010000
Set X to -22.010000
Set line to 108.000000
{"X":"-22.010000","VIT_TIR":"1750.000000","Y":"-24.476999","laser":"1.000000","H":"62.000000","F":"10.000000","laser_power":"250.000000","T":"1.000000","Z":"0.000000","line":"108.000000","G":"1.000000","B":"0.000000","D":"1.000000",},

A comment: ; Laser OFF
Set line to 109.000000
{"X":"-22.010000","VIT_TIR":"1750.000000","Y":"-24.476999","laser":"1.000000","H":"62.000000","F":"10.000000","laser_power":"250.000000","T":"1.000000","Z":"0.000000","line":"109.000000","G":"1.000000","B":"0.000000","D":"1.000000",},

Set line to 110.000000
{"X":"-22.010000","VIT_TIR":"1750.000000","Y":"-24.476999","laser":"1.000000","H":"62.000000","F":"10.000000","laser_power":"250.000000","T":"1.000000","Z":"0.000000","line":"110.000000","G":"1.000000","B":"0.000000","D":"1.000000",},

Set line to 111.000000
{"X":"-22.010000","VIT_TIR":"1750.000000","Y":"-24.476999","laser":"1.000000","H":"62.000000","F":"10.000000","laser_power":"250.000000","T":"1.000000","Z":"0.000000","line":"111.000000","G":"1.000000","B":"0.000000","D":"1.000000",},

A comment: ; Layer: 1
Set line to 112.000000
{"X":"-22.010000","VIT_TIR":"1750.000000","Y":"-24.476999","laser":"1.000000","H":"62.000000","F":"10.000000","laser_power":"250.000000","T":"1.000000","Z":"0.000000","line":"112.000000","G":"1.000000","B":"0.000000","D":"1.000000",},

A comment: ; Pocket: 3
Set line to 113.000000
{"X":"-22.010000","VIT_TIR":"1750.000000","Y":"-24.476999","laser":"1.000000","H":"62.000000","F":"10.000000","laser_power":"250.000000","T":"1.000000","Z":"0.000000","line":"113.000000","G":"1.000000","B":"0.000000","D":"1.000000",},

A command: G
An integer: 0
INT=0
Set G to 0.000000
A command: X
An arithmetic operator: -
A float: 21.559999
FLOAT=21.559999
Set X to -21.559999
Set line to 114.000000
{"X":"-21.559999","VIT_TIR":"1750.000000","Y":"-24.476999","laser":"1.000000","H":"62.000000","F":"10.000000","laser_power":"250.000000","T":"1.000000","Z":"0.000000","line":"114.000000","G":"0.000000","B":"0.000000","D":"1.000000",},

A comment: ; Laser ON
Set line to 115.000000
{"X":"-21.559999","VIT_TIR":"1750.000000","Y":"-24.476999","laser":"1.000000","H":"62.000000","F":"10.000000","laser_power":"250.000000","T":"1.000000","Z":"0.000000","line":"115.000000","G":"0.000000","B":"0.000000","D":"1.000000",},

A special command: /LASER_ON
Set laser to 1.000000
Set line to 116.000000
{"X":"-21.559999","VIT_TIR":"1750.000000","Y":"-24.476999","laser":"1.000000","H":"62.000000","F":"10.000000","laser_power":"250.000000","T":"1.000000","Z":"0.000000","line":"116.000000","G":"0.000000","B":"0.000000","D":"1.000000",},

A command: G
An integer: 1
INT=1
Set G to 1.000000
A command: X
An arithmetic operator: -
A float: 21.860001
FLOAT=21.860001
Set X to -21.860001
A command: Y
An arithmetic operator: -
A float: 24.051001
FLOAT=24.051001
Set Y to -24.051001
A command: F
SET
A custom Variable: VIT_TIR
Error: syntax error, in line: 116
Set line to 117.000000
{"X":"-21.860001","VIT_TIR":"1750.000000","Y":"-24.051001","laser":"1.000000","H":"62.000000","F":"10.000000","laser_power":"250.000000","T":"1.000000","Z":"0.000000","line":"117.000000","G":"1.000000","B":"0.000000","D":"1.000000",},

Set line to 118.000000
{"X":"-21.860001","VIT_TIR":"1750.000000","Y":"-24.051001","laser":"1.000000","H":"62.000000","F":"10.000000","laser_power":"250.000000","T":"1.000000","Z":"0.000000","line":"118.000000","G":"1.000000","B":"0.000000","D":"1.000000",},

A command: G
An integer: 1
INT=1
Set G to 1.000000
A command: X
An arithmetic operator: -
A float: 22.010000
FLOAT=22.010000
Set X to -22.010000
A command: Y
An arithmetic operator: -
A float: 23.646000
FLOAT=23.646000
Set Y to -23.646000
Set line to 119.000000
{"X":"-22.010000","VIT_TIR":"1750.000000","Y":"-23.646000","laser":"1.000000","H":"62.000000","F":"10.000000","laser_power":"250.000000","T":"1.000000","Z":"0.000000","line":"119.000000","G":"1.000000","B":"0.000000","D":"1.000000",},

A command: G
An integer: 1
INT=1
Set G to 1.000000
A command: Y
An arithmetic operator: -
A float: 23.016001
FLOAT=23.016001
Set Y to -23.016001
Set line to 120.000000
{"X":"-22.010000","VIT_TIR":"1750.000000","Y":"-23.016001","laser":"1.000000","H":"62.000000","F":"10.000000","laser_power":"250.000000","T":"1.000000","Z":"0.000000","line":"120.000000","G":"1.000000","B":"0.000000","D":"1.000000",},

A command: G
An integer: 1
INT=1
Set G to 1.000000
A command: X
An arithmetic operator: -
A float: 21.860001
FLOAT=21.860001
Set X to -21.860001
A command: Y
An arithmetic operator: -
A float: 22.590000
FLOAT=22.590000
Set Y to -22.590000
Set line to 121.000000
{"X":"-21.860001","VIT_TIR":"1750.000000","Y":"-22.590000","laser":"1.000000","H":"62.000000","F":"10.000000","laser_power":"250.000000","T":"1.000000","Z":"0.000000","line":"121.000000","G":"1.000000","B":"0.000000","D":"1.000000",},

A command: G
An integer: 1
INT=1
Set G to 1.000000
A command: X
An arithmetic operator: -
A float: 21.559999
FLOAT=21.559999
Set X to -21.559999
A command: Y
An arithmetic operator: -
A float: 22.160999
FLOAT=22.160999
Set Y to -22.160999
Set line to 122.000000
{"X":"-21.559999","VIT_TIR":"1750.000000","Y":"-22.160999","laser":"1.000000","H":"62.000000","F":"10.000000","laser_power":"250.000000","T":"1.000000","Z":"0.000000","line":"122.000000","G":"1.000000","B":"0.000000","D":"1.000000",},

A command: G
An integer: 1
INT=1
Set G to 1.000000
A command: X
An arithmetic operator: -
A float: 21.139999
FLOAT=21.139999
Set X to -21.139999
A command: Y
An arithmetic operator: -
A float: 21.957001
FLOAT=21.957001
Set Y to -21.957001
Set line to 123.000000
{"X":"-21.139999","VIT_TIR":"1750.000000","Y":"-21.957001","laser":"1.000000","H":"62.000000","F":"10.000000","laser_power":"250.000000","T":"1.000000","Z":"0.000000","line":"123.000000","G":"1.000000","B":"0.000000","D":"1.000000",},

A command: G
An integer: 1
INT=1
Set G to 1.000000
A command: X
An arithmetic operator: -
A float: 20.870001
FLOAT=20.870001
Set X to -20.870001
Set line to 124.000000
{"X":"-20.870001","VIT_TIR":"1750.000000","Y":"-21.957001","laser":"1.000000","H":"62.000000","F":"10.000000","laser_power":"250.000000","T":"1.000000","Z":"0.000000","line":"124.000000","G":"1.000000","B":"0.000000","D":"1.000000",},

A command: G
An integer: 1
INT=1
Set G to 1.000000
A command: X
An arithmetic operator: -
A float: 20.420000
FLOAT=20.420000
Set X to -20.420000
A command: Y
An arithmetic operator: -
A float: 22.160999
FLOAT=22.160999
Set Y to -22.160999
Set line to 125.000000
{"X":"-20.420000","VIT_TIR":"1750.000000","Y":"-22.160999","laser":"1.000000","H":"62.000000","F":"10.000000","laser_power":"250.000000","T":"1.000000","Z":"0.000000","line":"125.000000","G":"1.000000","B":"0.000000","D":"1.000000",},

A command: G
An integer: 1
INT=1
Set G to 1.000000
A command: X
An arithmetic operator: -
A float: 20.150000
FLOAT=20.150000
Set X to -20.150000
A command: Y
An arithmetic operator: -
A float: 22.590000
FLOAT=22.590000
Set Y to -22.590000
Set line to 126.000000
{"X":"-20.150000","VIT_TIR":"1750.000000","Y":"-22.590000","laser":"1.000000","H":"62.000000","F":"10.000000","laser_power":"250.000000","T":"1.000000","Z":"0.000000","line":"126.000000","G":"1.000000","B":"0.000000","D":"1.000000",},

tilo@tilo-Laptop:~/work/nc_geometry/flex/nc_lex$ 
