//
//  KinectTracker.cpp
//  Relief2
//
//  Created by Sean Follmer on 3/24/13.
//
//

#include "KinectTracker.h"

void KinectTracker::setup(){
    ofSetLogLevel(OF_LOG_VERBOSE);
	// enable depth->video image calibration
	kinect.setRegistration(true);

    // set depth range of interest in mm
	kinect.setDepthClipping(800, 1050); // 0.8 to 1.05 meters
    
	kinect.init();
	kinect.open();		// opens first available kinect

    colorImgRaw.allocate(kinect.width, kinect.height);
	depthImgRaw.allocate(kinect.width, kinect.height);

    colorImgRaw.setROI(223, 158, frameWidth, frameHeight);
    depthImgRaw.setROI(223, 158, frameWidth, frameHeight);

    colorImg.allocate(frameWidth, frameHeight);
	depthImg.allocate(frameWidth, frameHeight);
    depthNearThreshold.allocate(frameWidth, frameHeight);

    depthBG.allocate(frameWidth, frameHeight);
    depthBGPlusSurface.allocate(frameWidth, frameHeight);
    depthFiltered.allocate(frameWidth, frameHeight);
    depthThreshold.allocate(frameWidth, frameHeight);
    depthThresholdC.allocate(frameWidth, frameHeight);
    depthThresholdF.allocate(frameWidth, frameHeight);
    depthThresholdDilated.allocate(frameWidth, frameHeight);
    depthThresholdDilatedC.allocate(frameWidth, frameHeight);
    dThresholdedColor.allocate(frameWidth, frameHeight);
    dThresholdedColorDilated.allocate(frameWidth, frameHeight);
    dThresholdedColorDilatedG.allocate(frameWidth, frameHeight);
    cornerLikelihoodsRawF.allocate(frameWidth, frameHeight);
    cornerLikelihoods.allocate(frameWidth, frameHeight);

    src[0] = ofPoint(6, 4);
    src[1] = ofPoint(188, 6);
    src[2] = ofPoint(190, 189);
    src[3] = ofPoint(1, 187);
    dst[0] = ofPoint(0, 0);
    dst[1] = ofPoint(frameWidth, 0);
    dst[2] = ofPoint(frameWidth, frameHeight);
    dst[3] = ofPoint(0, frameHeight);

    hsvImage.allocate(frameWidth, frameHeight);
    hue.allocate(frameWidth, frameHeight);
    sat.allocate(frameWidth, frameHeight);
    bri.allocate(frameWidth, frameHeight);
    hueSatThresh.allocate(frameWidth, frameHeight);

    hueThreshNear.allocate(frameWidth, frameHeight);
    hueThreshFar.allocate(frameWidth, frameHeight);
    satThresh.allocate(frameWidth, frameHeight);
    hueThresh.allocate(frameWidth, frameHeight);

    finger_contourFinder.bTrackBlobs = true;
    finger_contourFinder.bTrackFingers = true;
    ball_contourFinder.bTrackBlobs = true;
    ball_contourFinder.bTrackFingers = false;

    calib.setup(kinect.width, kinect.height, &finger_tracker);
    calib.setup(frameWidth, frameHeight, &ball_tracker);
    verdana.loadFont("frabk.ttf", 8, true, true);
    
    loadDepthBackground();

    depthDisplayImage.allocate(frameWidth, frameHeight, OF_IMAGE_COLOR);
    detectedObjectsDisplayImage.allocate(frameWidth, frameHeight, OF_IMAGE_COLOR);
}

void KinectTracker::exit() {
    kinect.setCameraTiltAngle(0); // zero the tilt on exit
	kinect.close();
    
}

void KinectTracker::update(){
    kinect.update();
	
	// there is a new frame and we are connected
	if(kinect.isFrameNew()) {

        // get color image data in region of interest
        colorImgRaw.setFromPixels(kinect.getPixels(), kinect.width, kinect.height);
        colorImg.setFromPixels(colorImgRaw.getRoiPixels(), frameWidth, frameHeight);

        // get depth image data in region of interest
		depthImgRaw.setFromPixels(kinect.getDepthPixels(), kinect.width, kinect.height);
        depthImg.setFromPixels(depthImgRaw.getRoiPixels(), frameWidth, frameHeight);
        depthImg.dilate();
        depthImg.erode();

        // reject near depths
        int nearThreshold = 254; // all pixels closer than the minimum depth are 255, so clip at 254
        depthNearThreshold = depthImg;
        depthNearThreshold.threshold(nearThreshold, true);
        cvAnd(depthNearThreshold.getCvImage(), depthImg.getCvImage(), depthImg.getCvImage(), NULL);

        unsigned char * depthPixels = depthImg.getPixels();
        unsigned char * depthDisplayPixels = depthDisplayImage.getPixels();

        // for human display only, normalize depth pixels to easily visible values
        int displayFloor = 50;
        float displayScalar = (255.0f - displayFloor) / nearThreshold;
        unsigned char displayNormalizedValue;

        for (int i = 0; i < depthDisplayImage.width * depthDisplayImage.height; i++) {
            int indexRGB = i*3;

            if (depthPixels[i] == 0) {
                depthDisplayPixels[indexRGB] = 0;
                depthDisplayPixels[indexRGB+1] = 0;
                depthDisplayPixels[indexRGB+2] = 0;

            } else {
                displayNormalizedValue = depthPixels[i] * displayScalar + displayFloor;

                depthDisplayPixels[indexRGB] = displayNormalizedValue;
                depthDisplayPixels[indexRGB+1] = displayNormalizedValue;
                depthDisplayPixels[indexRGB+2] = displayNormalizedValue;
            }
        }
        depthDisplayImage.update();

        // depth threshold is depth image with all non-black pixels set to white
        depthThreshold = depthImg;
        depthThreshold.threshold(0);

        // relax thresholding restriction since depth information is noisy
        depthThreshold.dilate_3x3();

        // create a threshold that's relaxed even further for edge-detecting algorithms.
        // since some algorithms get confused by the threshold boundary's sharp drop to black, starting
        // with a dilated threshold lets us later shrink the threshold back to reject boundary points.
        depthThresholdDilated = depthThreshold;
        depthThresholdDilated.dilate_3x3();

        // convert grayscale depth thresholds to other image types
        depthThresholdC.setFromGrayscalePlanarImages(depthThreshold, depthThreshold, depthThreshold);
        depthThresholdDilatedC.setFromGrayscalePlanarImages(depthThresholdDilated, depthThresholdDilated, depthThresholdDilated);
        depthThresholdF = depthThresholdC;

        // black out color image regions that are outside the depth range
        cvAnd(colorImg.getCvImage(), depthThresholdC.getCvImage(), dThresholdedColor.getCvImage(), NULL);
        cvAnd(colorImg.getCvImage(), depthThresholdDilatedC.getCvImage(), dThresholdedColorDilated.getCvImage(), NULL);

        dThresholdedColorDilatedG.setFromColorImage(dThresholdedColorDilated);

        //findBlobs(172, 5, 200, redBlobs); // strict red threshold?
        findBlobs(172, 205, 100, redBlobs); // loose threshold

        size = redBlobs.size();
        detectedObjectsDisplayImage.getPixelsRef().setFromPixels(dThresholdedColor.getPixels(), dThresholdedColor.getWidth(), dThresholdedColor.getHeight(), dThresholdedColor.getPixelsRef().getNumChannels());

        int width = detectedObjectsDisplayImage.getPixelsRef().getWidth();
        int height = detectedObjectsDisplayImage.getPixelsRef().getHeight();
        int dx_radius = width/100;
        int dy_radius = height/100;
        pointLocationsText.str(""); // write out detected point coordinates for debugging use
        for(vector<Blob>::iterator blobs_itr = redBlobs.begin(); blobs_itr < redBlobs.end(); blobs_itr++) {
            cubeCenterX = blobs_itr->centroid.x;
            cubeCenterY = blobs_itr->centroid.y;
            pointLocationsText << '(' << cubeCenterX * 1.0 / width << ',' << cubeCenterY * 1.0 / height << ")  ";
            for (int dx = -dx_radius; dx < dx_radius * 2; dx++) {
                for (int dy = -dy_radius; dy < dy_radius * 2; dy++) {
                    detectedObjectsDisplayImage.getPixelsRef().setColor(cubeCenterX + dx, cubeCenterY + dy, ofColor::green);
                    /*
                    int i = (width * (y + dy) + x + dx);
                    detectedObjectsDisplayImage.getPixelsRef()[i] = 255;
                    detectedObjectsDisplayImage.getPixelsRef()[i+1] = 0;
                    detectedObjectsDisplayImage.getPixelsRef()[i+2] = 0;
                    */
                }
            }

            // draw corners of bounding rectangle
            cubeMinX = width, cubeMaxX = 0, cubeMinY = height, cubeMaxY = 0;
            for(vector<ofPoint>::iterator points_itr = blobs_itr->pts.begin(); points_itr < blobs_itr->pts.end(); points_itr++) {
                if (cubeMinX > points_itr->x) {
                    cubeMinX = points_itr->x;
                    cubeLeftCorner = *points_itr;
                }
                if (cubeMaxX < points_itr->x) {
                    cubeMaxX = points_itr->x;
                    cubeRightCorner = *points_itr;
                }
                if (cubeMinY > points_itr->y) {
                    cubeMinY = points_itr->y;
                    cubeTopCorner = *points_itr;
                }
                if (cubeMaxY < points_itr->y) {
                    cubeMaxY = points_itr->y;
                    cubeBottomCorner = *points_itr;
                }
                detectedObjectsDisplayImage.getPixelsRef().setColor(points_itr->x, points_itr->y, ofColor::yellow);
            }
            detectedObjectsDisplayImage.getPixelsRef().setColor(cubeMinX, cubeMinY, ofColor::green);
            detectedObjectsDisplayImage.getPixelsRef().setColor(cubeMaxX, cubeMinY, ofColor::green);
            detectedObjectsDisplayImage.getPixelsRef().setColor(cubeMinX, cubeMaxY, ofColor::green);
            detectedObjectsDisplayImage.getPixelsRef().setColor(cubeMaxX, cubeMaxY, ofColor::green);

            detectedObjectsDisplayImage.getPixelsRef().setColor(cubeLeftCorner.x, cubeLeftCorner.y, ofColor::blue);
            detectedObjectsDisplayImage.getPixelsRef().setColor(cubeRightCorner.x, cubeRightCorner.y, ofColor::blue);
            detectedObjectsDisplayImage.getPixelsRef().setColor(cubeTopCorner.x, cubeTopCorner.y, ofColor::blue);
            detectedObjectsDisplayImage.getPixelsRef().setColor(cubeBottomCorner.x, cubeBottomCorner.y, ofColor::blue);


            // Detector parameters
            int blockSize = 2;
            int apertureSize = 3;
            double k = 0.04;

            // detect corners. this computation is expensive! limit it to the region of interest
            ofRectangle blobRoi = ofRectangle(cubeMinX, cubeMinY, cubeMaxX - cubeMinX, cubeMaxY - cubeMinY);
            cornerLikelihoodsRawF.set(0);
            dThresholdedColorDilatedG.setROI(blobRoi);
            cornerLikelihoodsRawF.setROI(blobRoi);
            cvCornerHarris(dThresholdedColorDilatedG.getCvImage(), cornerLikelihoodsRawF.getCvImage(), blockSize, apertureSize, k);
            dThresholdedColorDilatedG.resetROI();
            cornerLikelihoodsRawF.resetROI();

            // reject points on the threshold boundary (the sharp black edges seduce the corner detector)
            cvAnd(cornerLikelihoodsRawF.getCvImage(), depthThresholdF.getCvImage(), cornerLikelihoodsRawF.getCvImage());

            // normalize results
            cvNormalize(cornerLikelihoodsRawF.getCvImage(), cornerLikelihoods.getCvImage(), 0, 255, cv::NORM_MINMAX);

            corners.clear();

            int cornerThreshold = 100;
            int w = cornerLikelihoods.getWidth();
            int h = cornerLikelihoods.getHeight();

            // Drawing a circle around corners
            for(int i = 0; i < w; i++) {
                for(int j = 0; j < h; j++) {
                    if(cornerLikelihoods.getPixelsRef()[i + j * w] > cornerThreshold) {
                        corners.push_back(ofPoint(i, j));
                        // draw circles around selected corners for debugging purposes
                        cvCircle(cornerLikelihoods.getCvImage(), cvPoint(i,j), 5,  cvScalar(50));
                    }
                }
            }
            cornerLikelihoodsImage = cornerLikelihoods.getPixelsRef();

            int xSpread = cubeMaxX - cubeMinX;
            int ySpread = cubeMaxY - cubeMinY;

            // categorize current state of detected objects
            if (size == 1) {
                currentBlob = *blobs_itr;
                if (xSpread <= 30 && ySpread <= 30) {
                    cubeIsReady = true;
                    cout << "ready" << endl;
                } else {
                    cubeIsReady = false;
                    cout << "not ready" << endl;
                }
                cubeIsReady = true;
            } else {
                cubeIsReady = false;
                cout << "WARNING: detected " << size << " objects, expected 1"<< endl;
            }
        }

        detectedObjectsDisplayImage.update();
    }
}

void KinectTracker::findBlobs(int hue_target, int hue_tolerance, int sat_limit, vector<Blob>& blobs){
    hsvImage.setFromPixels(dThresholdedColor.getPixelsRef());
    //hsvImage.warpIntoMe(thresholdedColor, src, dst); // use to better align input image
    hsvImage.convertRgbToHsv();
    hsvImage.convertToGrayscalePlanarImages(hue, sat, bri);

    hue.erode_3x3();
    hue.dilate_3x3();

    sat.erode_3x3();
    sat.dilate_3x3();

    hueThreshNear = hue;
    hueThreshFar = hue;
    hueThreshNear.threshold(hue_target+hue_tolerance, true);
    hueThreshFar.threshold(hue_target-hue_tolerance);
    
    satThresh = sat;
    satThresh.threshold(sat_limit);

    cvAnd(hueThreshNear.getCvImage(), hueThreshFar.getCvImage(), hueThresh.getCvImage(), NULL);
    cvAnd(hueThresh.getCvImage(), satThresh.getCvImage(), hueSatThresh.getCvImage(), NULL);

    int imgSize = hueSatThresh.width * hueSatThresh.height;
    ball_contourFinder.findContours(hueSatThresh, imgSize / 400, imgSize / 4, 20, 20.0, false);
    ball_tracker.track(&ball_contourFinder);
    
    blobs = ball_contourFinder.blobs;
}

void KinectTracker::findFingers(vector<ofPoint> &points) {
    
    int nearThreshold = 255;
	int farThreshold = 200;
    
    unsigned char * pix = depthImg.getPixels();
    unsigned char * filteredPix = depthFiltered.getPixels();
    unsigned char * bgPix = depthBG.getPixels();
    
    for(int i = 0; i < depthImg.getWidth() * depthImg.getHeight(); i++){
        filteredPix[i] = (pix[i]>(bgPix[i]+1))?pix[i]:0;
        pix[i] = (filteredPix[i] < nearThreshold && filteredPix[i] > farThreshold)?255:0;
        
    }
    depthImg.erode_3x3();
    depthImg.dilate_3x3();

    finger_contourFinder.findContours(depthImg,  (2 * 2) + 1, ((640 * 480) * .4) * (100 * .001), 20, 20.0, false);
    
    finger_tracker.track(&finger_contourFinder);
    
    points.clear();
    
    for(vector<Blob>::iterator itr = finger_contourFinder.fingers.begin(); itr < finger_contourFinder.fingers.end(); itr++){
        ofPoint tempPt = itr->centroid;
        tempPt.z = filteredPix[ int((tempPt.y*kinect.width) + tempPt.x) ] - bgPix[ int((tempPt.y*kinect.width) + tempPt.x) ] -1;
        //cout<<tempPt.x << " " << tempPt.y <<endl;
        
        //Not just any magic numbers! These are magic bean numbers. You put them in the ground and then they grow ; )
        //But serously these numbers convert from kinect space to display space (use them wisely)
        // corners of table are roughly... starting from top left clockwise (236,154), (427,152), (232, 345), (426, 345)
        //I know I suck (sean)
        tempPt.x = ((tempPt.x -232.0)/(427.0-232.0))*900.0;
        tempPt.y = ((tempPt.y - 152.0)/(345-152))*900.0;
        points.push_back(tempPt);
    }
}

void KinectTracker::findFingersAboveSurface(vector<ofPoint> &points) {
    
    int nearThreshold = 255;
	int farThreshold = 200;

    int roiStartX = 232;
    int roiStartY =  152;
    int roiWidth = 427-232;
    int roiHeight = 345-152;
    
    unsigned char * bgROIPix = depthBG.getPixels();
    unsigned char * bgPixAdded = depthBGPlusSurface.getPixels();
    
    ofxCvGrayscaleImage tempDepth;
    tempDepth.allocate(roiWidth, roiHeight);

    unsigned char * tempDepthPix = tempDepth.getPixels();
    
    //cout<<int(tempDepthPix[3])<<endl;
    int counter =0;
    
    for(int i = (232+152*kinect.width); i <  (427+345*kinect.width); i++){
        if(((i%kinect.width)>(roiStartX-1)) && ((i%kinect.width)<(roiStartX+roiWidth)) ){
            bgPixAdded[i] = bgROIPix[i]+ (tempDepthPix[counter])/33; //-1;
            //tempDepthPix[i]=50;
            counter++;
        }
    }
    
    unsigned char * pix = depthImg.getPixels();
    unsigned char * filteredPix = depthFiltered.getPixels();

    for(int i = 0; i < depthImg.getWidth() * depthImg.getHeight(); i++){
        filteredPix[i] = (pix[i]>(bgPixAdded[i]+1))?pix[i]:0;
        pix[i] = (filteredPix[i] < nearThreshold && filteredPix[i] > farThreshold)?255:0;
        
    }
    depthImg.erode_3x3();
    depthImg.dilate_3x3();

    finger_contourFinder.findContours(depthImg,  (2 * 2) + 1, ((640 * 480) * .4) * (100 * .001), 20, 20.0, false);
    
    finger_tracker.track(&finger_contourFinder);
    
    absFingers.clear();
    points.clear();
    for(vector<Blob>::iterator itr = finger_contourFinder.fingers.begin(); itr < finger_contourFinder.fingers.end(); itr++){
        ofPoint tempPt = itr->centroid;
        int tmpx = tempPt.x;
        int tmpy = tempPt.y;
        //maybe this should be changed from bgPixAdded to bgRoiPix
        tempPt.z = filteredPix[ int((tempPt.y*kinect.width) + tempPt.x) ] - bgPixAdded[ int((tempPt.y*kinect.width) + tempPt.x) ] -1;
        //cout<<tempPt.x << " " << tempPt.y <<endl;
        
        //Not just any magic numbers! These are magic bean numbers. You put them in the ground and then they grow ; )
        //But serously these numbers convert from kinect space to display space (use them wisely)
        // corners of table are roughly... starting from top left clockwise (236,154), (427,152), (232, 345), (426, 345)
        //I know I suck (sean)
        tempPt.x = ((tempPt.x -232.0)/(427.0-232.0))*900.0;
        tempPt.y = ((tempPt.y - 152.0)/(345-152))*900.0;
        points.push_back(tempPt);
        
        ofPoint tempPt2;
        //if there is some error in the finger tracking the problem could be here, that I am modifying tempPt after I pushed it to points vector
        
        tempPt2.x = ((tmpx -232.0)/(427.0-232.0))*900.0;
        tempPt2.y = ((tmpy - 152.0)/(345-152))*900.0;
        tempPt2.z = filteredPix[ int((tmpy*kinect.width) + tmpx) ] - bgROIPix[ int((tmpy*kinect.width) + tmpx) ] -1;
        absFingers.push_back(tempPt2);
    }
}

void KinectTracker::saveDepthImage(){
    ofImage tempBG;
    tempBG.setFromPixels(kinect.getDepthPixels(), kinect.width, kinect.height, OF_IMAGE_GRAYSCALE);
    tempBG.saveImage("background.png");
}

void KinectTracker::loadDepthBackground(){
    ofImage tempBG;
    tempBG.loadImage("backgroundGood.png");
    depthBG.setFromPixels(tempBG.getPixels(), kinect.width, kinect.height);
    depthBGPlusSurface.setFromPixels(tempBG.getPixels(), kinect.width, kinect.height);
}

void KinectTracker::draw(int x, int y, int width, int height, int probe_x, int probe_y) {
    
    ofSetColor(255, 255, 255);
    depthDisplayImage.draw(x,y,width,height);

    /*
    hueSatThresh.draw(x,y,width,height);
    // draw red circles around balls
    ofSetColor(ofColor::red);
    for(vector<ofPoint>::iterator itr = redBlobs.begin();itr < redBlobs.end();itr++) {
        ofCircle(x + itr->x / 900.0 * width, y + itr->y / 900.0 * height, 5.0f);
        
        int coord =((int)(itr->y * 190.0 / 900.0) * 190) + (int)(itr->x * 190.0 / 900.0);

        if(0 <= coord && coord < 190 * 190) {
            unsigned char * pix = hue.getPixels();
            pix[coord]= 0;

            int h = hue.getPixels()[coord];
            int s = sat.getPixels()[coord];
            int v = bri.getPixels()[coord];
            //int f = hue.getPixels()[coord];
            char idStr[1024];
            sprintf(idStr, "id: %f %f %f", itr->x, itr->y, itr->z);
            verdana.drawString(idStr, x + itr->x / 900.0 * width, y + itr->y / 900.0 * height);
        }
    }*/

    /*
    // draw blue circles around fingers
    ofSetColor(ofColor::blue);
    
    for(vector<ofPoint>::iterator itr = fingers.begin();itr < fingers.end();itr++)
        ofCircle(x + itr->x / depthImg.width * width, y + itr->y / depthImg.height * height, 5.0f);
    
    ofSetColor(ofColor::white);
    */
    //depthImg.draw(x, y, width, height);
    
    /*
    // pass in a probe x,y coordinate pair to paint that location green for comparison
    kinect.draw(x,y,width,height);
    ofSetColor(ofColor::green);
    ofCircle(probe_x, probe_y, 5.0f);

    int x_c = (probe_x - x) * kinect.width / width;
    int y_c = (probe_y - y) * kinect.height / height;

    if(0 <= x_c && x_c < kinect.width && 0 <= y_c && y_c < kinect.height) {
        char idStr[1024];
        ofVec3f point = kinect.getWorldCoordinateAt(x_c, y_c);
        sprintf(idStr, "%f %f %f", point.x, point.y, point.z);
        verdana.drawString(idStr, probe_x + 10, probe_y);
    }
    */
    
    //ofSetColor(ofColor::white);
}

void KinectTracker::drawColorImage(int x, int y, int width, int height) {
    ofSetColor(255, 255, 255);
    colorImg.flagImageChanged();
    colorImg.draw(x,y,width,height);
}

void KinectTracker::drawDepthImage(int x, int y, int width, int height) {
    ofSetColor(255, 255, 255);
    depthDisplayImage.draw(x,y,width,height);
}

void KinectTracker::drawDetectedObjects(int x, int y, int width, int height) {
    ofSetColor(255, 255, 255);
    detectedObjectsDisplayImage.draw(x,y,width,height);
}

void KinectTracker::drawDepthThresholdedColorImage(int x, int y, int width, int height) {
    ofSetColor(255, 255, 255);
    dThresholdedColorDilated.flagImageChanged();
    dThresholdedColorDilated.draw(x,y,width,height);
}

void KinectTracker::drawCornerLikelihoods(int x, int y, int width, int height) {
    ofSetColor(255, 255, 255);
    cornerLikelihoodsImage.draw(x,y,width,height);
}