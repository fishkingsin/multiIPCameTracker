#pragma once

#include "ofMain.h"
#include "ofxControlPanel.h"
#include "simpleLogger.h"
#include "VideoGrabberManager.h"
#include "cvTracker.h"
class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    void eventsIn(guiCallbackData & data);
    void grabBackgroundEvent(guiCallbackData & data);
    ofxControlPanel gui;
    ofParameterGroup vars;
    ofParameter <float> elapsedTime;
    ofParameter <int> appFrameCount;
    ofParameter <float> appFrameRate;
    ofParameter <bool> bDebug;
    void enableDebug(bool &b);
    simpleLogger logger;
    
    VideoGrabberManager videoManager;
    cvTracker cvtracker;
	ofParameter<bool>toggleMinimize;
    	ofParameter<bool>toggleDrawQuad;
    
    void enableMinize(bool &b);
    
};
