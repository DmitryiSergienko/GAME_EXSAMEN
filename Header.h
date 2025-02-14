#pragma once
#ifndef Header_h
#define Header_h
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
#include "MusicPlayer.h"
#include "Trap.h"
#include "Human.h"
using namespace std;
namespace fs = std::filesystem;

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
void printHP(int HP);

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
void playMusic(const string& musicFile);
void playMusicQueue(const vector<string>& musicFiles);
void playTrack(const wstring& filePath);
void track(int num);

string getTime();

int nextLVL(int code);
int moveMenu(int select, int choice, int startPosY);
void moveGame(int code);
bool exitGame();
bool deathGG(int HP);

#endif // Header

//////////////////////////////////////////////////////////////////////////////////////////

#ifndef GLOBAL_H
#define GLOBAL_H

// Директория и названия файлов
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

// Выгружаем Текст для игры и меню
extern vector <string> gameText;
extern vector <string> menu;
extern vector <string> save;
extern vector <string> saveName;
extern vector <string> load;
extern vector <string> settings;

// Выгружаем все карты и добавляем монеты
extern vector <vector <string>> maps;
extern vector <int> coinsRoom;
extern int Coins;
extern int sumCoins;
extern vector <char> bag;

// Интерфейс
extern int userX;
extern int userY;
extern int checkRoom;
extern int thisLvL;
extern bool flag;

// Проверка на Старт игры
extern bool checkStarGame;
extern bool outGame;
extern bool exitMenu;
extern bool offMenu;

// Музыка
extern int soundLVL;
extern vector<string> tracks;
extern MusicPlayer player;
extern MusicPlayer playerLvL1;
extern MusicPlayer playerLvL2;
extern MusicPlayer playerLvL3;
extern MusicPlayer playerLvL4;
extern MusicPlayer playerLvL5;

// Цвета
extern HANDLE hConsole;

// Ловушки
extern fakeDollar FakeDollar;
extern water Water;
extern mine Mine;
extern bridge Bridge;

// Человеки
extern GG GiGa;

#endif // GLOBAL_H