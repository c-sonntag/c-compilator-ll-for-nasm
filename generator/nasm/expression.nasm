%include  'io.asm'

section  .bss
sinput    : resb   255   ; reserve a 255 byte space in memory for the users input string
$a        : rest   1    
$b        : rest   1    

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
  pop   ebx             
  mov  [$a], ebx         ; variable global
  mov   eax, sinput     
  call readline         
  mov   eax, sinput     
  mov   eax, sinput     
  call atoi             
  push  eax             
  pop   ebx             
  mov  [$b], ebx         ; variable global
  push dword [$a]        ; variable global
  push dword [$b]        ; variable global
  pop   ebx              ; depile la seconde operande dans ebx
  pop   eax              ; depile la permière operande dans eax
  add   eax, ebx        
  push  eax              ; empile le résultat
  pop   eax             
  call iprintLF         
  pop   ebp              ; restaure la valeur de ebp
  ret                   

