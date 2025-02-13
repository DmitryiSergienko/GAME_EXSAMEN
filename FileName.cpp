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
#include <mmsystem.h> // Для PlaySound() и waveOutSetVolume() - Удалить
#pragma comment(lib, "winmm.lib") // Для PlaySound() и waveOutSetVolume() - Удалить
#include <thread> // Для многопотока
#include <SFML/Audio.hpp> // Для подключения SFML (audio)
#include <chrono> // Для управлением времени
#include "Header.h"
using namespace std;

// Функция для воспроизведения одного файла
void playM(const string & musicFile) {
    sf::SoundBuffer buffer; // Буфер для хранения звуковых данных
    if (!buffer.loadFromFile(musicFile)) {
        cout << "Error loading sound file: " << musicFile << endl;
        return;
    }

    sf::Sound sound(buffer); // Создаем объект Sound, связанный с буфером
    cout << "Playing: " << musicFile << endl;
    sound.play(); // Начинаем воспроизведение

    // Ждем, пока звук не завершится
    while (sound.getStatus() == sf::Sound::Status::Playing) {
        this_thread::sleep_for(chrono::milliseconds(100)); // Небольшая задержка
    }
}

// Функция для воспроизведения очереди треков
void playMusicQueue(const vector<string>& musicFiles) {
    for (const auto& file : musicFiles) {
        playM(file); // Воспроизводим каждый файл по очереди
    }
}

int main() { 
//	SetConsoleOutputCP(1251); SetConsoleCP(1251); srand(time(NULL));
//	setVolume(int(65535 * double(soundLVL) * 10/100)); // 50% | 65535 - max
//	//Menu(); return 0; 
//
//
//    // Пути к файлам с музыкой
    vector<string> musicQueue1 = {
        "C:/Users/Sergienko/source/repos/GAME_EXSAMEN/Sound/4.BloodMoney.wav"
    };

    vector<string> musicQueue2 = {
        "C:/Users/Sergienko/source/repos/GAME_EXSAMEN/Sound/10.CRY.wav",
        "C:/Users/Sergienko/source/repos/GAME_EXSAMEN/Sound/7.ByeBye.wav",
        "C:/Users/Sergienko/source/repos/GAME_EXSAMEN/Sound/2.AnimeWow.wav"
    };

    // Создаем два потока для воспроизведения разных очередей треков
    thread thread1(playMusicQueue, ref(musicQueue1));
    thread thread2(playMusicQueue, ref(musicQueue2));

    // Ожидаем завершения обоих потоков
    if (thread1.joinable()) {
        thread1.join();
    }

    if (thread2.joinable()) {
        thread2.join();
    }

    cout << "Both tracks have finished playing." << endl;

    return 0;
}