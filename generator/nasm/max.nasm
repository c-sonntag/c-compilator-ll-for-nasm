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

max:
  push  ebp              ; sauvegarde la valeur de ebp
  mov   ebp, esp         ; nouvelle valeur de ebp
  push dword [ebp + 12]  ; argument
  push dword [ebp + 8]   ; argument
  pop   ebx              ; depile la seconde operande dans ebx
  pop   eax              ; depile la permière operande dans eax
  cmp   eax, ebx        
  jl   max1_Si          
  jmp  max2_R           

  max1_Si:                 ; si
    push dword [ebp + 8]   ; argument
    pop   eax             
    mov  [ebp + 16], eax   ; ecriture de la valeur de retour
    jmp  max3_END         

max2_R:                  ; retour à la fonction
  push dword [ebp + 12]  ; argument
  pop   eax             
  mov  [ebp + 16], eax   ; ecriture de la valeur de retour
  jmp  max3_END         

max3_END:                ; appelé par un retour
  pop   ebp              ; restaure la valeur de ebp
  ret                   

main:
  push  ebp              ; sauvegarde la valeur de ebp
  mov   ebp, esp         ; nouvelle valeur de ebp
  sub   esp, 8           ; allocation variables locales type 'entier'
  mov   eax, sinput     
  call readline         
  mov   eax, sinput     
  mov   eax, sinput     
  call atoi             
  push  eax             
  pop   ebx             
  mov  [ebp - 4], ebx    ; variable local
  mov   eax, sinput     
  call readline         
  mov   eax, sinput     
  mov   eax, sinput     
  call atoi             
  push  eax             
  pop   ebx             
  mov  [ebp - 8], ebx    ; variable local
  sub   esp, 4           ; allocation valeur de retour
  push dword [ebp - 4]   ; variable local
  push dword [ebp - 8]   ; variable local
  call  max             
  add   esp, 8           ; desallocation parametres
  push dword [ebp - 4]   ; variable local
  pop   ebx              ; depile la seconde operande dans ebx
  pop   eax              ; depile la permière operande dans eax
  cmp   eax, ebx        
  je   main1_Si         
  jmp  main2_Sinon      

  main1_Si:                ; si
    push dword [ebp - 4]   ; variable local
    pop   eax             
    call iprintLF         
                          
    jmp  main3_R          

  main2_Sinon:             ; sinon
    push dword [ebp - 8]   ; variable local
    pop   eax             
    call iprintLF         

main3_R:                 ; retour à la fonction
  add   esp, 8           ; desallocation variables locales type 'entier' 
  pop   ebp              ; restaure la valeur de ebp
  ret                   

