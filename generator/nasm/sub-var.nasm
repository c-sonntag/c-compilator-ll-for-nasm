%include  'io.asm'

section  .bss
sinput    : resb   255   ; reserve a 255 byte space in memory for the users input string
$a        : rest   1    

section  .text
global _start

_start:                  ; entrée start
  push    0              ; valeur de retour initiale
  call main             
  mov   eax, 1           ; 1 est le code de SYS_EXIT
  pop   ebx              ; recup valeur de retour, pour 'exit'
  int  0x80              ; exit

afficheGlobal:
  push  ebp              ; sauvegarde la valeur de ebp
  mov   ebp, esp         ; nouvelle valeur de ebp
  push 123456789        
  pop   eax             
  call iprintLF         
  push   10             
  pop   eax             
  call iprintLF         
  push dword [$a]        ; variable global
  pop   eax             
  call iprintLF         
  pop   ebp              ; restaure la valeur de ebp
  ret                   

afficheParam:
  push  ebp              ; sauvegarde la valeur de ebp
  mov   ebp, esp         ; nouvelle valeur de ebp
  push 123456789        
  pop   eax             
  call iprintLF         
  push   30             
  pop   eax             
  call iprintLF         
  push dword [ebp + 8]   ; argument
  pop   eax             
  call iprintLF         
  pop   ebp              ; restaure la valeur de ebp
  ret                   

afficheLocal:
  push  ebp              ; sauvegarde la valeur de ebp
  mov   ebp, esp         ; nouvelle valeur de ebp
  sub   esp, 4           ; allocation variables locales type 'entier'
  push   20             
  pop   ebx             
  mov  [ebp - 4], ebx    ; variable local
  push 123456789        
  pop   eax             
  call iprintLF         
  push   20             
  pop   eax             
  call iprintLF         
  push dword [ebp - 4]   ; variable local
  pop   eax             
  call iprintLF         
  add   esp, 4           ; desallocation variables locales type 'entier' 
  pop   ebp              ; restaure la valeur de ebp
  ret                   

main:
  push  ebp              ; sauvegarde la valeur de ebp
  mov   ebp, esp         ; nouvelle valeur de ebp
  push   10             
  pop   ebx             
  mov  [$a], ebx         ; variable global
  sub   esp, 4           ; allocation valeur de retour
  call afficheLocal     
  add   esp, 4           ; valeur de retour ignoree
  sub   esp, 4           ; allocation valeur de retour
  call afficheGlobal    
  add   esp, 4           ; valeur de retour ignoree
  sub   esp, 4           ; allocation valeur de retour
  push   30             
  call afficheParam     
  add   esp, 8           ; 'desallocation parametres' et 'valeur de retour ignoree'
  pop   ebp              ; restaure la valeur de ebp
  ret                   

