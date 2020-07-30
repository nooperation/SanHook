EXTERN  ReturnPoint:QWORD
EXTERN ProcessClientRegionMessage:PROC

EXTERN ReturnPoint_HandleUpdateNotificationMessage:QWORD
EXTERN ProcessUpdateNotificationMessage:PROC


EXTERN ReturnPoint_HandleChatMessage:QWORD
EXTERN ProcessChatMessage:PROC

EXTERN ReturnPoint_HandleScriptConsoleStuff:QWORD
EXTERN ProcessScriptConsoleStuff:PROC

.code
	intercept_client_region_message PROC
		push rax
		push rbx
		push rcx
		push rdx
		push rbp
		push rdi
		push rsi
		push r8
		push r9
		push r10
		push r11
		push r12
		push r13
		push r14
		push r15

		mov RCX, RBX
		call ProcessClientRegionMessage

		pop r15
		pop r14
		pop r13
		pop r12
		pop r11
		pop r10
		pop r9
		pop r8
		pop rsi
		pop rdi
		pop rbp
		pop rdx
		pop rcx
		pop rbx
		pop rax

		mov qword ptr [rsp+10h],rbx  ; HandleClientRegionMessage_AdduserLogin
		mov qword ptr [rsp+20h],rsi  ; RBX = messageId | RDX = packet | R8 = length
		push rbp
		push rdi
		;push r12
		;push r14
		;push r15

		jmp ReturnPoint
	intercept_client_region_message ENDP


	intercept_HandleChatMessage PROC
		push rax
		push rbx
		push rcx
		push rdx
		push rbp
		push rdi
		push rsi
		push r8
		push r9
		push r10
		push r11
		push r12
		push r13
		push r14
		push r15

		mov RCX, RBX
		call ProcessChatMessage

		pop r15
		pop r14
		pop r13
		pop r12
		pop r11
		pop r10
		pop r9
		pop r8
		pop rsi
		pop rdi
		pop rbp
		pop rdx
		pop rcx
		pop rbx
		pop rax

		mov qword ptr [rsp+10h],rbx  ; HandleClientRegionMessage_AdduserLogin
		mov qword ptr [rsp+20h],rsi  ; RBX = messageId | RDX = packet | R8 = length
		push rbp
		push rdi

		jmp ReturnPoint_HandleChatMessage
	intercept_HandleChatMessage ENDP


	intercept_HandleUpdateNotificationMessage PROC
		push rax
		push rbx
		push rcx
		push rdx
		push rbp
		push rdi
		push rsi
		push r8
		push r9
		push r10
		push r11
		push r12
		push r13
		push r14
		push r15

		mov RCX, RBX
		call ProcessUpdateNotificationMessage

		pop r15
		pop r14
		pop r13
		pop r12
		pop r11
		pop r10
		pop r9
		pop r8
		pop rsi
		pop rdi
		pop rbp
		pop rdx
		pop rcx
		pop rbx
		pop rax


		mov qword ptr [rsp+10h],rbx  ; 
		push rbp
		push rsi
		push rdi
		lea rbp,qword ptr [rsp-1B20h]


		jmp ReturnPoint_HandleUpdateNotificationMessage
	intercept_HandleUpdateNotificationMessage ENDP

	intercept_HandleScriptConsoleStuff PROC
		push rax
		push rbx
		push rcx
		push rdx
		push rbp
		push rdi
		push rsi
		push r8
		push r9
		push r10
		push r11
		push r12
		push r13
		push r14
		push r15

		mov RCX, RBX
		call ProcessScriptConsoleStuff

		pop r15
		pop r14
		pop r13
		pop r12
		pop r11
		pop r10
		pop r9
		pop r8
		pop rsi
		pop rdi
		pop rbp
		pop rdx
		pop rcx
		pop rbx
		pop rax

		mov qword ptr [rsp+10h],rbx    ;Handle_ScriptConsoleLogStuff 
		push rbp
		push rsi
		push rdi
		lea rbp,qword ptr [rsp-1BC0h]   

		jmp ReturnPoint_HandleScriptConsoleStuff
	intercept_HandleScriptConsoleStuff ENDP

end
