#include <windows.h>
#include <tlhelp32.h>

// The full path to the DLL to be injected.
const char* dll_path = "D:\\projects\\cpp\\re4mp\\Debug\\inject.dll";

int main(int argc, char** argv)
{
	HANDLE snapshot = 0;
	PROCESSENTRY32 pe32 = { 0 };

	DWORD exitCode = 0;

	pe32.dwSize = sizeof(PROCESSENTRY32);

	// The snapshot code is a reduced version of the example code provided by Microsoft at 
	// https://docs.microsoft.com/en-us/windows/win32/toolhelp/taking-a-snapshot-and-viewing-processes
	snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	Process32First(snapshot, &pe32);

	do
	{
		// We only want to operate on the Urban Terror process
		if (wcscmp(pe32.szExeFile, L"bio4.exe") == 0)
		{
			// First, we need to get a process handle to use for the following calls
			HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, true, pe32.th32ProcessID);

			// So we don't corrupt any memory, allocate additional memory to hold our DLL path
			void* lpBaseAddress = VirtualAllocEx(process, NULL, strlen(dll_path) + 1, MEM_COMMIT, PAGE_READWRITE);

			// Write our DLL path into the memory we just allocated inside the game
			WriteProcessMemory(process, lpBaseAddress, dll_path, strlen(dll_path) + 1, NULL);

			// Create a remote thread inside the game that will execute LoadLibraryA
			// To this LoadLibraryA call, we will pass the full path of our DLL that we wrote into the game
			HMODULE kernel32base = GetModuleHandle(L"kernel32.dll");
			HANDLE thread = CreateRemoteThread(process, NULL, 0, (LPTHREAD_START_ROUTINE)GetProcAddress(kernel32base, "LoadLibraryA"), lpBaseAddress, 0, NULL);

			// To make sure that our DLL is injected, we can use the following two calls to block program execution
			WaitForSingleObject(thread, INFINITE);
			GetExitCodeThread(thread, &exitCode);

			// Finally free the memory and clean up the process handles
			VirtualFreeEx(process, lpBaseAddress, 0, MEM_RELEASE);
			CloseHandle(thread);
			CloseHandle(process);
			break;
		}
	} while (Process32Next(snapshot, &pe32));

	return 0;
}
