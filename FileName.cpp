#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <vector>
#include <fstream>
#include <string>
#include <mmsystem.h> // Äëÿ PlaySound() è waveOutSetVolume()
#include "Header.h"
using namespace std;

int main() { 
	SetConsoleOutputCP(1251); SetConsoleCP(1251); srand(time(NULL));
	setVolume(1000 /*65535*/);
	Menu(); return 0; }