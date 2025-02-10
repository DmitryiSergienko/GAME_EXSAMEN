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

void addPathFiles() {
    LvL.clear();
    for (int i = 0; i < LvLFiles.size(); i++) LvL.push_back(pathLvL + LvLFiles[i]);
}
vector <string> loadText(vector <string> text, string nameFile) {
    string textPush;
    fstream fileMap{ nameFile, ios::in };
    if (!fileMap.is_open()) {
        cout << "Файл не открыт!!" << endl;
    }
    for (int i = 0; getline(fileMap, textPush); i++) {
        text.push_back(textPush);
    }
    fileMap.close();
    return text;
}
void createFolder(const char* path) {
    if (_mkdir(path) == 0) cout << "Папка успешно создана: " << path << endl;
    else cerr << "Не удалось создать папку." << endl;
}
void saveText(const char* nameSaveFolder, string nameFile, vector <string> text) {
    string backSlach = "\\";
    string directoy = nameSaveFolder + backSlach + nameFile;
    fstream fileMap; fileMap.open(directoy, ios::out);
    if (!fileMap.is_open()) { cout << "Файл не открыт!!" << endl; system("pause"); }
    int length = text.size();
    for (int i = 0; i < length; i++) fileMap << text[i] << endl;
    fileMap.close();
}
void readText(string Text) {
    for (int j{ 0 }; j < Text.size(); j++) {
        cout << Text[j];
        if (!_kbhit()) Sleep(150); else Sleep(0);
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
void clearFolder(const string& path) {
    const char* path1 = "Save\\2025_2_10_1_29_48";
    SHFILEOPSTRUCTA fileOp = {};
    fileOp.wFunc = FO_DELETE; // Операция удаления
    fileOp.pFrom = path.c_str();      // Путь к папке
    fileOp.fFlags = FOF_NOCONFIRMATION | FOF_SILENT; // Без запросов и вывода сообщений

    int result = SHFileOperationA(&fileOp);
    if (result == 0) cout << "Папка успешно удалена." << endl;
    else { cerr << "Не удалось удалить папку." << endl; system("pause"); }
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
void printBag(vector <char> bag) {
    cout << "Количество $: " << bag.size() << endl;
    for (int i = 0; i < bag.size(); i++) {
        cout << bag[i];
    } cout << endl;
}

void saveWindow() {
    int startPosY{ 6 }, choice{ startPosY }, posX{ 17 };
    // Загрузка списка сохраненных игр в окне Сохранение
    for (int i{ 0 }; i < 4; i++) save[choice + i].replace(posX + 2, saveName[i].size(), saveName[i]);
    
    while (true) {
        system("cls");
        save[choice][posX] = '@';
        printMenu(save);
        int select = _getch(); if (select == 0) select = _getch();

        int moveY{ 0 }; save[choice][posX] = ' ';
        switch (select) {
        case 72: { if (choice > startPosY) moveY = -1; track(14); /*GtaMenu*/ break; } // Вверх
        case 80: { if (choice < startPosY + 4) moveY = 1; track(14); /*GtaMenu*/ break; } // Вниз
        case 13: {
            if (choice <= startPosY + 3) { saveLineWindow(posX + 2, choice, startPosY); break; } // Сохранить игру
            else { system("cls"); Menu(); break; } //Назад
        }
        case 27: { system("cls"); Menu(); break; } //Назад
        default: break;
        }
        if (choice >= startPosY && choice <= startPosY + 4) choice += moveY;
        save[choice - moveY][posX] = ' ';
    }
}
string saveGame(const char* directory, string nameSaveFolder) {
    directory = (saveName[4]).c_str();
    if (_access(directory, 0) == -1) createFolder(directory); // Корневая папка

    string nameSaveInWindow;
    if (nameSaveFolder == "Default") nameSaveInWindow = getTime(); // Дочерняя папка
    else nameSaveInWindow = nameSaveFolder;
    nameSaveFolder = directory; nameSaveFolder += "/"; nameSaveFolder += nameSaveInWindow;
    const char* path = nameSaveFolder.c_str();
    if (_access(path, 0) == -1) createFolder(path);

    for (int i{ 0 }; i < LvLFiles.size(); i++) saveText(path, LvLFiles[i], maps[i]); // Сохранение карт

    // Добавляем в вектор ресурсы
    vector<string> source;
    string _bag(bag.begin(), bag.end());
    source.push_back("Рюкзак"); source.push_back(_bag);

    // Добавляем в вектор состаяние элементов
    source.push_back("Номер карты"); source.push_back(to_string(checkRoom));
    source.push_back("Координата по X"); source.push_back(to_string(userX));
    source.push_back("Координата по Y"); source.push_back(to_string(userY));
    source.push_back("Номер уровня"); source.push_back(to_string(thisLvL));
    source.push_back("Проверка флага"); source.push_back(to_string(flag));

    saveText(path, "Source.txt", source); // Сохранение всех ресурсов и состояния элементов
    return nameSaveInWindow;
}
void saveLineWindow(int posX, int choice, int startPosY) {
    string backSlash = "/";
    string path = saveName[saveName.size() - 1] + backSlash + saveName[choice - startPosY];
    if (saveName[choice - startPosY] != "---") clearFolder(path);
    string emptyName = "                           ";
    save[choice].replace(posX, emptyName.size(), emptyName);
    saveName[choice - startPosY] = saveGame();
    saveName[5] = saveName[choice - startPosY];
    save[choice].replace(posX, saveName[choice - startPosY].size(), saveName[choice - startPosY]);
    saveText((SAVENAME.substr(0, 9)).c_str(), SAVENAME.substr(10, 21), saveName);
}
void loadWindow() {
    int startPosY{ 6 }, choice{ startPosY }, posX{ 17 };
    // Загрузка списка сохраненных игр в окне Загрузка        
    for (int i{ 0 }; i < 4; i++) load[choice + i].replace(posX + 2, saveName[i].size(), saveName[i]);

    while (true) {
        system("cls");
        load[choice][posX] = '@';
        printMenu(load);
        int select = _getch(); if (select == 0) select = _getch();

        int moveY{ 0 }; load[choice][posX] = ' ';
        switch (select) {
        case 72: { if (choice > startPosY) moveY = -1; break; } // Вверх
        case 80: { if (choice < startPosY + 4) moveY = 1; break; } // Вниз
        case 13: {
            if (choice < startPosY + 4) { // Загрузить игру
                checkStarGame = true;
                loadGame(choice, startPosY); break; } 
            else { system("cls"); Menu(); break; } //Назад
        }
        case 27: {
            if (checkStarGame) { Game(); break; }
            else { system("cls"); Menu(); break; }
        } //Назад
        default: break;
        }
        if (choice >= startPosY && choice <= startPosY + 4) choice += moveY;
        load[choice - moveY][posX] = ' ';
    }
}
void loadGame(int choice, int startPosY) {
    string nameSaveFolder = saveName[choice - startPosY];
    clearData();
    string directory = saveName[4], BackSlash = "/";
    pathLvL = directory + BackSlash + nameSaveFolder + BackSlash;
    addPathFiles(); //Меняем путь к папке загрузки
    
    for (int i{ 0 }; i < LvL.size(); i++) { // Загрузка карт
        vector <string> temp = loadText(temp, LvL[i]);
        maps.push_back(temp);
        coinsRoom.push_back(Coins * (i + 1));
    }
    
    sourceFile = "Source.txt"; sourceFile = pathLvL + BackSlash + sourceFile;
    // Добавляем в вектор ресурсы
    vector<string> source = loadText(source, sourceFile);
    for (int i{ 0 }; i < source[1].size(); i++) { bag.push_back(source[1][i]); cout << bag[i]; } // Заполняем инвентарь
    
    // Добавляем в вектор состаяние элементов
    checkRoom = stoi(source[3]);
    userX = stoi(source[5]);
    userY = stoi(source[7]);
    thisLvL = stoi(source[9]);
    flag = bool(stoi(source[11]));
    Game();
}

void setVolume(unsigned int volume) {
    // Устанавливаем одинаковую громкость для обоих каналов (левый и правый)
    unsigned long vol = static_cast<unsigned long>(volume | (volume << 16));
    waveOutSetVolume(NULL, vol); // NULL указывает на использование текущего устройства вывода
}
void playTrack(const wstring& filePath) {
    // Напрямую используем Unicode-версию PlaySound
    PlaySoundW(filePath.c_str(), NULL, SND_FILENAME | SND_ASYNC);
}
void track(int num) {
    switch (num) {
    case 1: PlaySound(L"Sound/1.5Coins.wav", NULL, SND_FILENAME | SND_ASYNC); break;
    case 2: PlaySound(L"Sound/2.AnimeWow.wav", NULL, SND_FILENAME | SND_ASYNC); break;
    case 3: PlaySound(L"Sound/3.Bald_Eagle.wav", NULL, SND_FILENAME | SND_ASYNC); break;
    case 4: PlaySound(L"Sound/4.BloodMoney.wav", NULL, SND_FILENAME | SND_ASYNC); break;
    case 5: PlaySound(L"Sound/5.BloodMoney2.wav", NULL, SND_FILENAME | SND_ASYNC); break;
    case 6: PlaySound(L"Sound/6.BossDarkSouls.wav", NULL, SND_FILENAME | SND_ASYNC); break;
    case 7: PlaySound(L"Sound/7.ByeBye.wav", NULL, SND_FILENAME | SND_ASYNC); break;
    case 8: PlaySound(L"Sound/8.CatRap.wav", NULL, SND_FILENAME | SND_ASYNC); break;
    case 9: PlaySound(L"Sound/9.CoinsMario.wav", NULL, SND_FILENAME | SND_ASYNC); break;
    case 10: PlaySound(L"Sound/10.CRY.wav", NULL, SND_FILENAME | SND_ASYNC); break;
    case 11: PlaySound(L"Sound/11.DarkSouls.wav", NULL, SND_FILENAME | SND_ASYNC); break;
    case 12: PlaySound(L"Sound/12.GeorgeMicael.wav", NULL, SND_FILENAME | SND_ASYNC); break;
    case 13: PlaySound(L"Sound/13.GiveYouUp.wav", NULL, SND_FILENAME | SND_ASYNC); break;
    case 14: PlaySound(L"Sound/14.GtaMenu.wav", NULL, SND_FILENAME | SND_ASYNC); break;
    case 15: PlaySound(L"Sound/15.Hurt.wav", NULL, SND_FILENAME | SND_ASYNC); break;
    case 16: PlaySound(L"Sound/16.Initial_D.wav", NULL, SND_FILENAME | SND_ASYNC); break;
    case 17: PlaySound(L"Sound/17.LOL_1Blood.wav", NULL, SND_FILENAME | SND_ASYNC); break;
    case 18: PlaySound(L"Sound/18.LOL_5Kill.wav", NULL, SND_FILENAME | SND_ASYNC); break;
    case 19: PlaySound(L"Sound/19.LOL_Defeat.wav", NULL, SND_FILENAME | SND_ASYNC); break;
    case 20: PlaySound(L"Sound/20.LOL_Welcome.wav", NULL, SND_FILENAME | SND_ASYNC); break;
    case 21: PlaySound(L"Sound/21.MissionComplite.wav", NULL, SND_FILENAME | SND_ASYNC); break;
    case 22: PlaySound(L"Sound/22.Sexy_Black.wav", NULL, SND_FILENAME | SND_ASYNC); break;
    case 23: PlaySound(L"Sound/23.StartMario.wav", NULL, SND_FILENAME | SND_ASYNC); break;
    case 24: PlaySound(L"Sound/24.WindowsXP.wav", NULL, SND_FILENAME | SND_ASYNC); break;
    case 25: PlaySound(L"Sound/25.Half-Life_Magnum.wav", NULL, SND_FILENAME | SND_ASYNC); break;
    case 26: PlaySound(L"Sound/26.Half-Life_Menu_Button.wav", NULL, SND_FILENAME | SND_ASYNC); break;
    case 27: PlaySound(L"Sound/27.Half-Life_Button1.wav", NULL, SND_FILENAME | SND_ASYNC); break;
    case 28: PlaySound(L"Sound/28.Half-Life_Button2.wav", NULL, SND_FILENAME | SND_ASYNC); break;
    case 29: PlaySound(L"Sound/29.CS_Flash_Hand.wav", NULL, SND_FILENAME | SND_ASYNC); break;
    case 30: PlaySound(L"Sound/30.Death_Shturm.wav", NULL, SND_FILENAME | SND_ASYNC); break;
    default: break;
    }
}

string getTime() {
    time_t now = time(nullptr);
    tm localTime;
    string timeName;
        
    if (localtime_s(&localTime, &now) == 0) // Используем localtime_s для потокобезопасного получения времени
        timeName = 
            to_string(localTime.tm_year + 1900) + "_" +
            to_string(localTime.tm_mon + 1) + "_" +
            to_string(localTime.tm_mday) + "_" +
            to_string(localTime.tm_hour) + "_" +
            to_string(localTime.tm_min) + "_" +
            to_string(localTime.tm_sec);
    else cerr << "Ошибка при получении локального времени." << endl;
    return timeName;
}

int nextLVL(int code) {
    if (maps[checkRoom][userY][userX + 1] == '>' && code == 77 && bag.size() >= coinsRoom[checkRoom]) {
        checkRoom++;
        if (checkRoom > thisLvL || bag.size() == sumCoins) {
            system("cls"); thisLvL++;
            switch (thisLvL) {
                case 1: { track(2); break; }  //AnimeWow
                case 2: { track(13); break; } //GiveYouUp
                case 3: { track(8); break; } //CatRap
                case 4: { track(16); break; } //Initial_D
                default: break;
            }
            cout << endl << endl; readText(gameText[11]); cout << endl << endl << endl; system("pause");
        }
        maps[checkRoom - 1][userY][userX] = ' ';
        userX = 1; return 1;
    }
    else if (maps[checkRoom][userY][userX + 1] == '>' && code == 77 && bag.size() < coinsRoom[checkRoom]) {
        code = 0; system("cls");
        track(1); //5Coins
        cout << endl << endl; readText(gameText[10]); cout << endl << endl << endl; system("pause");
        track(5); //BloodMoney2
        return 0;
    }
    else if (maps[checkRoom][userY][userX - 1] == '<' && code == 75) {
        maps[checkRoom][userY][userX] = ' ';
        checkRoom--; userX = maps[checkRoom][userY].size() - 2; return 1;
    }
    else return code;
}
void move(int code) {
    int moveX{ 0 }, moveY{ 0 };
    switch (code) {
        case 75: { moveX = -1; break; } // Влево
        case 77: { moveX = 1; break; } // Вправо
        case 72: { moveY = -1; break; } // Вверх
        case 80: { moveY = 1; break; } // Вниз
        default: break;
    } 
    if (maps[checkRoom][userY + moveY][userX + moveX] != '#') {
        //Ловушка - портал
        bool check = true;
        if (maps[checkRoom][userY + moveY][userX + moveX] == 'S') {
            track(15); //Hurt
            if (flag) { maps[checkRoom][userY][userX] = 'X'; flag = 0; }
            else maps[checkRoom][userY][userX] = ' ';
            userX = 1; system("cls"); check = false;
        }

        //Сбор денег
        if (check) {
            if (flag) { maps[checkRoom][userY][userX] = 'X'; flag = 0; }
            else maps[checkRoom][userY][userX] = ' ';
            if (maps[checkRoom][userY + moveY][userX + moveX] == '$') {
                track(9); //CoinsMario
                bag.push_back('$'); flag = 1;
            }
            else if (maps[checkRoom][userY + moveY][userX + moveX] == 'X') flag = 1;
            else flag = 0;

            userX += moveX; userY += moveY;
        }
    }
}

//Директория и названия файлов
string pathLvL = "Text/Data/";
vector <string> LvLFiles = { "LvL1.txt","LvL2.txt", "LvL3.txt", "LvL4.txt", "LvL5.txt" };
vector <string> LvL;
string sourceFile = "Source.txt";

string infoText = "Text/Data/InfoText.txt";
string MENU = "Text/Data/Menu.txt";
string SAVE = "Text/Save/Save.txt";
string SAVENAME = "Text/Save/SaveName.txt";
string LOAD = "Text/Save/Load.txt";

//Выгружаем Текст для игры и меню
vector <string> gameText = loadText(gameText, infoText);
vector <string> menu = loadText(menu, MENU);
vector <string> save = loadText(save, SAVE);
vector <string> saveName = loadText(saveName, SAVENAME);
vector <string> load = loadText(load, LOAD);

//Выгружаем все карты и добавляем монеты
vector <vector <string>> maps;
vector <int> coinsRoom;
int Coins = 5;
int sumCoins;
vector <char> bag;

//Интерфейс
int userX = 1;
int userY = 1;
int checkRoom = 0;
int thisLvL = 0;
bool flag = false;

//Проверка на Старт игры
bool checkStarGame = false;
bool exitGame = false;
bool exitMenu = false;
bool offMenu = true;