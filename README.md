# v5LabLogo
Old school win32 Opengl3 app controlled by Logo commands. 
Designed with voice commands in mind, and VR.

## Compilation directives
Visual Studio 2015  
Open v5Lab/v5Lab.sln

## Rules
```
<logo-clear-command> ::= CLEAR  
<logo-move-command> ::= MOVETO <integer> , <integer>  
<logo-direction-command> ::= FORWARD <integer> | BACK <integer>  
<logo-direction-command> ::= LEFT <integer> | RIGHT <integer>  
<logo-direction-command> ::= UP <integer> | DOWN <integer>  
<logo-direction-command> ::= ROLLRIGHT <integer> | ROLLLEFT <integer>  
<logo-repeat-command> ::= REPEAT <integer> {<logo-sentence>} END  
<logo-pen-command> ::= CLEAR  
<logo-pen-command> ::= PENUP | PENDOWN  
<logo-end-of-file> ::= IDENTIFIER  
<logo-end-of-file> ::= EOF 
```

