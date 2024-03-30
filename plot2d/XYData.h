#pragma once

#include "XYPoint.h"
#include "XYRect.h"
#include "XYTrack.h"

class XYData
{
public:
    std::vector<XYTrack> tracks;
    XYRect getDataBounds(void);
};