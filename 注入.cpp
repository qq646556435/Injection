#include<stdio.h>
#include<Windows.h>
#include<iostream>
#include<string>


//DLL远程线程注入
void CreateRemoteThread_Inject() {
	DWORD dwProcess = 0;
	char myDLL[] = "D:\\c++\\elementclient\\x64\\Debug\\elementclient.dll";
	//查找进程FlappyBird.exe
	 
	    unsigned long long pid = 0;
	    HWND 窗口句柄 = FindWindow(L"ZElementClient Window", L"诛仙");
		GetWindowThreadProcessId(窗口句柄, (LPDWORD) & pid);
		HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);    //打开进程
		LPVOID allocatedMem = VirtualAllocEx(hProcess, NULL, sizeof(myDLL), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);  //在进程中分配内存
		WriteProcessMemory(hProcess, allocatedMem, myDLL, sizeof(myDLL), NULL); //写入DLL路径
		HMODULE hModule = LoadLibrary(L"kernel32.dll");                //获取kernel32.dll模块句柄
		//获取LoadLibraryA函数地址
		LPTHREAD_START_ROUTINE pfnStarAddress = (LPTHREAD_START_ROUTINE)GetProcAddress(hModule, "LoadLibraryA");
		if (pfnStarAddress == NULL) {
			std::cout << "GetProcAddress failed" << std::endl;
			return;
		}
		HANDLE hRemoteThread = CreateRemoteThread(hProcess, NULL, NULL, pfnStarAddress, allocatedMem, NULL, NULL);  //创建远程线程
		if (hRemoteThread == NULL) {
			std::cout << "CreateRemoteThread failed" << std::endl;
			return;
		}
		WaitForSingleObject(hRemoteThread, INFINITE);  //等待远程线程结束
		CloseHandle(hRemoteThread);
		VirtualFreeEx(hProcess, allocatedMem, 0, MEM_FREE);
	
	return;
}

int main()
{
	//
	CreateRemoteThread_Inject();

	return 0;
}