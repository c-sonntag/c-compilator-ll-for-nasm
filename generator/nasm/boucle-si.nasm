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
    push    2             
    pop   ebx              ; depile la seconde operande dans ebx
    pop   eax              ; depile la permière operande dans eax
    div   ebx             
    push  edx              ; empile le résultat
    push    0             
    pop   ebx              ; depile la seconde operande dans ebx
    pop   eax              ; depile la permière operande dans eax
    cmp   eax, ebx        
    jne  main3_TqI1_Si    
    jmp  main3_TqI2_Sinon 
  
    main3_TqI1_Si:           ; si
      push dword [ebp - 4]   ; variable local
      pop   eax             
      call iprintLF         
                            
      jmp  main3_TqI3_R     
  
    main3_TqI2_Sinon:        ; sinon
      push dword [ebp - 4]   ; variable local
      push    1             
      pop   ebx              ; depile la seconde operande dans ebx
      pop   eax              ; depile la permière operande dans eax
      sub   eax, ebx        
      push  eax              ; empile le résultat
      pop   eax             
      call iprintLF         
  
  main3_TqI3_R:            ; retour à la fonction
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
  add   esp, 4           ; desallocation variables locales type 'entier' 
  pop   ebp              ; restaure la valeur de ebp
  ret                   

