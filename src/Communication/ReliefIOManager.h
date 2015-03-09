/*
 *  ReliefIOManager.h
 *  Relief
 *
 *  Created by Daniel Leithinger on 3/3/10.
 *  Copyright 2010 MIT Media Lab. All rights reserved.
 *
 */

#ifndef _RELIEF_IO_MANAGER
#define _RELIEF_IO_MANAGER

#include "ofMain.h"
#include "ReliefSerial.h"
#include "Constants.h"
#include "ofxXmlSettings.h"

class ReliefBoard{
public:
    unsigned char pinCoordinates[NUM_PINS_ARDUINO][2]; // what physical x and y does each pin on the board map to?
    bool invertHeight; // is it mounted upside down? if so, the height is inverted
    double rollingAverageHeight[NUM_PINS_ARDUINO];
    bool pinIsIdle[NUM_PINS_ARDUINO];
    unsigned char lastFramePinHeight[NUM_PINS_ARDUINO];
    unsigned char currentFramePinHeight[NUM_PINS_ARDUINO];
    int serialConnection; // what serial connection is it on?
    bool isBoardIdle() {
        bool boardIsIdle = true;
        for (int i = 0; i < NUM_PINS_ARDUINO; i++) {
            if (!pinIsIdle[i]) boardIsIdle = false;
        }
        return boardIsIdle;
    };
};

class ReliefIOManager {
private:
	ReliefSerial * mSerialConnections [NUM_SERIAL_CONNECTIONS];
	ReliefBoard reliefBoardValues [NUM_ARDUINOS];
	ofxXmlSettings mXML;
    void setPinsDeadzone();
	
public:
	ReliefIOManager();
	~ReliefIOManager();
	void sendPinHeightToRelief(unsigned char pPinHeightToRelief [RELIEF_SIZE_X][RELIEF_SIZE_Y]);
	void sendValueToAllBoards(unsigned char termId, unsigned char value);
	void sendValuesToBoard(unsigned char termId, unsigned char boardId, unsigned char value[NUM_PINS_ARDUINO], int serialConnection);
	void loadSettings(string pFileName);
    void loadSettings();
};


#endif
