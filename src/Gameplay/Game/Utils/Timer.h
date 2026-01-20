#pragma once

#define DEFAULT_TIMER_DURATION 1.0f
#define DEFAULT_TIMER_START false
#define DEFAULT_TIMER_BEHAVIOR TimerBehavior::Wait

#include <Engine.h>

enum class TimerBehavior {
	AutoRestart, // Automatically restart the timer once completed
	Wait // Wait until manually restarted
};

class Timer {
public:
	/// <summary>
	/// Constructor for the Timer class.
	/// </summary>
	/// <param name="_duration">(float) Duration of the timer in seconds.</param>
	/// <param name="_start">(bool) Whether to start the timer automaticaly.</param>
	/// <param name="_behavior">(TimerBehavior) Behavior of the timer once completed.</param>
	Timer(float _duration = DEFAULT_TIMER_DURATION, bool _start = DEFAULT_TIMER_START, TimerBehavior _behavior = DEFAULT_TIMER_BEHAVIOR);
	Timer(const Timer& other) = delete;
	virtual ~Timer();

	 bool IsRunning() { return m_running; }


	/// <summary>
	/// Set the duration of the timer.
	/// </summary>
	/// <param name="_duration"></param>
	void SetDuration(float _duration);

	/// <summary>
	/// Start the timer from the current time.
	/// </summary>
	void Resume();

	/// <summary>
	/// Alias for Resume().
	/// </summary>
	void Start();

	/// <summary>
	/// Pause the timer.
	/// </summary>
	void Pause();

	/// <summary>
	/// Alias for Pause().
	/// </summary>
	void Stop();

	/// <summary>
	/// Stop & reset the current time back to 0.
	/// </summary>
	void Reset();

	/// <summary>
	/// Like Reset() but also starts the timer back again.
	/// </summary>
	void Restart();

	/// <summary>
	/// Returns true if the timer has reached its full duration.
	/// </summary>
	/// <returns></returns>
	const bool IsCompleted() const;
	
	/// <summary>
	/// Returns true if the timer is currently running.
	/// </summary>
	/// <returns></returns>
	const bool IsRunning() const;

	/// <summary>
	/// Event that is triggered once when the timer is completed.
	/// </summary>
	gce::Event<> OnCompleted;

protected:
	void Update();

	gce::Event<>::ListenerBase* listener;
	friend class gce::GameManager;

private:
	TimerBehavior m_behavior;

	float m_duration;
	float m_time;

	bool m_running;
};