#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <vector>
#include <ctime> // Для получения времени
#include <fstream>
#include <direct.h> // Для создания папки
#include <shellapi.h> // Для удаления папки
#include <io.h> // Для _access - проверка наличия папки
#include <string>
#include <mmsystem.h> // Для PlaySound() и waveOutSetVolume()
#pragma comment(lib, "winmm.lib") // Подключение библиотеки winmm.lib
#include "Header.h"
using namespace std;

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
// Добавить цвета
// Добавить пункт Настройки - регулировка громкости, смена цвета, уровень сложности

void Menu() {
    addPathFiles(); // Добавляем путь к файлам
    sumCoins = LvL.size() * Coins; // Максимальное кол-во монет в игре
    if (offMenu) { offMenu = false; track(23); } //StartMario
    
    int startPosY{ 6 }, choice{ startPosY }, posX{ 17 };
    while (true) {
        system("cls");
        menu[choice][posX] = '@';
        printMenu(menu);
        int select = _getch(); if (select == 0) select = _getch();

        int moveY{ 0 }; menu[choice][posX] = ' ';
        switch (select) {
        case 72: { // Вверх
            if (choice > startPosY) {
                moveY = -1; track(14); //GtaMenu
            } break;
        }
        case 80: { // Вниз
            if (choice < startPosY + 4) {
                moveY = 1; track(14); //GtaMenu
            } break;
        }
        case 13: {
            if (choice == startPosY) {
                if (checkStarGame == false && saveName[5] != "Empty") { checkStarGame = true; loadGame(5, 0); } //Продолжить с последнего сохранения
                else if (checkStarGame == false) { track(26); readText(gameText[16]); cout << endl; system("pause"); break; }
                else Game(); break; } //Продолжить
            else if (choice == startPosY + 1) { //Новая игра
                checkStarGame = true; track(4); //BloodMoney
                pathLvL = "Text/Data/"; addPathFiles();
                createContent(); Game(); break;
            } 
            else if (choice == startPosY + 2 && checkStarGame) { saveWindow(); break; } //Сохранить
            else if (choice == startPosY + 2) { track(27); readText(gameText[17]); cout << endl; system("pause"); break; }
            else if (choice == startPosY + 3) { //Загрузить
                bool checkLoad{ false };
                for (int i{ 0 }; i < 4; i++) if (saveName[i] != "---") checkLoad = true;
                if (checkLoad) loadWindow(); 
                else { track(28); readText(gameText[16]); cout << endl; system("pause"); } break;
            }
            else if (choice == startPosY + 4) { exitGame = true; break; } //Выход
        }
        case 27: { if (checkStarGame) { Game(); break; } }
        default: break;
        } 
        if (exitGame && !checkStarGame) {
            system("cls");
            track(7); //ByeBye
            cout << endl << endl << endl << endl; readText(gameText[13]); cout << endl << endl << endl << endl;
            system("pause"); system("cls"); break;
        }
        else if (exitGame && bag.size() >= 25) {
            system("cls");
            track(24); //WindowsXP
            cout << endl << endl << endl << endl; readText(gameText[15]); cout << endl << endl << endl << endl;
            system("pause"); system("cls"); break; } //Выход
        else if (exitGame) {
            system("cls");
            track(10); //CRY
            cout << endl << endl << endl << endl; readText(gameText[14]); cout << endl << endl << endl << endl;
            system("pause"); system("cls"); break; } //Выход

        if (choice >= startPosY && choice <= startPosY + 4) choice += moveY;
        menu[choice - moveY][posX] = ' ';
    }
}

void Game() {
    while (true) {
        system("cls");
        track(5); //BloodMoney2
        while (true) {
            if (exitGame) { system("cls"); break; } //Выход
            if (bag.size() == sumCoins) { //Победа
                system("cls"); track(21); //MissionComplite
                cout << endl << endl << endl << endl; readText(gameText[12]); cout << endl << endl << endl << endl;
                system("pause"); bag.push_back('G'); bag.push_back('G'); bag.push_back('W'); bag.push_back('P'); system("cls");
            }
            maps[checkRoom][userY][userX] = '@';
            printMap(maps[checkRoom]);
            printBag(bag);

            int code = _getch(); if (code == 0) code = _getch();
            if (code == 27) { exitMenu = true; break; } // Выход в меню

            int checkCode = nextLVL(code);//Переход на новый уровень
            if (checkCode != 1) code = checkCode;
            move(code); system("cls"); //Ходьба
        } 
        if (bag.size() == sumCoins) break; //Победа
        if (exitGame || exitMenu) { track(14); /*GtaMenu*/ system("cls"); Menu(); break; } //Выход в меню
    }
}