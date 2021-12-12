#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <ctime>
#include "Winsock2.h"                // заголовок  WS2_32.dll
#pragma comment(lib, "WS2_32.lib") 
using namespace std;


WSAData wsa;
SOCKET clientSocket;
SOCKADDR_IN serverConfig;
SOCKADDR_IN serverConfigP;
int send_optval = 1;
int recv_optval = 2000;
char ibuf[50];


struct GETSINCHRO
{
    string cmd;
    int curvalue;
};

string  GetErrorMsgText(int code)    // cформировать текст ошибки 
{

    string msgText;
    switch (code)                      // проверка кода возврата  
    {
    case WSAEINTR:  msgText = "Работа функции прервана";
        break;
    case WSAEACCES: msgText = "Разрешение отвергнуто";
        break;
    case WSAEFAULT: msgText = "Ошибочный адрес";
        break;
    case WSAEINVAL: msgText = "Ошибка в аргументе";
        break;
    case WSAEMFILE: msgText = "Слишком много файлов открыто";
        break;
    case WSAEWOULDBLOCK: msgText = "Ресурс временно недоступен";
        break;
    case WSAEINPROGRESS: msgText = "Операция в процессе развития";
        break;
    case WSAEALREADY: msgText = "Операция уже выполняется";
        break;
    case WSAENOTSOCK: msgText = "Сокет задан неправильно";
        break;
    case WSAEDESTADDRREQ: msgText = "Требуется адресс расположения";
        break;
    case WSAEMSGSIZE: msgText = "Сообщение слишком длинное";
        break;
    case WSAEPROTOTYPE: msgText = "Неправильный тип протокола для сокета";
        break;
    case WSAENOPROTOOPT: msgText = "Ошибка в опции протокола";
        break;
    case WSAEPROTONOSUPPORT: msgText = "Протокол не поддерживается";
        break;
    case WSAESOCKTNOSUPPORT: msgText = "Тип сокета не поддерживается";
        break;
    case WSAEOPNOTSUPP: msgText = "Операция не поддерживается";
        break;
    case WSAEPFNOSUPPORT: msgText = "Тип протоколов не поддерживается";
        break;
    case WSAEAFNOSUPPORT: msgText = "Тип адресов не поддерживается протоколом";
        break;
    case WSAEADDRINUSE: msgText = "Адрес уже используется";
        break;
    case WSAEADDRNOTAVAIL: msgText = "Запрошенный адрес не может быть использован";
        break;
    case WSAENETDOWN: msgText = "Сеть отключена";
        break;
    case WSAENETUNREACH: msgText = "Сеть не достежима";
        break;
    case WSAENETRESET: msgText = "Сеть разорвала соединение";
        break;
    case WSAECONNABORTED: msgText = "Программный отказ связи";
        break;
    case WSAECONNRESET: msgText = "Связь восстановлена";
        break;
    case WSAENOBUFS: msgText = "Не хватает памяти для буфферов";
        break;
    case WSAEISCONN: msgText = "Сокет уже подключен";
        break;
    case WSAENOTCONN: msgText = "Сокет не пожключен";
        break;
    case WSAESHUTDOWN: msgText = "Нельзя выолнить send: сокет завершил работу";
        break;
    case WSAETIMEDOUT: msgText = "Закончился отведенный интервал времени";
        break;
    case WSAECONNREFUSED: msgText = "Соединение отклонено";
        break;
    case WSAEHOSTDOWN: msgText = "Хост в неработоспособном состоянии";
        break;
    case WSAEHOSTUNREACH: msgText = "Нет маршрута для хоста";
        break;
    case WSAEPROCLIM: msgText = "Слишком много процессов";
        break;
    case WSASYSNOTREADY: msgText = "Сеть не доступна";
        break;
    case WSAVERNOTSUPPORTED: msgText = "Данная версия недоступна";
        break;
    case WSANOTINITIALISED: msgText = "Не выполнена инициализация WS2_32.DLL";
        break;
    case WSAEDISCON: msgText = "Выполняется отключение";
        break;
    case WSATYPE_NOT_FOUND: msgText = "Класс не найден";
        break;
    case WSAHOST_NOT_FOUND: msgText = "Хост не найден";
        break;
    case WSATRY_AGAIN: msgText = "Неавторизованный хост не найден";
        break;
    case WSANO_RECOVERY: msgText = "Неопределенная ошибка";
        break;
    case WSANO_DATA: msgText = "Не записи запрошенного типа";
        break;
    case WSA_INVALID_HANDLE: msgText = "Указанный дескриптор события с ошибкой";
        break;
    case WSA_INVALID_PARAMETER: msgText = "Один или более параметров с ошибкой";
        break;
    case WSA_IO_INCOMPLETE: msgText = "Объект ввода-вывода не в сигнальном состоянии";
        break;
    case WSA_IO_PENDING: msgText = "Оцерация завершится позже";
        break;
    case WSA_NOT_ENOUGH_MEMORY: msgText = "Не достаточно памяти";
        break;
    case WSA_OPERATION_ABORTED: msgText = "Операция отвергнута";
        break;
    default: msgText = "Ошибка";
    };
    return msgText;
};
string  SetErrorMsgText(string msgText, int code)
{
    return  msgText + GetErrorMsgText(code);
};
//                                            SOCKADDR_IN
bool GetServer(const char* call, short port, sockaddr* from, int lfrom) {
    try {
        char buf[50];

        ((SOCKADDR_IN*)from)->sin_family = AF_INET;
        ((SOCKADDR_IN*)from)->sin_port = htons(port);
        ((SOCKADDR_IN*)from)->sin_addr.s_addr = INADDR_BROADCAST; 


        if ((sendto(clientSocket, call, sizeof(call) + 1, NULL, from, lfrom)) == SOCKET_ERROR) 
            throw  SetErrorMsgText("sendto:", WSAGetLastError());
        // дескриптор сокета
        // уровень действия
        // режим сокета для установки
        // значение режима сокета
        // длина буфера 
       
        //                           уровень     режим
        if (setsockopt(clientSocket, SOL_SOCKET, SO_RCVTIMEO, (char*)&recv_optval, sizeof(int)) == SOCKET_ERROR) 
            throw  SetErrorMsgText("opt:", WSAGetLastError());

        if ((recvfrom(clientSocket, buf, 100, NULL, (LPSOCKADDR)&serverConfigP, &lfrom)) == SOCKET_ERROR) 
            throw WSAGetLastError();

        if (strcmp(buf, call) == 0) 
            return true;
        else
            return false;
    }
    catch (int err_code)
    {
        if (err_code != WSAETIMEDOUT)
        {
            throw SetErrorMsgText("GS:", WSAGetLastError());
        }
        else {
            if (setsockopt(clientSocket, SOL_SOCKET, SO_BROADCAST, (char*)&send_optval, sizeof(int)) == SOCKET_ERROR)
                throw  SetErrorMsgText("opt:", WSAGetLastError());
            return false;
        }

    }
}

void main()
{
    SOCKET cS;
    SOCKADDR_IN serv;
    int maxcor = 0;
    int mincor = INT_MAX;
    int avgcorr = 0;
    serv.sin_family = AF_INET;
    serv.sin_port = htons(2000);
    SYSTEMTIME tm;
    GETSINCHRO getsincro, setsincro;
    ZeroMemory(&setsincro, sizeof(setsincro));
    ZeroMemory(&getsincro, sizeof(getsincro));
    getsincro.cmd = "SINC";
    getsincro.curvalue = 0;

    setlocale(LC_CTYPE, "Rus");
    cout << "Client" << endl;
    try {
        if (WSAStartup(MAKEWORD(2, 0), &wsa) != 0)
            throw SetErrorMsgText("WSA:", WSAGetLastError());
        if ((clientSocket = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET)
            throw  SetErrorMsgText("socket:", WSAGetLastError());
        if (setsockopt(clientSocket, SOL_SOCKET, SO_BROADCAST, (char*)&send_optval, sizeof(int)) == SOCKET_ERROR) //устанавливаем режим работы сокета
            throw  SetErrorMsgText("opt:", WSAGetLastError());

        if (GetServer("Hello", 2000, (LPSOCKADDR)&serverConfig, sizeof(serverConfig)))
        {
            cout << "Connect -->" << inet_ntoa(serverConfigP.sin_addr) << ":" << htons(serverConfigP.sin_port) << endl;
        }
        else
        {
            cout << "Not connected" << endl;
        }
        int lensockaddr = sizeof(serverConfigP);

        closesocket(clientSocket);
        WSACleanup();
    }
    catch (string errorMsg)
    {
        cout << errorMsg << endl;
    }
    system("pause");
}
