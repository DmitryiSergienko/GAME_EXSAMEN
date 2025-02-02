#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <vector>
#include <fstream>
#include <string>
#include "Header.h"
using namespace std;

//29.01.2025
//Переделать игру.
//- перенести текст и карту в файлы - в коде программы не должно быть карт и текста.
//- вывести информацию для игрока(что требуется и для чего).
//- отображение текста выполнить в формате игр с приставок 90 - х

vector <string> LvL = { "LvL1.txt","LvL2.txt", "LvL3.txt", "LvL4.txt", "LvL5.txt" };
string infoText = "InfoText.txt";

void Game() {
    //Выгружаем Текст для игры
    vector <string> gameText = loadText(gameText, infoText);
    for (int i{ 0 }; i <= 5; i++) { 
        if (i < 3) { ReadText(gameText[i]); cout << endl << endl; system("pause"); system("cls"); }
        else { ReadText(gameText[i]); cout << endl << endl; system("cls"); }
    }

    int userX{ 1 }, userY{ 1 }, checkRoom{ 0 }, Coins{ 5 }, sumCoins = LvL.size() * Coins, thisLvL{ 0 };
    bool flag{ false };
    short bagSize{ 0 };
    char* bag = new char[bagSize];
    int* coinsRoom = new int[LvL.size()];

    //Выгружаем все карты и добавляем монеты
    ReadText(gameText[6]); cout << endl << endl; system("pause"); system("cls");
    vector <vector <string>> maps;
    for (int i{ 0 }; i < LvL.size(); i++) {
        vector <string> temp = loadText(temp, LvL[i]);
        maps.push_back(temp);
        coinsRoom[i] = Coins * (i + 1);
    }

    //Правила игры
    for (int i{ 7 }; i <= 9; i++) { ReadText(gameText[i]); cout << endl;}
    cout << endl; system("pause"); system("cls");

    while (true) { system("cls");        
        while (true) {
            if (bagSize == sumCoins) break;
            maps[checkRoom][userY][userX] = '@';
            printMap(maps[checkRoom]);
            printBag(bag, bagSize);
            
            int code = _getch(); if (code == 0) code = _getch();
            
            if (maps[checkRoom][userY][userX + 1] == '>' && code == 77 && bagSize >= coinsRoom[checkRoom]) {
                checkRoom++;
                if (checkRoom > thisLvL) {//Переход на новый уровень
                    system("cls"); thisLvL++;
                    cout << endl << endl; ReadText(gameText[11]); cout << endl << endl << endl; system("pause");
                }
                maps[checkRoom - 1][userY][userX] = ' ';
                userX = 1; break;
            }
            else if (maps[checkRoom][userY][userX + 1] == '>' && code == 77 && bagSize < coinsRoom[checkRoom]) {
                code = 0; system("cls");
                cout << endl << endl; ReadText(gameText[10]); cout << endl << endl << endl; system("pause");
            }
            else if (maps[checkRoom][userY][userX - 1] == '<' && code == 75) {
                maps[checkRoom][userY][userX] = ' ';
                checkRoom--; userX = maps[checkRoom][userY].size() - 2; break;
            }

            //Ходьба
            int moveX{ 0 }, moveY{ 0 };
            switch (code) {
                case 75: { moveX = -1; break; } //влево
                case 77: { moveX = 1; break; } //вправо
                case 72: { moveY = -1; break; } //вверх
                case 80: { moveY = 1; break; } //вниз
                default: break;
            }
            if (maps[checkRoom][userY + moveY][userX + moveX] != '#') {
                //Ловушка - портал
                if (maps[checkRoom][userY + moveY][userX + moveX] == 'S') { 
                    if (flag) { maps[checkRoom][userY][userX] = 'X'; flag = 0; }
                    else maps[checkRoom][userY][userX] = ' ';
                    userX = 1; system("cls"); continue;
                }

                //Сбор денег
                if (flag) { maps[checkRoom][userY][userX] = 'X'; flag = 0; }
                else maps[checkRoom][userY][userX] = ' ';
                if (maps[checkRoom][userY + moveY][userX + moveX] == '$') { bagSize++; bagFill(bag, bagSize); flag = 1; }
                else if (maps[checkRoom][userY + moveY][userX + moveX] == 'X') flag = 1;
                else flag = 0;

                userX += moveX; userY += moveY;
            } system("cls");
        } if (bagSize == sumCoins) break; 
    } system("cls");
    cout << endl << endl << endl << endl; ReadText(gameText[12]); cout << endl << endl << endl << endl;
    delete[] coinsRoom; delete[] bag;
}