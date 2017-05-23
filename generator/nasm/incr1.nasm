%include  'io.asm'

section  .bss
sinput    : resb   255   ; reserve a 255 byte space in memory for the users input string
$v1       : rest   1    

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
  sub   esp, 4           ; allocation variables locales type 'entier'
  push    0             
  pop   ebx             
  mov  [$v1], ebx        ; variable global
  add dword [$v1], 1     ; incremente : variable global
  push dword [$v1]       ; variable global
  pop   eax             
  call iprintLF         
  push    1             
  pop   ebx             
  mov  [ebp - 4], ebx    ; variable local
  add dword [ebp - 4], 1 ; incremente : variable local
  push dword [ebp - 4]   ; variable local
  pop   eax             
  call iprintLF         
  add   esp, 4           ; desallocation variables locales type 'entier' 
  pop   ebp              ; restaure la valeur de ebp
  ret                   

