#pragma once

#include "my_direct2d.h"
#include <chrono>
#include <random>

using std::chrono::milliseconds;


struct Particle
{
	D2D1_POINT_2F speed = { 0 };
	D2D1_POINT_2F acceleration = { 0 };

	D2D1_POINT_2F position = { 0 };

	milliseconds life = milliseconds(0);
	milliseconds start_life = milliseconds(0);

	bool alive = false;
};


class ParticleSystem
{
private:
	// resources
	D2D1_COLOR_F particle_color;
	D2D1_SIZE_F particle_size;
	ID2D1SolidColorBrush* particle_brush;

	void createResources(ID2D1HwndRenderTarget* d2hrt);
	void deleteResources();

	//variables
	unsigned n;
	Particle* particles;

	float particles_per_second;

	// generators
	std::mt19937 generator;

	std::uniform_real_distribution<float> position_x;
	std::uniform_real_distribution<float> position_y;

	std::uniform_real_distribution<float> speed_x;
	std::uniform_real_distribution<float> speed_y;

	std::uniform_real_distribution<float> acceleration_x;
	std::uniform_real_distribution<float> acceleration_y;

	std::uniform_int_distribution<int> life_time;
public:
	ParticleSystem(
		unsigned count, float per_second,
		D2D1_RECT_F position, D2D1_RECT_F speed, D2D1_RECT_F acceleration,
		D2D1_POINT_2U life,
		D2D1_SIZE_F size,
		ID2D1HwndRenderTarget* d2hrt, D2D1_COLOR_F color
	);
	~ParticleSystem();

	void recreateResources(ID2D1HwndRenderTarget* d2hrt);

	void update(milliseconds delta);
	void createParticles(unsigned count);
	void render(ID2D1HwndRenderTarget* d2hrt) const;

	inline void setParticlesPerSecond(float per_second) { particles_per_second = per_second; };
};