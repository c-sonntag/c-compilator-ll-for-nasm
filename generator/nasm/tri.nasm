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

initialiser:
  push  ebp              ; sauvegarde la valeur de ebp
  mov   ebp, esp         ; nouvelle valeur de ebp
  push    8             
  push    0             
  pop   eax             
  add   eax, eax         ; (multiplie par 2)
  add   eax, eax         ; (multiplie par 2)
  pop   ebx             
  mov  [$tab + eax], ebx ; tableau entier indicée à partir de l'indice stocké dans 'eax'
  push    6             
  push    1             
  pop   eax             
  add   eax, eax         ; (multiplie par 2)
  add   eax, eax         ; (multiplie par 2)
  pop   ebx             
  mov  [$tab + eax], ebx ; tableau entier indicée à partir de l'indice stocké dans 'eax'
  push    9             
  push    2             
  pop   eax             
  add   eax, eax         ; (multiplie par 2)
  add   eax, eax         ; (multiplie par 2)
  pop   ebx             
  mov  [$tab + eax], ebx ; tableau entier indicée à partir de l'indice stocké dans 'eax'
  push    9             
  push    3             
  pop   eax             
  add   eax, eax         ; (multiplie par 2)
  add   eax, eax         ; (multiplie par 2)
  pop   ebx             
  mov  [$tab + eax], ebx ; tableau entier indicée à partir de l'indice stocké dans 'eax'
  push    4             
  push    4             
  pop   eax             
  add   eax, eax         ; (multiplie par 2)
  add   eax, eax         ; (multiplie par 2)
  pop   ebx             
  mov  [$tab + eax], ebx ; tableau entier indicée à partir de l'indice stocké dans 'eax'
  push    2             
  push    5             
  pop   eax             
  add   eax, eax         ; (multiplie par 2)
  add   eax, eax         ; (multiplie par 2)
  pop   ebx             
  mov  [$tab + eax], ebx ; tableau entier indicée à partir de l'indice stocké dans 'eax'
  push    3             
  push    6             
  pop   eax             
  add   eax, eax         ; (multiplie par 2)
  add   eax, eax         ; (multiplie par 2)
  pop   ebx             
  mov  [$tab + eax], ebx ; tableau entier indicée à partir de l'indice stocké dans 'eax'
  push    1             
  push    7             
  pop   eax             
  add   eax, eax         ; (multiplie par 2)
  add   eax, eax         ; (multiplie par 2)
  pop   ebx             
  mov  [$tab + eax], ebx ; tableau entier indicée à partir de l'indice stocké dans 'eax'
  push    4             
  push    8             
  pop   eax             
  add   eax, eax         ; (multiplie par 2)
  add   eax, eax         ; (multiplie par 2)
  pop   ebx             
  mov  [$tab + eax], ebx ; tableau entier indicée à partir de l'indice stocké dans 'eax'
  push    5             
  push    9             
  pop   eax             
  add   eax, eax         ; (multiplie par 2)
  add   eax, eax         ; (multiplie par 2)
  pop   ebx             
  mov  [$tab + eax], ebx ; tableau entier indicée à partir de l'indice stocké dans 'eax'
  pop   ebp              ; restaure la valeur de ebp
  ret                   

afficher:
  push  ebp              ; sauvegarde la valeur de ebp
  mov   ebp, esp         ; nouvelle valeur de ebp
  sub   esp, 4           ; allocation variables locales type 'entier'
  push    0             
  pop   ebx             
  mov  [ebp - 4], ebx    ; variable local

  afficher1_TqT:           ; tantque : test
    push dword [ebp - 4]   ; variable local
    push dword [ebp + 8]   ; argument
    pop   ebx              ; depile la seconde operande dans ebx
    pop   eax              ; depile la permière operande dans eax
    cmp   eax, ebx        
    jl   afficher2_TqV    
    push    0              ; état 'non-validé'
    jmp  afficher3_TqI    

  afficher2_TqV:           ; tantque : valide état
    push    1              ; état 'validé'

  afficher3_TqI:           ; tantque : bloc d'instruction
    pop   eax             
    cmp   eax, 00          ; check état
    jz   afficher4_R       ; si état non valide, quitte instruction tantque
                          
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
                          
    jmp  afficher1_TqT    

afficher4_R:             ; retour à la fonction
  push 123456789        
  pop   eax             
  call iprintLF         
  add   esp, 4           ; desallocation variables locales type 'entier' 
  pop   ebp              ; restaure la valeur de ebp
  ret                   

echanger:
  push  ebp              ; sauvegarde la valeur de ebp
  mov   ebp, esp         ; nouvelle valeur de ebp
  sub   esp, 4           ; allocation variables locales type 'entier'
  push dword [ebp + 8]   ; argument
  pop   eax             
  add   eax, eax         ; (multiplie par 2)
  add   eax, eax         ; (multiplie par 2)
  push dword [$tab + eax] ; tableau entier indicée à partir de l'indice stocké dans 'eax'
  pop   ebx             
  mov  [ebp - 4], ebx    ; variable local
  push dword [ebp + 12]  ; argument
  pop   eax             
  add   eax, eax         ; (multiplie par 2)
  add   eax, eax         ; (multiplie par 2)
  push dword [$tab + eax] ; tableau entier indicée à partir de l'indice stocké dans 'eax'
  push dword [ebp + 8]   ; argument
  pop   eax             
  add   eax, eax         ; (multiplie par 2)
  add   eax, eax         ; (multiplie par 2)
  pop   ebx             
  mov  [$tab + eax], ebx ; tableau entier indicée à partir de l'indice stocké dans 'eax'
  push dword [ebp - 4]   ; variable local
  push dword [ebp + 12]  ; argument
  pop   eax             
  add   eax, eax         ; (multiplie par 2)
  add   eax, eax         ; (multiplie par 2)
  pop   ebx             
  mov  [$tab + eax], ebx ; tableau entier indicée à partir de l'indice stocké dans 'eax'
  add   esp, 4           ; desallocation variables locales type 'entier' 
  pop   ebp              ; restaure la valeur de ebp
  ret                   

trier:
  push  ebp              ; sauvegarde la valeur de ebp
  mov   ebp, esp         ; nouvelle valeur de ebp
  sub   esp, 12          ; allocation variables locales type 'entier'
  push dword [ebp + 8]   ; argument
  pop   ebx             
  mov  [ebp - 12], ebx   ; variable local
  push    1             
  pop   ebx             
  mov  [ebp - 4], ebx    ; variable local

  trier1_TqT:              ; tantque : test
    push dword [ebp - 4]   ; variable local
    push    1             
    pop   ebx              ; depile la seconde operande dans ebx
    pop   eax              ; depile la permière operande dans eax
    cmp   eax, ebx        
    je   trier2_TqV       
    push    0              ; état 'non-validé'
    jmp  trier3_TqI       

  trier2_TqV:              ; tantque : valide état
    push    1              ; état 'validé'

  trier3_TqI:              ; tantque : bloc d'instruction
    pop   eax             
    cmp   eax, 00          ; check état
    jz   trier4_R          ; si état non valide, quitte instruction tantque
                          
    push    0             
    pop   ebx             
    mov  [ebp - 4], ebx    ; variable local
    push    0             
    pop   ebx             
    mov  [ebp - 8], ebx    ; variable local
  
    trier3_TqI1_TqT:         ; tantque : test
      push dword [ebp - 8]   ; variable local
      push dword [ebp - 12]  ; variable local
      push    1             
      pop   ebx              ; depile la seconde operande dans ebx
      pop   eax              ; depile la permière operande dans eax
      sub   eax, ebx        
      push  eax              ; empile le résultat
      pop   ebx              ; depile la seconde operande dans ebx
      pop   eax              ; depile la permière operande dans eax
      cmp   eax, ebx        
      jl   trier3_TqI2_TqV  
      push    0              ; état 'non-validé'
      jmp  trier3_TqI3_TqI  
  
    trier3_TqI2_TqV:         ; tantque : valide état
      push    1              ; état 'validé'
  
    trier3_TqI3_TqI:         ; tantque : bloc d'instruction
      pop   eax             
      cmp   eax, 00          ; check état
      jz   trier3_TqI4_R     ; si état non valide, quitte instruction tantque
                            
      push dword [ebp - 8]   ; variable local
      push    1             
      pop   ebx              ; depile la seconde operande dans ebx
      pop   eax              ; depile la permière operande dans eax
      add   eax, ebx        
      push  eax              ; empile le résultat
      pop   eax             
      add   eax, eax         ; (multiplie par 2)
      add   eax, eax         ; (multiplie par 2)
      push dword [$tab + eax] ; tableau entier indicée à partir de l'indice stocké dans 'eax'
      push dword [ebp - 8]   ; variable local
      pop   eax             
      add   eax, eax         ; (multiplie par 2)
      add   eax, eax         ; (multiplie par 2)
      push dword [$tab + eax] ; tableau entier indicée à partir de l'indice stocké dans 'eax'
      pop   ebx              ; depile la seconde operande dans ebx
      pop   eax              ; depile la permière operande dans eax
      cmp   eax, ebx        
      jl   trier3_TqI3_TqI1_Si
      jmp  trier3_TqI3_TqI2_R
    
      trier3_TqI3_TqI1_Si:     ; si
        sub   esp, 4           ; allocation valeur de retour
        push dword [ebp - 8]   ; variable local
        push dword [ebp - 8]   ; variable local
        push    1             
        pop   ebx              ; depile la seconde operande dans ebx
        pop   eax              ; depile la permière operande dans eax
        add   eax, ebx        
        push  eax              ; empile le résultat
        call echanger         
        add   esp, 12          ; 'desallocation parametres' et 'valeur de retour ignoree'
        push    1             
        pop   ebx             
        mov  [ebp - 4], ebx    ; variable local
    
    trier3_TqI3_TqI2_R:      ; retour à la fonction
      push dword [ebp - 8]   ; variable local
      push    1             
      pop   ebx              ; depile la seconde operande dans ebx
      pop   eax              ; depile la permière operande dans eax
      add   eax, ebx        
      push  eax              ; empile le résultat
      pop   ebx             
      mov  [ebp - 8], ebx    ; variable local
                            
      jmp  trier3_TqI1_TqT  
  
  trier3_TqI4_R:           ; retour à la fonction
    push dword [ebp - 12]  ; variable local
    push    1             
    pop   ebx              ; depile la seconde operande dans ebx
    pop   eax              ; depile la permière operande dans eax
    sub   eax, ebx        
    push  eax              ; empile le résultat
    pop   ebx             
    mov  [ebp - 12], ebx   ; variable local
                          
    jmp  trier1_TqT       

trier4_R:                ; retour à la fonction
  add   esp, 12          ; desallocation variables locales type 'entier' 
  pop   ebp              ; restaure la valeur de ebp
  ret                   

main:
  push  ebp              ; sauvegarde la valeur de ebp
  mov   ebp, esp         ; nouvelle valeur de ebp
  sub   esp, 4           ; allocation valeur de retour
  call initialiser      
  add   esp, 4           ; valeur de retour ignoree
  sub   esp, 4           ; allocation valeur de retour
  push   10             
  call afficher         
  add   esp, 8           ; 'desallocation parametres' et 'valeur de retour ignoree'
  sub   esp, 4           ; allocation valeur de retour
  push   10             
  call trier            
  add   esp, 8           ; 'desallocation parametres' et 'valeur de retour ignoree'
  sub   esp, 4           ; allocation valeur de retour
  push   10             
  call afficher         
  add   esp, 8           ; 'desallocation parametres' et 'valeur de retour ignoree'
  pop   ebp              ; restaure la valeur de ebp
  ret                   

