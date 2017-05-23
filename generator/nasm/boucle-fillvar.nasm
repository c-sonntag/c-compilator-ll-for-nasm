%include  'io.asm'

section  .bss
sinput    : resb   255   ; reserve a 255 byte space in memory for the users input string
$tab      : rest   10   

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
  mov  [ebp - 4], ebx    ; variable local

  main1_TqT:               ; tantque : test
    push dword [ebp - 4]   ; variable local
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
                          
    push dword [ebp - 4]   ; variable local
    push dword [ebp - 4]   ; variable local
    pop   ebx              ; depile la seconde operande dans ebx
    pop   eax              ; depile la permière operande dans eax
    mul   ebx             
    push  eax              ; empile le résultat
    push dword [ebp - 4]   ; variable local
    pop   eax             
    add   eax, eax         ; (multiplie par 2)
    add   eax, eax         ; (multiplie par 2)
    pop   ebx             
    mov  [$tab + eax], ebx ; tableau entier indicée à partir de l'indice stocké dans 'eax'
    push dword [ebp - 4]   ; variable local
    push    1             
    pop   ebx              ; depile la seconde operande dans ebx
    pop   eax              ; depile la permière operande dans eax
    add   eax, ebx        
    push  eax              ; empile le résultat
    pop   ebx             
    mov  [ebp - 4], ebx    ; variable local
                          
    jmp  main1_TqT        

main4_R:                 ; retour à la fonction
  push    0             
  pop   ebx             
  mov  [ebp - 4], ebx    ; variable local

  main5_TqT:               ; tantque : test
    push dword [ebp - 4]   ; variable local
    push   10             
    pop   ebx              ; depile la seconde operande dans ebx
    pop   eax              ; depile la permière operande dans eax
    cmp   eax, ebx        
    jl   main6_TqV        
    push    0              ; état 'non-validé'
    jmp  main7_TqI        

  main6_TqV:               ; tantque : valide état
    push    1              ; état 'validé'

  main7_TqI:               ; tantque : bloc d'instruction
    pop   eax             
    cmp   eax, 00          ; check état
    jz   main8_R           ; si état non valide, quitte instruction tantque
                          
    push dword [ebp - 4]   ; variable local
    pop   eax             
    add   eax, eax         ; (multiplie par 2)
    add   eax, eax         ; (multiplie par 2)
    push dword [$tab + eax] ; tableau entier indicée à partir de l'indice stocké dans 'eax'
    pop   eax             
    call iprintLF         
    push dword [ebp - 4]   ; variable local
    push    1             
    pop   ebx              ; depile la seconde operande dans ebx
    pop   eax              ; depile la permière operande dans eax
    add   eax, ebx        
    push  eax              ; empile le résultat
    pop   ebx             
    mov  [ebp - 4], ebx    ; variable local
                          
    jmp  main5_TqT        

main8_R:                 ; retour à la fonction
  add   esp, 4           ; desallocation variables locales type 'entier' 
  pop   ebp              ; restaure la valeur de ebp
  ret                   

