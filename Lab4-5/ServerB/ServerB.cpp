#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <ctime>
#include "Winsock2.h"                // заголовок  WS2_32.dll
#pragma comment(lib, "WS2_32.lib") 
using namespace std;

WSAData wsa;
SOCKET serv;
SOCKET serverSocket;
SOCKADDR_IN config;
SOCKADDR_IN serverConfig;
SOCKADDR_IN clientConfig;

int send_optval = 1;
int recv_optval = 3000;
int numOfServers = 0;
char ibuf[50];
char myName[50] = "Hello";

SOCKET GS;

struct SETSINCRO
{
    string cmd;
    int correction;
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
//                                                  SOCKADDR_IN  
bool GetRequestFromClient(char* name, short port, sockaddr* from, int* sizefrom) 
{
    try {
        serverConfig.sin_family = AF_INET;
        serverConfig.sin_port = htons(port);
        serverConfig.sin_addr.s_addr = INADDR_ANY; // любой адрес

        if ((bind(serverSocket, (LPSOCKADDR)&serverConfig, sizeof(serverConfig))) == SOCKET_ERROR) 
            throw SetErrorMsgText("Bind:", WSAGetLastError());
        while (true)
        {
            if ((recvfrom(serverSocket, ibuf, 2000, NULL, from, sizefrom)) == SOCKET_ERROR) 
                throw SetErrorMsgText("Recv:", WSAGetLastError());

            if (strcmp(ibuf, name) == 0) //сравнение
                return true;
        }

    }
    catch (int err_code)
    {
        if (err_code != WSAETIMEDOUT)
        {
            throw SetErrorMsgText("GRFC:", WSAGetLastError());
        }
        else return false;
    }
}
//                                  SOCKADDR_IN  
bool  PutAnswerToClient(char* name, sockaddr* to, int lto) 
{
    if ((sendto(serverSocket, name, strlen(name) + 1, NULL, to, sizeof(clientConfig))) == SOCKET_ERROR)
        throw SetErrorMsgText("Send:", WSAGetLastError());
    return true;
}

void FindTheSameServer(char* name)
{
    SOCKET bS;
    SOCKADDR_IN broadcast;
    SOCKADDR_IN sameServ;
    int sameServSizeOf = sizeof(sameServ);

    broadcast.sin_family = AF_INET;
    broadcast.sin_port = htons(2000);
    broadcast.sin_addr.s_addr = INADDR_BROADCAST;

    char msgFromServ[50];
    memset(&msgFromServ, 0, sizeof(msgFromServ));

    if ((bS = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET)
        throw SetErrorMsgText("socket: ", WSAGetLastError());

    int optval = 1;
    if (setsockopt(bS, SOL_SOCKET, SO_BROADCAST, (char*)&optval, sizeof(int)) == SOCKET_ERROR)
        throw SetErrorMsgText("setsockopt: ", WSAGetLastError());

    int timeout = 1000;
    if (setsockopt(bS, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout)) == SOCKET_ERROR) // позволяют нам устанавливать тайм-аут при получении и отправке через сокет.
        throw SetErrorMsgText("setsockopt: ", WSAGetLastError());


    if (sendto(bS, name, strlen(name), NULL, (sockaddr*)&broadcast, sizeof(broadcast)) == SOCKET_ERROR)
        throw SetErrorMsgText("sendto: ", WSAGetLastError());

    if (recvfrom(bS, msgFromServ, sizeof(msgFromServ), NULL, (sockaddr*)&sameServ, &sameServSizeOf) == SOCKET_ERROR)
    {
        if (WSAGetLastError() == WSAETIMEDOUT)
        {
            cout << "" << endl << endl;
            return;
        }
        throw SetErrorMsgText("recvfrom: ", WSAGetLastError());
    }

    if (strcmp(msgFromServ, name) == 0)
    {
        cout << " THIS CALLSIGN IS ALREADY IN USE!" << endl;
        cout << "IP: " << inet_ntoa(sameServ.sin_addr) << endl << endl;
        throw SetErrorMsgText("closesocket: ", WSAGetLastError());
    }

    if (closesocket(bS) == SOCKET_ERROR)
        throw SetErrorMsgText("closesocket: ", WSAGetLastError());
}

void main()
{
    setlocale(LC_CTYPE,"Rus");
    SETSINCRO setsincro, getsincro;
    ZeroMemory(&setsincro, sizeof(setsincro));
    ZeroMemory(&getsincro, sizeof(getsincro));
    setsincro.cmd = "SINCRO";
    setsincro.correction = 0;
    SYSTEMTIME tm;
    char hostname[25];
    hostent* clntInfo;

    clock_t c;
    int averageCorrection[10];
    cout << "Server";
    try {

        if (WSAStartup(MAKEWORD(2, 0), &wsa) != 0)  
            throw SetErrorMsgText("WSA:", WSAGetLastError());      

        FindTheSameServer(myName);

        int count = 1;                      // UDP
        if ((serverSocket = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET) 
            throw SetErrorMsgText("Socket:", WSAGetLastError());
        memset(&clientConfig, 0, sizeof(clientConfig));
        int szClientConfig = sizeof(clientConfig);

        if (gethostname(hostname, sizeof(hostname)) == SOCKET_ERROR)
            throw SetErrorMsgText("gethostname: ", WSAGetLastError());
        cout << "Server name: " << hostname << endl << endl;
        
        bool res = GetRequestFromClient(myName, 2000, (LPSOCKADDR)&clientConfig, &szClientConfig);
        PutAnswerToClient(myName, (LPSOCKADDR)&clientConfig, szClientConfig);
        // адрес, длинна, тип
        clntInfo = gethostbyaddr((const char*)&clientConfig.sin_addr, 4, AF_INET);
        cout << "Client port: " << clientConfig.sin_port << endl;
        cout << "Client IP: " << inet_ntoa(clientConfig.sin_addr) << endl;
        cout << "Client name: " << clntInfo->h_name << endl; 
        cout << endl;
        
        closesocket(serverSocket);
        WSACleanup();
    }
    catch (string errorMsg)
    {
        cout << errorMsg << endl;
    }
    system("pause");
}