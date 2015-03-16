#include "ReliefApplication.h"

//--------------------------------------------------------------
void ReliefApplication::setup(){
    if (USE_KINECT) {
        kinectTracker.setup();
    }
    
    if(SEAN_SCREEN){
        //For Sean Screen
        ofSetWindowShape(SEAN_SCREEN_RESOLUTION_X+PROJECTOR_RAW_RESOLUTION_X,PROJECTOR_RAW_RESOLUTION_Y);
        projectorOffsetX=SEAN_SCREEN_RESOLUTION_X+RELIEF_PROJECTOR_OFFSET_X;
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
    
    mIOManager->sendValueToAllBoards(TERM_ID_GAIN_P, (unsigned char) (gain_P * 25));
    mIOManager->sendValueToAllBoards(TERM_ID_GAIN_I, (unsigned char) (gain_I * 100));
    mIOManager->sendValueToAllBoards(TERM_ID_MAX_I, (unsigned char)max_I);
    mIOManager->sendValueToAllBoards(TERM_ID_DEADZONE, (unsigned char)deadZone);
    mIOManager->sendValueToAllBoards(TERM_ID_GRAVITYCOMP, (unsigned char)gravityComp);
    mIOManager->sendValueToAllBoards(TERM_ID_MAXSPEED, (unsigned char)(maxSpeed/2));
    
    colorInputImage.allocate(RELIEF_PROJECTOR_SIZE_X, RELIEF_PROJECTOR_SIZE_X, GL_RGBA);
    depthInputImage.allocate(RELIEF_PROJECTOR_SIZE_X, RELIEF_PROJECTOR_SIZE_X, GL_RGBA);
    detectedObjectsImage.allocate(RELIEF_PROJECTOR_SIZE_X, RELIEF_PROJECTOR_SIZE_X, GL_RGBA);

    pinDisplayImage.allocate(RELIEF_PROJECTOR_SIZE_X, RELIEF_PROJECTOR_SIZE_X, GL_RGBA);
    pinHeightMapImage.allocate(RELIEF_PROJECTOR_SIZE_X, RELIEF_PROJECTOR_SIZE_X, GL_RGBA);
    pinHeightMapImageSmall.allocate(RELIEF_SIZE_X, RELIEF_SIZE_Y, GL_RGBA);

    myMarbleMachine = new MarbleMachine();
    
    myRipple = new RRipple();
    myRipple->setPosition(ofPoint(450,450));
    myRipple->setSize(ofPoint(650,650));
    myRipple->render = true;
    
    myCurrentRenderedObject = myMarbleMachine;
    
    bool loadPlane = false;
    
    if (loadPlane) {
        model.loadModel("plane/plane.3ds", 18);
        model.setRotation(0, -5, 1, 0, 0); // plane
        model.setRotation(1, 29, 0, 0, 1); // plane
        model.setPosition(435, 420, -400); // plane
    }
    else {
        model.loadModel("VWBeetle/VWBeetle.3ds", 0.099);
        model.setScale(1,1,1);
        model.setPosition(470, 420, -250);
        model.setRotation(1, 180, 0, 0, 1);
    }
    
    //model.loadModel("plane/plane.3ds", 18); // plant
    
    mHeightMapShader.load("shaders/heightMapShader");
    
    show3DModel = false;
}

//--------------------------------------------------------------
void ReliefApplication::update(){

    
    if (USE_KINECT) {
        kinectTracker.update();
    }
    
    //app timebase, to send to all animatable objets
	float dt = 1.0f / ofGetFrameRate();
    
    myCurrentRenderedObject->update(dt);
    
    //for(int i=0; i < renderableObjects.size(); i++){
    //    renderableObjects[i]->update(dt);
    //}
    
    // render input color image
    colorInputImage.begin();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, 900, 900);
    glOrtho(0.0, 900, 0, 900, -500, 500);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPushMatrix();
    glTranslated(0, 0, -500);
    glPopMatrix();
    
    ofBackground(0);
    
    glEnable(GL_DEPTH_TEST);
    if(show3DModel) model.draw();
    glDisable(GL_DEPTH_TEST);
    
    kinectTracker.drawColorImage(0, 0, RELIEF_PROJECTOR_SIZE_X, RELIEF_PROJECTOR_SIZE_X);
    
    colorInputImage.end();
    
    // render input depth image
    depthInputImage.begin();
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, 900, 900);
    glOrtho(0.0, 900, 0, 900, -500, 500);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPushMatrix();
    glTranslated(0, 0, -500);
    glPopMatrix();
    
    ofBackground(0);

    mHeightMapShader.begin();
    
    glEnable(GL_DEPTH_TEST);
    if(show3DModel) model.draw();
    //glPopMatrix();
    glDisable(GL_DEPTH_TEST);
    mHeightMapShader.end();

    kinectTracker.drawDepthImage(0, 0, RELIEF_PROJECTOR_SIZE_X, RELIEF_PROJECTOR_SIZE_X);

    depthInputImage.end();
    
    // render detected objects
    detectedObjectsImage.begin();
    ofBackground(0);
    ofSetColor(255);
    kinectTracker.drawDetectedObjects(0, 0, RELIEF_PROJECTOR_SIZE_X, RELIEF_PROJECTOR_SIZE_X);
    detectedObjectsImage.end();
    
    // render large heightmap
    pinHeightMapImage.begin();
    ofBackground(0);
    ofSetColor(255);
    depthInputImage.draw(0, 0, RELIEF_PROJECTOR_SIZE_X, RELIEF_PROJECTOR_SIZE_X);
    pinHeightMapImage.end();
    
    // render small heightmap
    pinHeightMapImageSmall.begin();
    ofBackground(0);
    ofSetColor(255);
    pinHeightMapImage.draw(0, 0, RELIEF_SIZE_X, RELIEF_SIZE_Y);
    pinHeightMapImageSmall.end();

    sendHeightToRelief();
}

//--------------------------------------------------------------
void ReliefApplication::draw(){
    ofBackground(0,0,0);
    ofSetColor(255);

    ofRect(1, 1, 302, 302);
    colorInputImage.draw(2, 2, 300, 300);

    ofRect(305, 1, 302, 302);
    depthInputImage.draw(306, 2, 300, 300);

    ofRect(609, 1, 302, 302);
    detectedObjectsImage.draw(610, 2, 300, 300);
    
    ofRect(913, 1, 302, 302);
    pinHeightMapImageSmall.draw(914, 2, 300, 300);

    // for object detection debugging use
    ofDrawBitmapString(ofToString(kinectTracker.size), 1, 350);
    ofDrawBitmapString(kinectTracker.pointLocationsText.str(), 1, 380);


    //Draw Graphics onto projector
    pinDisplayImage.draw(projectorOffsetX, RELIEF_PROJECTOR_OFFSET_Y, RELIEF_PROJECTOR_SCALED_SIZE_X, RELIEF_PROJECTOR_SCALED_SIZE_Y);
    
    
    //kinectTracker.draw(1, 306, 640, 480, 0, 0);
   /* if (kinectTracker.fingers.size()>0) {
        for (int i = 0; i<kinectTracker.fingers.size(); i++){
            cout<< kinectTracker.fingers[i].x << " " << kinectTracker.fingers[i].y << " " << kinectTracker.fingers[i].z << endl;
            //ofCircle(projectorOffsetX+(kinectTracker.fingers[i].x / RELIEF_PROJECTOR_SCALED_SIZE_X), RELIEF_PROJECTOR_OFFSET_Y+ (kinectTracker.fingers[i].y / RELIEF_PROJECTOR_SCALED_SIZE_Y), 10);
        }
    }*/
}

//--------------------------------------------------------------
void ReliefApplication::keyPressed(int key){
    myCurrentRenderedObject->keyPressed(key);
    
    if(key == '1') {
        myCurrentRenderedObject = myMarbleMachine;
    }
    if(key == '2') {
        myCurrentRenderedObject = myRipple;
    }
    
    if(key == 'p') {
        kinectTracker.saveDepthImage();
    }
    
    if(key == 'r') {
        show3DModel = !show3DModel;
    }
}

//--------------------------------------------------------------
void ReliefApplication::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ReliefApplication::mouseMoved(int x, int y){
    
}

//--------------------------------------------------------------
void ReliefApplication::mouseDragged(int x, int y, int button){
    
    
}

//--------------------------------------------------------------
void ReliefApplication::mousePressed(int x, int y, int button){
    //test->position.animateTo(ofPoint((x - 2)*3, (y - 2)*3, 50));
    //printf("%f, %f, %f \n",renderableObjects[0].position.getCurrentPosition().x, renderableObjects[0].position.getCurrentPosition().y, renderableObjects[0].position.getCurrentPosition().z);

    //renderableObjects[i].drawGraphics();
}

//--------------------------------------------------------------
void ReliefApplication::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ReliefApplication::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ReliefApplication::gotMessage(ofMessage msg){
    
}



//-----------------------------------------------------------
void ReliefApplication::sendHeightToRelief(){
    
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
void ReliefApplication::exit(){
    if (USE_KINECT) {
        kinectTracker.exit();
    }
    mIOManager->sendValueToAllBoards(TERM_ID_MAXSPEED, (unsigned char) 0);
    ofSleepMillis(1000);
}


