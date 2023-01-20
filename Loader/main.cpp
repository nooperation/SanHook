#include <windows.h>
#include <iostream>
#include <filesystem>

void DumpError(DWORD error_code)
{
    TCHAR *messageBuffer = nullptr;

    auto result = FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_FROM_SYSTEM,
        nullptr,
        error_code,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&messageBuffer,
        0,
        nullptr
    );

    if(result == 0)
    {
        printf("Error: Unknown error code %d\n", error_code);
    }
    else
    {
        std::cout << "Error: " << &messageBuffer << std::endl;
        LocalFree(messageBuffer);
    }
}

bool InjectDll(std::filesystem::path dll_path, HANDLE process_handle)
{
    auto payload = dll_path.wstring();

    auto remote_memory = VirtualAllocEx(
        process_handle,
        nullptr,
        sizeof(payload[0]) * payload.length(),
        MEM_COMMIT,
        PAGE_READWRITE
    );
    if(remote_memory == nullptr)
    {
        printf("Failed to allocate memory in remote process\n");
        return false;
    }

    std::size_t number_of_bytes_written = 0;
    auto result_write_memory = WriteProcessMemory(
        process_handle,
        remote_memory,
        payload.c_str(),
        payload.length() * sizeof(payload[0]),
        &number_of_bytes_written
    );
    if(result_write_memory == false)
    {
        printf("Failed to write payload to process\n");
        return false;
    }

    auto kernel32_handle = GetModuleHandleW(TEXT("Kernel32"));
    if(kernel32_handle == nullptr)
    {
        printf("Failed to find module 'Kernel32'\n");
        return false;
    }

    auto remote_entrypoint = (LPTHREAD_START_ROUTINE)GetProcAddress(kernel32_handle, "LoadLibraryW");
    if(remote_entrypoint == nullptr)
    {
        printf("Failed to find process 'LoadLibraryW' in 'Kernel32'\n");
        return false;
    }

    auto remote_thread = CreateRemoteThread(
        process_handle,
        nullptr,
        0,
        remote_entrypoint,
        remote_memory,
        0,
        nullptr
    );
    if(remote_thread == nullptr)
    {
        printf("Failed to create remote thread\n");
        return false;
    }

    return true;
}

#include <tlhelp32.h>
uint64_t GetBaseAddress(DWORD process_id)
{
    uintptr_t modBaseAddr = 0;
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE| TH32CS_SNAPALL, process_id);
    if (hSnap == INVALID_HANDLE_VALUE)
    {
        return 0;
    }
     
    MODULEENTRY32 module_entry = {0};
    module_entry.dwSize = sizeof(module_entry);

    if (Module32First(hSnap, &module_entry))
    {
        do
        {
            if (module_entry.th32ProcessID == process_id)
            {
                return (uint64_t)module_entry.modBaseAddr;
            }
        } while (Module32Next(hSnap, &module_entry));
    }

    CloseHandle(hSnap);
    return 0;
}

#include <filesystem>
int main()
{
    PROCESS_INFORMATION process_info = {};
    STARTUPINFO startup_info = {};

    printf("Creating process...\n");
    auto result_create_process = CreateProcess(
        TEXT("C:\\Program Files\\Sansar\\Client\\SansarClient.exe"),
       // (TCHAR*)TEXT("\"C:\\Program Files\\Sansar\\Client\\SansarClient.exe\" -console.visible 1 -enablesteamlogin 0 -crash.enableCrashReporting false -sceneUri sansar://sansar.com/experience/nopnop/flatt "),
       // (TCHAR*)TEXT("\"C:\\Program Files\\Sansar\\Client\\SansarClient.exe\" -console.visible 1 -enablesteamlogin 0 -crash.enableCrashReporting false -audio.attenuationVoiceFactor 0.01 -enableVR false"),
       // (TCHAR*)TEXT("\"C:\\Program Files\\Sansar\\Client\\SansarClient.exe\" -console.visible 1 -enablesteamlogin 0 -crash.enableCrashReporting false  -singleInstance 1  -system.maxCores 1 -enableVR false"),
       // (TCHAR*)TEXT("\"C:\\Program Files\\Sansar\\Client\\SansarClient.exe\" -console.visible 1 -enablesteamlogin 0 -crash.enableCrashReporting false  -singleInstance 1 -enableVR false"),
       // (TCHAR*)TEXT("\"C:\\Program Files\\Sansar\\Client\\SansarClient.exe\" -console.visible 1 -enablesteamlogin 0 -crash.enableCrashReporting false  -singleInstance 1  -logging.logFilePath \"r:\\sansar123log.txt\" -logging.logAllTagged 1 -logging.logUntagged 0 -logging.disableTags Inventory ResourceLoader ComponentManager MeshDuplicateCheck EventQueue AssetSystem TextureStreamingManager TextureBudgetShareholder UserInputHints Audio UIPanelManager UIWorkspaceManager MaterialTypeDictionary LLHttp::HttpMessageParser LoadingProgress"),
       // (TCHAR*)TEXT("\"C:\\Program Files\\Sansar\\Client\\SansarClient.exe\" -console.visible 1 -enablesteamlogin 0 -crash.enableCrashReporting false  -singleInstance 1 -enableVR false"),
       // (TCHAR*)TEXT("\"C:\\Program Files\\Sansar\\Client\\SansarClient.exe\" -console.visible 0 -enablesteamlogin 0 -crash.enableCrashReporting false  -singleInstance 1 "),
       
       // (TCHAR*)TEXT("\"C:\\Program Files\\Sansar\\Client\\SansarClient.exe\" -enablesteamlogin 0 -crash.enableCrashReporting false  -singleInstance 1  "), // -networking.useTcp 1
       // (TCHAR*)TEXT("\"C:\\Program Files\\Sansar\\Client\\SansarClient.exe\" -console.visible 1 -enablesteamlogin 0 -crash.enableCrashReporting false  -singleInstance 0  "), // -networking.useTcp 1



 // (TCHAR*)TEXT("\"C:\\Program Files\\Sansar\\Client\\SansarClient.exe\" -graphics.maxFps 30 -console.visible 1 -enablesteamlogin 0 -crash.enableCrashReporting false  -singleInstance 1  -enableVR false  -media.enabled false"), // -networking.useTcp 1
 
       (TCHAR*)TEXT("\"C:\\Program Files\\Sansar\\Client\\SansarClient.exe\"  -console.visible 1 -enablesteamlogin 0 -crash.enableCrashReporting false  -singleInstance 1  -enableVR false -media.enabled true -developer true "), // -networking.useTcp 1
//(TCHAR*)TEXT("\"C:\\Program Files\\Sansar\\Client\\SansarClient.exe\"  -console.visible 1 -enablesteamlogin 0 -crash.enableCrashReporting false  -singleInstance 1  -enableVR false  -log.logAllTagged true -log.disableTags ComponentManager MeshDuplicateCheck AssetSystem EventQueue ResourceLoader TextureStreamingManager"), // -networking.useTcp 1
//(TCHAR*)TEXT("\"C:\\Program Files\\Sansar\\Client\\SansarClient.exe\"  -console.visible 1 -enablesteamlogin 0 -crash.enableCrashReporting false  -singleInstance 1  -enableVR false  -log.tags Configuration Application"), // -networking.useTcp 1

                                                                                                                                                                                                                                                                                                                                  //(TCHAR*)TEXT("\"C:\\Program Files\\Sansar\\Client\\SansarClient.exe\"  -console.visible 1 -enablesteamlogin 0 -crash.enableCrashReporting false  -singleInstance 1  -enableVR false  -log.logAllTagged true -log.disableTags ComponentManager MeshDuplicateCheck  AssetSystem TextureStreamingManager ResourceLoader WorldStateManager HttpClient WindowsUdpDriver"), // -networking.useTcp 1





   //(TCHAR*)TEXT("\"C:\\Program Files\\Sansar\\Client\\SansarClient.exe\" -console.visible 1 -enablesteamlogin 0 -crash.enableCrashReporting false  -singleInstance 0  -enableVR false  -media.enabled false"), // -networking.useTcp 1
    //   (TCHAR*)TEXT("\"C:\\Program Files\\Sansar\\Client\\SansarClient.exe\" -console.visible 1 -crash.enableCrashReporting false  -enablesteamlogin false -enableVR true  -singleInstance 1 -media.enabled false"), // -connection.conductor.insecureHost true

                                                                                                                                                                                            
                                                                                                                                                                                            
                                                                                                                                                                                            //  (TCHAR*)TEXT("\"C:\\Program Files\\Sansar\\Client\\SansarClient.exe\" -console.visible 1 -enablesteamlogin 0 -crash.enableCrashReporting false  -singleInstance 1 -enableVR false -logging.logAllTagged 0 -logging.logUntagged 0 -logging.tags OutgoingPacket IncomingPacket HttpClient ClientVoiceApi Voice ClientApplication Config"),
     //  (TCHAR*)TEXT("\"C:\\Program Files\\Sansar\\Client\\SansarClient.exe\" -console.visible 1 -enablesteamlogin 0 -crash.enableCrashReporting false  -singleInstance 1 -enableVR false -logging.logAllTagged 0 -logging.logUntagged 1 -logging.logFilePath \"r:\\sansar123log.txt\" -logging.tags Application OutgoingPacket IncomingPacket HttpClient ClientVoiceApi Voice ClientApplication Config"),
        // (TCHAR*)TEXT("\"C:\\Program Files\\Sansar\\Client\\SansarClient.exe\" -console.visible 1 -enablesteamlogin 0 -crash.enableCrashReporting false  -singleInstance 1 -enableVR false -logging.logAllTagged 0 -logging.logUntagged 0 -logging.disableTags DataSyncRoot ClientUI Window State"),
       // (TCHAR*)TEXT("\"C:\\Program Files\\Sansar\\Client\\SansarClient.exe\" -console.visible 1 -enablesteamlogin 0 -crash.enableCrashReporting false -system.maxCores 1 -enableVR false"),
        nullptr,
        nullptr,
        false,
        CREATE_SUSPENDED,
        nullptr,
        nullptr,
        &startup_info,
        &process_info
    );
    if(result_create_process == false)
    {
        printf("Failed to create process: %d\n", GetLastError()); 
        return 0;
    }
    

    auto dllPath = std::filesystem::current_path() / "SanHook.dll";

    printf("Injecting dll...\n");
#ifndef _DEBUG
    auto injection_was_successful = InjectDll(dllPath.c_str(), process_info.hProcess);
#else
    auto injection_was_successful = InjectDll(dllPath.c_str(), process_info.hProcess);
#endif

    if(injection_was_successful == false)
    {
        DumpError(GetLastError());
    }
    else
    {
        printf("OK\n");
    }

   // system("pause");
    printf("Resuming process...\n");
    ResumeThread(process_info.hThread);

    Sleep(500);

    if (true)
    {
        auto base = GetBaseAddress(process_info.dwProcessId);
        SIZE_T written = 0;

        // NOTE: COPY RVA, NOT FILE OFFSET!
        // NOTE: COPY RVA, NOT FILE OFFSET!
        // NOTE: COPY RVA, NOT FILE OFFSET!
        // NOTE: COPY RVA, NOT FILE OFFSET!

        ///////////////////////////////////
        /// GRAVITYS (right above collision hack, look for "skyCubemap", 4th one down)
        // skyCubemap/gravhack/camhack/etc (the second one?):
        // 48 3B C1 77 12 8B 02 89 87 B0 02 00 00 48 83 83 68 1B 00 00 04 EB 0E
        ///////////////////////////////////
        /*
            00007FF7B9780580 | 48:3BC1                         | cmp rax,rcx                                                  | Gravity hack...?
            00007FF7B9780583 | 77 12                           | ja sansarclient.7FF7B9780597                                 |
            00007FF7B9780585 | 8B02                            | mov eax,dword ptr ds:[rdx]                                   | rdx:EntryPoint
            00007FF7B9780587 | 8987 B0020000                   | mov dword ptr ds:[rdi+2B0],eax                               |
            00007FF7B978058D | 48:8383 681B0000 04             | add qword ptr ds:[rbx+1B68],4                                |
            00007FF7B9780595 | EB 0E                           | jmp sansarclient.7FF7B97805A5                                |
            00007FF7B9780597 | C683 581B0000 01                | mov byte ptr ds:[rbx+1B58],1                                 |
            00007FF7B978059E | 48:898B 681B0000                | mov qword ptr ds:[rbx+1B68],rcx                              |
            00007FF7B97805A5 | 83FD 08                         | cmp ebp,8                                                    |
            00007FF7B97805A8 | 7C 41                           | jl sansarclient.7FF7B97805EB                                 |
            00007FF7B97805AA | 48:8B93 681B0000                | mov rdx,qword ptr ds:[rbx+1B68]                              | rdx:EntryPoint
            00007FF7B97805B1 | 48:8B8B 701B0000                | mov rcx,qword ptr ds:[rbx+1B70]                              |
            00007FF7B97805B8 | 44:89AB 781B0000                | mov dword ptr ds:[rbx+1B78],r13d                             |
            00007FF7B97805BF | 48:8D42 01                      | lea rax,qword ptr ds:[rdx+1]                                 | rax:EntryPoint, rdx+1:sub_7FF7B9E0624C+9
            00007FF7B97805C3 | 48:3BC1                         | cmp rax,rcx                                                  | rax:EntryPoint
            00007FF7B97805C6 | 77 15                           | ja sansarclient.7FF7B97805DD                                 |
            00007FF7B97805C8 | 44:382A                         | cmp byte ptr ds:[rdx],r13b                                   | Avatar collision
        */

        auto gravityHackAddr = (void *)(base + 0x17A7B9B);
        uint8_t gravityHack[] = {
            0xB8, 0x14, 0xAE, 0xE7, 0x3F, 0x90, 0x90
        };
        *((float*)&gravityHack[1]) = 6.0f;

      WriteProcessMemory(process_info.hProcess, gravityHackAddr, gravityHack, sizeof(gravityHack), &written);
        printf("Written to %X: %d bytes\n", gravityHackAddr, written);


        ///////////////////////////////////
        /// COLLISION - ALWAYS COLLIDE AVATARS (right above freecam hack, look for "skyCubemap")
        ///////////////////////////////////
        /*
            00007FF7B97805C8 | 44:382A                         | cmp byte ptr ds:[rdx],r13b                                   | Avatar collision
            00007FF7B97805CB | 0F95C0                          | setne al                                                     |
            00007FF7B97805CE | 8887 B4020000                   | mov byte ptr ds:[rdi+2B4],al                                 |
            00007FF7B97805D4 | 48:FF83 681B0000                | inc qword ptr ds:[rbx+1B68]                                  |
            00007FF7B97805DB | EB 0E                           | jmp sansarclient.7FF7B97805EB                                |
            00007FF7B97805DD | C683 581B0000 01                | mov byte ptr ds:[rbx+1B58],1                                 |
            00007FF7B97805E4 | 48:898B 681B0000                | mov qword ptr ds:[rbx+1B68],rcx                              |
            00007FF7B97805EB | 83FD 0A                         | cmp ebp,A                                                    | A:'\n'
            00007FF7B97805EE | 0F8C 84000000                   | jl sansarclient.7FF7B9780678                                 |
            00007FF7B97805F4 | 48:8B93 681B0000                | mov rdx,qword ptr ds:[rbx+1B68]                              | rdx:EntryPoint
            00007FF7B97805FB | 48:8B8B 701B0000                | mov rcx,qword ptr ds:[rbx+1B70]                              |
            00007FF7B9780602 | 44:89AB 781B0000                | mov dword ptr ds:[rbx+1B78],r13d                             |
            00007FF7B9780609 | 48:8D42 01                      | lea rax,qword ptr ds:[rdx+1]                                 | rax:EntryPoint, rdx+1:sub_7FF7B9E0624C+9
            00007FF7B978060D | 48:3BC1                         | cmp rax,rcx                                                  | rax:EntryPoint
            00007FF7B9780610 | 77 15                           | ja sansarclient.7FF7B9780627                                 |
            00007FF7B9780612 | 44:382A                         | cmp byte ptr ds:[rdx],r13b                                   | AllowFreecamera
            00007FF7B9780615 | 0F95C0                          | setne al                                                     |
        */

        auto collisionHackAddr = (void *)(base + 0x17A7BE4);
        uint8_t collisionHack[] = {
           // 0x33, 0xC0, 0xFF, 0xC0, 0x89, 0x87, 0xB4, 0x02, 0x00, 0x00, 0x90, 0x90
             0xB0, 0x01, 0x90
        };
        //WriteProcessMemory(process_info.hProcess, collisionHackAddr, collisionHack, sizeof(collisionHack), &written);
       // printf("Written to %X: %d bytes\n", collisionHackAddr, written);
    

        ///////////////////////////////////
        /// FREECAM - ALLOW ALWAYS (rigth below freecam hack, look for "skyCubemap" (THIRD DOWN FROM 'defaultSkyCubeMap') (func below has 'environment' reference)
        ///////////////////////////////////
         /*
            00007FF7B9780612 | 44:382A                         | cmp byte ptr ds:[rdx],r13b                                   | AllowFreecamera
            00007FF7B9780615 | 0F95C0                          | setne al                                                     |
            00007FF7B9780618 | 8887 B5020000                   | mov byte ptr ds:[rdi+2B5],al                                 |
            00007FF7B978061E | 48:FF83 681B0000                | inc qword ptr ds:[rbx+1B68]                                  |
            00007FF7B9780625 | EB 0E                           | jmp sansarclient.7FF7B9780635                                |
            00007FF7B9780627 | C683 581B0000 01                | mov byte ptr ds:[rbx+1B58],1                                 |
            00007FF7B978062E | 48:898B 681B0000                | mov qword ptr ds:[rbx+1B68],rcx                              |
            00007FF7B9780635 | 83FD 0A                         | cmp ebp,A                                                    | A:'\n'
            00007FF7B9780638 | 7C 3E                           | jl sansarclient.7FF7B9780678                                 |
            00007FF7B978063A | 48:8B93 681B0000                | mov rdx,qword ptr ds:[rbx+1B68]                              | rdx:EntryPoint
            00007FF7B9780641 | 48:8B8B 701B0000                | mov rcx,qword ptr ds:[rbx+1B70]                              |
            00007FF7B9780648 | 44:89AB 781B0000                | mov dword ptr ds:[rbx+1B78],r13d                             |
            00007FF7B978064F | 48:8D42 04                      | lea rax,qword ptr ds:[rdx+4]                                 | rax:EntryPoint, rdx+4:sub_7FF7B9E0624C+C
            00007FF7B9780653 | 48:3BC1                         | cmp rax,rcx                                                  | rax:EntryPoint
            00007FF7B9780656 | 77 12                           | ja sansarclient.7FF7B978066A                                 |
            00007FF7B9780658 | 8B02                            | mov eax,dword ptr ds:[rdx]                                   | eax = TeleportRangeMaximum
            00007FF7B978065A | 8987 B8020000                   | mov dword ptr ds:[rdi+2B8],eax                               |
        */

        auto freecamHackAddr = (void *)(base + 0x17A7C2F);
        uint8_t freecamHack[] = {
            0xB0, 0x01, 0x90
        };
        written = 0;
        WriteProcessMemory(process_info.hProcess, freecamHackAddr, freecamHack, sizeof(freecamHack), &written);
        printf("Written to %X: %d bytes\n", freecamHackAddr, written);


    
        ///////////////////////////////////
        // updated for 2020-05-23
        /// INVENTORY - ALLOW ALWAYS (search for 'exposureBias', function just above that) 
        // exposurebias (other main, the function below is parser with __verison):
        // 8B 01 89 07 48 83 83 68 1B 00 00 04 48 8B 93 68 1B 00 00 48 8B 83 70 1B 00 00
        ///////////////////////////////////
        /*
            00007FF7B9786640 | 48:895C24 10                    | mov qword ptr ss:[rsp+10],rbx                                |
            00007FF7B9786645 | 57                              | push rdi                                                     |
            00007FF7B9786646 | 48:83EC 20                      | sub rsp,20                                                   |
            00007FF7B978664A | 48:8BDA                         | mov rbx,rdx                                                  | rdx:EntryPoint
            00007FF7B978664D | 4C:8D05 2C45AAFF                | lea r8,qword ptr ds:[<VERSION_RuntimeInventorySettings>]     |
            00007FF7B9786654 | 48:8BF9                         | mov rdi,rcx                                                  |
            00007FF7B9786657 | BA 01000000                     | mov edx,1                                                    |
            00007FF7B978665C | 48:8BCB                         | mov rcx,rbx                                                  |
            00007FF7B978665F | E8 7C8EA7FF                     | call <sansarclient.ReadVersion>                              |
            00007FF7B9786664 | 48:8B8B 681B0000                | mov rcx,qword ptr ds:[rbx+1B68]                              |
            00007FF7B978666B | 45:33C0                         | xor r8d,r8d                                                  |
            00007FF7B978666E | 48:8B93 701B0000                | mov rdx,qword ptr ds:[rbx+1B70]                              | rdx:EntryPoint
            00007FF7B9786675 | 44:8983 781B0000                | mov dword ptr ds:[rbx+1B78],r8d                              |
            00007FF7B978667C | 48:8D41 04                      | lea rax,qword ptr ds:[rcx+4]                                 | rax:EntryPoint
            00007FF7B9786680 | 48:3BC2                         | cmp rax,rdx                                                  | rax:EntryPoint, rdx:EntryPoint
            00007FF7B9786683 | 77 1C                           | ja sansarclient.7FF7B97866A1                                 |
            00007FF7B9786685 | 8B01                            | mov eax,dword ptr ds:[rcx]                                   | 0 = None, 1= Sansar Library, 2 = self library? <-- patch here
            00007FF7B9786687 | 8907                            | mov dword ptr ds:[rdi],eax                                   |
        */

        auto inventoryAlwaysHackAddr = (void *)(base + 0x17AE025);
        uint8_t inventoryAlwaysHack[] = {
           0xB0, 0x01, 0x88, 0x07
        };
        written = 0;
        WriteProcessMemory(process_info.hProcess, inventoryAlwaysHackAddr, inventoryAlwaysHack, sizeof(inventoryAlwaysHack), &written);
        printf("Written to %X: %d bytes\n", inventoryAlwaysHackAddr, written);

        //uint8_t hijackClientRegionMessage_AddUserLogin[] = {
        //    0x48, 0xB8, 0x30, 0x44, 0x8A, 0xEF, 0xF6, 0x7F, 0x00, 0x00, 0xFF, 0x20
        //};
        //*((uint64_t*)&hijackClientRegionMessage_AddUserLogin[2]) = 0;
        //uint64_t hijackClientRegionMessage_RETURN = 0x1242F9C;
        //written = 0;
        //WriteProcessMemory(process_info.hProcess, (void*)(base + 0x1242F90), inventoryAlwaysHack, sizeof(inventoryAlwaysHack), &written);
        //printf("Written to %X: %d bytes\n", base + 0x1242F90, written);





        
        // see main.cpp "no-input-source"
        uint8_t no1minTelemetry[] = {
            0xEB, 0x08
        };
        auto telemetryCallerAddress = 0x1167775;
        written = 0;
        WriteProcessMemory(process_info.hProcess, (void*)(base + telemetryCallerAddress), no1minTelemetry, sizeof(no1minTelemetry), &written);
        printf("Written to %X: %d bytes\n", base + telemetryCallerAddress, written);
        
    }


    printf("Process resumed. Cleaning up...\n");
    CloseHandle(process_info.hThread);
    CloseHandle(process_info.hProcess);

    return injection_was_successful ? EXIT_SUCCESS : EXIT_FAILURE;
}

