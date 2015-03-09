//
//  PenModel.cpp
//  Relief2
//
//  Created by Sean Follmer on 3/31/13.
//
//

#include "PenModel.h"

//--------------------------------------------------------------
void PenModel::setup(){
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
    int deadZone = 20;
    int gravityComp = 0;
    int maxSpeed = 220;
    
    mIOManager->sendValueToAllBoards(TERM_ID_GAIN_P, (unsigned char) (gain_P * 25));
    mIOManager->sendValueToAllBoards(TERM_ID_GAIN_I, (unsigned char) (gain_I * 100));
    mIOManager->sendValueToAllBoards(TERM_ID_MAX_I, (unsigned char)max_I);
    mIOManager->sendValueToAllBoards(TERM_ID_DEADZONE, (unsigned char)deadZone);
    mIOManager->sendValueToAllBoards(TERM_ID_GRAVITYCOMP, (unsigned char)gravityComp);
    mIOManager->sendValueToAllBoards(TERM_ID_MAXSPEED, (unsigned char)(maxSpeed/2));
    
    pinDisplayImage.allocate(RELIEF_PROJECTOR_SIZE_X, RELIEF_PROJECTOR_SIZE_X, GL_RGBA);
    pinHeightMapImage.allocate(RELIEF_PROJECTOR_SIZE_X, RELIEF_PROJECTOR_SIZE_X, GL_RGBA);
    
    pinHeightMapImageSmall.allocate(RELIEF_SIZE_X, RELIEF_SIZE_Y, GL_RGBA);
    
    
    for(unsigned int x=0;x<RELIEF_SIZE_X;x++)
        for(unsigned int y=0;y<RELIEF_SIZE_Y;y++)
            penDrawing[x][y] = 0.0f;
    
    verdana.loadFont("frabk.ttf", 100, true, true);
    
    mHeightMapShader.load("shaders/heightMapShader");
    
    
    R3DModel * squirrel = new R3DModel("squirrel/NewSquirrel.3ds");
    squirrel->model_scale(3.0f,3.0f,3.0f);
    squirrel->model_rotate(0,90,0,0,1);
    squirrel->model_translate(0,0,-100);
    squirrel->render=true;
    
    R3DModel * turbo = new R3DModel("turbosonic/turbosonic.3ds");
    turbo->model_scale(0.0035f,0.0035f,0.0035f);
    turbo->model_rotate(0,90,0,0,1);
    turbo->model_rotate(1,90,0,1,0);
    turbo->model_translate(0,0,-100);
    turbo->render=true;
    
    R3DModel * face = new R3DModel("FaceTS_3DS/faceTS_3DS.3DS");
    face->model_scale(2.0f,2.0f,2.0f);
    face->model_rotate(0,160,0,0,1);
    face->model_translate(-50,100,-250);
    face->render=true;
    
    R3DModel * cheese = new R3DModel(PRIM_CHEESE);
    cheese->model_translate(0,0,-200);
    cheese->render=true;

    R3DModel * cone = new R3DModel(PRIM_CONE);
    cone->model_translate(0,0,-200);
    cone->render=true;

    models.push_back(cone);
    models.push_back(cheese);
    models.push_back(face);
    models.push_back(squirrel);
    models.push_back(turbo);
    
    galleryAnim.animateFromTo(current_model,current_model);
    galleryAnim.setDuration(1.0f);
    galleryAnim.setCurve(BOUNCY);
}

//--------------------------------------------------------------
void PenModel::update(){
    
    // update the sound playing system:
	ofSoundUpdate();
    
    if (USE_KINECT) {
        kinectTracker.update();
    }
    
    //determine state and update
    determineCurrentState(kinectTracker.redBalls);
    models[current_model]->determineState(kinectTracker.redBalls);
    
    //app timebase, to send to all animatable objets
	float dt = 1.0f / ofGetFrameRate();
    for(vector<R3DModel*>::iterator itr = models.begin();itr != models.end();itr++)
        (*itr)->update(dt);
    
    galleryAnim.update(dt);

    // render graphics
    pinDisplayImage.begin();
    ofBackground(0);
    
    glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, 900, 900);
	glOrtho(0.0, 900, 0, 900, -500, 500);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
    
    for(int i=0; i < models.size(); i++) {
        ofPushMatrix();
        ofTranslate(0, 900.0f * ((float)(galleryAnim.getCurrentValue() - i)),0);
        models[i]->drawGraphics();
        ofPopMatrix();
    }
    
    
    for (unsigned int x=0;x<RELIEF_SIZE_X;x++)
        for(unsigned int y=0;y<RELIEF_SIZE_Y;y++) {
            float h = penDrawing[x][y];
            if(h) {
                ofSetColor(h * 28.0f,h * 28.0f,h * 28.0f);
                ofRect(30*x,30*y,30,30);
            }
        }
    

    ofSetColor(ofColor::forestGreen);
    ofSetLineWidth(40.0f);
    for(vector<ofPolyline>::iterator itr = paintLines.begin();itr != paintLines.end(); itr++) {
        itr->draw();
    }
    
    
    float a_start = -PI/3.0f + 3.0f * PI/2.0f - PI/5.0f;
    float a_end =    PI/3.0f + 3.0f * PI/2.0f - PI/5.0f;
    

    if(currentState == menuMode){

        ofPoint c = contextMenuLocation;
        int res = 20;
        float rad = 200;
        
        //Check position of hand inside the menu
        menuSelection = -1;
        for(int i = 0; i <kinectTracker.absFingers.size(); i++) {
            ofPoint p = kinectTracker.absFingers[i];
            if(p.distance(c) > 20) {
                float angle = atan2(p.y - c.y, p.x - c.x) + 2.0f * PI;
                for(int x=0;x<3;x++)
                    if (ofLerp(a_start,a_end,(float)x/3.0f) <= angle && angle < ofLerp(a_start,a_end,(float)(x+1)/3.0f))
                        menuSelection = x;
            }
        }

        ofSetColor(menuSelection==0 ? ofColor::white : ofColor::darkGreen);
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(c.x,c.y);
        for(int i = 0;i <= res;i++) {
            float a = ofLerp(a_start,a_end,(float)i / (float)(3 * res) + 0);
            glVertex2f(c.x + rad * cos(a),c.y + rad * sin(a));
        }
        glEnd();
        
        ofSetColor(menuSelection==1 ? ofColor::darkViolet : ofColor::darkViolet);
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(c.x,c.y);
        for(int i = 0;i <= res;i++) {
            float a = ofLerp(a_start,a_end,(float)i / (float)(3 * res) + 1.0f/3.0f);
            glVertex2f(c.x + rad * cos(a),c.y + rad * sin(a));
        }
        glEnd();
        
        ofSetColor(menuSelection==2 ? ofColor::white : ofColor::darkTurquoise);
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(c.x,c.y);
        for(int i = 0;i <= res;i++) {
            float a = ofLerp(a_start,a_end,(float)i / (float)(3 * res) + 2.0f/3.0f);
            glVertex2f(c.x + rad * cos(a),c.y + rad * sin(a));
        }
        glEnd();
    }
    
    pinDisplayImage.end();
    
    // render large heightmap
    pinHeightMapImage.begin();
    ofBackground(0);
    
    glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, 900, 900);
	
	glOrtho(0.0, 900, 0, 900, -500, 500);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
    
    for(int i=0; i < models.size(); i++) {
        ofPushMatrix();
        ofTranslate(0, 900.0f * ((float)(galleryAnim.getCurrentValue() - i)),0);
        models[i]->drawHeightMap();
        ofPopMatrix();

    }
    
    /*
    for(int i=0; i< geometryPoints.size(); i++){
        ofSetColor(geometryPoints[i].z*28, geometryPoints[i].z*28, geometryPoints[i].z*28);
        ofCircle(geometryPoints[i].x, geometryPoints[i].y, 30);
    }*/
    
    
    for (unsigned int x=0;x<RELIEF_SIZE_X;x++)
        for(unsigned int y=0;y<RELIEF_SIZE_Y;y++) {
            float h = penDrawing[x][y];
            if(h) {
                ofSetColor(h * 28.0f,h * 28.0f,h * 28.0f);
                ofRect(30*x,30*y,30,30);
            }
        }

    if(currentState == menuMode){
        ofPoint c = contextMenuLocation;
        int res = 20;
        float rad = 200;

        ofSetColor(28.0 * c.z,28.0 * c.z,28.0 * c.z);
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(c.x,c.y);
        for(int i = 0;i <= res;i++) {
            float a = ofLerp(a_start,a_end,(float)i / (float)(3 * res) + 0);
            glVertex2f(c.x + rad * cos(a),c.y + rad * sin(a));
        }
        glEnd();

        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(c.x,c.y);
        for(int i = 0;i <= res;i++) {
            float a = ofLerp(a_start,a_end,(float)i / (float)(3 * res) + 1.0f/3.0f);
            glVertex2f(c.x + rad * cos(a),c.y + rad * sin(a));
        }
        glEnd();

        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(c.x,c.y);
        for(int i = 0;i <= res;i++) {
            float a = ofLerp(a_start,a_end,(float)i / (float)(3 * res) + 2.0f/3.0f);
            glVertex2f(c.x + rad * cos(a),c.y + rad * sin(a));
        }
        glEnd();
        
    }
    
    ofSetColor(ofColor::black);
    //ofRect(0,0,50,900);
    
    pinHeightMapImage.end();
    
    // render small heightmap
    pinHeightMapImageSmall.begin();
    ofBackground(0);
    ofSetColor(255);
    pinHeightMapImage.draw(0, 0, RELIEF_SIZE_X, RELIEF_SIZE_Y);
    pinHeightMapImageSmall.end();
    
    
    sendHeightToRelief();
    
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
void PenModel::draw(){
    ofBackground(0,0,0);
    ofSetColor(255);
    ofRect(1, 1, 302, 302);
    ofRect(305, 1, 302, 302);
    pinDisplayImage.draw(2, 2, 300, 300);
    pinHeightMapImage.draw(306, 2, 300, 300);
    
    
    //Draw Graphics onto projector
    pinDisplayImage.draw(projectorOffsetX, RELIEF_PROJECTOR_OFFSET_Y, RELIEF_PROJECTOR_SCALED_SIZE_X, RELIEF_PROJECTOR_SCALED_SIZE_Y);
    
    
    kinectTracker.draw(1, 306, 640, 480, 0, 0);
    /* if (kinectTracker.fingers.size()>0) {
     for (int i = 0; i<kinectTracker.fingers.size(); i++){
     cout<< kinectTracker.fingers[i].x << " " << kinectTracker.fingers[i].y << " " << kinectTracker.fingers[i].z << endl;
     //ofCircle(projectorOffsetX+(kinectTracker.fingers[i].x / RELIEF_PROJECTOR_SCALED_SIZE_X), RELIEF_PROJECTOR_OFFSET_Y+ (kinectTracker.fingers[i].y / RELIEF_PROJECTOR_SCALED_SIZE_Y), 10);
     }
     }*/
}

//--------------------------------------------------------------
void PenModel::keyPressed(int key){
    if(key == 13) {
        if(!kinectTracker.absFingers.size() || currentState == menuMode || currentState == noneMode) {
            stillDown = false;
            return;
        }
        for(int i = 0; i <kinectTracker.absFingers.size(); i++) {
            ofPoint p = kinectTracker.absFingers[i];
            if(!pointIsInsideTable(p)) {
                continue;
            }
            
            int x1 = (int)p.x / 30;
            int y1 = (int)p.y / 30;

            if(!stillDown) {
                switch(currentState) {
                    case geometryMode:
                        penDrawing[x1][y1] = p.z;
                        break;
                    case flattenMode:
                        penDrawing[x1][y1] = 85.0f/28.0f;
                        break;
                    case paintMode:
                    {
                        ofPolyline pl;
                        pl.addVertex(ofPoint(p.x,p.y));
                        paintLines.push_back(pl);
                        break;
                    }
                    default:
                        break;
                }
            }
            else {
                if(currentState == paintMode) {
                    (paintLines.end()-1)->addVertex(ofPoint(p.x,p.y));
                } else {
                    int x0 = (int)lastFinger.x / 30;
                    int y0 = (int)lastFinger.y / 30;
                   
                    int dx = abs(x1 - x0);
                    int dy = abs(y1 - y0);
                    int sx = (x0 < x1) ? 1 : -1;
                    int sy = (y0 < y1) ? 1: -1;
                    int err = dx - dy;
                    int x = x0;int y = y0;
                    while(true) {
                        switch(currentState) {
                            case geometryMode:
                            {
                                float amount = (dx+dy) ? float(abs(x-x0) + abs(y-y0)) / float(dx+dy) : 0;
                                penDrawing[x][y] = ofLerp(lastFinger.z, p.z, amount);
                                break;
                            }
                            case flattenMode:
                                penDrawing[x][y] = 85.0f/28.0f;
                                break;
                            default:
                                break;
                        }
                        if(x == x1 && y == y1) break;
                        int e2 = 2 * err;
                        if(e2 > -dy) {err-=dy;x+=sx;}
                        if(e2 < dx) {err+=dx;y+=sy;}
                    };
                }
            }
            lastFinger = p;
            stillDown = true;
        }        
    }
    
    if(key == ' ') {
        if(kinectTracker.fingers.size() >0) {
            for(int i = 0; i <kinectTracker.fingers.size(); i++) {
                if(pointIsInsideTable(kinectTracker.fingers[i]) && currentState != menuMode) {
                    contextMenuLocation=kinectTracker.absFingers[i];
                    currentState = menuMode;
                }
            }
        }
    }
    if(key == 'c'){
        for(unsigned int x=0;x<RELIEF_SIZE_X;x++)
            for(unsigned int y=0;y<RELIEF_SIZE_Y;y++)
                penDrawing[x][y] = 0.0f;

        //geometryPoints.clear();
    }
    if(key == 'p') {
        
        //kinectTracker.saveDepthImage();
    }
    if(key == '7') {
        current_model -= 1;
        current_model = MAX(0,current_model);
        galleryAnim.animateFromTo(galleryAnim.getCurrentValue(), current_model);
    }
    if(key == '8') {
        current_model += 1;
        current_model = MIN(models.size()-1,current_model);
        galleryAnim.animateFromTo(galleryAnim.getCurrentValue(), current_model);
    }
}

//--------------------------------------------------------------
void PenModel::keyReleased(int key){
    if(key == 13) {
        stillDown = false;
    }
    if(key == ' ' ) {
        switch(menuSelection) {
            case 0:
                currentState = geometryMode;break;
            case 1:
                currentState = flattenMode;break;
            case 2:
                currentState = paintMode;break;
            default:
                currentState = noneMode;break;
        }
    }
    if(key == '3') {
        //menu
        if(kinectTracker.fingers.size() >0){
            for(int i = 0; i <kinectTracker.fingers.size(); i++){
                if(pointIsInsideTable(kinectTracker.fingers[i])){
                    //draw context menu
                    currentState = geometryMode;
                }
            }
        }
        else{
            currentState = geometryMode;
        }
    }
}

//--------------------------------------------------------------
void PenModel::mouseMoved(int x, int y){
    
}

//--------------------------------------------------------------
void PenModel::mouseDragged(int x, int y, int button){
    
    
}

//--------------------------------------------------------------
void PenModel::mousePressed(int x, int y, int button){
    
    
    
    //test->position.animateTo(ofPoint((x - 2)*3, (y - 2)*3, 50));
    //printf("%f, %f, %f \n",renderableObjects[0].position.getCurrentPosition().x, renderableObjects[0].position.getCurrentPosition().y, renderableObjects[0].position.getCurrentPosition().z);
    
    //renderableObjects[i].drawGraphics();
}

//--------------------------------------------------------------
void PenModel::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void PenModel::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void PenModel::gotMessage(ofMessage msg){
    
}

////--------------------------------
void PenModel::determineCurrentState(vector<ofPoint> ballLocations){
    for(int i=0;i<ballLocations.size();i++)
        if(ballLocations[i].x < 30) {
            int loc = models.size() * ((float)ballLocations[i].y / 900.0f);
            loc = MIN(MAX(loc,0),models.size() - 1);
            if(current_model != loc) {
                current_model = loc;
                galleryAnim.animateFromTo(galleryAnim.getCurrentValue(), current_model);
            }
        }
}

//-----------------------------
bool PenModel::pointIsInsideTable(ofPoint p1){
    return pointIsInsideRectangle(p1, 0, 0, 900, 900);
}

//-----------------------------
bool PenModel::pointIsInsideRectangle(ofPoint p1, int x, int y, int width, int height){
    
    if((p1.x>x) && (p1.x<x+width) && (p1.y>y) && (p1.y<y+height)){
        return true;
    }
    
    return false;
}

/////--------------------------
bool PenModel::pointIsCloseToPoint(ofPoint p1, ofPoint p2, int minDist){
    
    if (p1.distance(p2) < minDist) {
        return true;
    }
    return false;
    
    
}

//------------------------------
bool PenModel::pointIsFarFromPoint(ofPoint p1, ofPoint p2, int maxDist){
    if (p1.distance(p2) > maxDist) {
        return true;
    }
    return false;
}



//-----------------------------------------------------------
void PenModel::sendHeightToRelief(){
    
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
void PenModel::exit(){
    if (USE_KINECT) {
        kinectTracker.exit();
    }
    mIOManager->sendValueToAllBoards(TERM_ID_MAXSPEED, (unsigned char) 0);
    ofSleepMillis(1000);
}

float PenModel::getAngle(float _x, float _y) {
    if (_x == 0.0) {
        if(_y < 0.0)
            return 270;
        else
            return 90;
    } else if (_y == 0) {
        if(_x < 0)
            return 180;
        else
            return 0;
    }
    
    if ( _y > 0.0)
        if (_x > 0.0)
            return atan(_y/_x) * GRAD_PI;
        else
            return 180.0 - (atan(_y/-_x) * GRAD_PI);
        else
            if (_x > 0.0)
                return 360.0 - (atan(-_y/_x) * GRAD_PI);
            else
                return 180.0 + (atan(-_y/-_x) * GRAD_PI);
}


