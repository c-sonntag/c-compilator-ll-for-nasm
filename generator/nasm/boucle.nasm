%include  'io.asm'

section  .bss
sinput    : resb   255   ; reserve a 255 byte space in memory for the users input string
$i        : rest   1    
$carre    : rest   1    

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
  push    0             
  pop   ebx             
  mov  [$i], ebx         ; variable global

  main1_TqT:               ; tantque : test
    push dword [$i]        ; variable global
    push   10             
    pop   ebx              ; depile la seconde operande dans ebx
    pop   eax              ; depile la permière operande dans eax
    cmp   eax, ebx        
    jl   main2_TqV        
    push    0              ; état 'non-validé'
    jmp  main3_TqI        

  main2_TqV:               ; tantque : valide état
    push    1              ; état 'validé'

  main3_TqI:               ; tantque : bloc d'instruction
    pop   eax             
    cmp   eax, 00          ; check état
    jz   main4_R           ; si état non valide, quitte instruction tantque
                          
    push dword [$i]        ; variable global
    push dword [$i]        ; variable global
    pop   ebx              ; depile la seconde operande dans ebx
    pop   eax              ; depile la permière operande dans eax
    mul   ebx             
    push  eax              ; empile le résultat
    pop   ebx             
    mov  [$carre], ebx     ; variable global
    push dword [$carre]    ; variable global
    pop   eax             
    call iprintLF         
    push dword [$i]        ; variable global
    push    1             
    pop   ebx              ; depile la seconde operande dans ebx
    pop   eax              ; depile la permière operande dans eax
    add   eax, ebx        
    push  eax              ; empile le résultat
    pop   ebx             
    mov  [$i], ebx         ; variable global
                          
    jmp  main1_TqT        

main4_R:                 ; retour à la fonction
  pop   ebp              ; restaure la valeur de ebp
  ret                   

