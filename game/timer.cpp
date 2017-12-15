#include "timer.h"

Timer::Timer() {
    bPaused = false;
    runTime = 0;
    main_clock.restart();
}

void Timer::Reset() {
    main_clock.restart();
    runTime = 0;
    bPaused = false;
}

void Timer::Start() {
    if (bPaused) {
            main_clock.restart();
    }
    bPaused = false;
}

void Timer::Pause() {
    if (!bPaused) {
            runTime += main_clock.getElapsedTime().asSeconds();
    }
    bPaused = true;
}

float Timer::GetElapsedSeconds() {
    if (!bPaused) {
            return runTime + main_clock.getElapsedTime().asSeconds();
    }
    return runTime;
}
