#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <vector>
#include <ctime> // Для получения времени
#include <fstream>
#include <direct.h> // Для создания папки
#include <filesystem> // Для удаления папки
#include <shellapi.h> // Для удаления папки
#include <io.h> // Для _access - проверка наличия папки
#include <string>
#include <mmsystem.h> // Для PlaySound() и waveOutSetVolume() - (Старое)
#pragma comment(lib, "winmm.lib") // Для PlaySound() и waveOutSetVolume() - (Старое)
#include <thread> // Для многопотока
#include <SFML/Audio.hpp> // Для подключения SFML (audio)
#include <chrono> // Для управлением времени
#include "Header.h"
#include "MusicPlayer.h"
using namespace std;
namespace fs = std::filesystem;

int main() {
	SetConsoleOutputCP(1251); SetConsoleCP(1251); srand(time(NULL));
	setVolume(int(65535 * double(soundLVL) * 10 / 100)); // 50% | 65535 - max
	Menu(); return 0; }