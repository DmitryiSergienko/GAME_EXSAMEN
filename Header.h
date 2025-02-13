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
#include <shellapi.h> // ��� �������� �����
#include <io.h> // ��� _access - �������� ������� �����
#include <string>
#include <mmsystem.h> // ��� PlaySound() � waveOutSetVolume() - �������
#pragma comment(lib, "winmm.lib") // ��� PlaySound() � waveOutSetVolume() - �������
#include <thread> // ��� �����������
#include <SFML/Audio.hpp> // ��� ����������� SFML (audio)
using namespace std;

void addPathFiles();
vector <string> loadText(vector <string> map, string LVL);
void createFolder(const char* path);
void saveText(const char* nameSaveFolder, string nameFile, vector <string> text);
void readText(string Text);
void createContent();
void clearData();
void clearFolder(const string& path);
bool folderExists(const string& path);
int deleteSave(const char* directory, string nameSaveFolder);

void setColor(int color);
void printMap(vector <string> map);
void printMap(vector <string> map, int sizeMap, int colorType);
void printMenu(vector <string> menu);
void printBag(vector <char> bag);

void Menu();
void Game();
void saveWindow();
string saveGame(const char* directory = "Save", string nameSaveFolder = "Default");
void saveLineWindow(int posX, int choice, int startPosY);
void loadWindow();
void loadGame(int choice, int startPosY, int posX);
void settingsWindow();
void settingSound(int choice, int posX);
void changePathSave();

void setVolume(unsigned int volume);
void playMusic(const string& filename);
void playTrack(const wstring& filePath);
void track(int num);

string getTime();

int nextLVL(int code);
int moveMenu(int select, int choice, int startPosY);
void moveGame(int code);
bool exitGame();

#endif //Header

//////////////////////////////////////////////////////////////////////////////////////////

#ifndef GLOBAL_H
#define GLOBAL_H

//���������� � �������� ������
extern string pathLvL;
extern vector <string> LvLFiles;
extern vector <string> LvL;
extern string sourceFile;

extern string infoText;
extern string MENU;
extern string SAVE;
extern string SAVENAME;
extern string LOAD;
extern string SETTINGS;

//��������� ����� ��� ���� � ����
extern vector <string> gameText;
extern vector <string> menu;
extern vector <string> save;
extern vector <string> saveName;
extern vector <string> load;
extern vector <string> settings;

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
extern bool outGame;
extern bool exitMenu;
extern bool offMenu;

extern int soundLVL;

extern HANDLE hConsole;

#endif // GLOBAL_H