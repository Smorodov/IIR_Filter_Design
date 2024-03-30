#pragma once
#include "XYDrawingBackend.h"
#include <blend2d.h>
#include <string>

void getTextSize(
	BLFontFace* face,
	BLFont* font,	
	std::string str,
	float& w,
	float& h);

void putText(
	BLContext&ctx,
	float x, float y,
	std::string str,
	BLFontFace* face,
	BLFont* font,
	TextAlign align = CC
	);
