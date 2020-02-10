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

int main()
{
    PROCESS_INFORMATION process_info = {};
    STARTUPINFO startup_info = {};

    printf("Creating process...\n");
    auto result_create_process = CreateProcess(
        TEXT("C:\\Program Files\\Sansar\\Client\\SansarClient.exe"),
        (TCHAR*)TEXT("\"C:\\Program Files\\Sansar\\Client\\SansarClient.exe\" -console.visible 1 -enablesteamlogin 0 -crash.enableCrashReporting false -system.maxCores 1"),
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

    printf("Injecting dll...\n");
    auto injection_was_successful = InjectDll("R:\\cpp\\SanHook\\SanHook\\x64\\Debug\\SanHook.dll", process_info.hProcess);
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

    printf("Process resumed. Cleaning up...\n");
    CloseHandle(process_info.hThread);
    CloseHandle(process_info.hProcess);

    return injection_was_successful ? EXIT_SUCCESS : EXIT_FAILURE;
}
