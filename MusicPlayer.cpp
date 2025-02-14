#include "MusicPlayer.h"
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <chrono>
#include <SFML/Audio.hpp>

using namespace std;

// Метод для воспроизведения музыки в отдельном потоке
void MusicPlayer::play() {
    if (!music.openFromFile(file)) {
        //cout << "ERROR: Unable to load music file: " << file << endl;
        return;
    }

    music.play(); // Начинаем воспроизведение
    //cout << "PLAY: Music started." << endl;

    while (true) {
        unique_lock<mutex> lock(mtx);

        // Если музыка остановлена, выходим из цикла
        if (isStopped) {
            //cout << "THREAD: Exiting due to stop request." << endl;
            break;
        }

        // Если музыка на паузе, ждем сигнал для возобновления
        if (isPaused) {
            //cout << "THREAD: Waiting for resume signal..." << endl;
            cv.wait(lock, [this]() { return !isPaused || isStopped; }); // Ждем, пока не будет вызван resume()
            if (!isStopped) { // Проверяем, что музыка не остановлена
                music.play(); // Возобновляем воспроизведение
                //cout << "THREAD: Resumed playback after wait." << endl;
            }
            else {
                //cout << "THREAD: Stop requested during wait." << endl;
            }
        }

        lock.unlock(); // Разблокируем мьютекс, чтобы не блокировать выполнение

        // Проверяем статус музыки
        if (music.getStatus() != sf::Music::Status::Playing) {
            //cout << "THREAD: Playback finished." << endl;
            break; // Если музыка закончилась, выходим
        }

        this_thread::sleep_for(chrono::milliseconds(100)); // Небольшая задержка
    }

    //cout << "THREAD: Playback thread terminated." << endl;
}

// Загрузить и начать воспроизведение нового трека
void MusicPlayer::loadTrack(const string& musicFile) {
    unique_lock<mutex> lock(mtx);

    // Если музыка уже играет, останавливаем её
    if (!isStopped) {
        stop();
    }

    file = musicFile; // Обновляем путь к файлу
    isStopped = false; // Сбрасываем флаг остановки
    thread playbackThread(&MusicPlayer::play, this);
    playbackThread.detach(); // Отсоединяем поток, чтобы он жил независимо
    cout << "TRACK LOADED: " << file << endl;
}

// Начать воспроизведение
void MusicPlayer::start() {
    thread playbackThread(&MusicPlayer::play, this);
    playbackThread.detach(); // Отсоединяем поток, чтобы он жил независимо
}

// Поставить на паузу
void MusicPlayer::pause() {
    unique_lock<mutex> lock(mtx);
    if (!isPaused && !isStopped) { // Проверяем, что музыка играет
        music.pause(); // Вызываем метод pause() из sf::Music
        isPaused = true;
       //cout << "PAUSE: Music paused." << endl;
    }
    else {
       //cout << "PAUSE: Music already paused or stopped." << endl;
    }
}

// Возобновить воспроизведение
void MusicPlayer::resume() {
    unique_lock<mutex> lock(mtx);
    if (isPaused && !isStopped) { // Проверяем, что музыка на паузе
        isPaused = false; // Сбрасываем флаг паузы
        cv.notify_one(); // Пробуждаем поток для возобновления
        //cout << "RESUME: Music resumed (signal sent)." << endl;
    }
    else {
        //cout << "RESUME: Music not paused or already stopped." << endl;
    }
}

// Остановить воспроизведение
void MusicPlayer::stop() {
    unique_lock<mutex> lock(mtx);
    if (!isStopped) { // Проверяем, что музыка не уже остановлена
        isStopped = true;
        music.stop(); // Вызываем метод stop() из sf::Music
        cv.notify_one(); // Пробуждаем поток для завершения
        //cout << "STOP: Music stopped." << endl;
    }
    else {
        //cout << "STOP: Music already stopped." << endl;
    }
}