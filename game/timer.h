#pragma once

#include <SFML/System.hpp>

struct Timer {
    Timer();

    void Reset();
    void Start();
    void Pause();
    float GetElapsedSeconds();

    sf::Clock main_clock;
    float runTime;
    bool bPaused;
};
