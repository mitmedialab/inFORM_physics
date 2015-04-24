//
//  ColorBand.cpp
//  Relief2
//
//  Created by Daniel Windham on 4/24/15.
//
//

#include "ColorBand.h"

ColorBand::ColorBand(int _hueTarget, int _hueTolerance, int _saturationThreshold) {
    hueTarget = min(_hueTarget, 255);
    hueTolerance = min(_hueTolerance, 128);
    saturationThreshold = min(_saturationThreshold, 255);
};