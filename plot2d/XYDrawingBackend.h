#pragma once
#include <cstdint>
#include <map>
#include <string>
#include "XYPoint.h"
#include "XYRect.h"
#include "dash.h"

#ifndef TextAlign
enum TextAlign { CC, LC, RC, CT, LT, RT, CB, LB, RB };
#endif
class XYDrawingBackend
{
public:
	virtual void scale(float sx,float sy) = 0;
	virtual void translate(float dx, float dy)=0;
	virtual void rotate(float a, float cx, float cy) = 0;
	// --------------------------------------------
	// Color and width
	// --------------------------------------------
	virtual void setStrokeColorI(uint32_t color)=0;
	virtual void setStrokeColor4I(uint8_t r, uint8_t g, uint8_t b, uint8_t a)=0;
	virtual void setStrokeColor4F(float r, float g, float b, float a) = 0;
	virtual void setStrokeWidth(float w) = 0;
	virtual void setFillColorI(uint32_t color) = 0;
	virtual void setFillColor4I(uint8_t r, uint8_t g, uint8_t b, uint8_t a) = 0;
	virtual void setFillColor4F(float r, float g, float b, float a) = 0;
	// --------------------------------------------
	// Line
	// --------------------------------------------	
	virtual void strokeLine(XYPoint p1, XYPoint p2) = 0;
	virtual void strokeLine(float p1_x, float p1_y, float p2_x, float p2_y) = 0;
	// --------------------------------------------
	// Dashed Line and polyline
	// --------------------------------------------	
	inline void setStrokePattern(std::vector<float> pattern, float phase)
	{
		this->pattern = pattern;
		dashStrokePattern.setPattern(pattern);
		dash_phase = phase;		
	}
	virtual void dashedLine(XYPoint p1, XYPoint p2) = 0;
	virtual void dashedLine(float p1_x, float p1_y, float p2_x, float p2_y) = 0;
	virtual void dashedPolyline(std::vector<XYPoint> pts) = 0;
	virtual void dashedPolyline(std::vector<float> px, std::vector<float> py) = 0;
	// --------------------------------------------
	// Rectangle
	// --------------------------------------------	
	virtual void strokeRect(XYRect rect) = 0;
	virtual void strokeRect(XYPoint p1, XYPoint p2) = 0;
	virtual void strokeRect(float x, float y, float w, float h) = 0;

	virtual void fillRect(XYRect rect) = 0;
	virtual void fillRect(XYPoint p1, XYPoint p2) = 0;
	virtual void fillRect(float x, float y, float w, float h) = 0;
	// --------------------------------------------
	// Rounded rectangle
	// --------------------------------------------	
	virtual void strokeRoundedRect(XYRect rect,float r) = 0;
	virtual void strokeRoundedRect(XYPoint p1, XYPoint p2, float r) = 0;
	virtual void strokeRoundedRect(float x, float y, float w, float h, float r) = 0;

	virtual void fillRoundedRect(XYRect rect, float r) = 0;
	virtual void fillRoundedRect(XYPoint p1, XYPoint p2, float r) = 0;
	virtual void fillRoundedRect(float x, float y, float w, float h, float r) = 0;
	
	// --------------------------------------------
	// Circle
	// --------------------------------------------		
	virtual void strokeCircle(XYPoint p, float r) = 0;
	virtual void strokeCircle(float x, float y, float r)=0;
	virtual void fillCircle(XYPoint p, float r) = 0;
	virtual void fillCircle(float x, float y, float r)=0;
	// --------------------------------------------
	// Font and text
	// --------------------------------------------	
	virtual void appendFont(std::string fiileName)=0;
	virtual void* getFont(std::string fiileName, float size) = 0;
	virtual void removeFont(std::string filename)=0;
	virtual void clearFonts()=0;
	virtual void putText(		
		float x, float y,
		std::string str,
		std::string fontFileName,
		float fontSize,
		TextAlign align = TextAlign::CC,
		float angle = 0
	)=0;

	virtual void getTextSize(
		std::string fontFileName,
		float fontSize,
		std::string str,
		float& w,
		float& h) = 0;

	Dash dashStrokePattern;
	// --------------------------------------------
	// 
	// --------------------------------------------
protected:
	void* ctx;	
	std::map<std::string ,void*> fonts;
	float dash_phase = 0;
	
	std::vector<float> pattern;
};