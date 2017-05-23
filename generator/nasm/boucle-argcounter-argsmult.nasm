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

loop:
  push  ebp              ; sauvegarde la valeur de ebp
  mov   ebp, esp         ; nouvelle valeur de ebp

  loop1_TqT:               ; tantque : test
    push dword [ebp + 12]  ; argument
    push   10             
    pop   ebx              ; depile la seconde operande dans ebx
    pop   eax              ; depile la permière operande dans eax
    cmp   eax, ebx        
    jl   loop2_TqV        
    push    0              ; état 'non-validé'
    jmp  loop3_TqI        

  loop2_TqV:               ; tantque : valide état
    push    1              ; état 'validé'

  loop3_TqI:               ; tantque : bloc d'instruction
    pop   eax             
    cmp   eax, 00          ; check état
    jz   loop4_R           ; si état non valide, quitte instruction tantque
                          
    push dword [ebp + 12]  ; argument
    push dword [ebp + 12]  ; argument
    pop   ebx              ; depile la seconde operande dans ebx
    pop   eax              ; depile la permière operande dans eax
    mul   ebx             
    push  eax              ; empile le résultat
    push dword [ebp + 8]   ; argument
    pop   ebx              ; depile la seconde operande dans ebx
    pop   eax              ; depile la permière operande dans eax
    mul   ebx             
    push  eax              ; empile le résultat
    pop   eax             
    call iprintLF         
    push dword [ebp + 12]  ; argument
    push    1             
    pop   ebx              ; depile la seconde operande dans ebx
    pop   eax              ; depile la permière operande dans eax
    add   eax, ebx        
    push  eax              ; empile le résultat
    pop   ebx             
    mov  [ebp + 12], ebx   ; argument
                          
    jmp  loop1_TqT        

loop4_R:                 ; retour à la fonction
  pop   ebp              ; restaure la valeur de ebp
  ret                   

main:
  push  ebp              ; sauvegarde la valeur de ebp
  mov   ebp, esp         ; nouvelle valeur de ebp
  sub   esp, 4           ; allocation valeur de retour
  push    0             
  push    1             
  call loop             
  add   esp, 12          ; 'desallocation parametres' et 'valeur de retour ignoree'
  sub   esp, 4           ; allocation valeur de retour
  push    0             
  push    2             
  call loop             
  add   esp, 12          ; 'desallocation parametres' et 'valeur de retour ignoree'
  pop   ebp              ; restaure la valeur de ebp
  ret                   

