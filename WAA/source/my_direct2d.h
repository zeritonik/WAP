#pragma once

#include <Windows.h>
#include <d2d1.h>
#include <wincodec.h>

#include "../resource.h"

#include "wap.h"


#if defined(_DEBUG)
#define CHECK_HR(hr, text) if (hr != S_OK) WAA_ERROR(text)
#else
#define CHECK_HR(hr, text) hr
#endif



void myLoadBitmap(IWICImagingFactory * wf, ID2D1HwndRenderTarget * d2hrt, const wchar_t* filename, ID2D1Bitmap **bitmap);
void myLoadResourceBitmap(IWICImagingFactory * wf, ID2D1HwndRenderTarget * d2hrt, int resource_id, ID2D1Bitmap **bitmap);