#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <vector>
#include <ctime> // ��� ��������� �������
#include <fstream>
#include <direct.h> // ��� �������� �����
#include <shellapi.h> // ��� �������� �����
#include <io.h> // ��� _access - �������� ������� �����
#include <string>
#include <mmsystem.h> // ��� PlaySound() � waveOutSetVolume()
#pragma comment(lib, "winmm.lib") // ����������� ���������� winmm.lib
#include "Header.h"
using namespace std;

int main() { 
	SetConsoleOutputCP(1251); SetConsoleCP(1251); srand(time(NULL));
	setVolume(1000 /*65535*/);
	Menu(); return 0; }