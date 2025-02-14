#include "MusicPlayer.h"
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <chrono>
#include <SFML/Audio.hpp>

using namespace std;

// ����� ��� ��������������� ������ � ��������� ������
void MusicPlayer::play() {
    if (!music.openFromFile(file)) {
        //cout << "ERROR: Unable to load music file: " << file << endl;
        return;
    }

    music.play(); // �������� ���������������
    //cout << "PLAY: Music started." << endl;

    while (true) {
        unique_lock<mutex> lock(mtx);

        // ���� ������ �����������, ������� �� �����
        if (isStopped) {
            //cout << "THREAD: Exiting due to stop request." << endl;
            break;
        }

        // ���� ������ �� �����, ���� ������ ��� �������������
        if (isPaused) {
            //cout << "THREAD: Waiting for resume signal..." << endl;
            cv.wait(lock, [this]() { return !isPaused || isStopped; }); // ����, ���� �� ����� ������ resume()
            if (!isStopped) { // ���������, ��� ������ �� �����������
                music.play(); // ������������ ���������������
                //cout << "THREAD: Resumed playback after wait." << endl;
            }
            else {
                //cout << "THREAD: Stop requested during wait." << endl;
            }
        }

        lock.unlock(); // ������������ �������, ����� �� ����������� ����������

        // ��������� ������ ������
        if (music.getStatus() != sf::Music::Status::Playing) {
            //cout << "THREAD: Playback finished." << endl;
            break; // ���� ������ �����������, �������
        }

        this_thread::sleep_for(chrono::milliseconds(100)); // ��������� ��������
    }

    //cout << "THREAD: Playback thread terminated." << endl;
}

// ��������� � ������ ��������������� ������ �����
void MusicPlayer::loadTrack(const string& musicFile) {
    unique_lock<mutex> lock(mtx);

    // ���� ������ ��� ������, ������������� �
    if (!isStopped) {
        stop();
    }

    file = musicFile; // ��������� ���� � �����
    isStopped = false; // ���������� ���� ���������
    thread playbackThread(&MusicPlayer::play, this);
    playbackThread.detach(); // ����������� �����, ����� �� ��� ����������
    cout << "TRACK LOADED: " << file << endl;
}

// ������ ���������������
void MusicPlayer::start() {
    thread playbackThread(&MusicPlayer::play, this);
    playbackThread.detach(); // ����������� �����, ����� �� ��� ����������
}

// ��������� �� �����
void MusicPlayer::pause() {
    unique_lock<mutex> lock(mtx);
    if (!isPaused && !isStopped) { // ���������, ��� ������ ������
        music.pause(); // �������� ����� pause() �� sf::Music
        isPaused = true;
       //cout << "PAUSE: Music paused." << endl;
    }
    else {
       //cout << "PAUSE: Music already paused or stopped." << endl;
    }
}

// ����������� ���������������
void MusicPlayer::resume() {
    unique_lock<mutex> lock(mtx);
    if (isPaused && !isStopped) { // ���������, ��� ������ �� �����
        isPaused = false; // ���������� ���� �����
        cv.notify_one(); // ���������� ����� ��� �������������
        //cout << "RESUME: Music resumed (signal sent)." << endl;
    }
    else {
        //cout << "RESUME: Music not paused or already stopped." << endl;
    }
}

// ���������� ���������������
void MusicPlayer::stop() {
    unique_lock<mutex> lock(mtx);
    if (!isStopped) { // ���������, ��� ������ �� ��� �����������
        isStopped = true;
        music.stop(); // �������� ����� stop() �� sf::Music
        cv.notify_one(); // ���������� ����� ��� ����������
        //cout << "STOP: Music stopped." << endl;
    }
    else {
        //cout << "STOP: Music already stopped." << endl;
    }
}