#include "Common.hpp"

#include <iostream>
#include <random>
#include <cassert>

std::mt19937 Random::rng{std::random_device{}()};

void errorExit(const std::string& msg) {
	std::cerr << "ERROR: " << msg << '\n';
	exit(EXIT_FAILURE);
}

Timer::Timer(float timeLeft) :
	timeLeft(timeLeft), lastTime(std::chrono::high_resolution_clock::now()) {

}

bool Timer::isTimeLeft() {
	assert(isStarted);
	passTime();
	return timeLeft > 0;
}

void Timer::start() {
	assert(!isStarted);
	isStarted = true;
	lastTime = std::chrono::high_resolution_clock::now();
}

void Timer::stop() {
	assert(isStarted);
	isStarted = false;
	passTime();
}

void Timer::passTime() {
	auto end = std::chrono::high_resolution_clock::now();
	timeLeft -= std::chrono::duration<float>(end - lastTime).count() * 1000;
	lastTime = end;
}