%include  'io.asm'

section  .bss
sinput    : resb   255   ; reserve a 255 byte space in memory for the users input string
$a        : rest   1    
$b        : rest   1    

section  .text
global _start

_start:                  ; entrée start
  push    0              ; valeur de retour initiale
  call main             
  mov   eax, 1           ; 1 est le code de SYS_EXIT
  pop   ebx              ; recup valeur de retour, pour 'exit'
  int  0x80              ; exit

pgcd:
  push  ebp              ; sauvegarde la valeur de ebp
  mov   ebp, esp         ; nouvelle valeur de ebp
  sub   esp, 4           ; allocation variables locales type 'entier'
  push dword [ebp + 12]  ; argument
  push dword [ebp + 12]  ; argument
  push dword [ebp + 8]   ; argument
  pop   ebx              ; depile la seconde operande dans ebx
  pop   eax              ; depile la permière operande dans eax
  div   ebx             
  push  eax              ; empile le résultat
  push dword [ebp + 8]   ; argument
  pop   ebx              ; depile la seconde operande dans ebx
  pop   eax              ; depile la permière operande dans eax
  mul   ebx             
  push  eax              ; empile le résultat
  pop   ebx              ; depile la seconde operande dans ebx
  pop   eax              ; depile la permière operande dans eax
  sub   eax, ebx        
  push  eax              ; empile le résultat
  pop   ebx             
  mov  [ebp - 4], ebx    ; variable local
  push dword [ebp - 4]   ; variable local
  push    0             
  pop   ebx              ; depile la seconde operande dans ebx
  pop   eax              ; depile la permière operande dans eax
  cmp   eax, ebx        
  je   pgcd1_Si         
  jmp  pgcd2_Sinon      

  pgcd1_Si:                ; si
    push dword [ebp + 8]   ; argument
    pop   eax             
    mov  [ebp + 16], eax   ; ecriture de la valeur de retour
    jmp  pgcd4_END        
                          
    jmp  pgcd3_R          

  pgcd2_Sinon:             ; sinon
    sub   esp, 4           ; allocation valeur de retour
    push dword [ebp + 8]   ; argument
    push dword [ebp - 4]   ; variable local
    call pgcd             
    add   esp, 8           ; desallocation parametres
    pop   eax             
    mov  [ebp + 16], eax   ; ecriture de la valeur de retour
    jmp  pgcd4_END        

pgcd3_R:                 ; retour à la fonction

pgcd4_END:               ; appelé par un retour
  add   esp, 4           ; desallocation variables locales type 'entier' 
  pop   ebp              ; restaure la valeur de ebp
  ret                   

main:
  push  ebp              ; sauvegarde la valeur de ebp
  mov   ebp, esp         ; nouvelle valeur de ebp
  mov   eax, sinput     
  call readline         
  mov   eax, sinput     
  mov   eax, sinput     
  call atoi             
  push  eax             
  pop   ebx             
  mov  [$a], ebx         ; variable global
  mov   eax, sinput     
  call readline         
  mov   eax, sinput     
  mov   eax, sinput     
  call atoi             
  push  eax             
  pop   ebx             
  mov  [$b], ebx         ; variable global
  push dword [$b]        ; variable global
  push dword [$a]        ; variable global
  pop   ebx              ; depile la seconde operande dans ebx
  pop   eax              ; depile la permière operande dans eax
  cmp   eax, ebx        
  jl   main1_Si         
  jmp  main2_Sinon      

  main1_Si:                ; si
    sub   esp, 4           ; allocation valeur de retour
    push dword [$a]        ; variable global
    push dword [$b]        ; variable global
    call pgcd             
    add   esp, 8           ; desallocation parametres
    pop   eax             
    call iprintLF         
                          
    jmp  main3_R          

  main2_Sinon:             ; sinon
    sub   esp, 4           ; allocation valeur de retour
    push dword [$b]        ; variable global
    push dword [$a]        ; variable global
    call pgcd             
    add   esp, 8           ; desallocation parametres
    pop   eax             
    call iprintLF         

main3_R:                 ; retour à la fonction
  pop   ebp              ; restaure la valeur de ebp
  ret                   

