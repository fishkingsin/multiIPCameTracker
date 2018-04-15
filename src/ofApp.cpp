#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetLogLevel(OF_LOG_VERBOSE);
    ofEnableSmoothing();
    ofEnableAntiAliasing();
    ofEnableAlphaBlending();
    videoManager.setup();
    cvtracker.setup(videoManager.fbo.getWidth(), videoManager.fbo.getHeight());
    gui.loadFont("MONACO.TTF", 8);
    gui.setup("multiIPCam", 0, 0, ofGetWidth(), ofGetHeight());
    gui.setBackgroundColor(0, 0, 0, 125);
    gui.addPanel("app info", 4, false);
    
    //    ofxControlPanel::setBackgroundColor(simpleColor(60, 30, 30, 200));
    gui.addPanel("grabber", 4, false);
    //    gui.setBackgroundColor(0, 0, 0, 125);
    //    ofxControlPanel::setBackgroundColor(simpleColor(70, 70, 30, 200));
    gui.addPanel("fbo", 4, false);
    //    ofxControlPanel::setBackgroundColor(simpleColor(30, 30, 30, 200));
    
    //--------- PANEL 1
    gui.setWhichPanel(0);
    gui.setWhichColumn(0);
    //some dummy vars we will update to show the variable lister object
    elapsedTime		= ofGetElapsedTimef();
    appFrameCount	= ofGetFrameNum();
    appFrameRate	= ofGetFrameRate();
    
    
    vars.setName("app vars");
    vars.add( appFrameCount.set("frame count", 0) );
    vars.add( appFrameRate.set("frame rate", 60.0) );
    vars.add( elapsedTime.set("elapsed time", 0.0) );
    gui.addToggle(toggleMinimize.set("MINIMIZE",false));
    gui.addToggle(toggleDrawQuad.set("DRAW_QUAD",false));
    toggleDrawQuad.addListener(&videoManager, &VideoGrabberManager::enableDrawQuad);
    gui.addToggle( bDebug.set("DEBUG" , true));
    toggleMinimize.addListener(this, &ofApp::enableMinize);
    gui.addVariableLister(vars);
    
    gui.setWhichColumn(1);
    
    gui.enableIgnoreLayoutFlag();
    gui.addLogger("events logger", &logger, 410, 300);
    gui.disableIgnoreLayoutFlag();
    
    bDebug.addListener(this, &ofApp::enableDebug);
    //--------- PANEL 2
    gui.setWhichPanel(1);
    
    gui.setWhichColumn(0);
    for(int i = 0 ; i < videoManager.ipcamRectControls.size(); i++)
    {
        
        gui.addGroup(*videoManager.undistortControls[i].get());
    }
    gui.setWhichColumn(1);
    for (int i = 0 ; i < videoManager.ipcamRectControls.size(); i++){
        gui.addDrawableRect("videos"+ofToString(i), videoManager.grabbers[i].get(), DRAWABLE_WIDTH, DRAWABLE_HEIGHT);
        gui.addDrawableRect("cvimage"+ofToString(i), videoManager.cvImages[i].get(), DRAWABLE_WIDTH, DRAWABLE_HEIGHT);
    }
    
    gui.setWhichPanel(2);
    gui.setWhichColumn(0);
    gui.addToggle(videoManager.enableShader.set("ENABLE_SHADER", false));
    
    for(int i = 0 ; i < videoManager.ipcamRectControls.size(); i++)
    {
        gui.addGroup( *videoManager.ipcamRectControls[i].get() );
    }
    for(int i = 0 ; i < videoManager.shaderControls.size(); i++)
    {
        gui.addGroup( *videoManager.shaderControls[i].get() );
    }
    gui.setWhichColumn(1);
    gui.addDrawableRect("fbo", &videoManager, DRAWABLE_WIDTH, DRAWABLE_HEIGHT);
    
    
    gui.setWhichPanel(3);
    gui.setWhichColumn(0);
    gui.addGroup(cvtracker.trackerControl);
    
    gui.addDrawableRect("fbo", &cvtracker.fbo, DRAWABLE_WIDTH, DRAWABLE_HEIGHT);
    
    gui.addDrawableRect("cvImage", &cvtracker.cvImage, DRAWABLE_WIDTH, DRAWABLE_HEIGHT);
    gui.addDrawableRect("cvGrayImage", &cvtracker.grayImage, DRAWABLE_WIDTH, DRAWABLE_HEIGHT);
    gui.addDrawableRect("grayDiff", &cvtracker.grayDiff , DRAWABLE_WIDTH, DRAWABLE_HEIGHT);
    gui.addDrawableRect("contourFinder", &cvtracker.contourFinder , DRAWABLE_WIDTH, DRAWABLE_HEIGHT);
    //SETTINGS AND EVENTS
    
    //load from xml!
    gui.loadSettings("controlPanelSettings.xml");
    
    //if you want to use events call this after you have added all your gui elements
    gui.setupEvents();
    gui.enableEvents();
    //  -- PANEL EVENTS -- this approach gives you back an ofEvent for only the events from panel 0
    //	ofAddListener(gui.getEventsForPanel(0), this, &testApp::eventsIn);
    
    //  -- this gives you back an ofEvent for all events in this control panel object
    ofAddListener(gui.guiEvent, this, &ofApp::eventsIn);
    
    //  --EVENT FOR SINGLE GUI OBJECT
    //    ofAddListener(gui.createEventGroup("GRAB_BACKGROUND"), this, &ofApp::grabBackgroundEvent);
}
// this is our callback function for the GRAB_BACKGROUND toggle - everytime it changes this gets fired
//--------------------------------------------------------------
void ofApp::grabBackgroundEvent(guiCallbackData & data){
    
    //we use the event callback to capture the background - we then set the toggle value back to its previous value
    if( data.isElement( "GRAB_BACKGROUND" ) && data.getInt(0) == 1 ){
        //        bgExample.captureBackground();
        //        gui.setValueB("GRAB_BACKGROUND", false);
    }
}
//this captures all our control panel events - unless its setup differently in testApp::setup
//--------------------------------------------------------------
void ofApp::eventsIn(guiCallbackData & data){
    
    //lets send all events to our logger
    if( !data.isElement( "events logger" ) ){
        string logStr = data.getXmlName();
        
        for(int k = 0; k < data.getNumValues(); k++){
            logStr += " - " + data.getString(k);
        }
        
        logger.log(OF_LOG_NOTICE, "event - %s", logStr.c_str());
    }
    
    // print to terminal if you want to
    //this code prints out the name of the events coming in and all the variables passed
    printf("testApp::eventsIn - name is %s - \n", data.getXmlName().c_str());
    if( data.getDisplayName() != "" ){
        printf(" element name is %s \n", data.getDisplayName().c_str());
    }
    for(int k = 0; k < data.getNumValues(); k++){
        if( data.getType(k) == CB_VALUE_FLOAT ){
            printf("%i float  value = %f \n", k, data.getFloat(k));
        }
        else if( data.getType(k) == CB_VALUE_INT ){
            printf("%i int    value = %i \n", k, data.getInt(k));
        }
        else if( data.getType(k) == CB_VALUE_STRING ){
            printf("%i string value = %s \n", k, data.getString(k).c_str());
        }
    }
    
    printf("\n");
}

//--------------------------------------------------------------
void ofApp::update(){
    elapsedTime		= ofGetElapsedTimef();
    appFrameCount	= ofGetFrameNum();
    appFrameRate	= ofGetFrameRate();
    
    videoManager.update();
    cvtracker.update(videoManager.fbo , videoManager.fbo.getWidth(), videoManager.fbo.getHeight());
    gui.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    if(toggleDrawQuad)
    {
        videoManager.drawQuadGui();
    }
    //    cvtracker.opticalFlow.draw();
    //    ofPushStyle();
    gui.draw();
    //    ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key==OF_KEY_TAB)
    {
        bDebug = !bDebug;
        if(!bDebug)
        {
            ofSetLogLevel(OF_LOG_SILENT);
            gui.hide();
        }
        else{
            ofSetLogLevel(OF_LOG_VERBOSE);
            gui.show();
        }
    }
    switch(key)
    {
        case  's':
            gui.saveSettings();
            videoManager.saveQuad();
            break;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    gui.mouseDragged(x, y, button);
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    //    gui.mousePressed(x, y, button);
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    //    gui.mouseReleased();
}


//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}


void ofApp::enableDebug(bool &b)
{
    if(!bDebug)
    {
        gui.hide();
        ofSetLogLevel(OF_LOG_WARNING);
    }
    else{
        gui.show();
        ofSetLogLevel(OF_LOG_VERBOSE);
    }
}

void ofApp::enableMinize(bool &b)
{
    if(b)
    {
        ofSetWindowShape(200, 100);
    }
    else{
        ofSetWindowShape(1280, 720);
    }
    
}
