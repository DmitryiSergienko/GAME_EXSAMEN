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
#include <chrono> // ��� ����������� �������
#include "Header.h"
using namespace std;

// ������� ��� ��������������� ������ �����
void playM(const string & musicFile) {
    sf::SoundBuffer buffer; // ����� ��� �������� �������� ������
    if (!buffer.loadFromFile(musicFile)) {
        cout << "Error loading sound file: " << musicFile << endl;
        return;
    }

    sf::Sound sound(buffer); // ������� ������ Sound, ��������� � �������
    cout << "Playing: " << musicFile << endl;
    sound.play(); // �������� ���������������

    // ����, ���� ���� �� ����������
    while (sound.getStatus() == sf::Sound::Status::Playing) {
        this_thread::sleep_for(chrono::milliseconds(100)); // ��������� ��������
    }
}

// ������� ��� ��������������� ������� ������
void playMusicQueue(const vector<string>& musicFiles) {
    for (const auto& file : musicFiles) {
        playM(file); // ������������� ������ ���� �� �������
    }
}

int main() { 
//	SetConsoleOutputCP(1251); SetConsoleCP(1251); srand(time(NULL));
//	setVolume(int(65535 * double(soundLVL) * 10/100)); // 50% | 65535 - max
//	//Menu(); return 0; 
//
//
//    // ���� � ������ � �������
    vector<string> musicQueue1 = {
        "C:/Users/Sergienko/source/repos/GAME_EXSAMEN/Sound/4.BloodMoney.wav"
    };

    vector<string> musicQueue2 = {
        "C:/Users/Sergienko/source/repos/GAME_EXSAMEN/Sound/10.CRY.wav",
        "C:/Users/Sergienko/source/repos/GAME_EXSAMEN/Sound/7.ByeBye.wav",
        "C:/Users/Sergienko/source/repos/GAME_EXSAMEN/Sound/2.AnimeWow.wav"
    };

    // ������� ��� ������ ��� ��������������� ������ �������� ������
    thread thread1(playMusicQueue, ref(musicQueue1));
    thread thread2(playMusicQueue, ref(musicQueue2));

    // ������� ���������� ����� �������
    if (thread1.joinable()) {
        thread1.join();
    }

    if (thread2.joinable()) {
        thread2.join();
    }

    cout << "Both tracks have finished playing." << endl;

    return 0;
}