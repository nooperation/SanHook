EXTERN ReturnPoint_ProcessPacketRecv:QWORD
EXTERN ProcessPacketRecv:PROC

EXTERN ReturnPoint_ProcessPacketSend:QWORD
EXTERN ProcessPacketSend:PROC

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

		sub rsp, 16*16
		movdqu  [rsp + 16*0], xmm0
		movdqu  [rsp + 16*1], xmm1
		movdqu  [rsp + 16*2], xmm2
		movdqu  [rsp + 16*3], xmm3
		movdqu  [rsp + 16*4], xmm4
		movdqu  [rsp + 16*5], xmm5
		movdqu  [rsp + 16*6], xmm6
		movdqu  [rsp + 16*7], xmm7
		movdqu  [rsp + 16*8], xmm8
		movdqu  [rsp + 16*9], xmm9
		movdqu  [rsp + 16*10], xmm10
		movdqu  [rsp + 16*11], xmm11
		movdqu  [rsp + 16*12], xmm12
		movdqu  [rsp + 16*13], xmm13
		movdqu  [rsp + 16*14], xmm14
		movdqu  [rsp + 16*15], xmm15

		sub rsp, 24

		mov RCX, RBX				  ; RBX = messageId
		call ProcessPacketRecv

		add rsp, 24

		movdqu  xmm15, [rsp + 16*15]
		movdqu  xmm14, [rsp + 16*14]
		movdqu  xmm13, [rsp + 16*13]
		movdqu  xmm12, [rsp + 16*12]
		movdqu  xmm11, [rsp + 16*11]
		movdqu  xmm10, [rsp + 16*10]
		movdqu  xmm9, [rsp + 16*9]
		movdqu  xmm8, [rsp + 16*8]
		movdqu  xmm7, [rsp + 16*7]
		movdqu  xmm6, [rsp + 16*6]
		movdqu  xmm5, [rsp + 16*5]
		movdqu  xmm4, [rsp + 16*4]
		movdqu  xmm3, [rsp + 16*3]
		movdqu  xmm2, [rsp + 16*2]
		movdqu  xmm1, [rsp + 16*1]
		movdqu  xmm0, [rsp + 16*0]
		add rsp, 16*16

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

	intercept_ProcessPacketSend PROC
		;;;; pop rdx ; restore RDX, which we stored our jmp address in


		; R14 = [packet including messageIdPrefix]
		; R8 = packetLength
		
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
		
		sub rsp, 16*16
		movdqu  [rsp + 16*0], xmm0
		movdqu  [rsp + 16*1], xmm1
		movdqu  [rsp + 16*2], xmm2
		movdqu  [rsp + 16*3], xmm3
		movdqu  [rsp + 16*4], xmm4
		movdqu  [rsp + 16*5], xmm5
		movdqu  [rsp + 16*6], xmm6
		movdqu  [rsp + 16*7], xmm7
		movdqu  [rsp + 16*8], xmm8
		movdqu  [rsp + 16*9], xmm9
		movdqu  [rsp + 16*10], xmm10
		movdqu  [rsp + 16*11], xmm11
		movdqu  [rsp + 16*12], xmm12
		movdqu  [rsp + 16*13], xmm13
		movdqu  [rsp + 16*14], xmm14
		movdqu  [rsp + 16*15], xmm15
		
		sub rsp, 24
		
		; R14 = packet
		; R8 = packetLength
		
		mov rcx, r14  ; arg1 = packet
		mov rdx, r8   ; arg2 = packetLength
		call ProcessPacketSend ; ProcessPacketSend(packet, packetLength)
		
		add rsp, 24
		
		movdqu  xmm15, [rsp + 16*15]
		movdqu  xmm14, [rsp + 16*14]
		movdqu  xmm13, [rsp + 16*13]
		movdqu  xmm12, [rsp + 16*12]
		movdqu  xmm11, [rsp + 16*11]
		movdqu  xmm10, [rsp + 16*10]
		movdqu  xmm9, [rsp + 16*9]
		movdqu  xmm8, [rsp + 16*8]
		movdqu  xmm7, [rsp + 16*7]
		movdqu  xmm6, [rsp + 16*6]
		movdqu  xmm5, [rsp + 16*5]
		movdqu  xmm4, [rsp + 16*4]
		movdqu  xmm3, [rsp + 16*3]
		movdqu  xmm2, [rsp + 16*2]
		movdqu  xmm1, [rsp + 16*1]
		movdqu  xmm0, [rsp + 16*0]
		add rsp, 16*16
		
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

		mov qword ptr [rsp+78h], r15
		lea rcx, qword ptr [rax+1788h]

		jmp ReturnPoint_ProcessPacketSend ; Jump back to where we left off
	intercept_ProcessPacketSend ENDP
end
