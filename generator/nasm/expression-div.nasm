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
  sub   esp, 16          ; allocation variables locales type 'entier'
  mov   eax, sinput     
  call readline         
  mov   eax, sinput     
  mov   eax, sinput     
  call atoi             
  push  eax             
  mov   eax, sinput     
  call readline         
  mov   eax, sinput     
  mov   eax, sinput     
  call atoi             
  push  eax             
  pop   ebx              ; depile la seconde operande dans ebx
  pop   eax              ; depile la permière operande dans eax
  div   ebx             
  push  eax              ; empile le résultat
  pop   eax             
  call iprintLF         
  add   esp, 16          ; desallocation variables locales type 'entier' 
  pop   ebp              ; restaure la valeur de ebp
  ret                   

