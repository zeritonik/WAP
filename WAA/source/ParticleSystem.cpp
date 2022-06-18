#include "ParticleSystem.h"


void ParticleSystem::createResources(ID2D1HwndRenderTarget* d2hrt)
{
	d2hrt->CreateSolidColorBrush(
		particle_color,
		&particle_brush
	);
}

void ParticleSystem::deleteResources()
{
	particle_brush->Release();
}


ParticleSystem::ParticleSystem(
	unsigned count, float per_second,
	D2D1_RECT_F position, D2D1_RECT_F speed, D2D1_RECT_F acceleration,
	D2D1_POINT_2U life,
	D2D1_SIZE_F size,
	ID2D1HwndRenderTarget* d2hrt, D2D1_COLOR_F color
) :
	n(count),
	particles(new Particle[count]),
	particles_per_second(per_second),

	position_x(position.left - size.width / 2, position.right - size.width / 2), position_y(position.top - size.height / 2, position.bottom - size.height / 2),
	speed_x(speed.left, speed.right), speed_y(speed.top, speed.bottom),
	acceleration_x(acceleration.left, acceleration.right), acceleration_y(acceleration.top, acceleration.bottom),
	life_time(life.x, life.y),

	particle_color(color),
	particle_size(size)
{
	std::random_device rd;
	generator = std::mt19937(rd());

	createResources(d2hrt);
}

ParticleSystem::~ParticleSystem()
{
	deleteResources();
	delete[] particles;
}


void ParticleSystem::recreateResources(ID2D1HwndRenderTarget* d2hrt)
{
	deleteResources();
	createResources(d2hrt);
}


void ParticleSystem::update(milliseconds delta)
{
	float time = float(delta.count()) / 1000.0f;
	for (unsigned i = 0; i < n; i++) {
		if (particles[i].alive) {
			particles[i].life -= delta;
			if (particles[i].life.count() <= 0) {
				particles[i].alive = false;
			}
			else {
				particles[i].position.x += particles[i].speed.x * time + particles[i].acceleration.x * time * time / 2;
				particles[i].position.y += particles[i].speed.y * time + particles[i].acceleration.y * time * time / 2;

				particles[i].speed.x += particles[i].acceleration.x * time;
				particles[i].speed.y += particles[i].acceleration.y * time;
			}
		}
	}

	createParticles(unsigned(particles_per_second * time));
}

void ParticleSystem::createParticles(unsigned count)
{
	for (unsigned i = 0; i < n && count; i++) {
		if (!particles[i].alive) {
			particles[i].position = D2D1::Point2F(position_x(generator), position_y(generator));
			particles[i].speed = D2D1::Point2F(speed_x(generator), speed_y(generator));
			particles[i].acceleration = D2D1::Point2F(acceleration_x(generator), acceleration_y(generator));
			particles[i].start_life = particles[i].life = milliseconds(life_time(generator));
			particles[i].alive = true;

			count--;
		}
	}
}

void ParticleSystem::render(ID2D1HwndRenderTarget* d2hrt) const
{
	for (unsigned i = 0; i < n; i++) {
		if (particles[i].alive) {
			d2hrt->FillRectangle(
				D2D1::RectF(
					particles[i].position.x, particles[i].position.y,
					particles[i].position.x + particle_size.width, particles[i].position.y + particle_size.height
				),
				particle_brush
			);
		}
	}
}