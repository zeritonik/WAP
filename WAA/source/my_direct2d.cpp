#include "my_direct2d.h"


void myLoadBitmap(IWICImagingFactory* wf, ID2D1HwndRenderTarget* d2hrt, const wchar_t* filename, ID2D1Bitmap** bitmap)
{
	IWICBitmapDecoder* decoder;
	CHECK_HR(
		wf->CreateDecoderFromFilename(
			filename,
			NULL,
			GENERIC_READ,
			WICDecodeMetadataCacheOnDemand,
			&decoder
		),
		"Error createDecoderFromFilename"
	);

	IWICBitmapFrameDecode* frame;
	CHECK_HR(
		decoder->GetFrame(0, &frame),
		"Error getFrame"
	);

	IWICFormatConverter* converter;
	CHECK_HR(
		wf->CreateFormatConverter(&converter),
		"Error createFormatConverter"
	);
	CHECK_HR(
		converter->Initialize(
			frame,
			GUID_WICPixelFormat32bppPBGRA,
			WICBitmapDitherTypeNone,
			nullptr,
			0.0,
			WICBitmapPaletteTypeMedianCut
		),
		"Error initialize converter"
	);

	CHECK_HR(
		d2hrt->CreateBitmapFromWicBitmap(
			converter,
			bitmap
		),
		"Error createBitmapFromWicBitmap"
	);

	frame->Release();
	converter->Release();
	decoder->Release();
}

void myLoadResourceBitmap(IWICImagingFactory* wf, ID2D1HwndRenderTarget* d2hrt, int resource_id, ID2D1Bitmap** bitmap)
{
	HRESULT hr;

	HRSRC bitmap_resource = FindResource(NULL, MAKEINTRESOURCE(resource_id), RT_PNG);
	CHECK_HR(bitmap_resource != NULL ? S_OK : E_FAIL, "Error findResource");

	DWORD bitmap_size = SizeofResource(NULL, bitmap_resource);
	CHECK_HR(bool(bitmap_size) ? S_OK : E_FAIL, "Error sizeofResource");

	HGLOBAL bitmap_data = LoadResource(NULL, bitmap_resource);
	CHECK_HR(bool(bitmap_data) ? S_OK : E_FAIL, "Error loadResource");
	
	void* bitmap_pointer;
	bitmap_pointer = LockResource(bitmap_data);
	CHECK_HR(bool(bitmap_pointer) ? S_OK : E_FAIL, "Error lockResource");

	IWICStream* stream;
	hr = wf->CreateStream(&stream); CHECK_HR(hr, "Error createStream");
	hr = stream->InitializeFromMemory(
		(BYTE*)bitmap_pointer,
		bitmap_size
	); CHECK_HR(hr, "Error initalizeFromMemory stream");

	IWICBitmapDecoder* decoder;
	hr = wf->CreateDecoderFromStream(
		stream,
		NULL,
		WICDecodeMetadataCacheOnDemand,
		&decoder
	); CHECK_HR(hr, "Error createDecoderFromStream");

	IWICBitmapFrameDecode* frame;
	hr = decoder->GetFrame(0, &frame); CHECK_HR(hr, "Error getFrame");

	IWICFormatConverter* converter;
	hr = wf->CreateFormatConverter(&converter); CHECK_HR(hr, "Error createFormatConverter");
	hr = converter->Initialize(
		frame,
		GUID_WICPixelFormat32bppPBGRA,
		WICBitmapDitherTypeNone,
		NULL,
		0.0,
		WICBitmapPaletteTypeMedianCut
	);  CHECK_HR(hr, "Error initialize converter");

	hr = d2hrt->CreateBitmapFromWicBitmap(
		converter,
		bitmap
	);  CHECK_HR(hr, "Error createBitmapFromWicBitmap");

	FreeResource(&bitmap_data);
	converter->Release();
	frame->Release();
	decoder->Release();
	stream->Release();
}