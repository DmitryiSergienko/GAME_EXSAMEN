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
#include "Header.h"
#include "MusicPlayer.h"
#include "Trap.h"
#include "Human.h"
using namespace std;
namespace fs = std::filesystem;

// Добавить жизни и при взаимодействии с ловушкой отнимать их - мины и падение с моста
// Добавить сундук с ключом и бомжом
// Добавить подсказку к мосту
// Добавить меч в магазин
// Добавить металло-искател для поиска мин и лопату
// Добавить крипов (1х2 клетки, бить сзади)
// Добавить босс-вертолет (механика: 1 стадия - 1 линия, 2 стадия - крест, 3 стадия - сетка)
// Добавить возможность покупать HP
// Доработать пункт Настройки: смена цвета, уровень сложности, режим рогалик

void Menu() {
    addPathFiles(); // Добавляем путь к файлам
    sumCoins = LvL.size() * Coins; // Максимальное кол-во монет в игре
    if (offMenu) { offMenu = false; track(23); } //StartMario
    
    int startPosY{ 6 }, choice{ startPosY }, posX{ 17 };
    while (true) {
        setColor(0x0B);
        system("cls");
        menu[choice][posX] = GiGa.getAvatar();
        printMenu(menu);
        int select = _getch(); if (select == 0 || select == 224) select = _getch();
        
        menu[choice][posX] = ' ';
        int moveY = moveMenu(select, choice, startPosY); exitMenu = false;
        
        if (choice >= startPosY && choice <= startPosY + 5) choice += moveY;
        menu[choice - moveY][posX] = ' ';

        if (outGame) break;
    } setColor(0x07);
}

void Game() {
    while (true) {
        system("cls");
        while (true) {
            player.resume(); /*BloodMoney*/
            if (outGame) { system("cls"); break; } // Выход
            if (bag.size() == sumCoins) { // Победа
                player.pause();
                system("cls"); playerLvL5.start(); // MissionComplite
                cout << endl << endl << endl << endl; readText(gameText[12]); cout << endl << endl << endl << endl;
                system("pause"); bag.push_back('G'); bag.push_back('G'); bag.push_back('W'); bag.push_back('P'); system("cls");
                playerLvL5.stop();
            }
            maps[checkRoom][userY][userX] = GiGa.getAvatar();
            printMap(maps[checkRoom], 1, 0);
            printHP(GiGa.getHP());
            printBag(bag);

            int select = _getch(); if (select == 0 || select == 224) select = _getch();
            if (select == 27) { exitMenu = true; player.pause(); /*BloodMoney*/ break; } // Выход в меню

            int checkCode = nextLVL(select);// Переход на новый уровень
            if (checkCode != 1) select = checkCode;
            moveGame(select); system("cls"); // Ходьба
            if (deathGG(GiGa.getHP())) break;; // Смерть ГГ
        } 
        if (bag.size() == sumCoins) break; // Победа
        if (exitMenu) { track(14); /*GtaMenu*/ system("cls"); Menu(); break; } // Выход в меню
    }
}