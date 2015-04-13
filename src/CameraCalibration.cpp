//
//  CameraCalibration.cpp
//  Relief2
//
//  Created by Daniel Windham on 4/13/15.
//
//

#include "CameraCalibration.h"


// calibration calculated on inFORM 1. may need recalibration. offsets assume normalized coordinates.
// note: for now, assume conversion is always from cube height to ground.
void reprojectColorCameraCoordinateFromHeight(ofPoint &rawXYZPoint, ofPoint &output) {
    output.x = 0.9557 * rawXYZPoint.x + 0.02038;
    output.y = 0.9538 * rawXYZPoint.y + 0.03888;
    output.z = 0;
};