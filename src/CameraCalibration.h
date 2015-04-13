//
//  CameraCalibration.h
//  Relief2
//
//  Created by Daniel Windham on 4/13/15.
//
//

#include "ofMain.h"


#ifndef __Relief2__CameraCalibration__
#define __Relief2__CameraCalibration__


void reprojectColorCameraCoordinateFromHeight(ofPoint &rawXYZPoint, ofPoint &output);

#endif /* defined(__Relief2__CameraCalibration__) */