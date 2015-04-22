//
//  CameraCalibration.h
//  Relief2
//
//  Created by Daniel Windham on 4/13/15.
//
//

#include "ofMain.h"
#include "Constants.h"


#ifndef __Relief2__CameraCalibration__
#define __Relief2__CameraCalibration__


// All calibrations were calculated on inFORM 1. They may need recalibration.
// All functions assume coordinates normalized to [0, 1].

void reprojectColorCameraCoordinateFromHeight(ofPoint &rawXYZPoint, ofPoint &output);
void reprojectProjectorPixelsForHeightMap(ofPixels &pixels2D, ofPixels &heightMap, ofPixels &reprojectedPixels);
float reprojectProjectorXCoordinateFromHeight(float x, int height);
float reprojectProjectorYCoordinateFromHeight(float x, int height);

const float projectorMinXCoordinateBoundaryAtCubeHeight = -0.0278;
const float projectorMinYCoordinateBoundaryAtCubeHeight = 0;
const float projectorMaxXCoordinateBoundaryAtCubeHeight = 1.0245;
const float projectorMaxYCoordinateBoundaryAtCubeHeight = 1.0412;

#endif /* defined(__Relief2__CameraCalibration__) */