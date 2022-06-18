#pragma once

#include <thread>
#include <vector>

#include "../resource.h"

#include "my_direct2d.h"
#include "ParticleSystem.h"

#include "wap.h"


using std::chrono::milliseconds;
using std::chrono::system_clock;
using std::chrono::duration_cast;


class WaaWindow : public WapWindow
{
private:
	// Direct 2d resources
	ID2D1Factory* d2f;
	ID2D1HwndRenderTarget* d2hrt;
	ID2D1Bitmap* back_bitmap;

	void createIndependentResources();
	void deleteIndependentResources();
	void createDependentResources();
	void deleteDependentResources();

	const float fire_per_second = 120.0f;
	const unsigned fire_per_click = 200;
	std::vector<ParticleSystem*> particle_systems;
	void createParticleSystems();

	std::vector<std::pair<float, float>> torches;
	void loadTorchesPostions();

	// Waa functions
	int fps;
	milliseconds frame_duration;

	void waaCycle();
	void waaUpdate(const milliseconds delta);
	void waaRender();

	// Wap messages
	int onMouseMove(int x, int y);
	int onMouseDown(MouseButton mb, int x, int y);
	int onMouseUp(MouseButton mb, int x, int y);

	friend void myLoadBitmap(IWICImagingFactory* wf, ID2D1HwndRenderTarget* d2hrt, const wchar_t* filename, ID2D1Bitmap** bitmap);
	friend void myLoadResourceBitmap(IWICImagingFactory* wf, ID2D1HwndRenderTarget* d2hrt, int resource_id, ID2D1Bitmap** bitmap);
public:
	WaaWindow(int fps = 60);
	~WaaWindow();
};

