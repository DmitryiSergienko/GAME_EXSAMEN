#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <vector>
#include <ctime> // Для получения времени
#include <fstream>
#include <direct.h> // Для создания папки
#include <filesystem> // Для удаления папки
#include <io.h> // Для _access - проверка наличия папки
#include <string>
#include <mmsystem.h> // Для PlaySound() и waveOutSetVolume()
#pragma comment(lib, "winmm.lib") // Для PlaySound() и waveOutSetVolume()
#include "Header.h"
using namespace std;
namespace fs = std::filesystem;

void addPathFiles() {
    LvL.clear();
    for (int i = 0; i < LvLFiles.size(); i++) LvL.push_back(pathLvL + LvLFiles[i]);
}
vector <string> loadText(vector <string> text, string nameFile) {
    try {
        string textPush;
        fstream fileMap{ nameFile, ios::in };
        if (!fileMap.is_open()) throw "Файл не найден!!!";
        for (int i = 0; getline(fileMap, textPush); i++) {
            text.push_back(textPush);
        }
        fileMap.close();
    }
    catch (const char* err) { cerr << err << endl; }
    return text;
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
void createFolder(const char* path) {
    if (_mkdir(path) == 0) cout << "Папка успешно создана: " << path << endl;
    else cerr << "Не удалось создать папку." << endl;
}
void clearFolder(const string& path) {
    try {
        // Проверяем, существует ли путь
        if (!fs::exists(path)) cerr << "Путь не существует: " << path << endl;

        // Проверяем, является ли путь директорией
        if (!fs::is_directory(path)) cerr << "Указанный путь не является директорией: " << path << endl;

        // Удаляем директорию и все её содержимое
        fs::remove_all(path);
    }
    catch (const fs::filesystem_error& e) { cerr << "Ошибка при удалении директории: " << e.what() << endl; }
}
bool folderExists(const string& path) {
    return fs::exists(path) && fs::is_directory(path);
}
int deleteSave(const char* directory, string nameSaveFolder) {
    vector<string> questionDelete; // Вы точно хотите удалить сохранение?
    for (int i{ 23 }; i < 30; i++) questionDelete.push_back(gameText[i]);
    int y{ 4 }, x{ 4 };
    questionDelete[y][x] = '@';
    while (true) {
        system("cls"); printMenu(questionDelete);
        int select = _getch(); if (select == 0 || select == 224) select = _getch();
        questionDelete[y][x] = ' ';
        
        switch (select) {
        case 75: {
            if (x == 23) x = 13;
            else if (x == 13) x = 4;
            break; } // Влево
        case 77: {
            if (x == 4) x = 13;
            else if (x == 13) x = 23;
            break; } // Вправо
        case 13: { 
            if (x == 4) { system("cls"); return 0; } // Закрыть окно
            else if (x == 13){ // Удалить одно сохранение
                string backSpace = "\\";
                string path = directory; path += backSpace + nameSaveFolder;
                clearFolder(path); return 1; }
            else if (x == 23) { // Удалить все сохранения
                clearFolder(saveName[4]);
                return 2; }
            break;
        }
        case 27: { system("cls"); return 0; } // Закрыть окно
        default: break;
        }
        questionDelete[y][x] = '@';
    }
}

void setColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}
void printMap(vector <string> map) {
    for (int i = 0; i < map.size(); i++) {
        for (int j = 0; j < map[i].size(); j++) {
            cout << map[i][j] << " ";
        } cout << endl;
    } cout << endl << endl;
}
void printMap(vector <string> map, int sizeMap, int colorType) {
    for (int y = 0; y < map.size(); y++) {
        for (int spaceY{ 0 }; spaceY < sizeMap; spaceY++) {
            for (int x = 0; x < map[y].size(); x++) {
                for (int spaceX{ 0 }; spaceX < (2 * sizeMap); spaceX++) {
                    if(map[y][x] == '#') setColor(0x88);         // Темно-серый - 136
                    else if (map[y][x] == ' ')  setColor(0x77);  // Светло-серый - 119
                    else if (map[y][x] == '@')  setColor(0xCC);  // Малиновый - 204
                    else if (map[y][x] == '!')  setColor(0xDD);  // Розовый - 221
                    else if (map[y][x] == '$')  setColor(0x67);  // Желтый фон и светлосерый текст - 103
                    else if (map[y][x] == 'X')  setColor(0x77);  // Светло-серый - 119
                    else if (map[y][x] == 'S')  setColor(0x67);  // Желтый фон и светлосерый текст - 103
                    else if (map[y][x] == '>')  setColor(0x55);  // Фиолетовый - 85
                    else if (map[y][x] == '<')  setColor(0x55);  // Фиолетовый - 85
                    else if (map[y][x] == 'H')  setColor(0x00);  // Черный - 0
                    else if (map[y][x] == 'W')  setColor(0x11);  // Синий - 17
                    else if (map[y][x] == 'M')  setColor(0x22);  // Зеленый - 34
                    else if (map[y][x] == 'G')  setColor(0x22);  // Зеленый - 34
                    cout << map[y][x];
                    setColor(0x70);
                }
            } cout << endl;
        }
    } cout << endl << endl;
    setColor(0x07); // Возвращение к стандартным настройкам
}
void printMenu(vector <string> menu) {
    for (int i = 0; i < menu.size(); i++) {
        for (int j = 0; j < menu[i].size(); j++) {
            cout << menu[i][j];
        } cout << endl;
    } cout << endl << endl;
}
void printBag(vector <char> bag) {
    setColor(0x06);  // Желтый текст - 103
    cout << "Монет $: " << bag.size() << endl;
    /*for (int i = 0; i < bag.size(); i++) {
        cout << bag[i];
    } cout << endl;*/
    setColor(0x07); // Возвращение к стандартным настройкам
}

void saveWindow() {
    int startPosY{ 6 }, choice{ startPosY }, posX{ 17 }; exitMenu = false;
    // Загрузка списка сохраненных игр в окне Сохранение
    for (int i{ 0 }; i < 4; i++) save[choice + i].replace(posX + 2, saveName[i].size(), saveName[i]);
    
    while (true) {
        system("cls");
        save[choice][posX] = '@';
        printMenu(save);
        int select = _getch(); if (select == 0 || select == 224) select = _getch();

        int moveY{ 0 }; save[choice][posX] = ' ';
        switch (select) {
        case 72: { if (choice > startPosY) moveY = -1; track(14); /*GtaMenu*/ break; } // Вверх
        case 80: { if (choice < startPosY + 4) moveY = 1; track(14); /*GtaMenu*/ break; } // Вниз
        case 13: {
            if (choice <= startPosY + 3) { saveLineWindow(posX + 2, choice, startPosY); break; } // Сохранить игру
            else { system("cls"); exitMenu = true; break; } // Выход в меню
            break;
        }
        case 83: { // Удалить сохранение
            if (saveName[choice - startPosY] != "---") {
                int check = deleteSave((saveName[4]).c_str(), saveName[choice - startPosY]);
                
                if (check == 0) break; // Выход в меню
                else if (check == 1) {
                    if (saveName[choice - startPosY] == saveName[5]) saveName[5] = "Empty"; // Очищаем сохранение для пункта Продолжить

                    save[choice] = save[3]; // Удаляем одно сохранение
                    load[choice] = load[3];
                    saveName[choice - startPosY] = "---";
                    save[choice].replace(posX + 2, saveName[choice - startPosY].size(), saveName[choice - startPosY]);
                    load[choice].replace(posX + 2, saveName[choice - startPosY].size(), saveName[choice - startPosY]);
                                        
                    for (int i{ 0 }; i < 4; i++) if (saveName[i] != "---") saveName[5] = saveName[i]; // Добавляем сейв если есть для Продолжить

                    saveText("Text/Data/", "SaveName.txt", saveName);
                    system("cls"); printMenu(save);
                    track(10); // CRY
                    readText(gameText[30]); cout << endl; system("pause");
                    break;
                }
                else if (check == 2) { // Удаляем все сохранения
                    for (int i{ 0 }; i < 4; i++) {
                        saveName[i] = "---";
                        load[startPosY + i] = load[3];
                        save[startPosY + i] = save[3];
                        load[startPosY + i].replace(posX + 2, saveName[0].size(), saveName[0]);
                        save[startPosY + i].replace(posX + 2, saveName[0].size(), saveName[0]);
                    }
                    saveName[5] = "Empty";
                    saveText("Text/Data/", "SaveName.txt", saveName);
                    system("cls"); printMenu(save);
                    track(10); // CRY
                    readText(gameText[31]); cout << endl; system("pause");
                    break;
                }
            }
            else break; break;
        }
        case 27: { system("cls"); exitMenu = true; break; } // Выход в меню
        default: break;
        }
        if (choice >= startPosY && choice <= startPosY + 4) choice += moveY;
        save[choice - moveY][posX] = ' ';
        if (exitMenu) break; // Выход в меню
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
    string path = saveName[saveName.size() - 2] + backSlash + saveName[choice - startPosY];
    if (saveName[choice - startPosY] != "---") { clearFolder(path);
        readText(gameText[19]); cout << endl; system("pause"); }
    else { readText(gameText[18]); cout << endl; system("pause"); }
    string emptyName = "                           ";
    save[choice].replace(posX, emptyName.size(), emptyName);
    saveName[choice - startPosY] = saveGame();
    saveName[5] = saveName[choice - startPosY];
    save[choice].replace(posX, saveName[choice - startPosY].size(), saveName[choice - startPosY]);
    saveText((SAVENAME.substr(0, 9)).c_str(), SAVENAME.substr(10, 21), saveName);
}
void loadWindow() {
    int startPosY{ 6 }, choice{ startPosY }, posX{ 17 }; exitMenu = false;
    // Загрузка списка сохраненных игр в окне Загрузка        
    for (int i{ 0 }; i < 4; i++) load[choice + i].replace(posX + 2, saveName[i].size(), saveName[i]);

    while (true) {
        system("cls");
        load[choice][posX] = '@';
        printMenu(load);
        int select = _getch(); if (select == 0 || select == 224) select = _getch();

        int moveY{ 0 }; load[choice][posX] = ' ';
        switch (select) {
        case 72: { if (choice > startPosY) moveY = -1; track(14); /*GtaMenu*/ break; } // Вверх
        case 80: { if (choice < startPosY + 4) moveY = 1; track(14); /*GtaMenu*/ break; } // Вниз
        case 13: {
            if ((choice < startPosY + 4) && saveName[choice - startPosY] != "---") { // Загрузить игру
                checkStarGame = true;
                loadGame(choice, startPosY, posX); break;
            }
            else if (choice == startPosY + 4) { system("cls"); exitMenu = true; break; } // Выход в меню
            else break;
        }
        case 83: { // Удалить сохранение
            if (saveName[choice - startPosY] != "---") {
                int check = deleteSave((saveName[4]).c_str(), saveName[choice - startPosY]);

                if (check == 0) break; // Выход в меню
                else if (check == 1) {
                    if (saveName[choice - startPosY] == saveName[5]) saveName[5] = "Empty"; // Очищаем сохранение для пункта Продолжить
                    
                    save[choice] = save[3]; // Удаляем одно сохранение
                    load[choice] = load[3];
                    saveName[choice - startPosY] = "---";
                    save[choice].replace(posX + 2, saveName[choice - startPosY].size(), saveName[choice - startPosY]);
                    load[choice].replace(posX + 2, saveName[choice - startPosY].size(), saveName[choice - startPosY]);
                    
                    for (int i{ 0 }; i < 4; i++) if (saveName[i] != "---") saveName[5] = saveName[i]; // Добавляем сейв если есть для Продолжить
                    
                    saveText("Text/Data/", "SaveName.txt", saveName);
                    system("cls"); printMenu(load);
                    track(10); // CRY
                    readText(gameText[30]); cout << endl; system("pause"); exitMenu = true;
                    break;
                }
                else if (check == 2) { // Удаляем все сохранения
                    for (int i{ 0 }; i < 4; i++) {
                        saveName[i] = "---";
                        load[startPosY + i] = load[3];
                        save[startPosY + i] = save[3];
                        load[startPosY + i].replace(posX + 2, saveName[0].size(), saveName[0]);
                        save[startPosY + i].replace(posX + 2, saveName[0].size(), saveName[0]);
                    }
                    saveName[5] = "Empty";
                    saveText("Text/Data/", "SaveName.txt", saveName);
                    system("cls"); printMenu(load);
                    track(10); // CRY
                    readText(gameText[31]); cout << endl; system("pause"); exitMenu = true;
                    break;
                }
            }
            else break; break;
        }
        case 27: { system("cls"); exitMenu = true; break; } // Выход в меню
        default: break;
        }
        if (choice >= startPosY && choice <= startPosY + 4) choice += moveY;
        load[choice - moveY][posX] = ' ';
        if (exitMenu) break; // Выход в меню
    }
}
void loadGame(int choice, int startPosY, int posX) {
    try {
        string nameSaveFolder = saveName[choice - startPosY];
        string directory = saveName[4], BackSlash = "/";
        pathLvL = directory + BackSlash + nameSaveFolder + BackSlash;
        addPathFiles(); // Добавляем часть пути к папке загрузки

        if (!(folderExists(pathLvL))) throw gameText[34].c_str(); // Проверяем папку на наличие
        clearData(); // Очищаем все данные перед загрузкой
        for (int i{ 0 }; i < LvL.size(); i++) { // Загрузка карт
            vector <string> temp = loadText(temp, LvL[i]);
            maps.push_back(temp);
            coinsRoom.push_back(Coins * (i + 1));
        }

        sourceFile = "Source.txt"; sourceFile = pathLvL + BackSlash + sourceFile;
        // Добавляем в вектор ресурсы
        vector<string> source = loadText(source, sourceFile);
        for (int i{ 0 }; i < source[1].size(); i++) bag.push_back(source[1][i]); // Заполняем инвентарь

        // Добавляем в вектор состаяние элементов
        checkRoom = stoi(source[3]);
        userX = stoi(source[5]);
        userY = stoi(source[7]);
        thisLvL = stoi(source[9]);
        flag = bool(stoi(source[11]));
        Game();
    }
    catch (const char* err) { cerr << err << endl;
        save[choice] = save[3]; // Удаляем одно сохранение
        load[choice] = load[3];
        saveName[choice - startPosY] = "---";
        save[choice].replace(posX + 2, saveName[choice - startPosY].size(), saveName[choice - startPosY]);
        load[choice].replace(posX + 2, saveName[choice - startPosY].size(), saveName[choice - startPosY]);

        for (int i{ 0 }; i < 4; i++) if (saveName[i] != "---") saveName[5] = saveName[i]; // Добавляем сейв если есть для Продолжить

        saveText("Text/Data/", "SaveName.txt", saveName);
        system("pause");
    }
}
void settingsWindow() {
    int startPosY{ 6 }, choice{ startPosY }, posX{ 17 }; exitMenu = false;
    
    while (true) {
        system("cls");
        settings[choice][posX] = '@';
        printMenu(settings);
        int select = _getch(); if (select == 0) select = _getch();

        int moveY{ 0 }; settings[choice][posX] = ' ';
        switch (select) {
        case 72: { if (choice > startPosY) moveY = -1; track(14); /*GtaMenu*/ break; } // Вверх
        case 80: { if (choice < startPosY + 5) moveY = 1; track(14); /*GtaMenu*/ break; } // Вниз
        case 13: {
            if (choice == startPosY) { settingSound(choice, posX); exitMenu = false; break; } // Настройка звука
            else if (choice > startPosY && choice < startPosY + 4) { // Выбор настройки
                cout << "Круто!\n"; break;
            }
            else if (choice == startPosY + 4) { changePathSave(); exitMenu = false; break; }
            else { system("cls"); exitMenu = true; break; } // Выход в меню
            break;
        }
        case 27: { system("cls"); exitMenu = true; break; } // Выход в меню
        default: break;
        }
        if (choice >= startPosY && choice <= startPosY + 5) choice += moveY;
        settings[choice - moveY][posX] = ' ';
        if (exitMenu) break; // Выход в меню
    }
}
void settingSound(int choice, int posX) {
    settings[choice][posX] = '<'; settings[choice][posX + 12] = '>';
    string lvlSound = to_string(soundLVL * 10) + "%";
    while (true) {
        lvlSound = to_string(soundLVL * 10) + "%  ";
        settings[choice].replace(posX + 14, lvlSound.size(), lvlSound);
        system("cls"); printMenu(settings);

        int select = _getch(); if (select == 0 || select == 224) select = _getch();
        
        switch (select) {
        case 77: { if (soundLVL < 10) soundLVL++; track(14); /*GtaMenu*/ break; } // Вверх
        case 75: { if (soundLVL > 0) soundLVL--; track(14); /*GtaMenu*/ break; } // Вниз
        case 27: { 
            lvlSound = "      "; settings[choice].replace(posX + 12, lvlSound.size(), lvlSound);
            settings[choice][posX + 12] = ' '; exitMenu = true; break; } // Назад
        default: break;
        }
        setVolume(int(double(65535) * (double(soundLVL) * 10 / 100))); // 65535 - max

        gameText[22] = to_string(soundLVL);
        saveText("Text/Data/", "InfoText.txt", gameText);
        if (exitMenu) break;
    }   
}
void changePathSave() {
    readText(gameText[32]); readText(saveName[4]); cout << endl;
    readText(gameText[33]); string directory; getline(cin, directory);

    saveName[4] = directory;
    saveText("Text/Data/", "SaveName.txt", saveName);
    track(3);
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
//--------------------------------------------------------------------------------------Удалить и обновить на С++20
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

int nextLVL(int select) {
    if (maps[checkRoom][userY][userX + 1] == '>' && select == 77 && bag.size() >= coinsRoom[checkRoom]) {
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
    else if (maps[checkRoom][userY][userX + 1] == '>' && select == 77 && bag.size() < coinsRoom[checkRoom]) {
        select = 0; system("cls");
        track(1); //5Coins
        cout << endl << endl; readText(gameText[10]); cout << endl << endl << endl; system("pause");
        track(5); //BloodMoney2
        return 0;
    }
    else if (maps[checkRoom][userY][userX - 1] == '<' && select == 75) {
        maps[checkRoom][userY][userX] = ' ';
        checkRoom--; userX = maps[checkRoom][userY].size() - 2; return 1;
    }
    else return select;
}
int moveMenu(int select, int choice, int startPosY) {
    int moveY{ 0 };
    switch (select) {
    case 72: { if (choice > startPosY) { moveY = -1; track(14); /*GtaMenu*/ } break; } // Вверх            
    case 80: { if (choice < startPosY + 5) { moveY = 1; track(14); /*GtaMenu*/ } break; } // Вниз
    case 13: {
        if (choice == startPosY) {
            if (checkStarGame == false && saveName[5] != "Empty") { checkStarGame = true; loadGame(5, 0, 0); } // Продолжить с последнего сохранения
            else if (checkStarGame == false) { track(26); readText(gameText[16]); cout << endl; system("pause"); break; }
            else Game(); break;
        } // Продолжить
        else if (choice == startPosY + 1) { // Новая игра
            checkStarGame = true; track(4); // BloodMoney
            pathLvL = "Text/Data/"; addPathFiles();
            createContent(); Game(); break;
        }
        else if (choice == startPosY + 2 && checkStarGame) { saveWindow(); break; } // Сохранить
        else if (choice == startPosY + 2) { track(27); readText(gameText[17]); cout << endl; system("pause"); break; }
        else if (choice == startPosY + 3) { // Загрузить
            bool checkLoad{ false };
            for (int i{ 0 }; i < 4; i++) if (saveName[i] != "---") checkLoad = true;
            if (checkLoad) loadWindow();
            else { track(28); readText(gameText[16]); cout << endl; system("pause"); } break;
        }
        else if (choice == startPosY + 4) { settingsWindow(); break; } // Настройки
        else if (choice == startPosY + 5) { outGame = true; exitGame(); break; } // Выход
        else break;
    }
    case 27: { if (checkStarGame) { Game(); break; } else break; }
    default: break;
    }
    return moveY;
}
void moveGame(int select) {
    int moveX{ 0 }, moveY{ 0 };
    switch (select) {
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
bool exitGame() {
    if (outGame && !checkStarGame) {
        system("cls");
        track(7); // ByeBye
        cout << endl << endl << endl << endl; readText(gameText[13]); cout << endl << endl << endl << endl;
        system("pause"); system("cls"); return true;
    }
    else if (outGame && bag.size() >= 25) {
        system("cls");
        track(24); // WindowsXP
        cout << endl << endl << endl << endl; readText(gameText[15]); cout << endl << endl << endl << endl;
        system("pause"); system("cls"); return true;
    } // Выход
    else if (outGame) {
        system("cls");
        track(10); // CRY
        cout << endl << endl << endl << endl; readText(gameText[14]); cout << endl << endl << endl << endl;
        system("pause"); system("cls"); return true;
    } // Выход
    else return false;
}

//Директория и названия файлов
string pathLvL = "Text/Data/";
vector <string> LvLFiles = { "LvL1.txt","LvL2.txt", "LvL3.txt", "LvL4.txt", "LvL5.txt" };
vector <string> LvL;
string sourceFile = "Source.txt";

string infoText = "Text/Data/InfoText.txt";
string SAVENAME = "Text/Data/SaveName.txt";
string MENU = "Text/Windows/Menu.txt";
string SAVE = "Text/Windows/Save.txt";
string LOAD = "Text/Windows/Load.txt";
string SETTINGS = "Text/Windows/Settings.txt";


//Выгружаем Текст для игры и меню
vector <string> gameText = loadText(gameText, infoText);
vector <string> menu = loadText(menu, MENU);
vector <string> save = loadText(save, SAVE);
vector <string> saveName = loadText(saveName, SAVENAME);
vector <string> load = loadText(load, LOAD);
vector <string> settings = loadText(settings, SETTINGS);

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
bool outGame = false;
bool exitMenu = false;
bool offMenu = true;

int soundLVL = stoi(gameText[22]);

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);