#include <iostream>
#include <string>
#include <limits>
#include <windows.h>
#include <shellapi.h>
using namespace std;

// Function to check if the current user has administrator privileges
bool IsRunAsAdmin()
{
    BOOL fIsRunAsAdmin = FALSE;
    PSID pAdminSid = NULL;
    DWORD dwSize = SECURITY_MAX_SID_SIZE;
    
    // Allocate memory for the SID
    pAdminSid = LocalAlloc(LMEM_FIXED, SECURITY_MAX_SID_SIZE);
    if (pAdminSid == NULL)
        return FALSE;

    // Create the well-known SID for the admin group
    if (CreateWellKnownSid(WinBuiltinAdministratorsSid, NULL, pAdminSid, &dwSize))
    {
        if (!CheckTokenMembership(NULL, pAdminSid, &fIsRunAsAdmin))
        {
            fIsRunAsAdmin = FALSE;
        }
    }
    
    LocalFree(pAdminSid);
    return fIsRunAsAdmin != FALSE;
}

void askReboot() {
    char response;
    cout << "\nDo you want to restart your computer now? (y/n): ";
    cin >> response;
    
    if (response == 'y' || response == 'Y') {
        cout << "Restarting computer..." << endl;
        system("shutdown /r /t 10");
    } else {
        cout << "Restart cancelled." << endl;
    }
}

int main() {
    system("@echo off");
    system("title toolset -- GitHub: 0xKale");
    system("color 0a");
    // Check for admin privileges
    if(!IsRunAsAdmin())
    {
        cout << "Please Run as admin!" << endl;
        system("pause");
        return 0;
    }
    int option = 0;
    bool validInput = false;
   
    cout << "What do you want to do?\n";
    cout << "1. dism cleanup\n";
    cout << "2. Check disk\n";
    cout << "3. Remove temp and Prefetch\n";
    cout << "4. Fix Network\n";
    cout << "Option: ";
   
    while (!validInput) {
        if (cin >> option) {
            if (option >= 1 && option <= 4) {
                validInput = true;
            } else {
                cout << "Please enter a number between 1 and 4: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        } else {
            cout << "Invalid input. Please enter a number between 1 and 4: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    switch(option) {
        case 1:
            cout << "Executing DISM cleanup..." << endl;
            system("dism /online /cleanup-image /startcomponentcleanup");
            system("pause");
            system("dism /online /cleanup-image /restorehealth");
            askReboot();
            break;
        case 2:
            cout << "Executing disk check..." << endl;
            system("sfc /scannow");
            system("pause");
            system("chkdsk /f /r");
            askReboot();
            break;
        case 3:
            cout << "Removing temp files." << endl;
            system("del /q/f/s %TEMP%\\*");
            cout << "Removing temp Prefetch files." << endl;
            system("del /q/f/s C:\\Windows\\Prefetch\\*");
            askReboot();
            break;
        case 4:
            cout << "Renewing IP and Flush DNS";
            system("ipconfig /release");
            system("ipconfig /renew");
            system("ipconfig /flushdns");
            system("pause");
            cout << "Resetting IP and Winsock";
            system("netsh int ip reset");
            system("netsh winsock reset");
            askReboot();
    }

    cout << "\nPress Enter to exit...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
   
    return 0;
}