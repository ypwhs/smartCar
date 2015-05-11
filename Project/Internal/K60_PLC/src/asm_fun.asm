	AREA    |.text|, CODE, READONLY

	EXPORT GetPcValue
	
GetPcValue
	PRESERVE8
	
	mov r0, pc ;return value stored in R0
	bx r14
	
	END
