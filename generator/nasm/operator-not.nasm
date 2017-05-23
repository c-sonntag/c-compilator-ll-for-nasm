%include  'io.asm'

section  .bss
sinput    : resb   255   ; reserve a 255 byte space in memory for the users input string

section  .text
global _start

_start:                  ; entrée start
  push    0              ; valeur de retour initiale
  call main             
  mov   eax, 1           ; 1 est le code de SYS_EXIT
  pop   ebx              ; recup valeur de retour, pour 'exit'
  int  0x80              ; exit

main:
  push  ebp              ; sauvegarde la valeur de ebp
  mov   ebp, esp         ; nouvelle valeur de ebp
  mov   eax, sinput     
  call readline         
  mov   eax, sinput     
  mov   eax, sinput     
  call atoi             
  push  eax             
  pop   eax              ; depile la permière operande dans eax
  call _cmpBool_non      ; sous fonction d'opération
  push  eax              ; empile le résultat
  pop   eax             
  call iprintLF         
  pop   ebp              ; restaure la valeur de ebp
  ret                   

_cmpBool__1:
  mov   eax, 1          
  ret                   

_cmpBool__0:
  mov   eax, 0          
  ret                   

_cmpBool_non:
  cmp   eax, 0          
  jne  _cmpBool__0      
  jmp  _cmpBool__1      

