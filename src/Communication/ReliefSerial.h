/*
 *  ReliefSerial.h
 *  Relief
 *
 *  Created by Daniel Leithinger on 2/27/10.
 *  Copyright 2010 MIT Media Lab. All rights reserved.
 *
 */

#ifndef _RELIEF_SERIAL
#define _RELIEF_SERIAL

#include "ofMain.h"
#include "ofThread.h"
#include "Constants.h"

#define MSGS_SIZE 8


struct reliefMessage {
    unsigned char messageContent[MSGS_SIZE];
};

class ReliefSerial: public ofThread{
private:
	ofSerial mSerial;
    
	void start();
	void stop();
	void threadedFunction();
        
    vector<reliefMessage> messageBuffer;
	
public:
	ReliefSerial(string pPortName, int pBaudrate);
	~ReliefSerial();
    
    void writeMessage(unsigned char messageContent[MSGS_SIZE]);
};

#endif