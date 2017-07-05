.syntax unified
.global main

@Justin Distasio
@Sonia Mannan
main: 
   @Stack the return addr (lr) in addition to a dummy
   @reg (ip) to keep the stack 8 byte aligned

   push {ip, lr}

   @Load the arg and perform the call. Similar to printf
   @in C.

   ldr r0, =label
   bl printf
   ldr r0, =label1
   bl printf
   ldr r0, =label2
   bl printf

   @Exit from 'main', similar to 'return 0'
   mov r0, #0

   @Pop dummy ip to reverse alignment, and pop original
   @lr val directly into Program Counter (pc) to return
   pop {ip, pc}

   @Data for printf call. GNU assembler ".asciz" directive
   @automatically adds NULL char termination.

label:
   .asciz "This is a haiku\n"

label1:
   .asciz "Raspberri Pis are the best\n"

label2:
   .asciz "This haiku is done\n"
