#include "WaaWindow.h"

#include <sstream>



void WaaWindow::createIndependentResources()
{
	CHECK_HR(
		D2D1CreateFactory(
		D2D1_FACTORY_TYPE_SINGLE_THREADED,
		&d2f), 
		"Didn't create factory"
	);
}

void WaaWindow::deleteIndependentResources()
{
	d2f->Release();
}

void WaaWindow::createDependentResources()
{
	RECT wr;
	GetWindowRect(getHWND(), &wr);

	CHECK_HR(
		d2f->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(
				D2D1_RENDER_TARGET_TYPE_HARDWARE, 
				D2D1::PixelFormat(DXGI_FORMAT_R8G8B8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED),
				0.0f, 0.0f,
				D2D1_RENDER_TARGET_USAGE_NONE,
				D2D1_FEATURE_LEVEL_DEFAULT
			),

			D2D1::HwndRenderTargetProperties(
				getHWND(), 
				D2D1::SizeU(wr.right - wr.left, wr.bottom - wr.top),
				D2D1_PRESENT_OPTIONS_IMMEDIATELY
			),
			&d2hrt
		),
		"Error create HwndRenderTarget"
	);

	IWICImagingFactory* wf = nullptr;

	CHECK_HR(
		CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&wf)),
		"Error create WICfactory"
	);

	myLoadResourceBitmap(wf, d2hrt, BACK_BITMAP_ID, &back_bitmap);

	wf->Release();
}

void WaaWindow::deleteDependentResources()
{
	back_bitmap->Release();
	d2hrt->Release();
}




void WaaWindow::createParticleSystems()
{
	float kx = d2hrt->GetSize().width / 1920.0f, ky = d2hrt->GetSize().height / 1080.0f;

	// fire
	for (auto p : torches) {
		particle_systems.push_back(new ParticleSystem(
			1000, fire_per_second,
			D2D1::RectF((p.first - 20.0f) * kx, (p.second - 20.0f) * ky, (p.first + 20.0f) * kx, (p.second + 10.0f) * ky),
			D2D1::RectF(-22.5f, -90.0f, 22.5f, -30.0f),
			D2D1::RectF(0.0f, 9.8f, 0.0f, 9.8f),
			D2D1::Point2U(400, 1000),
			D2D1::SizeF(25.0f * kx, 25.0f * ky),
			d2hrt,
			D2D1::ColorF(D2D1::ColorF::Orange, 0.2f)
		));
	}
}

void WaaWindow::loadTorchesPostions()
{
	std::stringstream ss;

	char* data = new char[100];
	int length = LoadStringA(NULL, TORCH_DATA_ID, data, 100);
	CHECK_HR(length ? S_OK : 0, "Error loading torches");

	ss << data;
	delete[] data;

	while (!ss.eof()) {
		float x, y;
		ss >> x >> y;

		torches.push_back({ x, y });
	}
}




void WaaWindow::waaCycle()
{
	CHECK_HR(CoInitialize(NULL), "Error CoInitialize");
	createIndependentResources();
	createDependentResources();
	loadTorchesPostions();
	createParticleSystems();

	system_clock::time_point frame_start;
	milliseconds last_frame_duration(0);

	timeBeginPeriod(4);
	while (running) {
		frame_start = system_clock::now();

		waaUpdate(last_frame_duration);
		waaRender();

		std::this_thread::sleep_until(frame_start + frame_duration);

		last_frame_duration = duration_cast<milliseconds>(system_clock::now() - frame_start);
	}
	timeEndPeriod(4);


	while (!particle_systems.empty()) {
		delete particle_systems.back();
		particle_systems.pop_back();
	}
	deleteDependentResources();
	deleteIndependentResources();
}

void WaaWindow::waaUpdate(const milliseconds delta)
{
	for (auto ps : particle_systems)
		ps->update(delta);
}

void WaaWindow::waaRender()
{
	HRESULT hr;

	d2hrt->BeginDraw();

	d2hrt->DrawBitmap(
		back_bitmap,
		D2D1::RectF(0.0f, 0.0f, d2hrt->GetSize().width, d2hrt->GetSize().height),
		1.0f,
		D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
		D2D1::RectF(0.0f, 0.0f, back_bitmap->GetSize().width, back_bitmap->GetSize().height)
	);

	for (auto ps : particle_systems)
		ps->render(d2hrt);

	hr = d2hrt->EndDraw();
	CHECK_HR(hr, "Error render");

	if (hr == D2DERR_RECREATE_TARGET) {
		WAA_ERROR("Recreating target");
		deleteDependentResources();
		createDependentResources();
		for (auto ps : particle_systems)
			ps->recreateResources(d2hrt);
	}
}


int WaaWindow::onMouseMove(int x, int y)
{
	return 0;
}

int WaaWindow::onMouseDown(MouseButton mb, int x, int y)
{
	for (auto ps : particle_systems)
		ps->createParticles(fire_per_click);
	return 0;
}

int WaaWindow::onMouseUp(MouseButton mb, int x, int y)
{
	return 0;
}




WaaWindow::WaaWindow(int fps) :
	WapWindow(),
	fps(fps),
	frame_duration(1000 / fps),

	d2f(nullptr),
	d2hrt(nullptr),
	back_bitmap(nullptr)
{
}

WaaWindow::~WaaWindow()
{
}