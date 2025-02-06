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

void printMap(vector <string> map);
void printMenu(vector <string> menu);
void printBag(char* bag, short bagSize);
void bagFill(char*& bag, short bagSize);

void Menu();
void Game();

#endif //Header