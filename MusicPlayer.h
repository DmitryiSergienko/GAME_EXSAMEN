#pragma once
#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <chrono>
#include <SFML/Audio.hpp>

using namespace std;

class MusicPlayer {
    string file; // Путь к файлу
    sf::Music music; // Объект для воспроизведения музыки
    atomic<bool> isPaused{ false }; // Флаг паузы
    atomic<bool> isStopped{ false }; // Флаг остановки
    mutex mtx; // Мьютекс для синхронизации
    condition_variable cv; // Условная переменная для управления паузой

    void play(); // Метод для воспроизведения музыки в отдельном потоке

public:
    MusicPlayer(const string& musicFile) : file(musicFile), isPaused(false), isStopped(false) {}

    void loadTrack(const string& musicFile);
    void start();
    void pause();
    void resume();
    void stop();
};

#endif // MUSICPLAYER_H