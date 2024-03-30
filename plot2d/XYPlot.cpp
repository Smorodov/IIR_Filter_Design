#pragma once
#define _USE_MATH_DEFINES 
#include <cmath>
#include "XYPoint.h"
#include "XYRect.h"
#include "XYData.h"
#include "XYTrack.h"
#include "XYData.h"
#include "XYAxis.h"
#include "XYPlot.h"
#include <string>
#include <dash.h>
// these_labels is vector of [label,units,max.min]
void plotGrid(XYDrawingBackend* ctx,
	float w, float h,
	std::string title,
	std::string  xAxisLabel,
	std::string  yAxisLabel,
	std::string textFontName,
	std::string labelFontName,
	std::string headerFontName,
	float textHeight,
	float labelsTextHeight,
	float headerHeight,
	unsigned int xN,
	unsigned int yN,
	int xE,
	int yE,
	unsigned int bgColor,
	unsigned int gridColor,
	unsigned int axisTextColor,
	unsigned int titleTextColor,
	unsigned int axisLabelsTextColor,
	float gridLineWidth,
	float gridZeroLineWidth,
	XYData* data
)
{
	// высота шрифта значений отсчетов на осях
	float headerWidth;
	// минимумы и максимумы всех точек графика
	if (data->tracks.empty())
	{
		return;
	}
	XYRect r = data->getDataBounds();
	float xMin = r.left;
	float xMax = r.right;
	float yMin = r.top;
	float yMax = r.bottom;

	Axis xAxis = Axis();
	// Расчет только размеров строк
	xAxis.computeAxis(ctx,
		xMin,
		xMax,
		xN,
		0.0,
		1.0,
		xE,
		textFontName,
		textHeight
	);

	Axis yAxis = Axis();
	yAxis.computeAxis(ctx,
		yMin,
		yMax,
		yN,
		0.0,
		1.0,
		yE,
		textFontName,
		textHeight
	);

	// ---------------------------
	// 
	// ---------------------------
	float headerMargin = headerHeight * 1.2f;
	// отступ цифр от горизонтальной оси
	float textMarginForX = 0.7f * textHeight;
	// отступ ддя метки веоьткадбглй оси
	float textMarginForY = 1.2f * textHeight;
	// отступы сетки от краев холста
	float leftMargin = yAxis.maxValStrW + 2.0 * textMarginForY;
	float rightMargin = xAxis.maxValStrW;
	float topMargin = headerMargin + headerHeight;
	float legendStep = 1.5f * textHeight;
	// -----
	// compute max legend length
	// -----
	float maxTrW = 0, maxTrH = 0;
	float maxLegendLineLength = 0;
	for (auto track : data->tracks)
	{
		float trW, trH;
		ctx->getTextSize(textFontName, textHeight, track.name, trW, trH);
		if (maxTrW < trW)
		{
			maxTrW = trW;
		}
		if (maxTrH < trH)
		{
			maxTrH = trH;
		}
		float minLegendLineLength = ctx->dashStrokePattern.getTotalLength() * 2;
		if (maxLegendLineLength < minLegendLineLength)
		{
			maxLegendLineLength == minLegendLineLength;
		}
	}

	if (maxLegendLineLength < w / 8.0f)
	{
		maxLegendLineLength = w / 8.0f;
	}
	float totalLegendLength = maxTrW + maxLegendLineLength;
	int nLegendColumns = floor(float(w) / float(totalLegendLength));
	int nLegendRows = ceil(float(data->tracks.size()) / float(nLegendColumns));
	float legendHeight = float(nLegendRows + 1) * legendStep;	
	// ----------------------------------
	float bottomMargin = 1.7f * textMarginForX + 2.0f * textHeight + legendHeight;
	XYRect rect = XYRect(0, 0, 0, 0);
	rect.bottom = h;
	rect.top = h - legendHeight;
	rect.left = 0;
	rect.right = w;

	ctx->setStrokeWidth(1);
	//ctx->setFillColorI(0xEF555555);
	ctx->setStrokeColorI(gridColor);
	//ctx->fillRoundedRect(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, 4);
	ctx->strokeRoundedRect(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, 4);
	ctx->strokeRoundedRect(0, 0, w, h, 4);
	xAxis.computeAxis(
		ctx,
		xMin,
		xMax,
		xN,
		leftMargin,
		w - rightMargin,
		xE,
		textFontName,
		textHeight
	);

	yAxis.computeAxis(
		ctx,
		yMin,
		yMax,
		yN,
		topMargin,
		h - bottomMargin,
		yE,
		textFontName,
		textHeight
	);

	for (int i = 0; i <= xAxis.nSteps; ++i)
	{
		float curX = xAxis.canvasValues[i];
		if (xAxis.values[i] == 0.0f)
		{
			ctx->setStrokeWidth(gridZeroLineWidth);
		}
		else
		{
			ctx->setStrokeWidth(gridLineWidth);
		}

		std::string str = xAxis.strValues[i];
		ctx->setStrokeColorI(axisTextColor);
		ctx->setFillColorI(axisTextColor);
		ctx->setStrokeWidth(1);
		ctx->putText(			
			curX,
			h - bottomMargin + textMarginForX,
			str,
			textFontName,
			textHeight,
			CC
		);
		ctx->setStrokeColorI(gridColor);
		
		ctx->strokeLine(curX, topMargin, curX, h - bottomMargin);
	}

	for (int i = 0; i <= yAxis.nSteps; ++i)
	{
		float curY = topMargin + yAxis.canvasMaxValue - yAxis.canvasValues[i];
		if (yAxis.values[i] == 0.0f)
		{
			ctx->setStrokeWidth(gridZeroLineWidth);
		}
		else {
			ctx->setStrokeWidth(gridLineWidth);
		}
		ctx->setStrokeColorI(axisTextColor);
		ctx->setFillColorI(axisTextColor);
		ctx->setStrokeWidth(1);
		std::string str = yAxis.strValues[i];

		ctx->putText(
			leftMargin - textMarginForY / 2.0f,
			curY,// + float(textHeight) / 3.0f,
			str,
			textFontName,
			textHeight,
			RC
		);

		ctx->setStrokeColorI(gridColor);
		ctx->strokeLine(leftMargin, curY, w - rightMargin, curY);
	}

	float scaleX = (xAxis.canvasMaxValue - xAxis.canvasMinValue) / (xAxis.maxValue - xAxis.minValue);
	float scaleY = (yAxis.canvasMaxValue - yAxis.canvasMinValue) / (yAxis.maxValue - yAxis.minValue);
	float offsetX = xAxis.canvasMinValue - xAxis.minValue * scaleX;
	float offsetY = yAxis.canvasMinValue - yAxis.minValue * scaleY;

	

	int j = 0;
	std::vector<float> xp;
	std::vector<float> yp;
	float rr = 0;
	std::vector<float> pt_nodes_x;
	std::vector<float> pt_nodes_y;

	for (auto track : data->tracks)
	{		
		int i = 0;
		pt_nodes_x.clear();
		pt_nodes_y.clear();
		xp.clear();
		yp.clear();


		for (auto p : track.points)
		{

				float xx = p.x;
				float yy = yAxis.maxValue - p.y + yAxis.minValue;
				float X = xx * scaleX + offsetX;
				float Y = yy * scaleY + offsetY;
				pt_nodes_x.push_back(X);
				pt_nodes_y.push_back(Y);
			++i;
		}

		//std::vector<float> dash = { 20,6,1,6 };
		float dash_offset = 0.0;
		ctx->setStrokePattern(track.pathEffect,dash_offset);
		ctx->setStrokeColorI(track.color);
		ctx->setStrokeWidth(track.width);
		ctx->dashedPolyline(pt_nodes_x, pt_nodes_y);
		// -----------------------------------------
		// legend
		// -----------------------------------------
		float ly = h - legendHeight + legendStep * floor(float(j) / nLegendColumns) + legendStep * 0.5f;
		float t = (float(j) / (nLegendColumns))-floor(float(j) / (nLegendColumns));
		t *= nLegendColumns;
		float lx = t * totalLegendLength + legendStep * 0.5f;
		pt_nodes_x.clear();
		pt_nodes_y.clear();

		pt_nodes_x.push_back(lx);
		pt_nodes_y.push_back(ly + textHeight/1.5);
		pt_nodes_x.push_back(lx+maxLegendLineLength);
		pt_nodes_y.push_back(ly + textHeight/1.5);
		rr = 0;
		ctx->setStrokeColorI(track.color);
		ctx->setStrokeWidth(track.width);
		ctx->dashedPolyline(pt_nodes_x, pt_nodes_y);

		ctx->setStrokeColorI(axisTextColor);
		ctx->setFillColorI(axisTextColor);
		ctx->setStrokeWidth(1);				
		ctx->putText(
			lx + maxLegendLineLength,
			pt_nodes_y[0],
			track.name,
			textFontName,
			textHeight,
			LC
		);
		// -----------------------------------------
		//
		// -----------------------------------------
		++j;
	}
// Метки осей
ctx->setStrokeColorI(axisLabelsTextColor);
ctx->setFillColorI(axisLabelsTextColor);
float y = (yAxis.canvasMaxValue + yAxis.canvasMinValue) * 0.5f;

ctx->setStrokeWidth(1);
ctx->putText(	
	textMarginForY, y,
	yAxisLabel,
	textFontName,
	textHeight,
	CC,
	-90
);

float x = (xAxis.canvasMaxValue + xAxis.canvasMinValue) * 0.5f;
ctx->setStrokeWidth(1);
ctx->putText(	
	x, h - textMarginForX - legendHeight,
	xAxisLabel,
	labelFontName,
	labelsTextHeight,
	CC
);
ctx->setStrokeWidth(1);
ctx->setStrokeColorI(titleTextColor);
ctx->setFillColorI(titleTextColor);
ctx->putText(	
	x, headerMargin,
	title,
	headerFontName,
	headerHeight,
	CC
);
// ---------------------------------------------
rect.bottom = h - bottomMargin;
rect.top = topMargin;
rect.left = leftMargin;
rect.right = w - rightMargin;
//paint.color = gridColor!!
ctx->setStrokeColorI(gridColor);
ctx->setStrokeWidth(gridZeroLineWidth * 2.0f);
ctx->strokeRect(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top);
}

// ----------------------------------
// вход функции рисования графмков
// ----------------------------------
void plotMain(
	XYDrawingBackend* ctx,
	int bitmapWidth,
	int bitmapHeight,
	std::string title,
	std::string  xAxisLabel,
	std::string  yAxisLabel,
	std::string textFontName,
	std::string labelFontName,
	std::string headerFontName,
	float textHeight,
	float labelsTextHeight,
	float headerHeight,
	unsigned int xN,
	unsigned int yN,
	int xE,
	int yE,
	unsigned int bgColor,
	unsigned int gridColor,
	unsigned int axisTextColor,
	unsigned int titleTextColor,
	unsigned int axisLabelsTextColor,
	// высота шрифта значений отсчетов на осях
	float gridLineWidth,
	float gridZeroLineWidth,
	XYData* data
)
{
	float offsetX = 0;
	float offsetY = 0;
	float scaleX = 1;
	float scaleY = 1;

	XYRect rect(0, 0, bitmapWidth, bitmapHeight);
	XYRect rectF(0, 0, bitmapWidth, bitmapHeight);
	float roundPx = 12;
	
	ctx->setFillColorI(bgColor);
	ctx->fillRect(0, 0, bitmapWidth, bitmapHeight);
	plotGrid(ctx,		
		bitmapWidth,
		bitmapHeight,
		title,
		xAxisLabel,
		yAxisLabel,
		textFontName,
		labelFontName,
		headerFontName,
		textHeight,
		labelsTextHeight,
		headerHeight,
		xN,
		yN,
		xE,
		yE,
		bgColor,
		gridColor,
		axisTextColor,
		titleTextColor,
		axisLabelsTextColor,
		// высота шрифта значений отсчетов на осях
		gridLineWidth,
		gridZeroLineWidth,
		data
	);
}

