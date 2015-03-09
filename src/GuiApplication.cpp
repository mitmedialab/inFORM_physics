#include "GuiApplication.h"

//--------------------------------------------------------------
void GuiApplication::setup(){
    if (USE_KINECT) {
        kinectTracker.setup();
    }
	
    if(SEAN_SCREEN){
        //For Sean Screen
        ofSetWindowShape(SEAN_SCREEN_RESOLUTION_X+PROJECTOR_RAW_RESOLUTION_X,PROJECTOR_RAW_RESOLUTION_Y);
        projectorOffsetX = SEAN_SCREEN_RESOLUTION_X+RELIEF_PROJECTOR_OFFSET_X;
        if(CINEMA_DISPLAY){
            ofSetWindowPosition(CINEMA_DISPLAY_OFFSET, 0);
        }
        else{
            ofSetWindowPosition(0, 0);
            
        }
    }
    else{
        //For Daniel Screen
        ofSetWindowShape(DANIEL_SCREEN_RESOLUTION_X+PROJECTOR_RAW_RESOLUTION_X,PROJECTOR_RAW_RESOLUTION_Y);
        projectorOffsetX=DANIEL_SCREEN_RESOLUTION_X+RELIEF_PROJECTOR_OFFSET_X;
        ofSetWindowPosition(0, 0);
    }
    
    ofSetFrameRate(30);
    
	// initialize communication with Relief table
	mIOManager = new ReliefIOManager();
    
    float gain_P = 1.3;
    float gain_I = 0.2;
    int max_I = 60;
    int deadZone = 30;
    int gravityComp = 0;
    int maxSpeed = 220;
    
    int probe_x = 0;
    int probe_y = 0;
    
    mIOManager->sendValueToAllBoards(TERM_ID_GAIN_P, (unsigned char) (gain_P * 25));
    mIOManager->sendValueToAllBoards(TERM_ID_GAIN_I, (unsigned char) (gain_I * 100));
    mIOManager->sendValueToAllBoards(TERM_ID_MAX_I, (unsigned char)max_I);
    mIOManager->sendValueToAllBoards(TERM_ID_DEADZONE, (unsigned char)deadZone);
    mIOManager->sendValueToAllBoards(TERM_ID_GRAVITYCOMP, (unsigned char)gravityComp);
    mIOManager->sendValueToAllBoards(TERM_ID_MAXSPEED, (unsigned char)(maxSpeed/2));
    
    pinDisplayImage.allocate(RELIEF_PROJECTOR_SIZE_X, RELIEF_PROJECTOR_SIZE_X, GL_RGBA);
    pinHeightMapImage.allocate(RELIEF_PROJECTOR_SIZE_X, RELIEF_PROJECTOR_SIZE_X, GL_RGBA);
    pinHeightMapImageSmall.allocate(RELIEF_SIZE_X, RELIEF_SIZE_Y, GL_RGBA);
    
    
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
	
	glOrtho(0.0, ofGetWindowWidth(), ofGetWindowHeight(), 0, 300, 800);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

//--------------------------------------------------------------
void GuiApplication::update(){
    
    if (USE_KINECT) {
        kinectTracker.update();
    }
    
    //app timebase, to send to all animatable objets
	float dt = 1.0f / ofGetFrameRate();
    for(int i=0; i < renderableObjects.size(); i++){
        renderableObjects[i]->update(dt);
        renderableObjects[i]->touchTest(kinectTracker.fingers);
        renderableObjects[i]->objectTest(kinectTracker.redBalls);
    }
    
    // render graphics
    pinDisplayImage.begin();
    ofBackground(0);
    
    for(int i=0; i < renderableObjects.size(); i++){
        renderableObjects[i]->drawGraphics();
    }
    
    if (USE_KINECT && kinectTracker.fingers.size()>0) {
        for (int i = 0; i<kinectTracker.fingers.size(); i++){
            //cout<< kinectTracker.fingers[i].x << " " << kinectTracker.fingers[i].y << " " << kinectTracker.fingers[i].z << endl;
            if(kinectTracker.fingers[i].z<2) {
                ofSetColor(0, 255, 0);
            }
            else{
                ofSetColor(0, 0, 255);
            }
            ofCircle(kinectTracker.fingers[i].x, kinectTracker.fingers[i].y, 10);
        }
    }
    
    pinDisplayImage.end();
    
    // render large heightmap
    pinHeightMapImage.begin();
    ofBackground(0);
    
    
    for(int i=0; i < renderableObjects.size(); i++){
        renderableObjects[i]->drawHeightMap();
    }
    pinHeightMapImage.end();
    
    // render small heightmap
    pinHeightMapImageSmall.begin();
    ofBackground(0);
    ofSetColor(255);
    pinHeightMapImage.draw(0, 0, RELIEF_SIZE_X, RELIEF_SIZE_Y);
    pinHeightMapImageSmall.end();
    
    sendHeightToRelief();
    
    // send height to kinect tracker
    if(USE_KINECT){
        
        ofPixels tempPixels;
        pinHeightMapImageSmall.readToPixels(tempPixels);
        unsigned char* pixels;
        pixels = tempPixels.getPixels();
        
        ofPixels tempPix2;
        tempPix2.allocate(30, 30, 1);
        
        unsigned char* pixels2;
        pixels2 = tempPix2.getPixels();
        
        for(int i = 0; i < (30*30); i++){
            pixels2[i]= tempPixels[i*4];
        }
        
        kinectTracker.setPinHeightMap(tempPix2);
    }
}

//--------------------------------------------------------------
void GuiApplication::draw(){
    
    ofBackground(0,0,0);
    ofSetColor(255);
    ofRect(1, 1, 302, 302);
    ofRect(305, 1, 302, 302);
    pinDisplayImage.draw(2, 2, 300, 300);
    pinHeightMapImage.draw(306, 2, 300, 300);
    
    if(USE_KINECT)
        kinectTracker.draw(2, 306, 320, 240, probe_x, probe_y);
    
    //Draw Graphics onto projector
    ofSetColor(255);
    pinDisplayImage.draw(projectorOffsetX, RELIEF_PROJECTOR_OFFSET_Y, RELIEF_PROJECTOR_SCALED_SIZE_X, RELIEF_PROJECTOR_SCALED_SIZE_Y);
    
}

//--------------------------------------------------------------
void GuiApplication::keyPressed(int key){
    if(key == 'q') {
        renderableObjects.clear();
        
        RButton * tempButton = new RButton(square, false);
        tempButton->position.setPosition(ofPoint(90,600, 60));
        tempButton->setSize(ofPoint(60,60));
        tempButton->render=true;
        renderableObjects.push_back(tempButton);
        
        
        /*RRipple * testRipple = new RRipple();
         testRipple->setPosition(ofPoint(450,450));
         testRipple->setSize(ofPoint(650,650));
         testRipple->render = true;*/
        // renderableObjects.push_back(testRipple);*/
    }
    if(key == 'w') {
        RButton * tempButton2 = new RButton(square, false);
        tempButton2->position.setPosition(ofPoint(90,690, 60));
        tempButton2->setSize(ofPoint(60,60));
        tempButton2->render=true;
        renderableObjects.push_back(tempButton2);
        
        RTouchElement * tempSlider = new RTouchElement();
        tempSlider->position.setPosition(ofPoint(180,480, 60));
        tempSlider->setSize(ofPoint(60,240));
        tempSlider->render=true;
        tempSlider->type=vertical;
        renderableObjects.push_back(tempSlider);
        
        RTouchElement * tempSlider2 = new RTouchElement();
        tempSlider2->position.setPosition(ofPoint(600,720, 60));
        tempSlider2->setSize(ofPoint(240,60));
        tempSlider2->render=true;
        tempSlider2->type=horizontal;
        renderableObjects.push_back(tempSlider2);
        
        RTouchElement * tempSlider3 = new RTouchElement();
        tempSlider3->position.setPosition(ofPoint(270,480, 60));
        tempSlider3->setSize(ofPoint(240,240));
        tempSlider3->render=true;
        tempSlider3->type=pad;
        renderableObjects.push_back(tempSlider3);
    }
    if(key == 'e') {
        
        renderableObjects[2]->size.animateTo(ofPoint(60,540));
        renderableObjects[2]->position.animateTo(ofPoint(180, 270, 60));
        renderableObjects[3]->size.animateTo(ofPoint(390, 60));
        renderableObjects[3]->position.animateTo(ofPoint(330,720,60));
        renderableObjects[4]->size.animateTo(ofPoint(360,360));
        renderableObjects[4]->position.animateTo(ofPoint(330,240,80));
    }
    if (key == '2') {
        renderableObjects.clear();
        
        for (int x = 0; x < 3; x++){
            for (int y = 0; y < 3; y++) {
                RButton * tempButton = new RButton(circle, true);
                tempButton->position.setPosition(ofPoint(225 + (x * 210), 225 + (y * 210), 20));
                tempButton->setSize(ofPoint(70,70,50));
                tempButton->render = true;
                renderableObjects.push_back(tempButton);
            }
        }
    }
    if (key == '3') {
        renderableObjects.clear();
        
        int differenceDistance = 120;
        
        RRamp *test2 = new RRamp();
        test2->lowPoint.setPosition(ofPoint(300,660 - differenceDistance,0));
        test2->lowPoint.setDuration(3);
        test2->highPoint.setPosition(ofPoint(300,120 - differenceDistance,20));
        test2->highPoint.animateTo(ofPoint(300,120 - differenceDistance,240));
        test2->highPoint.setDuration(3);
        test2->highPoint.animateToAfterDelay(ofPoint(300,360 - differenceDistance,240), 1.0);
        
        test2->lowPoint.animateToAfterDelay(ofPoint(300,840,0), 2.0);
        test2->highPoint.animateToAfterDelay(ofPoint(300,300,240), 2.0);
        
        test2->highPoint.animateToAfterDelay(ofPoint(300,520,240), 5.0);
        test2->renderRampRails = false;
        renderableObjects.push_back(test2);
        
        RRamp *test = new RRamp();
        test->lowPoint.setPosition(ofPoint(600,660 - differenceDistance,0));
        test->lowPoint.setDuration(3);
        test->highPoint.setPosition(ofPoint(600,120 - differenceDistance,20));
        test->highPoint.animateTo(ofPoint(600,120 - differenceDistance,240));
        test->highPoint.animateToAfterDelay(ofPoint(600,360 - differenceDistance,240), 1.0);
        test->highPoint.setDuration(3);
        
        test->lowPoint.animateToAfterDelay(ofPoint(600,840,0), 2.0);
        test->highPoint.animateToAfterDelay(ofPoint(600,300,240), 2.0);
        
        test->highPoint.animateToAfterDelay(ofPoint(600,520,240), 5.0);
        test->renderRampRails = false;
        renderableObjects.push_back(test);
        
        
        RRectangle * r = new RRectangle();
        r->setSize(ofPoint(360,30));
        r->position.setDuration(3);
        r->setPosition(ofPoint(270,630 - differenceDistance,50));
        r->position.animateToAfterDelay(ofPoint(270,810,90), 2.0);
        r->render = true;
        r->setColor(ofColor(0));
        renderableObjects.push_back(r);
        
    }
    if (key == '4') {
        renderableObjects.clear();
        /*RRectangle * r = new RRectangle();
        r->setSize(ofPoint(900,900));
        r->setPosition(ofPoint(0,0,200));
        r->position.animateTo(ofPoint(0,0,30));
        r->position.setRepeatType(LOOP_BACK_AND_FORTH);
        r->position.setCurve(LINEAR);
        r->position.setDuration(0.5);
        r->render = true;
        r->setColor(ofColor(0));
        renderableObjects.push_back(r);*/
    }
    if (key == '5') {
        renderableObjects.clear();
        RRectangle * r = new RRectangle();
        r->setSize(ofPoint(900,900));
        r->setPosition(ofPoint(0,0,50));
        r->render = true;
        r->setColor(ofColor(0));
        renderableObjects.push_back(r);
        
        RRectangle * r1 = new RRectangle();
        r1->setSize(ofPoint(150,150));
        r1->setPosition(ofPoint(150,480,20));
        r1->render = true;
        r1->setColor(ofColor(0,0,255));
        renderableObjects.push_back(r1);
        
        RRectangle * r2 = new RRectangle();
        r2->setSize(ofPoint(600,60));
        r2->setPosition(ofPoint(120,720,20));
        r2->render = true;
        r2->setColor(ofColor(0,0,255));
        renderableObjects.push_back(r2);
        
        RWell * rw = new RWell();
        rw->render = true;
        rw->setPosition(ofPoint(240, 240, 50));
        rw->setColor(ofColor(0,0,255));
        rw->setSize(ofPoint(120,90,-80));
        renderableObjects.push_back(rw);
        
        RRamp *test = new RRamp();
        test->lowPoint.setPosition(ofPoint(600,660,0));
        test->highPoint.setPosition(ofPoint(600,100,200));
        renderableObjects.push_back(test);
        
    }
}

//--------------------------------------------------------------
void GuiApplication::keyReleased(int key){
    
}

//--------------------------------------------------------------
void GuiApplication::mouseMoved(int x, int y){
    
}

//--------------------------------------------------------------
void GuiApplication::mouseDragged(int x, int y, int button){
    probe_x = x;
    probe_y = y;
}

//--------------------------------------------------------------
void GuiApplication::mousePressed(int x, int y, int button){
    probe_x = x;
    probe_y = y;
    
}

//--------------------------------------------------------------
void GuiApplication::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void GuiApplication::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void GuiApplication::gotMessage(ofMessage msg){
    
}



//-----------------------------------------------------------
void GuiApplication::sendHeightToRelief(){
    
    unsigned char* pixels;
    ofPixels tempPixels;
    pinHeightMapImageSmall.readToPixels(tempPixels);
    //Rotate to align all coordinate systems
    tempPixels.rotate90(1);
    tempPixels.mirror(true, false);
    pixels = tempPixels.getPixels();
    
    for (int j = 0; j < RELIEF_SIZE_Y; j ++) {
        for (int i = 0; i < RELIEF_SIZE_X; i ++) {
            int y = j;
            int x = RELIEF_SIZE_X - 1 - i;
            int heightMapValue = pixels[((x + (y * RELIEF_SIZE_X)) * 4)];
            mPinHeightToRelief[i][j] = heightMapValue;
        }
    }
    
    // send the height map to the hardware interface
	mIOManager->sendPinHeightToRelief(mPinHeightToRelief);
}

//------------------------------------------------------------
void GuiApplication::exit(){
    if (USE_KINECT) {
        kinectTracker.exit();
    }
    mIOManager->sendValueToAllBoards(TERM_ID_MAXSPEED, (unsigned char) 0);
    ofSleepMillis(1000);
}


