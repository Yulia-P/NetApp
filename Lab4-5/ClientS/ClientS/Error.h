#pragma once
#include <string>
#include "Winsock2.h" 
#pragma comment(lib, "WS2_32.lib")
using namespace std;
string SetErrorMsgText(string msgText, int code);
string GetErrorMsgText(int code);