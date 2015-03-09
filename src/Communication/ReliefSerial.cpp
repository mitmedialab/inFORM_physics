/*
 *  ReliefSerial.cpp
 *  Relief
 *
 *  Created by Daniel Leithinger on 2/27/10.
 *  Copyright 2010 MIT Media Lab. All rights reserved.
 *
 */

#include "ReliefSerial.h"

ReliefSerial::ReliefSerial(string pPortName, int pBaudrate) {
	
	mSerial.enumerateDevices();
	mSerial.setup(pPortName, pBaudrate);
	start();
}

ReliefSerial::~ReliefSerial(){
    
	this->stop();
	mSerial.close();
}

void ReliefSerial::start(){
	startThread(true, false);   // blocking, verbose
}

void ReliefSerial::stop(){
	stopThread();
}

void ReliefSerial::threadedFunction(){ 
	ofSleepMillis(500);
	
	//while (mSerial.available()) {
	//	mSerial.readByte();
	//}
	
    unsigned char messageContent[MSGS_SIZE];
    
	while( isThreadRunning() != 0 ){
		lock();
        bool sendMessage = false;
        if(messageBuffer.size() > 0) { // if there is an element in my buffer
            for (int i = 0; i < MSGS_SIZE; i++) { // copy the content
                messageContent[i] = messageBuffer.front().messageContent[i];
            }
            messageBuffer.erase(messageBuffer.begin());
            sendMessage = true;
        }
        unlock();
        
        if (sendMessage) mSerial.writeBytes(messageContent, 8);
        else ofSleepMillis(4);
	}
}


void ReliefSerial::writeMessage(unsigned char messageContent[MSGS_SIZE]) {
    reliefMessage newMessage;
    for (int i = 0; i < MSGS_SIZE; i++) {
        newMessage.messageContent[i] = messageContent[i];
    }
	lock();
        messageBuffer.push_back(newMessage);
	unlock();
}
