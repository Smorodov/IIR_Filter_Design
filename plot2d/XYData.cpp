#include "XYData.h"
    
XYRect XYData::getDataBounds()
    {
        // xmin ymin xmax ymax
        // left top right bottom
        XYRect r(FLT_MAX, FLT_MAX, -FLT_MAX, -FLT_MAX);
        for (XYTrack track : tracks)
        {
            for (XYPoint p : track.points)
            {
                if (r.left > p.x)
                {
                    r.left = p.x;
                }
                if (r.right < p.x)
                {
                    r.right = p.x;
                }
                if (r.top > p.y)
                {
                    r.top = p.y;
                }
                if (r.bottom < p.y)
                {
                    r.bottom = p.y;
                }
            }
        }
        return r;
    }
