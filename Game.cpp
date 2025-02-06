#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <vector>
#include <fstream>
#include <string>
#include "Header.h"
using namespace std;

//Названия файлов
vector <string> LvL = { "LvL1.txt","LvL2.txt", "LvL3.txt", "LvL4.txt", "LvL5.txt" };
string infoText = "InfoText.txt";
string MENU = "Menu.txt";

//Выгружаем Текст для игры и меню
vector <string> gameText = loadText(gameText, infoText);
vector <string> menu = loadText(menu, MENU);

//Выгружаем все карты и добавляем монеты
vector <vector <string>> maps;
int* coinsRoom = new int[LvL.size()];
int Coins{ 5 }, sumCoins = LvL.size() * Coins;

// Добавить меню
// Добавить функцию загрузки и сохранения
// Добавить жизни и при взаимодействии с ловушкой отнимать их
// Добавить врагов (босс-вертолет)
// Добавить возможность покупать HP

bool checkStarGame{ false };

void Menu() {
    int startPosY{ 6 }, choice{ startPosY }, posX{ 17 };
    while (true) {
        system("cls");
        menu[choice][posX] = '@';
        printMenu(menu);
        int select = _getch(); if (select == 0) select = _getch();
        
        int moveY{ 0 };
        switch (select) {
            case 72: { if (choice > startPosY) moveY = -1; break; } //вверх
            case 80: { if (choice < startPosY + 4) moveY = 1; break; } //вниз
            case 13: {
                if (choice == startPosY && checkStarGame) { Game(); cout << "Функция продолжить!"; system("pause"); break; }
                else if (choice == startPosY + 1) { checkStarGame = true; createContent(); Game(); break; }
                else if (choice == startPosY + 2 && checkStarGame) { cout << "Функция сохранить!"; system("pause"); break; }
                else if (choice == startPosY + 3) { cout << "Функция загрузить!"; system("pause"); break; }
                else if (choice == startPosY + 4) { cout << "Функция выход!"; system("pause"); break; }
            }
            case 27: { if (checkStarGame) { Game(); cout << "Функция продолжить!"; system("pause"); break; } }
            default: break;
        }

        if (choice >= startPosY && choice <= startPosY + 4) choice += moveY;
        menu[choice - moveY][posX] = ' ';
    }
}



void createContent() {
    for (int i{ 0 }; i <= 5; i++) {
        if (i < 3) { readText(gameText[i]); cout << endl << endl; system("pause"); system("cls"); }
        else { readText(gameText[i]); cout << endl << endl; system("cls"); }
    }

    readText(gameText[6]); cout << endl << endl; system("pause"); system("cls");
    for (int i{ 0 }; i < LvL.size(); i++) {
        vector <string> temp = loadText(temp, LvL[i]);
        maps.push_back(temp);
        coinsRoom[i] = Coins * (i + 1);
    }

    //Правила игры
    for (int i{ 7 }; i <= 9; i++) { readText(gameText[i]); cout << endl; }
    cout << endl; system("pause"); system("cls");
}

void Game() {
    int userX{ 1 }, userY{ 1 }, checkRoom{ 0 }, thisLvL{ 0 };
    bool flag{ false };
    short bagSize{ 0 };
    char* bag = new char[bagSize];
    
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
                    cout << endl << endl; readText(gameText[11]); cout << endl << endl << endl; system("pause");
                }
                maps[checkRoom - 1][userY][userX] = ' ';
                userX = 1; break;
            }
            else if (maps[checkRoom][userY][userX + 1] == '>' && code == 77 && bagSize < coinsRoom[checkRoom]) {
                code = 0; system("cls");
                cout << endl << endl; readText(gameText[10]); cout << endl << endl << endl; system("pause");
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
                case 27: { Menu(); break; } //вниз
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
    cout << endl << endl << endl << endl; readText(gameText[12]); cout << endl << endl << endl << endl;
    delete[] coinsRoom; delete[] bag;
}