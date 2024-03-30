#pragma once
#include "BLAlignedText.h"
#include "XYAxis.h"
#include "XYBlend2dBackend.h"
#include "XYPlot.h"
#include "XYPoint.h"
#include <blend2d.h>
#include <dash.h>
#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

std::vector<unsigned int> colors = { 0xffff0000,
        0xffffff00,
        0xffffffff,
        0xffff00ff,
        0xffff44ff,
        0xff00ff00,
        0xff00ffff,
        0xff0000ff,
        0xff55ff55,
        0xff0055ff,
        0xffff5555 };

std::vector<std::vector<float>> patttrns = {
        { 20, 0 },
        { 4, 2 },
        { 6, 2, 6 },
        { 10, 2, 2, 2 },
        { 30, 5, 5, 5 },
        { 10, 20, 30 },
        { 20, 20 },
        { 40, 10, 4, 10, 4, 2 },
        { 40, 20, 40 },
        { 50 }
    };


inline void draw_plots(
    std::vector<double>& data1_x,
    std::vector<double>& data1_y,
    std::vector<double>& data2_x,
    std::vector<double>& data2_y
)
{
    XYBlend2dBackend be(1200, 1200);
    be.setFillColor4I(255, 20, 255, 255);

    XYData data;
    {
        XYTrack track;
        track.color = colors[2 % colors.size()];
        track.name = "data 1";
        track.pathEffect = patttrns[0 % patttrns.size()];        
        track.width = 3;
        for (int i=0;i<data1_x.size();++i ) {
            track.points.push_back(XYPoint(data1_x[i], data1_y[i]));
        }
        data.tracks.push_back(track);
    }

    be.translate(0, 0);
    be.scale(1.0, 1.0);
    plotMain(
        &be,
        1200,
        600,
        "Title",
        "X",
        "Y",
        "fonts/OpenSans-Light.ttf",
        "fonts/OpenSans-Light.ttf",
        "fonts/OpenSans-Light.ttf",
        20,
        20,
        40,
        5,
        5,
        -1,
        -3,
        0xFF666666,
        0xffffffff,
        0xffffffff,
        0xffffffff,
        0xffffffff,
        // высота шрифта значений отсчетов на осях
        1,
        2,
        &data);
    be.scale(1, 1);
    be.translate(0, 0);

    data.tracks.clear();
    {
       XYTrack track;
        track.color = colors[2 % colors.size()];
        track.name = "data 1";
        track.pathEffect = patttrns[0 % patttrns.size()];        
        track.width = 3;
        for (int i=0;i<data2_x.size();++i ) {
            track.points.push_back(XYPoint(data2_x[i], data2_y[i]));
        }
        data.tracks.push_back(track);
    }

    be.translate(0, 600);
    be.scale(1.0, 1.0);
    plotMain(
        &be,
        1200,
        600,
        "Title",
        "X",
        "Y",
        "fonts/OpenSans-Light.ttf",
        "fonts/OpenSans-Light.ttf",
        "fonts/OpenSans-Light.ttf",
        20,
        20,
        40,
        5,
        5,
        -1,
        -3,
        0xFF666666,
        0xffffffff,
        0xffffffff,
        0xffffffff,
        0xffffffff,
        // высота шрифта значений отсчетов на осях
        1,
        2,
        &data);
    be.scale(1, 1);
    be.translate(0, -600);
    
}
