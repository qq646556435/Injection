#include<stdio.h>
#include<Windows.h>
#include<iostream>
#include<string>


//DLLԶ���߳�ע��
void CreateRemoteThread_Inject() {
	DWORD dwProcess = 0;
	char myDLL[] = "D:\\c++\\elementclient\\x64\\Debug\\elementclient.dll";
	//���ҽ���FlappyBird.exe
	 
	    unsigned long long pid = 0;
	    HWND ���ھ�� = FindWindow(L"ZElementClient Window", L"����");
		GetWindowThreadProcessId(���ھ��, (LPDWORD) & pid);
		HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);    //�򿪽���
		LPVOID allocatedMem = VirtualAllocEx(hProcess, NULL, sizeof(myDLL), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);  //�ڽ����з����ڴ�
		WriteProcessMemory(hProcess, allocatedMem, myDLL, sizeof(myDLL), NULL); //д��DLL·��
		HMODULE hModule = LoadLibrary(L"kernel32.dll");                //��ȡkernel32.dllģ����
		//��ȡLoadLibraryA������ַ
		LPTHREAD_START_ROUTINE pfnStarAddress = (LPTHREAD_START_ROUTINE)GetProcAddress(hModule, "LoadLibraryA");
		if (pfnStarAddress == NULL) {
			std::cout << "GetProcAddress failed" << std::endl;
			return;
		}
		HANDLE hRemoteThread = CreateRemoteThread(hProcess, NULL, NULL, pfnStarAddress, allocatedMem, NULL, NULL);  //����Զ���߳�
		if (hRemoteThread == NULL) {
			std::cout << "CreateRemoteThread failed" << std::endl;
			return;
		}
		WaitForSingleObject(hRemoteThread, INFINITE);  //�ȴ�Զ���߳̽���
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