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
  sub   esp, 8           ; allocation variables locales type 'entier'
  mov   eax, sinput     
  call readline         
  mov   eax, sinput     
  mov   eax, sinput     
  call atoi             
  push  eax             
  pop   ebx             
  mov  [ebp - 4], ebx    ; variable local
  mov   eax, sinput     
  call readline         
  mov   eax, sinput     
  mov   eax, sinput     
  call atoi             
  push  eax             
  pop   ebx             
  mov  [ebp - 8], ebx    ; variable local
  push dword [ebp - 4]   ; variable local
  push dword [ebp - 8]   ; variable local
  pop   ebx              ; depile la seconde operande dans ebx
  pop   eax              ; depile la permière operande dans eax
  call _cmpBool_ou       ; sous fonction d'opération
  push  eax              ; empile le résultat
  push dword [ebp - 4]   ; variable local
  push dword [ebp - 8]   ; variable local
  pop   ebx              ; depile la seconde operande dans ebx
  pop   eax              ; depile la permière operande dans eax
  call _cmpBool_et       ; sous fonction d'opération
  push  eax              ; empile le résultat
  pop   eax              ; depile la permière operande dans eax
  call _cmpBool_non      ; sous fonction d'opération
  push  eax              ; empile le résultat
  pop   ebx              ; depile la seconde operande dans ebx
  pop   eax              ; depile la permière operande dans eax
  call _cmpBool_et       ; sous fonction d'opération
  push  eax              ; empile le résultat
  pop   eax             
  call iprintLF         
  add   esp, 8           ; desallocation variables locales type 'entier' 
  pop   ebp              ; restaure la valeur de ebp
  ret                   

_cmpBool__1:
  mov   eax, 1          
  ret                   

_cmpBool__0:
  mov   eax, 0          
  ret                   

_cmpBool_et:
  cmp   eax, 0          
  jne  _cmpBool_et1_2ndOp
  jmp  _cmpBool__0      

  _cmpBool_et1_2ndOp:
    cmp   ebx, 0          
    jne  _cmpBool__1      
    jmp  _cmpBool__0      

_cmpBool_ou:
  cmp   eax, 0          
  jne  _cmpBool__1      
  jmp  _cmpBool_ou1_2ndOp

  _cmpBool_ou1_2ndOp:
    cmp   ebx, 0          
    jne  _cmpBool__1      
    jmp  _cmpBool__0      

_cmpBool_non:
  cmp   eax, 0          
  jne  _cmpBool__0      
  jmp  _cmpBool__1      

