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
void ReadText(string Text);

void printMap(vector <string> map);
void printBag(char* bag, short bagSize);
void bagFill(char*& bag, short bagSize);

void Game();

#endif //Header