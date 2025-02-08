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

// �������� ������� �������� � ����������
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
// �������� �����

void Menu() {
    addPathFiles();
    if (off) {
        off = false;
        track(23); //StartMario
    }

    int startPosY{ 6 }, choice{ startPosY }, posX{ 17 };
    while (true) {
        system("cls");
        menu[choice][posX] = '@';
        printMenu(menu);
        int select = _getch(); if (select == 0) select = _getch();

        int moveY{ 0 }; menu[choice][posX] = ' ';
        switch (select) {
        case 72: { // �����
            if (choice > startPosY) {
                moveY = -1; track(14); //GtaMenu
            } break;
        }
        case 80: { // ����
            if (choice < startPosY + 4) {
                moveY = 1; track(14); //GtaMenu
            } break;
        }
        case 13: {
            if (choice == startPosY && checkStarGame) { Game(); break; } //����������
            else if (choice == startPosY + 1) {
                checkStarGame = true; track(4); //BloodMoney
                createContent(); Game(); break;
            } //����� ����
            else if (choice == startPosY + 2 && checkStarGame) { saveWindow(); break; } //���������
            else if (choice == startPosY + 3) { loadWindow(); break; } //���������
            else if (choice == startPosY + 4) { exitGame = true; break; } //�����
        }
        case 27: { if (checkStarGame) { Game(); break; } }
        default: break;
        } if (exitGame && !checkStarGame) {
            system("cls");
            track(7); //ByeBye
            cout << endl << endl << endl << endl; readText(gameText[13]); cout << endl << endl << endl << endl;
            system("pause"); system("cls"); break;
        }
        else if (exitGame && bag.size() >= 25) {
            system("cls");
            track(24); //WindowsXP
            cout << endl << endl << endl << endl; readText(gameText[15]); cout << endl << endl << endl << endl;
            system("pause"); system("cls"); break; } //�����
        else if (exitGame) {
            system("cls");
            track(10); //CRY
            cout << endl << endl << endl << endl; readText(gameText[14]); cout << endl << endl << endl << endl;
            system("pause"); system("cls"); break; } //�����

        if (choice >= startPosY && choice <= startPosY + 4) choice += moveY;
        menu[choice - moveY][posX] = ' ';
    }
}

void Game() {
    sumCoins = LvL.size() * Coins;
    while (true) {
        system("cls");
        track(5); //BloodMoney2
        while (true) {
            if (exitGame) { system("cls"); break; } //�����
            if (bag.size() == sumCoins) { //������
                system("cls"); track(21); //MissionComplite
                cout << endl << endl << endl << endl; readText(gameText[12]); cout << endl << endl << endl << endl;
                system("pause"); bag.push_back('G'); bag.push_back('G'); bag.push_back('W'); bag.push_back('P'); system("cls");
            }
            maps[checkRoom][userY][userX] = '@';
            printMap(maps[checkRoom]);
            printBag(bag);

            int code = _getch(); if (code == 0) code = _getch();
            if (code == 27) { exitMenu = true; break; } // ����� � ����

            int checkCode = nextLVL(code);//������� �� ����� �������
            if (checkCode != 1) code = checkCode;
            move(code); system("cls"); //������

        } if (bag.size() == sumCoins) break; //������
        if (exitGame || exitMenu) { track(14); /*GtaMenu*/ system("cls"); break; } //����� � ����
    } system("cls");
    if (exitGame) {
        
    }
    system("cls");
}