#pragma once
#ifndef Header_h
#define Header_h
#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <vector>
#include <fstream>
#include <string>
using namespace std;

vector <string> loadText(vector <string> map, string LVL);
void readText(string Text);
void createContent();
void clearData();

void printMap(vector <string> map);
void printMenu(vector <string> menu);
void printBag(vector <char> bag);

void Menu();
void saveWindow();
void loadWindow();
void Game();

void setVolume(unsigned int volume);

#endif //Header