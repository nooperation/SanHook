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
        //(TCHAR*)TEXT("\"C:\\Program Files\\Sansar\\Client\\SansarClient.exe\" -console.visible 1 -enablesteamlogin 0 -crash.enableCrashReporting false -sceneUri sansar://sansar.com/experience/nopnop/flatt"),
      //  (TCHAR*)TEXT("\"C:\\Program Files\\Sansar\\Client\\SansarClient.exe\" -console.visible 1 -enablesteamlogin 0 -crash.enableCrashReporting false -audio.attenuationVoiceFactor 0.01 "),
        (TCHAR*)TEXT("\"C:\\Program Files\\Sansar\\Client\\SansarClient.exe\" -console.visible 1 -enablesteamlogin 0 -crash.enableCrashReporting false  -singleInstance 0"),
       // (TCHAR*)TEXT("\"C:\\Program Files\\Sansar\\Client\\SansarClient.exe\" -console.visible 1 -enablesteamlogin 0 -crash.enableCrashReporting false -system.maxCores 1"),
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
        /// GRAVITYS (right above collision hack, look for "skyCubemap")
        ///////////////////////////////////

        uint8_t gravityHack[] = {
            0xB8, 0x14, 0xAE, 0xE7, 0x3F, 0x90, 0x90
        };
        *((float*)&gravityHack[1]) = 2.0f;

        WriteProcessMemory(process_info.hProcess, (void*)(base + 0x175366A), gravityHack, sizeof(gravityHack), &written);
        printf("Written to %X: %d bytes\n", base + 0x175366A, written);



        uint8_t collisionHack[] = {
           // 0x33, 0xC0, 0xFF, 0xC0, 0x89, 0x87, 0xB4, 0x02, 0x00, 0x00, 0x90, 0x90
             0xB0, 0x01, 0x90
        };
        //WriteProcessMemory(process_info.hProcess, (void*)(base + 0x17536B3), collisionHack, sizeof(collisionHack), &written);
        printf("Written to %X: %d bytes\n", base + 0x17536B3, written);
    

        ///////////////////////////////////
        /// FREECAM - ALLOW ALWAYS (rigth below freecam hack, look for "skyCubemap" (THIRD DOWN FROM 'defaultSkyCubeMap') (func below has 'environment' reference)
        ///////////////////////////////////
        uint8_t freecamHack[] = {
            0xB0, 0x01, 0x90
        };
        written = 0;
        WriteProcessMemory(process_info.hProcess, (void*)(base + 0x17536FE), freecamHack, sizeof(freecamHack), &written);
        printf("Written to %X: %d bytes\n", base + 0x17536FE, written);


        uint8_t inventoryAlwaysHack[] = {
           0xB0, 0x01, 0x88, 0x07
        };
        written = 0;
        WriteProcessMemory(process_info.hProcess, (void*)(base + 0x1759CF5), inventoryAlwaysHack, sizeof(inventoryAlwaysHack), &written);
        printf("Written to %X: %d bytes\n", base + 0x1759CF5, written);



        //uint8_t hijackClientRegionMessage_AddUserLogin[] = {
        //    0x48, 0xB8, 0x30, 0x44, 0x8A, 0xEF, 0xF6, 0x7F, 0x00, 0x00, 0xFF, 0x20
        //};
        //*((uint64_t*)&hijackClientRegionMessage_AddUserLogin[2]) = 0;
        //uint64_t hijackClientRegionMessage_RETURN = 0x1242F9C;
        //written = 0;
        //WriteProcessMemory(process_info.hProcess, (void*)(base + 0x1242F90), inventoryAlwaysHack, sizeof(inventoryAlwaysHack), &written);
        //printf("Written to %X: %d bytes\n", base + 0x1242F90, written);






        /*
        // see main.cpp "no-input-source"
        uint8_t no1minTelemetry[] = {
            0xEB, 0x08
        };
        auto telemetryCallerAddress = 0x1165D9B;
        written = 0;
        //WriteProcessMemory(process_info.hProcess, (void*)(base + telemetryCallerAddress), no1minTelemetry, sizeof(no1minTelemetry), &written);
        printf("Written to %X: %d bytes\n", base + telemetryCallerAddress, written);
        */
    }


    printf("Process resumed. Cleaning up...\n");
    CloseHandle(process_info.hThread);
    CloseHandle(process_info.hProcess);

    return injection_was_successful ? EXIT_SUCCESS : EXIT_FAILURE;
}

