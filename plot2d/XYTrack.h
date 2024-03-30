#pragma once

#include <string>
#include <vector>
#include <blend2d.h>
#include "XYPoint.h"

class XYTrack
{
public:
    std::string name = "";
    std::vector<XYPoint> points;
    uint32_t color;
    float width;
    std::vector<float>  pathEffect;
};