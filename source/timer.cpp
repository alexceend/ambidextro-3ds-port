#include "timer.h"

Timer::Timer(int seconds) : duration_sec_(seconds) {}

void Timer::start()
{
    start_time_ = std::chrono::steady_clock::now();
    running_ = true;
    paused_ = false;
    finished_ = false;
}


void Timer::reset(int seconds)
{
    // -1 reuses last timer;
    if (seconds > 0) duration_sec_ = seconds;
    start();
}

void Timer::pause()
{
    if (!running_ || paused_) return;
    pause_tp_ = std::chrono::steady_clock::now();
    paused_   = true;
}

void Timer::resume()
{
    if (!running_ || !paused_) return;
    paused_accum_ += std::chrono::steady_clock::now() - pause_tp_;
    paused_        = false;
}

void Timer::stop()
{
    running_ = false;
    paused_  = false;
}

bool Timer::isRunning() const  { return running_ && !paused_ && !finished_; }
bool Timer::isFinished() const { return finished_; }

float Timer::getRemainingTime() const
{
    if (!running_) return 0.0f;

    auto now = std::chrono::steady_clock::now();
    auto elapsed = now - start_time_ - paused_accum_;
    if (paused_) elapsed -= (now - pause_tp_);

    float remaining = duration_sec_ - std::chrono::duration<float>(elapsed).count();
    return remaining > 0.0f ? remaining : 0.0f;
}

int Timer::getRemainingTimeInSeconds() const
{
    return static_cast<int>(std::ceil(getRemainingTime()));
}


void Timer::Update(EventType event, void* callback)
{
    switch (event)
    {
        case PUASE:  paused_ ? resume() : pause(); break;
        case WIN: stop(); break;
        case DEATH:  reset(-1);  break;
        default: break;
    }
}