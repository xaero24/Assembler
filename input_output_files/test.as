;Comment line just for shows
      
  
     .entry     MAIN       START
.extern   f1 f2       TAG
MAIN:    mov  START   , r0
         mov DATA ,  r1
add #230  , r1
lea                f1, r4

START:     lea     DATA, r5
lea STR, r7
    inc *r5
not r7
sub    r0,   *r7

;Loop of something
L1: cmp   r2, *r3
                inc *r3
add DATA   , #-19
jsr    SUBR
    jsr     f2
    jmp L1
  cmp #-13    , DATA
  jmp END







      ;Another loop here
 L2:     cmp    r5, #3
    dec    *r5
 bne    STR
cmp   TAG,   #904
jmp   L2
   cmp *r6   , r3
     jmp END

END: stop
clr r1
clr r2

;Data segment
DATA: .data 31, -2, 27, -9, 0, 16
STR: .string "assembly"
STR2: .string "Hello World!""""

SUBR:     inc *r2
add       r2, *r6
lea     STR2,       r3
sub r3,   *r2
ENDSUBR: rts

.entry    SUBR

        ;End of program
