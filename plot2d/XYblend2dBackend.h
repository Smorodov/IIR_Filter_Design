#pragma once
#include "blend2d.h"
#include "XYPoint.h"
#include "XYDrawingBackend.h"
class XYBlend2dBackend : public XYDrawingBackend
{
public:
	void scale(float sx, float sy);
	void translate(float dx, float dy);
	void rotate(float a, float cx, float cy);

	XYBlend2dBackend(int w, int h);
	~XYBlend2dBackend();
	void setStrokeColorI(uint32_t color);
	void setStrokeColor4I(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
	void setStrokeColor4F(float r, float g, float b, float a);
	void setStrokeWidth(float w);
	// --------------------------------------------
	// Dashed Line and polyline
	// --------------------------------------------	
	void dashedLine(XYPoint p1, XYPoint p2);
	void dashedLine(float p1_x, float p1_y, float p2_x, float p2_y);
	void dashedPolyline(std::vector<XYPoint> pts);
	void dashedPolyline(std::vector<float> px, std::vector<float> py);


	void setFillColorI(uint32_t color);
	void setFillColor4I(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
	void setFillColor4F(float r, float g, float b, float a);

	void strokeLine(XYPoint p1, XYPoint p2);
	void strokeLine(float p1_x, float p1_y, float p2_x, float p2_y);

	void strokeRect(XYRect rect);
	void strokeRect(XYPoint p1, XYPoint p2);
	void strokeRect(float x, float y, float w, float h);

	void fillRect(XYRect rect);
	void fillRect(XYPoint p1, XYPoint p2);
	void fillRect(float x, float y, float w, float h);

	// --------------------------------------------
	// Rounded rectangle
	// --------------------------------------------	
	void strokeRoundedRect(XYRect rect, float r);
	void strokeRoundedRect(XYPoint p1, XYPoint p2, float r);
	void strokeRoundedRect(float x, float y, float w, float h, float r);
	void fillRoundedRect(XYRect rect, float r);
	void fillRoundedRect(XYPoint p1, XYPoint p2, float r);
	void fillRoundedRect(float x, float y, float w, float h, float r);
	// --------------------------------------------
	// Circle
	// --------------------------------------------		
	void strokeCircle(XYPoint p, float r);
	void strokeCircle(float x, float y, float r);
	void fillCircle(XYPoint p, float r);
	void fillCircle(float x, float y, float r);

	void appendFont(std::string fiileName);
	void* getFont(std::string fiileName, float size);
	void removeFont(std::string filename);
	void clearFonts();
	void putText(
		float x, float y,
		std::string str,
		std::string fontFileName,
		float fontSize,
		TextAlign align,
		float angle
	);

	void getTextSize(
		std::string fontFileName,
		float fontSize,
		std::string str,
		float& w,
		float& h);

protected:
	std::map<std::string, BLFont*> fontsCache;
	BLImage* img;
};