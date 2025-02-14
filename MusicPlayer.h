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
    string file; // ���� � �����
    sf::Music music; // ������ ��� ��������������� ������
    atomic<bool> isPaused{ false }; // ���� �����
    atomic<bool> isStopped{ false }; // ���� ���������
    mutex mtx; // ������� ��� �������������
    condition_variable cv; // �������� ���������� ��� ���������� ������

    void play(); // ����� ��� ��������������� ������ � ��������� ������

public:
    MusicPlayer(const string& musicFile) : file(musicFile), isPaused(false), isStopped(false) {}

    void loadTrack(const string& musicFile);
    void start();
    void pause();
    void resume();
    void stop();
};

#endif // MUSICPLAYER_H