#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <vector>
#include <ctime> // ��� ��������� �������
#include <fstream>
#include <direct.h> // ��� �������� �����
#include <filesystem> // ��� �������� �����
#include <shellapi.h> // ��� �������� �����
#include <io.h> // ��� _access - �������� ������� �����
#include <string>
#include <mmsystem.h> // ��� PlaySound() � waveOutSetVolume() - (������)
#pragma comment(lib, "winmm.lib") // ��� PlaySound() � waveOutSetVolume() - (������)
#include <thread> // ��� �����������
#include <SFML/Audio.hpp> // ��� ����������� SFML (audio)
#include <chrono> // ��� ����������� �������
#include "Header.h"
#include "MusicPlayer.h"
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
        if (!fileMap.is_open()) throw "���� �� ������!!!";
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
    if (!fileMap.is_open()) { cout << "���� �� ������!!" << endl; system("pause"); }
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

    //������� ����
    for (int i{ 7 }; i <= 9; i++) { readText(gameText[i]); cout << endl; }
    cout << endl; system("pause"); system("cls");
}
void clearData() {
    maps.clear(); coinsRoom.clear(); bag.clear();
    userX = 1; userY = 1; checkRoom = 0; thisLvL = 0; flag = false;
}
void createFolder(const char* path) {
    if (_mkdir(path) == 0) cout << "����� ������� �������: " << path << endl;
    else cerr << "�� ������� ������� �����." << endl;
}
void clearFolder(const string& path) {
    try {
        // ���������, ���������� �� ����
        if (!fs::exists(path)) cerr << "���� �� ����������: " << path << endl;

        // ���������, �������� �� ���� �����������
        if (!fs::is_directory(path)) cerr << "��������� ���� �� �������� �����������: " << path << endl;

        // ������� ���������� � ��� � ����������
        fs::remove_all(path);
    }
    catch (const fs::filesystem_error& e) { cerr << "������ ��� �������� ����������: " << e.what() << endl; }
}
bool folderExists(const string& path) {
    return fs::exists(path) && fs::is_directory(path);
}
int deleteSave(const char* directory, string nameSaveFolder) {
    vector<string> questionDelete; // �� ����� ������ ������� ����������?
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
            break; } // �����
        case 77: {
            if (x == 4) x = 13;
            else if (x == 13) x = 23;
            break; } // ������
        case 13: { 
            if (x == 4) { system("cls"); return 0; } // ������� ����
            else if (x == 13){ // ������� ���� ����������
                string backSpace = "\\";
                string path = directory; path += backSpace + nameSaveFolder;
                clearFolder(path); return 1; }
            else if (x == 23) { // ������� ��� ����������
                clearFolder(saveName[4]);
                return 2; }
            break;
        }
        case 27: { system("cls"); return 0; } // ������� ����
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
                    if(map[y][x] == '#') setColor(0x88);         // �����-����� - 136
                    else if (map[y][x] == ' ')  setColor(0x77);  // ������-����� - 119
                    else if (map[y][x] == '@')  setColor(0xCC);  // ��������� - 204
                    else if (map[y][x] == '!')  setColor(0xDD);  // ������� - 221
                    else if (map[y][x] == '$')  setColor(0x67);  // ������ ��� � ����������� ����� - 103
                    else if (map[y][x] == 'X')  setColor(0x77);  // ������-����� - 119
                    else if (map[y][x] == 'S')  setColor(0x67);  // ������ ��� � ����������� ����� - 103
                    else if (map[y][x] == '>')  setColor(0x55);  // ���������� - 85
                    else if (map[y][x] == '<')  setColor(0x55);  // ���������� - 85
                    else if (map[y][x] == 'H')  setColor(0x00);  // ������ - 0
                    else if (map[y][x] == 'W')  setColor(0x11);  // ����� - 17
                    else if (map[y][x] == 'M')  setColor(0x22);  // ������� - 34
                    else if (map[y][x] == 'G')  setColor(0x22);  // ������� - 34
                    cout << map[y][x];
                    setColor(0x70);
                }
            } cout << endl;
        }
    } cout << endl << endl;
    setColor(0x07); // ����������� � ����������� ����������
}
void printMenu(vector <string> menu) {
    for (int i = 0; i < menu.size(); i++) {
        for (int j = 0; j < menu[i].size(); j++) {
            cout << menu[i][j];
        } cout << endl;
    } cout << endl << endl;
}
void printBag(vector <char> bag) {
    setColor(0x06);  // ������ ����� - 103
    cout << "����� $: " << bag.size() << endl;
    /*for (int i = 0; i < bag.size(); i++) {
        cout << bag[i];
    } cout << endl;*/
    setColor(0x07); // ����������� � ����������� ����������
}

void saveWindow() {
    int startPosY{ 6 }, choice{ startPosY }, posX{ 17 }; exitMenu = false;
    // �������� ������ ����������� ��� � ���� ����������
    for (int i{ 0 }; i < 4; i++) save[choice + i].replace(posX + 2, saveName[i].size(), saveName[i]);
    
    while (true) {
        system("cls");
        save[choice][posX] = '@';
        printMenu(save);
        int select = _getch(); if (select == 0 || select == 224) select = _getch();

        int moveY{ 0 }; save[choice][posX] = ' ';
        switch (select) {
        case 72: { if (choice > startPosY) moveY = -1; track(14); /*GtaMenu*/ break; } // �����
        case 80: { if (choice < startPosY + 4) moveY = 1; track(14); /*GtaMenu*/ break; } // ����
        case 13: {
            if (choice <= startPosY + 3) { saveLineWindow(posX + 2, choice, startPosY); break; } // ��������� ����
            else { system("cls"); exitMenu = true; break; } // ����� � ����
            break;
        }
        case 83: { // ������� ����������
            if (saveName[choice - startPosY] != "---") {
                int check = deleteSave((saveName[4]).c_str(), saveName[choice - startPosY]);
                
                if (check == 0) break; // ����� � ����
                else if (check == 1) {
                    if (saveName[choice - startPosY] == saveName[5]) saveName[5] = "Empty"; // ������� ���������� ��� ������ ����������

                    save[choice] = save[3]; // ������� ���� ����������
                    load[choice] = load[3];
                    saveName[choice - startPosY] = "---";
                    save[choice].replace(posX + 2, saveName[choice - startPosY].size(), saveName[choice - startPosY]);
                    load[choice].replace(posX + 2, saveName[choice - startPosY].size(), saveName[choice - startPosY]);
                                        
                    for (int i{ 0 }; i < 4; i++) if (saveName[i] != "---") saveName[5] = saveName[i]; // ��������� ���� ���� ���� ��� ����������

                    saveText("Text/Data/", "SaveName.txt", saveName);
                    system("cls"); printMenu(save);
                    track(10); // CRY
                    readText(gameText[30]); cout << endl; system("pause");
                    break;
                }
                else if (check == 2) { // ������� ��� ����������
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
        case 27: { system("cls"); exitMenu = true; break; } // ����� � ����
        default: break;
        }
        if (choice >= startPosY && choice <= startPosY + 4) choice += moveY;
        save[choice - moveY][posX] = ' ';
        if (exitMenu) break; // ����� � ����
    }
}
string saveGame(const char* directory, string nameSaveFolder) {
    directory = (saveName[4]).c_str();
    if (_access(directory, 0) == -1) createFolder(directory); // �������� �����

    string nameSaveInWindow;
    if (nameSaveFolder == "Default") nameSaveInWindow = getTime(); // �������� �����
    else nameSaveInWindow = nameSaveFolder;
    nameSaveFolder = directory; nameSaveFolder += "/"; nameSaveFolder += nameSaveInWindow;
    const char* path = nameSaveFolder.c_str();
    if (_access(path, 0) == -1) createFolder(path);

    for (int i{ 0 }; i < LvLFiles.size(); i++) saveText(path, LvLFiles[i], maps[i]); // ���������� ����

    // ��������� � ������ �������
    vector<string> source;
    string _bag(bag.begin(), bag.end());
    source.push_back("������"); source.push_back(_bag);

    // ��������� � ������ ��������� ���������
    source.push_back("����� �����"); source.push_back(to_string(checkRoom));
    source.push_back("���������� �� X"); source.push_back(to_string(userX));
    source.push_back("���������� �� Y"); source.push_back(to_string(userY));
    source.push_back("����� ������"); source.push_back(to_string(thisLvL));
    source.push_back("�������� �����"); source.push_back(to_string(flag));

    saveText(path, "Source.txt", source); // ���������� ���� �������� � ��������� ���������
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
    // �������� ������ ����������� ��� � ���� ��������        
    for (int i{ 0 }; i < 4; i++) load[choice + i].replace(posX + 2, saveName[i].size(), saveName[i]);

    while (true) {
        system("cls");
        load[choice][posX] = '@';
        printMenu(load);
        int select = _getch(); if (select == 0 || select == 224) select = _getch();

        int moveY{ 0 }; load[choice][posX] = ' ';
        switch (select) {
        case 72: { if (choice > startPosY) moveY = -1; track(14); /*GtaMenu*/ break; } // �����
        case 80: { if (choice < startPosY + 4) moveY = 1; track(14); /*GtaMenu*/ break; } // ����
        case 13: {
            if ((choice < startPosY + 4) && saveName[choice - startPosY] != "---") { // ��������� ����
                checkStarGame = true; player.start(); /*BloodMoney*/
                loadGame(choice, startPosY, posX); break;
            }
            else if (choice == startPosY + 4) { system("cls"); exitMenu = true; break; } // ����� � ����
            else break;
        }
        case 83: { // ������� ����������
            if (saveName[choice - startPosY] != "---") {
                int check = deleteSave((saveName[4]).c_str(), saveName[choice - startPosY]);

                if (check == 0) break; // ����� � ����
                else if (check == 1) {
                    if (saveName[choice - startPosY] == saveName[5]) saveName[5] = "Empty"; // ������� ���������� ��� ������ ����������
                    
                    save[choice] = save[3]; // ������� ���� ����������
                    load[choice] = load[3];
                    saveName[choice - startPosY] = "---";
                    save[choice].replace(posX + 2, saveName[choice - startPosY].size(), saveName[choice - startPosY]);
                    load[choice].replace(posX + 2, saveName[choice - startPosY].size(), saveName[choice - startPosY]);
                    
                    for (int i{ 0 }; i < 4; i++) if (saveName[i] != "---") saveName[5] = saveName[i]; // ��������� ���� ���� ���� ��� ����������
                    
                    saveText("Text/Data/", "SaveName.txt", saveName);
                    system("cls"); printMenu(load);
                    track(10); // CRY
                    readText(gameText[30]); cout << endl; system("pause"); exitMenu = true;
                    break;
                }
                else if (check == 2) { // ������� ��� ����������
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
        case 27: { system("cls"); exitMenu = true; break; } // ����� � ����
        default: break;
        }
        if (choice >= startPosY && choice <= startPosY + 4) choice += moveY;
        load[choice - moveY][posX] = ' ';
        if (exitMenu) break; // ����� � ����
    }
}
void loadGame(int choice, int startPosY, int posX) {
    try {
        string nameSaveFolder = saveName[choice - startPosY];
        string directory = saveName[4], BackSlash = "/";
        pathLvL = directory + BackSlash + nameSaveFolder + BackSlash;
        addPathFiles(); // ��������� ����� ���� � ����� ��������

        if (!(folderExists(pathLvL))) throw gameText[34].c_str(); // ��������� ����� �� �������
        clearData(); // ������� ��� ������ ����� ���������
        for (int i{ 0 }; i < LvL.size(); i++) { // �������� ����
            vector <string> temp = loadText(temp, LvL[i]);
            maps.push_back(temp);
            coinsRoom.push_back(Coins * (i + 1));
        }

        sourceFile = "Source.txt"; sourceFile = pathLvL + BackSlash + sourceFile;
        // ��������� � ������ �������
        vector<string> source = loadText(source, sourceFile);
        for (int i{ 0 }; i < source[1].size(); i++) bag.push_back(source[1][i]); // ��������� ���������

        // ��������� � ������ ��������� ���������
        checkRoom = stoi(source[3]);
        userX = stoi(source[5]);
        userY = stoi(source[7]);
        thisLvL = stoi(source[9]);
        flag = bool(stoi(source[11]));
        Game();
    }
    catch (const char* err) { cerr << err << endl;
        save[choice] = save[3]; // ������� ���� ����������
        load[choice] = load[3];
        saveName[choice - startPosY] = "---";
        save[choice].replace(posX + 2, saveName[choice - startPosY].size(), saveName[choice - startPosY]);
        load[choice].replace(posX + 2, saveName[choice - startPosY].size(), saveName[choice - startPosY]);

        for (int i{ 0 }; i < 4; i++) if (saveName[i] != "---") saveName[5] = saveName[i]; // ��������� ���� ���� ���� ��� ����������

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
        case 72: { if (choice > startPosY) moveY = -1; track(14); /*GtaMenu*/ break; } // �����
        case 80: { if (choice < startPosY + 5) moveY = 1; track(14); /*GtaMenu*/ break; } // ����
        case 13: {
            if (choice == startPosY) { settingSound(choice, posX); exitMenu = false; break; } // ��������� �����
            else if (choice > startPosY && choice < startPosY + 4) { // ����� ���������
                cout << "�����!\n"; break;
            }
            else if (choice == startPosY + 4) { changePathSave(); exitMenu = false; break; }
            else { system("cls"); exitMenu = true; break; } // ����� � ����
            break;
        }
        case 27: { system("cls"); exitMenu = true; break; } // ����� � ����
        default: break;
        }
        if (choice >= startPosY && choice <= startPosY + 5) choice += moveY;
        settings[choice - moveY][posX] = ' ';
        if (exitMenu) break; // ����� � ����
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
        case 77: { if (soundLVL < 10) soundLVL++; track(14); /*GtaMenu*/ break; } // �����
        case 75: { if (soundLVL > 0) soundLVL--; track(14); /*GtaMenu*/ break; } // ����
        case 27: { 
            lvlSound = "      "; settings[choice].replace(posX + 12, lvlSound.size(), lvlSound);
            settings[choice][posX + 12] = ' '; exitMenu = true; break; } // �����
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
    // ������������� ���������� ��������� ��� ����� ������� (����� � ������)
    unsigned long vol = static_cast<unsigned long>(volume | (volume << 16));
    waveOutSetVolume(NULL, vol); // NULL ��������� �� ������������� �������� ���������� ������
}
void playMusic(const string& musicFile) {
    // ������� ��� ��������������� ������ �����
    sf::SoundBuffer buffer; // ����� ��� �������� �������� ������
    if (!buffer.loadFromFile(musicFile)) {
        cout << "Error loading sound file: " << musicFile << endl;
        return;
    }

    sf::Sound sound(buffer); // ������� ������ Sound, ��������� � �������
    sound.play(); // �������� ���������������

    // ����, ���� ���� �� ����������
    while (sound.getStatus() == sf::Sound::Status::Playing)
        this_thread::sleep_for(chrono::milliseconds(100)); // ��������� ��������
}
void playMusicQueue(const vector<string>& musicFiles) {
    // ������� ��� ��������������� ������� ������
    for (const auto& file : musicFiles) 
        playMusic(file);
}
void playTrack(const wstring& filePath) {
    // �������� ���������� Unicode-������ PlaySound
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
        
    if (localtime_s(&localTime, &now) == 0) // ���������� localtime_s ��� ����������������� ��������� �������
        timeName = 
            to_string(localTime.tm_year + 1900) + "_" +
            to_string(localTime.tm_mon + 1) + "_" +
            to_string(localTime.tm_mday) + "_" +
            to_string(localTime.tm_hour) + "_" +
            to_string(localTime.tm_min) + "_" +
            to_string(localTime.tm_sec);
    else cerr << "������ ��� ��������� ���������� �������." << endl;
    return timeName;
}

int nextLVL(int select) {
    if (maps[checkRoom][userY][userX + 1] == '>' && select == 77 && bag.size() >= coinsRoom[checkRoom]) {
        checkRoom++;
        if (checkRoom > thisLvL || bag.size() == sumCoins) {
            system("cls"); thisLvL++;
            switch (thisLvL) {
                case 1: { player.pause(); /*BloodMoney*/ playerLvL1.start(); break; }  //AnimeWow
                case 2: { player.pause(); /*BloodMoney*/ playerLvL2.start(); break; } //GiveYouUp
                case 3: { player.pause(); /*BloodMoney*/ playerLvL3.start(); break; } //CatRap
                case 4: { player.pause(); /*BloodMoney*/ playerLvL4.start(); break; } //Initial_D
                default: break;
            }
            cout << endl << endl; readText(gameText[11]); cout << endl << endl << endl; 
            system("pause");
            switch (thisLvL) {
                case 1: { playerLvL1.stop(); break; }  //AnimeWow
                case 2: { playerLvL2.stop(); break; } //GiveYouUp
                case 3: { playerLvL3.stop(); break; } //CatRap
                case 4: { playerLvL4.stop(); break; } //Initial_D
            default: break;
            }
            /*this_thread::sleep_for(chrono::seconds(1));*/ player.resume();
        }
        maps[checkRoom - 1][userY][userX] = ' ';
        userX = 1; return 1;
    }
    else if (maps[checkRoom][userY][userX + 1] == '>' && select == 77 && bag.size() < coinsRoom[checkRoom]) {
        select = 0; system("cls");
        track(1); //5Coins
        cout << endl << endl; readText(gameText[10]); cout << endl << endl << endl; system("pause");
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
    case 72: { if (choice > startPosY) { moveY = -1; track(14); /*GtaMenu*/ } break; } // �����            
    case 80: { if (choice < startPosY + 5) { moveY = 1; track(14); /*GtaMenu*/ } break; } // ����
    case 13: {
        if (choice == startPosY) {
            if (checkStarGame == false && saveName[5] != "Empty") { 
                checkStarGame = true; player.start(); /*BloodMoney*/ loadGame(5, 0, 0);
                break; } // ���������� � ���������� ����������
            else if (checkStarGame == false) { track(26); readText(gameText[16]); cout << endl; system("pause"); break; }
            else { player.resume(); /*BloodMoney*/ Game(); break; }
        } // ����������
        else if (choice == startPosY + 1) { // ����� ����
            checkStarGame = true; pathLvL = "Text/Data/"; addPathFiles();
            player.start(); /*BloodMoney*/ createContent(); Game(); break;
        }
        else if (choice == startPosY + 2 && checkStarGame) { saveWindow(); break; } // ���������
        else if (choice == startPosY + 2) { track(27); readText(gameText[17]); cout << endl; system("pause"); break; }
        else if (choice == startPosY + 3) { // ���������
            bool checkLoad{ false };
            for (int i{ 0 }; i < 4; i++) if (saveName[i] != "---") checkLoad = true;
            if (checkLoad) loadWindow();
            else { track(28); readText(gameText[16]); cout << endl; system("pause"); } break;
        }
        else if (choice == startPosY + 4) { settingsWindow(); break; } // ���������
        else if (choice == startPosY + 5) { outGame = true; exitGame(); break; } // �����
        else break;
    }
    case 27: { if (checkStarGame) { player.resume(); /*BloodMoney*/ Game(); break; } else break; }
    default: break;
    }
    return moveY;
}
void moveGame(int select) {
    int moveX{ 0 }, moveY{ 0 };
    switch (select) {
        case 75: { moveX = -1; break; } // �����
        case 77: { moveX = 1; break; } // ������
        case 72: { moveY = -1; break; } // �����
        case 80: { moveY = 1; break; } // ����
        default: break;
    } 
    if (maps[checkRoom][userY + moveY][userX + moveX] != '#') {
        //������� - ������
        bool check = true;
        if (maps[checkRoom][userY + moveY][userX + moveX] == 'S') {
            track(15); //Hurt
            if (flag) { maps[checkRoom][userY][userX] = 'X'; flag = 0; }
            else maps[checkRoom][userY][userX] = ' ';
            userX = 1; system("cls"); check = false;
        }

        //���� �����
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
    } // �����
    else if (outGame) {
        system("cls");
        track(10); // CRY
        cout << endl << endl << endl << endl; readText(gameText[14]); cout << endl << endl << endl << endl;
        system("pause"); system("cls"); return true;
    } // �����
    else return false;
}

//���������� � �������� ������
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


//��������� ����� ��� ���� � ����
vector <string> gameText = loadText(gameText, infoText);
vector <string> menu = loadText(menu, MENU);
vector <string> save = loadText(save, SAVE);
vector <string> saveName = loadText(saveName, SAVENAME);
vector <string> load = loadText(load, LOAD);
vector <string> settings = loadText(settings, SETTINGS);

//��������� ��� ����� � ��������� ������
vector <vector <string>> maps;
vector <int> coinsRoom;
int Coins = 5;
int sumCoins;
vector <char> bag;

//���������
int userX = 1;
int userY = 1;
int checkRoom = 0;
int thisLvL = 0;
bool flag = false;

//�������� �� ����� ����
bool checkStarGame = false;
bool outGame = false;
bool exitMenu = false;
bool offMenu = true;

int soundLVL = stoi(gameText[22]);
vector<string> tracks = {
    "C:/Users/Sergienko/source/repos/GAME_EXSAMEN/Sound/4.BloodMoney.wav",
    "C:/Users/Sergienko/source/repos/GAME_EXSAMEN/Sound/2.AnimeWow.wav",
    "C:/Users/Sergienko/source/repos/GAME_EXSAMEN/Sound/13.GiveYouUp.wav",
    "C:/Users/Sergienko/source/repos/GAME_EXSAMEN/Sound/8.CatRap.wav",
    "C:/Users/Sergienko/source/repos/GAME_EXSAMEN/Sound/16.Initial_D.wav",
    "C:/Users/Sergienko/source/repos/GAME_EXSAMEN/Sound/21.MissionComplite.wav",
};
MusicPlayer player(tracks[0]);
MusicPlayer playerLvL1(tracks[1]);
MusicPlayer playerLvL2(tracks[2]);
MusicPlayer playerLvL3(tracks[3]);
MusicPlayer playerLvL4(tracks[4]);
MusicPlayer playerLvL5(tracks[5]);

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);