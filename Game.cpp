#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <vector>
#include <fstream>
#include <string>
#include <mmsystem.h> // Для PlaySound()
#pragma comment(lib, "winmm.lib") // Подключение библиотеки winmm.lib
#include "Header.h"
using namespace std;

//Названия файлов
vector <string> LvL = { "Text/Data/LvL1.txt","Text/Data/LvL2.txt", "Text/Data/LvL3.txt", 
                        "Text/Data/LvL4.txt", "Text/Data/LvL5.txt" };
string infoText = "Text/Data/InfoText.txt";
string MENU = "Text/Data/Menu.txt";
string SAVE = "Text/Save/Save.txt";
string LOAD = "Text/Save/Load.txt";

//Выгружаем Текст для игры и меню
vector <string> gameText = loadText(gameText, infoText);
vector <string> menu = loadText(menu, MENU);
vector <string> save = loadText(save, SAVE);
vector <string> load = loadText(load, LOAD);

//Выгружаем все карты и добавляем монеты
vector <vector <string>> maps;
vector <int> coinsRoom;
int Coins{ 5 }, sumCoins = LvL.size() * Coins;
vector <char> bag;

//Интерфейс
int userX{ 1 }, userY{ 1 }, checkRoom{ 0 }, thisLvL{ 0 }; bool flag{ false };

//Проверка на Старт игры
bool checkStarGame{ false }, exitGame{ false }, off{ true };

// Добавить функцию загрузки и сохранения
// Добавить жизни и при взаимодействии с ловушкой отнимать их - мины и падение с моста
// Добавить уровень с минами
// Добавить секретный магазин
// Добавить уровень с мостом
// Добавить сундук с ключом и бомжом
// Добавить подсказку к мосту
// Добавить меч в магазин
// Добавить металло-искател для поиска мин и лопату
// Добавить крипов (1х2 клетки, бить сзади)
// Добавить босс-вертолет (механика: 1 стадия - 1 линия, 2 стадия - крест, 3 стадия - сетка)
// Добавить возможность покупать HP

void Game() {
    while (true) { system("cls");
    PlaySound(L"Sound/BloodMoney2.wav", NULL, SND_FILENAME | SND_ASYNC); /*BloodMoney2*/
        while (true) {
            if (bag.size() == sumCoins) break;
            maps[checkRoom][userY][userX] = '@';
            printMap(maps[checkRoom]);
            printBag(bag);
            
            int code = _getch(); if (code == 0) code = _getch();
            
            if (maps[checkRoom][userY][userX + 1] == '>' && code == 77 && bag.size() >= coinsRoom[checkRoom]) {
                checkRoom++;
                if (checkRoom > thisLvL) {//Переход на новый уровень
                    system("cls"); thisLvL++;
                    switch (thisLvL) {
                        case 1: { PlaySound(L"Sound/AnimeWow.wav", NULL, SND_FILENAME | SND_ASYNC); /*AnimeWow*/ break; }
                        case 2: { PlaySound(L"Sound/GiveYouUp.wav", NULL, SND_FILENAME | SND_ASYNC); /*GiveYouUp*/ break; }
                        case 3: { PlaySound(L"Sound/Sexy_Black.wav", NULL, SND_FILENAME | SND_ASYNC); /*Sexy_Black*/ break; }
                        case 4: { PlaySound(L"Sound/Initial_D.wav", NULL, SND_FILENAME | SND_ASYNC); /*Initial_D*/ break; }
                        default: break;
                    }
                    
                    cout << endl << endl; readText(gameText[11]); cout << endl << endl << endl; system("pause");
                }
                maps[checkRoom - 1][userY][userX] = ' ';
                userX = 1; break;
            }
            else if (maps[checkRoom][userY][userX + 1] == '>' && code == 77 && bag.size() < coinsRoom[checkRoom]) {
                code = 0; system("cls");
                PlaySound(L"Sound/5Coins.wav", NULL, SND_FILENAME | SND_ASYNC); /*5Coins*/
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
                case 27: { PlaySound(L"Sound/GtaMenu.wav", NULL, SND_FILENAME | SND_ASYNC); /*GtaMenu*/ 
                    Menu(); break; } //вниз
                default: break;
            } if (exitGame) { system("cls"); break; } //Выход
            if (maps[checkRoom][userY + moveY][userX + moveX] != '#') {
                //Ловушка - портал
                if (maps[checkRoom][userY + moveY][userX + moveX] == 'S') { 
                    PlaySound(L"Sound/Hurt.wav", NULL, SND_FILENAME | SND_ASYNC); /*Hurt*/
                    if (flag) { maps[checkRoom][userY][userX] = 'X'; flag = 0; }
                    else maps[checkRoom][userY][userX] = ' ';
                    userX = 1; system("cls"); continue;
                }

                //Сбор денег
                if (flag) { maps[checkRoom][userY][userX] = 'X'; flag = 0; }
                else maps[checkRoom][userY][userX] = ' ';
                if (maps[checkRoom][userY + moveY][userX + moveX] == '$') { 
                    PlaySound(L"Sound/CoinsMario.wav", NULL, SND_FILENAME | SND_ASYNC); /*CoinsMario*/
                    bag.push_back('$'); flag = 1; }
                else if (maps[checkRoom][userY + moveY][userX + moveX] == 'X') flag = 1;
                else flag = 0;

                userX += moveX; userY += moveY;
            } system("cls");
        } if (bag.size() == sumCoins) break; 
        if (exitGame) { system("cls"); break; } //Выход
    } system("cls");
    if (!exitGame) {
        PlaySound(L"Sound/MissionComplite.wav", NULL, SND_FILENAME | SND_ASYNC); /*MissionComplite*/
        cout << endl << endl << endl << endl; readText(gameText[12]); cout << endl << endl << endl << endl; }
    else {
        PlaySound(L"Sound/CRY.wav", NULL, SND_FILENAME | SND_ASYNC); /*CRY*/
        cout << endl << endl << endl << endl; readText(gameText[14]); cout << endl << endl << endl << endl; }
    system("pause"); system("cls");
}
void Menu() {
    if (off) { off = false;
        PlaySound(L"Sound/StartMario.wav", NULL, SND_FILENAME | SND_ASYNC); /*StartMario*/ }
    
    int startPosY{ 6 }, choice{ startPosY }, posX{ 17 };
    while (true) {
        system("cls");
        menu[choice][posX] = '@';
        printMenu(menu);
        int select = _getch(); if (select == 0) select = _getch();
        
        int moveY{ 0 }; menu[choice][posX] = ' ';
        switch (select) {
        case 72: { if (choice > startPosY) { moveY = -1; 
            PlaySound(L"Sound/GtaMenu.wav", NULL, SND_FILENAME | SND_ASYNC); /*GtaMenu*/
        } break; } //вверх
        case 80: { if (choice < startPosY + 4) { moveY = 1;
            PlaySound(L"Sound/GtaMenu.wav", NULL, SND_FILENAME | SND_ASYNC); /*GtaMenu*/
        } break; } //вниз
        case 13: {
            if (choice == startPosY && checkStarGame) { Game(); break; } //Продолжить
            else if (choice == startPosY + 1) { checkStarGame = true; 
            PlaySound(L"Sound/BloodMoney.wav", NULL, SND_FILENAME | SND_ASYNC); /*BloodMoney*/
            createContent(); Game(); break; } //Новая игра
            else if (choice == startPosY + 2 && checkStarGame) { saveWindow(); break; } //Сохранить
            else if (choice == startPosY + 3) { loadWindow(); break; } //Загрузить
            else if (choice == startPosY + 4) { exitGame = true; break; } //Выход
        }
        case 27: { if (checkStarGame) { Game(); break; } }
        default: break;
        } if (exitGame && !checkStarGame) {
            system("cls");
            PlaySound(L"Sound/ByeBye.wav", NULL, SND_FILENAME | SND_ASYNC); /*ByeBye*/
            cout << endl << endl << endl << endl; readText(gameText[13]); cout << endl << endl << endl << endl;
            system("pause"); system("cls"); break; }
        else if (exitGame) break; //Выход

        if (choice >= startPosY && choice <= startPosY + 4) choice += moveY;
        menu[choice - moveY][posX] = ' ';
    }
}
void saveWindow() {
    int startPosY{ 6 }, choice{ startPosY }, posX{ 17 }; bool exitSave{ false };
    while (true) {
        system("cls");
        save[choice][posX] = '@';
        printMenu(save);
        int select = _getch(); if (select == 0) select = _getch();

        int moveY{ 0 }; save[choice][posX] = ' ';
        switch (select) {
        case 72: { if (choice > startPosY) moveY = -1; break; } //вверх
        case 80: { if (choice < startPosY + 4) moveY = 1; break; } //вниз
        case 13: {
            if (choice == startPosY) { cout << "Загрузка 1!"; break; } //Загрузить 1 игру
            else if (choice == startPosY + 1) { cout << "Загрузка 2!"; break; } //Загрузить 2 игру
            else if (choice == startPosY + 2) { cout << "Загрузка 3!"; break; } //Загрузить 3 игру
            else if (choice == startPosY + 3) { cout << "Загрузка 4!"; break; } //Загрузить 4 игру
            else if (choice == startPosY + 4) { exitSave = true; break; } //Назад
        }
        case 27: { exitSave = true; break; } //Назад
        default: break;
        }if (exitSave) break; //Назад
        if (choice >= startPosY && choice <= startPosY + 4) choice += moveY;
        save[choice - moveY][posX] = ' ';
    }
}
void loadWindow() {
    int startPosY{ 6 }, choice{ startPosY }, posX{ 17 }; bool exitLoad{ false };
    while (true) {
        system("cls");
        load[choice][posX] = '@';
        printMenu(load);
        int select = _getch(); if (select == 0) select = _getch();

        int moveY{ 0 }; load[choice][posX] = ' ';
        switch (select) {
        case 72: { if (choice > startPosY) moveY = -1; break; } //вверх
        case 80: { if (choice < startPosY + 4) moveY = 1; break; } //вниз
        case 13: {
            if (choice == startPosY) { cout << "Загрузка 1!"; break; } //Загрузить 1 игру
            else if (choice == startPosY + 1) { cout << "Загрузка 2!"; break; } //Загрузить 2 игру
            else if (choice == startPosY + 2) { cout << "Загрузка 3!"; break; } //Загрузить 3 игру
            else if (choice == startPosY + 3) { cout << "Загрузка 4!"; break; } //Загрузить 4 игру
            else if (choice == startPosY + 4) { exitLoad = true; break; } //Назад
        }
        case 27: { exitLoad = true; break; } //Назад
        default: break;
        } if (exitLoad) break; //Назад
        if (choice >= startPosY && choice <= startPosY + 4) choice += moveY;
        load[choice - moveY][posX] = ' ';
    }
}
void createContent() {
    clearData();
    for (int i{ 0 }; i <= 5; i++) {
        if (i < 3) { readText(gameText[i]); cout << endl << endl; system("pause"); system("cls"); }
        else { readText(gameText[i]); cout << endl << endl; system("cls"); }
    }

    readText(gameText[6]); cout << endl << endl; system("pause"); system("cls");
    for (int i{ 0 }; i < LvL.size(); i++) {
        vector <string> temp = loadText(temp, LvL[i]);
        maps.push_back(temp);
        coinsRoom.push_back(Coins * (i + 1));
    }

    //Правила игры
    for (int i{ 7 }; i <= 9; i++) { readText(gameText[i]); cout << endl; }
    cout << endl; system("pause"); system("cls");
}
void clearData() {
    maps.clear(); coinsRoom.clear(); bag.clear();
    userX = 1; userY = 1; checkRoom = 0; thisLvL = 0; flag = false;
}