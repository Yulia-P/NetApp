#include "Files.h"
#include "AcceptServer.h"
#include "GarbageCleaner.h"
#include "Error.h"
#include "ConsolePipe.h"
#include "DispathServer.h"
#include "ResponseServer.h"

int _tmain(int argc, _TCHAR* argv[])
{
   InitializeCriticalSection(&scListContact);
   volatile TalkersCmd cmd = Start;

   st1 = LoadLibrary(TEXT("Servicelibrary.dll"));
   ts1 = (HANDLE(*)(char*, LPVOID))GetProcAddress(st1, "SSS"); //импортируем функцию
   if (st1 == NULL) cout << "Fail DLL;" << endl;
   else cout << "Loaded DLL:      " << dllname << ";" << endl << endl;

   hAcceptServer = CreateThread(NULL, NULL, AcceptServer, (LPVOID)&cmd, NULL, NULL);
  //DispathServer((LPVOID)&cmd);
   HANDLE hDispathServer = CreateThread(NULL, NULL, DispathServer, (LPVOID)&cmd, NULL, NULL);
   //GarbageCleaner((LPVOID)&cmd);
   HANDLE hGarbageCleaner = CreateThread(NULL, NULL, GarbageCleaner, (LPVOID)&cmd, NULL, NULL);
   HANDLE hConsolePipe = CreateThread(NULL, NULL, ConsolePipe, (LPVOID)&cmd, NULL, NULL);
   HANDLE hResponseServer = CreateThread(NULL, NULL, ResponseServer, (LPVOID)&cmd, NULL, NULL);

   SetThreadPriority(hGarbageCleaner, THREAD_PRIORITY_BELOW_NORMAL);
   SetThreadPriority(hDispathServer, THREAD_PRIORITY_NORMAL);
   SetThreadPriority(hConsolePipe, THREAD_PRIORITY_NORMAL);
   SetThreadPriority(hResponseServer, THREAD_PRIORITY_ABOVE_NORMAL);
   SetThreadPriority(hAcceptServer, THREAD_PRIORITY_HIGHEST);

   WaitForSingleObject(hAcceptServer, INFINITE);
   WaitForSingleObject(hDispathServer, INFINITE);
   WaitForSingleObject(hGarbageCleaner, INFINITE);
   WaitForSingleObject(hConsolePipe, INFINITE);
   WaitForSingleObject(hResponseServer, INFINITE);

   CloseHandle(hAcceptServer);
   CloseHandle(hDispathServer);
   CloseHandle(hGarbageCleaner);
   CloseHandle(hConsolePipe);
   CloseHandle(hResponseServer);

   DeleteCriticalSection(&scListContact);

   FreeLibrary(st1);

   system("pause");
   return 0;
//    AcceptServer((LPVOID)&cmd);
   // WSAGetLastError();

}
