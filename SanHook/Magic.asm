EXTERN ReturnPoint_ProcessPacketRecv:QWORD
EXTERN ProcessPacketRecv:PROC

.code
	intercept_ProcessPacketRecv PROC
		mov rdx, qword ptr [rdi + 8h]  ; RDX = packet
		mov r8d, dword ptr [rdi + 10h] ; R8 = Pakcet Length

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

		mov RCX, RBX                    ; RBX = messageId (double check this)
		call ProcessPacketRecv

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
		
		call qword ptr [rax + 8h]         ; Call original packet handler
		jmp ReturnPoint_ProcessPacketRecv ; Jump back to where we left off
	intercept_ProcessPacketRecv ENDP
end
