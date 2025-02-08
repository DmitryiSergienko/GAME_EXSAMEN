#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <vector>
#include <ctime> // ��� ��������� �������
#include <fstream>
#include <direct.h> // ��� �������� �����
#include <io.h> // ��� _access - �������� ������� �����
#include <string>
#include <mmsystem.h> // ��� PlaySound() � waveOutSetVolume()
#pragma comment(lib, "winmm.lib") // ����������� ���������� winmm.lib
#include "Header.h"
using namespace std;

void addPathFiles() {
    for (int i = 0; i < LvLFiles.size(); i++) LvL.push_back(pathLvL + LvLFiles[i]);
}
vector <string> loadText(vector <string> text, string nameFile) {
    string textPush;
    fstream fileMap{ nameFile, ios::in };
    if (!fileMap.is_open()) {
        cout << "���� �� ������!!" << endl;
    }
    for (int i = 0; getline(fileMap, textPush); i++) {
        text.push_back(textPush);
    }
    fileMap.close();
    return text;
}
void createFolder(const char* path) {
    if (_mkdir(path) == 0) cout << "����� ������� �������: " << path << endl;
    else cerr << "�� ������� ������� �����." << endl;
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
    cout << "���������� $: " << bag.size() << endl;
    for (int i = 0; i < bag.size(); i++) {
        cout << bag[i];
    } cout << endl;
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
        case 72: { if (choice > startPosY) moveY = -1; break; } // �����
        case 80: { if (choice < startPosY + 4) moveY = 1; break; } // ����
        case 13: {
            if (choice == startPosY) { saveGame(); cout << "���� ���������!\n"; break; } //��������� 1 ����
            else if (choice == startPosY + 1) { cout << "�������� 2!"; break; } //��������� 2 ����
            else if (choice == startPosY + 2) { cout << "�������� 3!"; break; } //��������� 3 ����
            else if (choice == startPosY + 3) { cout << "�������� 4!"; break; } //��������� 4 ����
            else if (choice == startPosY + 4) { exitSave = true; break; } //�����
        }
        case 27: { exitSave = true; break; } //�����
        default: break;
        }if (exitSave) break; //�����
        if (choice >= startPosY && choice <= startPosY + 4) choice += moveY;
        save[choice - moveY][posX] = ' ';
    }
}
void saveGame(const char* directory, string nameSaveFolder) {
    if (_access(directory, 0) == -1) createFolder(directory); // �������� �����
    cout << "�������� �����: " << directory << endl << endl; system("pause");

    if (nameSaveFolder == "Standart") {                         // �������� �����
        nameSaveFolder = directory; nameSaveFolder += "/"; nameSaveFolder += getTime();
    }
    else {
        string temp = nameSaveFolder;
        nameSaveFolder = directory; nameSaveFolder += "/"; nameSaveFolder += temp;
    }
    const char* path = nameSaveFolder.c_str();
    if (_access(path, 0) == -1) createFolder(path);
    cout << "�������� �����: " << path << endl << endl; system("pause");

    for (int i{ 0 }; i < LvLFiles.size(); i++) saveText(path, LvLFiles[i], maps[i]);
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
        case 72: { if (choice > startPosY) moveY = -1; break; } // �����
        case 80: { if (choice < startPosY + 4) moveY = 1; break; } // ����
        case 13: {
            if (choice == startPosY) { cout << "�������� 1!"; break; } //��������� 1 ����
            else if (choice == startPosY + 1) { cout << "�������� 2!"; break; } //��������� 2 ����
            else if (choice == startPosY + 2) { cout << "�������� 3!"; break; } //��������� 3 ����
            else if (choice == startPosY + 3) { cout << "�������� 4!"; break; } //��������� 4 ����
            else if (choice == startPosY + 4) { exitLoad = true; break; } //�����
        }
        case 27: { exitLoad = true; break; } //�����
        default: break;
        } if (exitLoad) break; //�����
        if (choice >= startPosY && choice <= startPosY + 4) choice += moveY;
        load[choice - moveY][posX] = ' ';
    }
}

void setVolume(unsigned int volume) {
    // ������������� ���������� ��������� ��� ����� ������� (����� � ������)
    unsigned long vol = static_cast<unsigned long>(volume | (volume << 16));
    waveOutSetVolume(NULL, vol); // NULL ��������� �� ������������� �������� ���������� ������
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
    default: break;
    }
}

string getTime() {
    time_t now = time(nullptr);
    tm localTime;
    string timeName;
        
    if (localtime_s(&localTime, &now) == 0) // ���������� localtime_s ��� ����������������� ��������� �������
        timeName = 
            to_string(localTime.tm_year + 1900) + "\'" +
            to_string(localTime.tm_mon + 1) + "\'" +
            to_string(localTime.tm_mday) + "\'" +
            to_string(localTime.tm_hour) + "\'" +
            to_string(localTime.tm_min) + "\'" +
            to_string(localTime.tm_sec);
    else cerr << "������ ��� ��������� ���������� �������." << endl;
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
                case 3: { track(22); break; } //Sexy_Black
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

//���������� � �������� ������
string pathLvL = "Text/Data/";
vector <string> LvLFiles = { "LvL1.txt","LvL2.txt", "LvL3.txt", "LvL4.txt", "LvL5.txt" };
vector <string> LvL;

string infoText = "Text/Data/InfoText.txt";
string MENU = "Text/Data/Menu.txt";
string SAVE = "Text/Save/Save.txt";
string LOAD = "Text/Save/Load.txt";

//��������� ����� ��� ���� � ����
vector <string> gameText = loadText(gameText, infoText);
vector <string> menu = loadText(menu, MENU);
vector <string> save = loadText(save, SAVE);
vector <string> load = loadText(load, LOAD);

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
bool exitGame = false;
bool exitMenu = false;
bool off = true;