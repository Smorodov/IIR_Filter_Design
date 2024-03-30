#include "XYblend2dBackend.h"
#include <sstream>
#include <iostream>
#include "BLAlignedText.h"
// --------------------------
// 
// --------------------------
XYBlend2dBackend::XYBlend2dBackend(int w,int h)
{	
	img=new BLImage(w, h, BL_FORMAT_PRGB32);
	ctx = new BLContext(*img);
	static_cast<BLContext*>(ctx)->setCompOp(BL_COMP_OP_SRC_COPY);
	static_cast<BLContext*>(ctx)->setFillStyle(BLRgba32(0xFF555555));
	static_cast<BLContext*>(ctx)->fillAll();
}

void XYBlend2dBackend::scale(float sx, float sy)
{
	static_cast<BLContext*>(ctx)->scale(sx, sy);
}
void XYBlend2dBackend::translate(float dx, float dy)
{
	static_cast<BLContext*>(ctx)->translate(dx, dy);
}
void XYBlend2dBackend::rotate(float a, float cx, float cy)
{
	static_cast<BLContext*>(ctx)->rotate(a*M_PI/180.0f, cx, cy);
}
// --------------------------
// 
// --------------------------
XYBlend2dBackend::~XYBlend2dBackend()
{
	std::cout << "cleaning.." << std::endl;
	static_cast<BLContext*>(ctx)->end();
	std::cout << "cleaning fonts cahe.." << std::endl;
	for (auto f : fontsCache)
	{
		std::cout << "removing " << f.first << std::endl;
		delete f.second;
	}
	fontsCache.clear();
	std::cout << "cleaning fonts.." << std::endl;
	for (auto f : fonts)
	{
		std::cout << "removing " << f.first << std::endl;
		delete f.second;
	}
	fonts.clear();

	img->writeToFile("be.png");	
	std::cout << "deleting context.." << std::endl;
	delete ctx;
	std::cout << "deleting image.." << std::endl;
	delete img;
	std::cout << "fonts size=" << fonts.size() << std::endl;
	std::cout << "fonts cache size=" << fontsCache.size() << std::endl;
}
// --------------------------
// 
// --------------------------
void  XYBlend2dBackend::appendFont(std::string fileName)
{
	if (fonts.find(fileName) != fonts.end()) 
	{
		// if exists - ignore
		return;
	}
	// else - append to font faces map
	fonts[fileName] = new BLFontFace();	
	BLResult err = static_cast<BLFontFace*>(fonts[fileName])->createFromFile(fileName.c_str());
	if (err)
	{
		printf("Failed to load a font-face (err=%u)\n", err);
		return;
	}
}
// --------------------------
// 
// --------------------------
void* XYBlend2dBackend::getFont(std::string fileName, float size)
{
	std::ostringstream ss;
	ss << fileName << "@" << size;
	std::string name = (ss.str());
	if (fontsCache.find(name) == fontsCache.end())
	{
		if (fonts.find(fileName) != fonts.end())
		{
			BLFont* font = new BLFont();
			font->createFromFace(*static_cast<BLFontFace*>(fonts[fileName]), size);
			fontsCache[name] = font;
			return font;
		}
		else
		{
			appendFont(fileName);
			BLFont* font = new BLFont();
			font->createFromFace(*static_cast<BLFontFace*>(fonts[fileName]), size);
			fontsCache[name] = font;
			return font;
		}
	}
	else
	{
		return fontsCache[name];
	}
}
// --------------------------
// 
// --------------------------
void  XYBlend2dBackend::removeFont(std::string fileName)
{
	delete fonts[fileName];
	fonts.erase (fileName);
}
// --------------------------
// 
// --------------------------
void  XYBlend2dBackend::clearFonts()
{
	for (auto f : fonts)
	{
		delete f.second;
	}
	fonts.clear();
}

// --------------------------
// Stroke width
// --------------------------
void XYBlend2dBackend::setStrokeWidth(float width)
{
	static_cast<BLContext*>(ctx)->setStrokeWidth(width);
}
// --------------------------
// Stroke color
// --------------------------
void XYBlend2dBackend::setStrokeColorI(uint32_t color)
{
	static_cast<BLContext*>(ctx)->setStrokeStyle(BLRgba32(color));
}
// --------------------------
// 
// --------------------------
void XYBlend2dBackend::setStrokeColor4I(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	static_cast<BLContext*>(ctx)->setStrokeStyle(BLRgba(float(r) / 255.0f, float(g) / 255.0f, float(b) / 255.0f, float(a) / 255.0f));
}
// --------------------------
// 
// --------------------------
void XYBlend2dBackend::setStrokeColor4F(float r, float g, float b, float a)
{
	static_cast<BLContext*>(ctx)->setStrokeStyle(BLRgba(r,g,b,a));
}

// --------------------------
// Fill color
// --------------------------
void XYBlend2dBackend::setFillColorI(uint32_t color)
{
	static_cast<BLContext*>(ctx)->setFillStyle(BLRgba32(color));
}
// --------------------------
// 
// --------------------------
void XYBlend2dBackend::setFillColor4I(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	static_cast<BLContext*>(ctx)->setFillStyle(BLRgba(float(r) / 255.0f, float(g) / 255.0f, float(b) / 255.0f, float(a) / 255.0f));
}
// --------------------------
// 
// --------------------------
void XYBlend2dBackend::setFillColor4F(float r, float g, float b, float a)
{
	static_cast<BLContext*>(ctx)->setFillStyle(BLRgba(r, g, b, a));
}
// --------------------------
// Line
// --------------------------
void XYBlend2dBackend::strokeLine( XYPoint p1, XYPoint p2)
{
		static_cast<BLContext*>(ctx)->strokeLine(p1.x, p1.y, p2.x, p2.y);
}
// --------------------------
// 
// --------------------------
void XYBlend2dBackend::strokeLine( float p1_x, float p1_y, float p2_x, float p2_y)
{
	static_cast<BLContext*>(ctx)->strokeLine(p1_x, p1_y, p2_x, p2_y);
}
// --------------------------------------------
// Dashed Line and polyline
// --------------------------------------------	
void XYBlend2dBackend::dashedLine(XYPoint p1, XYPoint p2)
{
	if (!pattern.empty())
	{
		std::vector<float> xp;//(выход) вектор x-координат для отрисовки полилинии
		std::vector<float> yp;//(выход) вектор y-координат для отрисовки полилинии
		dash_phase = makeDashedLineSegment(
			// вход
			p1.x,// координата x начальной точки
			p1.y,// координата y начальной точки
			p2.x,// координата x конечной точки
			p2.y,// координата y конечной точки
			pattern, // паттерн 
			dash_phase, // смещение фазы [-1:1]
			// результат
			xp,// x координаты точек для отрисовки отрезков.
			yp // y координаты точек для отрисовки отрезков.
		);
		for (int i = 0; i < xp.size(); i += 2)
		{
			strokeLine(xp[i], yp[i], xp[i + 1], yp[i + 1]);
		}
		xp.clear();
		yp.clear();
	}
	else
	{
		strokeLine(p1,p2);
	}
}

void XYBlend2dBackend::dashedLine(float p1_x, float p1_y, float p2_x, float p2_y)
{
	if (!pattern.empty())
	{
		dashedLine(XYPoint(p1_x, p1_y), XYPoint(p2_x, p2_y));
	}
	else
	{
		strokeLine(XYPoint(p1_x, p1_y), XYPoint(p2_x, p2_y));
	}
}
void XYBlend2dBackend::dashedPolyline(std::vector<XYPoint> pts)
{
	if (!pattern.empty())
	{
		std::vector<float> x;//(выход) вектор x-координат для отрисовки полилинии
		std::vector<float> y;//(выход) вектор y-координат для отрисовки полилинии
		for (int i = 0; i < pts.size(); ++i)
		{
			x.push_back(pts[i].x);
			x.push_back(pts[i].y);
		}
		dashedPolyline(x, y);
	}
	else
	{
		for (int i = 0; i < pts.size()-1; ++i)
		{
			strokeLine(pts[i], pts[i + 1]);
		}
	}
}
void XYBlend2dBackend::dashedPolyline(std::vector<float> px, std::vector<float> py)
{
	if (!pattern.empty())
	{
		std::vector<float> xp;//(выход) вектор x-координат для отрисовки полилинии
		std::vector<float> yp;//(выход) вектор y-координат для отрисовки полилинии
		makeDashedPolyline(
			px,	// (вход) вектор x-координат узлоых точек отрезков полилинии
			py,	// (вход) вектор y-координат узлоых точек отрезков полилинии
			pattern, //(вход) паттерн
			dash_phase, //(вход) смещение фазы паттерна [-1:+1]
			xp,//(выход) вектор x-координат для отрисовки полилинии
			yp,//(выход) вектор y-координат для отрисовки полилинии
			dash_phase //(вход/выход) остаток, нужен для отписовки соединенных последовательно линий.
		);
		for (int i = 0; i < xp.size(); i += 2)
		{
			strokeLine(xp[i], yp[i], xp[i + 1], yp[i + 1]);
		}
		xp.clear();
		yp.clear();
	}
	else
	{
		for (int i = 0; i < px.size() - 1; ++i)
		{
			strokeLine(px[i], py[i],px[i + 1], py[i + 1]);
		}
	}
}
// --------------------------
// Rectangle
// --------------------------
void XYBlend2dBackend::strokeRect(XYRect rect)
{
	static_cast<BLContext*>(ctx)->strokeRect(rect.left, rect.top, rect.right-rect.left, rect.bottom-rect.top);
}
void XYBlend2dBackend::strokeRect(XYPoint p1, XYPoint p2)
{
	static_cast<BLContext*>(ctx)->strokeRect(p1.x, p1.y, p2.x-p1.x, p2.y- p1.y);
}
void XYBlend2dBackend::strokeRect(float x, float y, float w, float h)
{
	static_cast<BLContext*>(ctx)->strokeRect(x, y, w, h);
}
void XYBlend2dBackend::fillRect(XYRect rect)
{	
	static_cast<BLContext*>(ctx)->fillRect(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top);
}
void XYBlend2dBackend::fillRect(XYPoint p1, XYPoint p2)
{
	static_cast<BLContext*>(ctx)->fillRect(p1.x, p1.y, p1.x - p2.x, p2.y - p1.y);
}
void XYBlend2dBackend::fillRect(float x, float y, float w, float h)
{
	static_cast<BLContext*>(ctx)->fillRect(x, y, w, h);
}
// --------------------------
// Rounded rectangle
// --------------------------
void XYBlend2dBackend::strokeRoundedRect(XYRect rect,float r)
{
	static_cast<BLContext*>(ctx)->strokeRoundRect(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top,r,r);
}
void XYBlend2dBackend::strokeRoundedRect(XYPoint p1, XYPoint p2, float r)
{
	static_cast<BLContext*>(ctx)->strokeRoundRect(p1.x, p1.y, p2.x - p1.x, p2.y - p1.y,r,r);
}
void XYBlend2dBackend::strokeRoundedRect(float x, float y, float w, float h, float r)
{
	static_cast<BLContext*>(ctx)->strokeRoundRect(x, y, w, h,r,r);
}
void XYBlend2dBackend::fillRoundedRect(XYRect rect, float r)
{
	static_cast<BLContext*>(ctx)->fillRoundRect(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top,r,r);
}
void XYBlend2dBackend::fillRoundedRect(XYPoint p1, XYPoint p2,float r)
{
	static_cast<BLContext*>(ctx)->fillRoundRect(p1.x, p1.y, p1.x - p2.x, p2.y - p1.y, r,r);
}
void XYBlend2dBackend::fillRoundedRect(float x, float y, float w, float h, float r)
{
	static_cast<BLContext*>(ctx)->fillRoundRect(x, y, w, h,r,r);
}
// --------------------------------------------
// Circle
// --------------------------------------------		
void XYBlend2dBackend::strokeCircle(XYPoint p, float r)
{
	static_cast<BLContext*>(ctx)->strokeCircle(p.x, p.y, r);
}
void XYBlend2dBackend::strokeCircle(float x, float y, float r)
{
	static_cast<BLContext*>(ctx)->strokeCircle(x, y,r);
}
void XYBlend2dBackend::fillCircle(XYPoint p, float r)
{
	static_cast<BLContext*>(ctx)->fillCircle(p.x, p.y,r);
}
void XYBlend2dBackend::fillCircle(float x, float y, float r)
{
	static_cast<BLContext*>(ctx)->fillCircle(x, y,r);
}
// --------------------------
// 
// --------------------------
void XYBlend2dBackend::putText(
	float x, float y,
	std::string str,
	std::string fontFileName,
	float fontSize,
	TextAlign align,
	float angle
)
{
	static_cast<BLContext*>(ctx)->rotate(angle*M_PI/180.0f, x, y);
	::putText(
		*static_cast<BLContext*>(ctx),
		x, y,
		str,
		static_cast<BLFontFace*>(fonts[fontFileName]),
		static_cast<BLFont*>(getFont(fontFileName, fontSize)),
		align
	);
	static_cast<BLContext*>(ctx)->rotate(-angle * M_PI / 180.0f, x, y);
}
// --------------------------
// 
// --------------------------
void XYBlend2dBackend::getTextSize(
	std::string fontFileName,
	float fontSize,
	std::string str,
	float& w,
	float& h)
{
	::getTextSize(
		static_cast<BLFontFace*>(fonts[fontFileName]),
		static_cast<BLFont*>(getFont(fontFileName, fontSize)),
		str,
		w,
		h);
}
