#pragma once
#include "XYPoint.h"
#include "XYRect.h"
#include "XYData.h"
#include "XYTrack.h"
#include "XYAxis.h"
#include "XYDrawingBackend.h"
#include <string>
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
);


