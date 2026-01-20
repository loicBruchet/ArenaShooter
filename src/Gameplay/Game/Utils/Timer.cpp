#include "Timer.h"

Timer::Timer(float _duration, bool _start, TimerBehavior _behavior) {
	m_behavior = _behavior;

	m_running = _start;

	m_duration = _duration;
	m_time = _start ? m_duration : 0.0f;

	listener = gce::GameManager::OnUpdate.AddListener(&Timer::Update, this);
}

Timer::~Timer() {
	OnCompleted.ClearListeners();
	gce::GameManager::OnUpdate.RemoveListener(listener);
}

void Timer::SetDuration(float _duration) {
	m_duration = _duration;
}

void Timer::Resume() { m_running = true; }
void Timer::Start() { Resume(); }

void Timer::Pause() { m_running = false; }
void Timer::Stop() { Pause(); }

void Timer::Reset() {
	m_time = 0.0f;
	m_running = false;
}

void Timer::Restart() {
	m_time = 0.0f;
	m_running = true;
}

const bool Timer::IsCompleted() const {
	return m_time >= m_duration;
}

const bool Timer::IsRunning() const {
	return m_running;
}

void Timer::Update() {
	if(!m_running)
		return;

	if (m_time >= m_duration) {
		switch (m_behavior) {
		case TimerBehavior::AutoRestart:
			Restart(); break;
		case TimerBehavior::Wait:
			Pause(); break;
		default:
			break;
		}

		OnCompleted.Invoke();
		return;
	}

	m_time += gce::GameManager::DeltaTime();
}