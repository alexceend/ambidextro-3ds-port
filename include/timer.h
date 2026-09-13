#ifndef TIMER_H
#define TIMER_H

#include <chrono>
#include <cstdint>

#include "game_manager.h"

extern Timer levelTimer;

class Timer : public IObserver
{
    public:
        Timer(int seconds);
        void start();
        void stop();
        void pause();
        void resume();
        void reset(int seconds);
        
        bool isRunning() const;
        bool isFinished() const;

        float getRemainingTime() const;
        int getRemainingTimeInSeconds() const;

        void Update(EventType event, void* callback) override;
    private:
        int duration_sec_;
        std::chrono::steady_clock::time_point start_time_;
        std::chrono::steady_clock::duration paused_accum_{};
        std::chrono::steady_clock::time_point pause_tp_{};

        bool running_ = false;
        bool paused_ = false;
        bool finished_ = false;

};

#endif