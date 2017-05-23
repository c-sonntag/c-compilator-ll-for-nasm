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
                          
    push 123456789        
    pop   eax             
    call iprintLF         
    push dword [ebp - 4]   ; variable local
    pop   eax             
    call iprintLF         
    push dword [ebp - 4]   ; variable local
    push    5             
    pop   ebx              ; depile la seconde operande dans ebx
    pop   eax              ; depile la permière operande dans eax
    call _cmpBin_je        ; comparaison expression
    push  eax             
    pop   eax             
    call iprintLF         
    push dword [ebp - 4]   ; variable local
    push    5             
    pop   ebx              ; depile la seconde operande dans ebx
    pop   eax              ; depile la permière operande dans eax
    call _cmpBin_jne       ; comparaison expression
    push  eax             
    pop   eax             
    call iprintLF         
    push dword [ebp - 4]   ; variable local
    push    5             
    pop   ebx              ; depile la seconde operande dans ebx
    pop   eax              ; depile la permière operande dans eax
    call _cmpBin_jg        ; comparaison expression
    push  eax             
    pop   eax             
    call iprintLF         
    push dword [ebp - 4]   ; variable local
    push    5             
    pop   ebx              ; depile la seconde operande dans ebx
    pop   eax              ; depile la permière operande dans eax
    call _cmpBin_jge       ; comparaison expression
    push  eax             
    pop   eax             
    call iprintLF         
    push dword [ebp - 4]   ; variable local
    push    5             
    pop   ebx              ; depile la seconde operande dans ebx
    pop   eax              ; depile la permière operande dans eax
    call _cmpBin_jl        ; comparaison expression
    push  eax             
    pop   eax             
    call iprintLF         
    push dword [ebp - 4]   ; variable local
    push    5             
    pop   ebx              ; depile la seconde operande dans ebx
    pop   eax              ; depile la permière operande dans eax
    call _cmpBin_jle       ; comparaison expression
    push  eax             
    pop   eax             
    call iprintLF         
    push dword [ebp - 4]   ; variable local
    push    5             
    pop   ebx              ; depile la seconde operande dans ebx
    pop   eax              ; depile la permière operande dans eax
    call _cmpBool_et       ; sous fonction d'opération
    push  eax              ; empile le résultat
    pop   eax             
    call iprintLF         
    push dword [ebp - 4]   ; variable local
    push    5             
    pop   ebx              ; depile la seconde operande dans ebx
    pop   eax              ; depile la permière operande dans eax
    call _cmpBool_ou       ; sous fonction d'opération
    push  eax              ; empile le résultat
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
                          
    jmp  main1_TqT        

main4_R:                 ; retour à la fonction
  add   esp, 4           ; desallocation variables locales type 'entier' 
  pop   ebp              ; restaure la valeur de ebp
  ret                   

_cmpBin__1:
  mov   eax, 1          
  ret                   

_cmpBin__0:
  mov   eax, 0          
  ret                   

_cmpBin_je:
  cmp   eax, ebx        
  je   _cmpBin__1       
  jmp  _cmpBin__0       

_cmpBin_jne:
  cmp   eax, ebx        
  jne  _cmpBin__1       
  jmp  _cmpBin__0       

_cmpBin_jl:
  cmp   eax, ebx        
  jl   _cmpBin__1       
  jmp  _cmpBin__0       

_cmpBin_jg:
  cmp   eax, ebx        
  jg   _cmpBin__1       
  jmp  _cmpBin__0       

_cmpBin_jle:
  cmp   eax, ebx        
  jle  _cmpBin__1       
  jmp  _cmpBin__0       

_cmpBin_jge:
  cmp   eax, ebx        
  jge  _cmpBin__1       
  jmp  _cmpBin__0       

_cmpBool__1:
  mov   eax, 1          
  ret                   

_cmpBool__0:
  mov   eax, 0          
  ret                   

_cmpBool_et:
  cmp   eax, 0          
  jne  _cmpBool_et1_2ndOp
  jmp  _cmpBool__0      

  _cmpBool_et1_2ndOp:
    cmp   ebx, 0          
    jne  _cmpBool__1      
    jmp  _cmpBool__0      

_cmpBool_ou:
  cmp   eax, 0          
  jne  _cmpBool__1      
  jmp  _cmpBool_ou1_2ndOp

  _cmpBool_ou1_2ndOp:
    cmp   ebx, 0          
    jne  _cmpBool__1      
    jmp  _cmpBool__0      

