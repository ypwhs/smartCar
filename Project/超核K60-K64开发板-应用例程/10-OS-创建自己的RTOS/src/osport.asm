NVIC_INT_CTRL   EQU     0xE000ED04                              ; Interrupt control state register.
NVIC_SYSPRI14   EQU     0xE000ED22                              ; System priority register (priority 14).
NVIC_PENDSV_PRI EQU           0xFF                              ; PendSV priority value (lowest).
NVIC_PENDSVSET  EQU     0x10000000                              ; Value to trigger PendSV exception.

    IMPORT  OSTaskSwHook
    EXTERN  OSTCBCur
    EXTERN  OSTCBHighRdy
        
    EXPORT OSCtxSw

        
    AREA |.text|, CODE, READONLY, ALIGN=2


OSCtxSw  PROC                   ;Trigger a PendSV Interrupt
    EXPORT  OSCtxSw
    LDR     R0, =NVIC_INT_CTRL
    LDR     R1, =NVIC_PENDSVSET
    STR     R1, [R0]
    BX      LR
    ENDP
        
PendSV_Handler  PROC            ;PendSV Interrupt
    EXPORT  PendSV_Handler
    
    CPSID   I
    ; call  OSTaskSwHook
    PUSH    {LR}                
    LDR     R0, =OSTaskSwHook
    BLX     R0
    POP     {LR}
    
    MRS     R0, PSP						;R0 now is PSP
    CBZ     R0, OS_CPU_PendSVHandler_nosave;
	
    ;save last content
	;R0 R1 R2 R3 R12 LR PC is auto pushed, and xSP is pointing to top of stack
	
    SUBS    R0, R0, #0x20				; sub 32(4*8) to get addr for R4-R11
    STM     R0, {R4-R11}				; save R4-R11 to stack
	
	; Modified OSTCBCur's SP to now's PSP
    LDR     R1, =OSTCBCur                  
    LDR     R1, [R1]
    STR     R0, [R1]    
    
OS_CPU_PendSVHandler_nosave
    ; load next content
    LDR     R0, =OSTCBCur                   ;OSPrioCur = OSPrioHighRdy;
    LDR     R1, =OSTCBHighRdy
    LDR     R2, [R1]
    STR     R2, [R0]
    ; PUSH R4-R11
    LDR     R0, [R2]                       ; R0 is new task's SP; SP = OSTCBHighRdy->OSTCBStkPtr;
    LDM     R0, {R4-R11}                   ; Restore R4-R11 from Stack
    ADDS    R0, R0, #0x20
    ; set MSP
    MSR     PSP, R0                         ; Load PSP with new process SP
    ORR     LR, LR, #0x04                   ; Ensure exception return uses process stack
    
    CPSIE   I
    BX      LR  ;expection return
    ENDP



    END
