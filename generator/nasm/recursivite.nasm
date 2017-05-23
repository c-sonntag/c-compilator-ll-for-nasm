%include  'io.asm'

section  .bss
sinput    : resb   255   ; reserve a 255 byte space in memory for the users input string
$tab      : rest   100  

section  .text
global _start

_start:                  ; entrée start
  push    0              ; valeur de retour initiale
  call main             
  mov   eax, 1           ; 1 est le code de SYS_EXIT
  pop   ebx              ; recup valeur de retour, pour 'exit'
  int  0x80              ; exit

remplir:
  push  ebp              ; sauvegarde la valeur de ebp
  mov   ebp, esp         ; nouvelle valeur de ebp
  sub   esp, 4           ; allocation variables locales type 'entier'
  push    0             
  pop   ebx             
  mov  [ebp - 4], ebx    ; variable local

  remplir1_TqT:            ; tantque : test
    push dword [ebp - 4]   ; variable local
    push  100             
    pop   ebx              ; depile la seconde operande dans ebx
    pop   eax              ; depile la permière operande dans eax
    cmp   eax, ebx        
    jl   remplir2_TqV     
    push    0              ; état 'non-validé'
    jmp  remplir3_TqI     

  remplir2_TqV:            ; tantque : valide état
    push    1              ; état 'validé'

  remplir3_TqI:            ; tantque : bloc d'instruction
    pop   eax             
    cmp   eax, 00          ; check état
    jz   remplir4_R        ; si état non valide, quitte instruction tantque
                          
    push dword [ebp - 4]   ; variable local
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
                          
    jmp  remplir1_TqT     

remplir4_R:              ; retour à la fonction
  add   esp, 4           ; desallocation variables locales type 'entier' 
  pop   ebp              ; restaure la valeur de ebp
  ret                   

recurse_count:
  push  ebp              ; sauvegarde la valeur de ebp
  mov   ebp, esp         ; nouvelle valeur de ebp
  push dword [ebp + 8]   ; argument
  push  100             
  pop   ebx              ; depile la seconde operande dans ebx
  pop   eax              ; depile la permière operande dans eax
  cmp   eax, ebx        
  jge  recurse_count1_Si
  jmp  recurse_count2_Sinon

  recurse_count1_Si:       ; si
    push    0             
    pop   eax             
    mov  [ebp + 12], eax   ; ecriture de la valeur de retour
    jmp  recurse_count4_END
                          
    jmp  recurse_count3_R 

  recurse_count2_Sinon:    ; sinon
    push dword [ebp + 8]   ; argument
    pop   eax             
    add   eax, eax         ; (multiplie par 2)
    add   eax, eax         ; (multiplie par 2)
    push dword [$tab + eax] ; tableau entier indicée à partir de l'indice stocké dans 'eax'
    sub   esp, 4           ; allocation valeur de retour
    push dword [ebp + 8]   ; argument
    push    1             
    pop   ebx              ; depile la seconde operande dans ebx
    pop   eax              ; depile la permière operande dans eax
    add   eax, ebx        
    push  eax              ; empile le résultat
    call recurse_count    
    add   esp, 4           ; desallocation parametres
    pop   ebx              ; depile la seconde operande dans ebx
    pop   eax              ; depile la permière operande dans eax
    add   eax, ebx        
    push  eax              ; empile le résultat
    pop   eax             
    mov  [ebp + 12], eax   ; ecriture de la valeur de retour
    jmp  recurse_count4_END

recurse_count3_R:        ; retour à la fonction

recurse_count4_END:      ; appelé par un retour
  pop   ebp              ; restaure la valeur de ebp
  ret                   

main:
  push  ebp              ; sauvegarde la valeur de ebp
  mov   ebp, esp         ; nouvelle valeur de ebp
  sub   esp, 4           ; allocation valeur de retour
  call remplir          
  add   esp, 4           ; valeur de retour ignoree
  sub   esp, 4           ; allocation valeur de retour
  push    0             
  call recurse_count    
  add   esp, 4           ; desallocation parametres
  pop   eax             
  call iprintLF         
  push 4950              ; constexpr !
  pop   eax             
  call iprintLF         
  pop   ebp              ; restaure la valeur de ebp
  ret                   

