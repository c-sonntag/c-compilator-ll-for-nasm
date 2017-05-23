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

  main1_Si:                ; si
    push    1             
    pop   eax             
    call iprintLF         
                          
    jmp  main3_R          

  main2_Sinon:             ; sinon
    push    0             
    pop   eax             
    call iprintLF         

main3_R:                 ; retour à la fonction
  jmp  main5_Sinon      

  main4_Si:                ; si
    push    1             
    pop   eax             
    call iprintLF         
                          
    jmp  main6_R          

  main5_Sinon:             ; sinon
    push    0             
    pop   eax             
    call iprintLF         

main6_R:                 ; retour à la fonction
  add   esp, 4           ; desallocation variables locales type 'entier' 
  pop   ebp              ; restaure la valeur de ebp
  ret                   

