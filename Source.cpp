#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <vector>
#include <fstream>
#include <string>
#include "Header.h"
using namespace std;


vector <string> loadText(vector <string> map, string LVL) {
    string textPush;
    fstream fileMap{ LVL, ios::in };
    if (!fileMap.is_open()) {
        cout << "Файл не открыт!!" << endl;
    }
    for (int i = 0; getline(fileMap, textPush); i++) {
        map.push_back(textPush);
    }
    fileMap.close();
    return map;
}
void readText(string Text) {
    for (int j{ 0 }; j < Text.size(); j++) {
        cout << Text[j];
        if (!_kbhit()) Sleep(150); else Sleep(0);
    }
}

void printMap(vector <string> map) {
    for (int i = 0; i < map.size(); i++) {
        for (int j = 0; j < map[i].size(); j++) {
            cout << map[i][j] << " ";
        } cout << endl;
    } cout << endl << endl;
}
void printMenu(vector <string> menu) {
    for (int i = 0; i < menu.size(); i++) {
        for (int j = 0; j < menu[i].size(); j++) {
            cout << menu[i][j];
        } cout << endl;
    } cout << endl << endl;
}
void printBag(char* bag, short bagSize) {
    cout << "Количество $: " << bagSize << endl;
    for (int i = 0; i < bagSize; i++) {
        cout << bag[i];
    } cout << endl;
}
void bagFill(char*& bag, short bagSize) {
    delete[] bag; bag = new char[bagSize];
    for (int i = 0; i < bagSize; i++) bag[i] = '$';
}