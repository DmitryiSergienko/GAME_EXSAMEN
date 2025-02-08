#pragma once
#ifndef Header_h
#define Header_h
#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <vector>
#include <ctime> // ��� ��������� �������
#include <fstream>
#include <direct.h> // ��� �������� �����
#include <io.h> // ��� _access - �������� ������� �����
#include <string>
#include <mmsystem.h> // ��� PlaySound() � waveOutSetVolume()
#pragma comment(lib, "winmm.lib") // ����������� ���������� winmm.lib
using namespace std;

void addPathFiles();
vector <string> loadText(vector <string> map, string LVL);
void createFolder(const char* path);
void saveText(const char* nameSaveFolder, string nameFile, vector <string> text);
void readText(string Text);
void createContent();
void clearData();

void printMap(vector <string> map);
void printMenu(vector <string> menu);
void printBag(vector <char> bag);

void Menu();
void saveWindow();
void saveGame(const char* directory = "Save", string nameSaveFolder = "Standart");
void loadWindow();
void Game();

void setVolume(unsigned int volume);
void playTrack(const wstring& filePath);
void track(int num);

string getTime();

int nextLVL(int code);
void move(int code);

#endif //Header

//////////////////////////////////////////////////////////////////////////////////////////

#ifndef GLOBAL_H
#define GLOBAL_H

//���������� � �������� ������
extern string pathLvL;
extern vector <string> LvLFiles;
extern vector <string> LvL;

extern string infoText;
extern string MENU;
extern string SAVE;
extern string LOAD;

//��������� ����� ��� ���� � ����
extern vector <string> gameText;
extern vector <string> menu;
extern vector <string> save;
extern vector <string> load;

//��������� ��� ����� � ��������� ������
extern vector <vector <string>> maps;
extern vector <int> coinsRoom;
extern int Coins;
extern int sumCoins;
extern vector <char> bag;

//���������
extern int userX;
extern int userY;
extern int checkRoom;
extern int thisLvL;
extern bool flag;

//�������� �� ����� ����
extern bool checkStarGame;
extern bool exitGame;
extern bool exitMenu;
extern bool off;

#endif // GLOBAL_H