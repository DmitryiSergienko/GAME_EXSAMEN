#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <vector>
#include <ctime> // ��� ��������� �������
#include <fstream>
#include <direct.h> // ��� �������� �����
#include <filesystem> // ��� �������� �����
#include <shellapi.h> // ��� �������� �����
#include <io.h> // ��� _access - �������� ������� �����
#include <string>
#include <mmsystem.h> // ��� PlaySound() � waveOutSetVolume() - (������)
#pragma comment(lib, "winmm.lib") // ��� PlaySound() � waveOutSetVolume() - (������)
#include <thread> // ��� �����������
#include <SFML/Audio.hpp> // ��� ����������� SFML (audio)
#include <chrono> // ��� ����������� �������
#include "Header.h"
#include "MusicPlayer.h"
using namespace std;
namespace fs = std::filesystem;

int main() {
	SetConsoleOutputCP(1251); SetConsoleCP(1251); srand(time(NULL));
	setVolume(int(65535 * double(soundLVL) * 10 / 100)); // 50% | 65535 - max
	Menu(); return 0; }