
#include "BLAlignedText.h"

void getTextSize(
	BLFontFace* face,
	BLFont* font,	
	std::string str,
	float& w,
	float& h)
{
	BLFontMetrics fm;
	BLTextMetrics tm;
	BLGlyphBuffer gb;
	fm = font->metrics();
	gb.setUtf8Text(str.c_str(), str.length());
	font->shape(gb);
	font->getTextMetrics(gb, tm);
	w = tm.boundingBox.x1 - tm.boundingBox.x0;
	h = fm.ascent + fm.descent + fm.lineGap;
}

void textAlignCC(
	BLFontFace* face,
	BLFont* font,	
	std::string str,
	float& dx, float& dy)
{
	BLFontMetrics fm;
	BLTextMetrics tm;
	BLGlyphBuffer gb;
	fm = font->metrics();
	gb.setUtf8Text(str.c_str(), str.length());
	font->shape(gb);
	font->getTextMetrics(gb, tm);
	float textWidth = tm.boundingBox.x1 - tm.boundingBox.x0;	
	dx = -textWidth * 0.5;
	dy = (fm.ascent - fm.descent) * 0.5;
}

void textAlignLC(
	BLFontFace* face,
	BLFont* font,
	std::string str,
	float& dx, float& dy)
{
	BLFontMetrics fm;
	fm = font->metrics();
	dx = 0;
	dy = (fm.ascent - fm.descent) * 0.5;
}

void textAlignRC(
	BLFontFace* face,
	BLFont* font,
	std::string str,
	float& dx, float& dy)
{
	BLFontMetrics fm;
	BLTextMetrics tm;
	BLGlyphBuffer gb;
	fm = font->metrics();
	gb.setUtf8Text(str.c_str(), str.length());
	font->shape(gb);
	font->getTextMetrics(gb, tm);
	float textWidth = tm.boundingBox.x1 - tm.boundingBox.x0;
	dx = -textWidth;
	dy = (fm.ascent - fm.descent) * 0.5;
}

void textAlignCB(
	BLFontFace* face,
	BLFont* font,
	std::string str,
	float& dx, float& dy)
{
	BLFontMetrics fm;
	BLTextMetrics tm;
	BLGlyphBuffer gb;
	fm = font->metrics();
	gb.setUtf8Text(str.c_str(), str.length());
	font->shape(gb);
	font->getTextMetrics(gb, tm);
	float textWidth = tm.boundingBox.x1 - tm.boundingBox.x0;	
	dx = -textWidth * 0.5;
	dy = 0;
}

void textAlignLB(
	BLFontFace* face,
	BLFont* font,
	std::string str,
	float& dx, float& dy)
{
	dx = 0;
	dy = 0;
}

void textAlignRB(
	BLFontFace* face,
	BLFont* font,
	std::string str,
	float& dx, float& dy)
{
	BLFontMetrics fm;
	BLTextMetrics tm;
	BLGlyphBuffer gb;
	fm = font->metrics();
	gb.setUtf8Text(str.c_str(), str.length());
	font->shape(gb);
	font->getTextMetrics(gb, tm);
	float textWidth = tm.boundingBox.x1 - tm.boundingBox.x0;
	dx = -textWidth;
	dy = 0;
}

void textAlignCT(
	BLFontFace* face,
	BLFont* font,
	std::string str,
	float& dx, float& dy)
{
	BLFontMetrics fm;
	BLTextMetrics tm;
	BLGlyphBuffer gb;
	fm = font->metrics();
	gb.setUtf8Text(str.c_str(), str.length());
	font->shape(gb);
	font->getTextMetrics(gb, tm);
	float textWidth = tm.boundingBox.x1 - tm.boundingBox.x0;	
	dx = -textWidth * 0.5;
	dy = (fm.ascent - fm.descent);
}

void textAlignLT(
	BLFontFace* face,
	BLFont* font,
	std::string str,
	float& dx, float& dy)
{
	BLFontMetrics fm;
	fm = font->metrics();
	dx = 0;
	dy = (fm.ascent - fm.descent);
}

void textAlignRT(
	BLFontFace* face,
	BLFont* font,
	std::string str,
	float& dx, float& dy)
{
	BLFontMetrics fm;
	BLTextMetrics tm;
	BLGlyphBuffer gb;
	fm = font->metrics();
	gb.setUtf8Text(str.c_str(), str.length());
	font->shape(gb);
	font->getTextMetrics(gb, tm);
	float textWidth = tm.boundingBox.x1 - tm.boundingBox.x0;
	dx = -textWidth;
	dy = (fm.ascent - fm.descent);
}

void putText(
	BLContext&ctx,
	float x, float y,
	std::string str,
	BLFontFace* face,
	BLFont* font,
	TextAlign align
	)
{
	float dx = 0;
	float dy = 0;
	BLGlyphBuffer gb;
	gb.setUtf8Text(str.c_str(), str.length());
	font->shape(gb);
	BLPoint p(x, y);
	switch (align)
	{
	case CC:
		textAlignCC(face,font,str,dx,dy);
		break;
	case LC:
		textAlignLC(face, font, str, dx, dy);
		break;
	case RC:
		textAlignRC(face, font, str, dx, dy);
		break;

	case CB:
		textAlignCB(face, font, str, dx, dy);
		break;
	case LB:
		textAlignLB(face, font, str, dx, dy);
		break;
	case RB:
		textAlignRB(face, font, str, dx, dy);
		break;

	case CT:
		textAlignCT(face, font, str, dx, dy);
		break;
	case LT:
		textAlignLT(face, font, str, dx, dy);
		break;
	case RT:
		textAlignRT(face, font, str, dx, dy);
		break;
	}
	p.x += dx;
	p.y += dy;
	ctx.strokeGlyphRun(p, *font, gb.glyphRun());
	ctx.fillGlyphRun(p, *font, gb.glyphRun());
}
