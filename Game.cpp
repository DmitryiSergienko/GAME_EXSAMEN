#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <vector>
#include <ctime> // ��� ��������� �������
#include <fstream>
#include <direct.h> // ��� �������� �����
#include <shellapi.h> // ��� �������� �����
#include <io.h> // ��� _access - �������� ������� �����
#include <string>
#include <mmsystem.h> // ��� PlaySound() � waveOutSetVolume() - �������
#pragma comment(lib, "winmm.lib") // ��� PlaySound() � waveOutSetVolume() - �������
#include <thread> // ��� �����������
#include <SFML/Audio.hpp> // ��� ����������� SFML (audio)
#include "Header.h"
using namespace std;

// �������� ����� � ��� �������������� � �������� �������� �� - ���� � ������� � �����
// �������� ������� � ������
// �������� ��������� �������
// �������� ������� � ������
// �������� ������ � ������ � ������
// �������� ��������� � �����
// �������� ��� � �������
// �������� �������-������� ��� ������ ��� � ������
// �������� ������ (1�2 ������, ���� �����)
// �������� ����-�������� (��������: 1 ������ - 1 �����, 2 ������ - �����, 3 ������ - �����)
// �������� ����������� �������� HP
// �������� ���� �����, ������� ���������, ����� �������
// ���������� ����� ���������

void Menu() {
    addPathFiles(); // ��������� ���� � ������
    sumCoins = LvL.size() * Coins; // ������������ ���-�� ����� � ����
    if (offMenu) { offMenu = false; track(23); } //StartMario
    
    int startPosY{ 6 }, choice{ startPosY }, posX{ 17 };
    while (true) {
        setColor(0x0B);
        system("cls");
        menu[choice][posX] = '@';
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
        track(5); // BloodMoney2
        while (true) {
            if (outGame) { system("cls"); break; } // �����
            if (bag.size() == sumCoins) { // ������
                system("cls"); track(21); // MissionComplite
                cout << endl << endl << endl << endl; readText(gameText[12]); cout << endl << endl << endl << endl;
                system("pause"); bag.push_back('G'); bag.push_back('G'); bag.push_back('W'); bag.push_back('P'); system("cls");
            }
            maps[checkRoom][userY][userX] = '@';
            printMap(maps[checkRoom], 1, 0);
            printBag(bag);

            int select = _getch(); if (select == 0 || select == 224) select = _getch();
            if (select == 27) { exitMenu = true; break; } // ����� � ����

            int checkCode = nextLVL(select);// ������� �� ����� �������
            if (checkCode != 1) select = checkCode;
            moveGame(select); system("cls"); // ������
        } 
        if (bag.size() == sumCoins) break; // ������
        if (exitMenu) { track(14); /*GtaMenu*/ system("cls"); Menu(); break; } // ����� � ����
    }
}