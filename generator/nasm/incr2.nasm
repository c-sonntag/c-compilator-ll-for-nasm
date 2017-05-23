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
  sub   esp, 8           ; allocation variables locales type 'entier'
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
                          
    push    0             
    push dword [ebp - 4]   ; variable local
    pop   eax             
    add   eax, eax         ; (multiplie par 2)
    add   eax, eax         ; (multiplie par 2)
    pop   ebx             
    mov  [$tab + eax], ebx ; tableau entier indicée à partir de l'indice stocké dans 'eax'
    push    0             
    pop   ebx             
    mov  [ebp - 8], ebx    ; variable local
  
    main3_TqI1_TqT:          ; tantque : test
      push dword [ebp - 8]   ; variable local
      push dword [ebp - 4]   ; variable local
      pop   ebx              ; depile la seconde operande dans ebx
      pop   eax              ; depile la permière operande dans eax
      cmp   eax, ebx        
      jl   main3_TqI2_TqV   
      push    0              ; état 'non-validé'
      jmp  main3_TqI3_TqI   
  
    main3_TqI2_TqV:          ; tantque : valide état
      push    1              ; état 'validé'
  
    main3_TqI3_TqI:          ; tantque : bloc d'instruction
      pop   eax             
      cmp   eax, 00          ; check état
      jz   main3_TqI4_R      ; si état non valide, quitte instruction tantque
                            
      push dword [ebp - 4]   ; variable local
      pop   eax             
      add   eax, eax         ; (multiplie par 2)
      add   eax, eax         ; (multiplie par 2)
      add dword [$tab + eax], 1 ; incremente : tableau entier indicée à partir de l'indice stocké dans 'eax'
      add dword [ebp - 8], 1 ; incremente : variable local
                            
      jmp  main3_TqI1_TqT   
  
  main3_TqI4_R:            ; retour à la fonction
    push dword [ebp - 4]   ; variable local
    pop   eax             
    add   eax, eax         ; (multiplie par 2)
    add   eax, eax         ; (multiplie par 2)
    push dword [$tab + eax] ; tableau entier indicée à partir de l'indice stocké dans 'eax'
    pop   eax             
    call iprintLF         
    add dword [ebp - 4], 1 ; incremente : variable local
                          
    jmp  main1_TqT        

main4_R:                 ; retour à la fonction
  add   esp, 8           ; desallocation variables locales type 'entier' 
  pop   ebp              ; restaure la valeur de ebp
  ret                   

