//
//  cvTracker.cpp
//  multiIPCameTracker
//
//  Created by James Kong on 9/1/15.
//
//

#include "cvTracker.h"
void cvTracker::setup(int width, int height)
{
    fbo.allocate(width,height,GL_RGB);
    pixels.allocate(width,height,OF_IMAGE_COLOR);
    
    
    opticalFlow.setup(ofRectangle(0, 0, width, height));
    cvImage.allocate(width, height);
    grayImage.allocate(width, height);
    opticalFlow.setOpticalFlowSize(14);
    opticalFlow.setOpticalFlowBlur(14);
    grayBg.allocate(width,height);
    grayDiff.allocate(width,height);
    trackerControl = ofPtr<ofParameterGroup>(new ofParameterGroup);
    threshold = ofPtr<ofParameter<int> > (new ofParameter<int>);
    minArea = ofPtr<ofParameter<int> > (new ofParameter<int>);
    maxArea = ofPtr<ofParameter<int> > (new ofParameter<int>);
    nConsidered = ofPtr<ofParameter<int> > (new ofParameter<int>);
    bFindHoles = ofPtr<ofParameter<bool> > (new ofParameter<bool>);
    bUseApproximation  = ofPtr<ofParameter<bool> > (new ofParameter<bool>);
    bBlur = ofPtr<ofParameter<bool> > (new ofParameter<bool>);
    trackerControl->setName("CV opticalFlow Tracking");
    trackerControl->add( threshold->set("threashold",80,1,255));
    trackerControl->add( minArea->set("minArea",20,20,width*height));
    trackerControl->add( maxArea->set("maxArea",80,20,width*height));
    trackerControl->add( nConsidered->set("nConsidered",10,1,100));
    trackerControl->add( bFindHoles->set("bFindHoles",true));
    trackerControl->add( bUseApproximation->set("bUseApproximation",true));
    trackerControl->add( bBlur->set("bBlur",true));
}
void cvTracker::update(ofFbo &infbo, int width, int height)
{
    if(!incoming_pixels.isAllocated())
    {
        incoming_pixels.allocate(infbo.getWidth(), infbo.getHeight(), OF_IMAGE_COLOR);
    }
    infbo.readToPixels(incoming_pixels);
    opticalFlow.update(incoming_pixels.getPixels() , infbo.getWidth(), infbo.getHeight() , OF_IMAGE_COLOR);
    fbo.begin();
    ofPushStyle();
    ofSetColor(0);
    ofFill();
    ofRect(0, 0, width,height);
    ofPopStyle();
    ofSetColor(255);
    opticalFlow.draw(width,height);
    fbo.end();
    fbo.readToPixels(pixels);
    cvImage.setFromPixels(pixels.getPixels(), width,height);
    grayImage = cvImage;
    
    if(bBlur)grayImage.blur();
    
    
    
    
    // take the abs value of the difference between background and incoming and then threshold:
    grayDiff.absDiff(grayBg, grayImage);
    grayDiff.threshold(threshold->get());
    
    // find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
    // also, find holes is set to true so we will get interior contours as well....
    contourFinder.findContours(grayImage,
                               minArea->get(),
                               maxArea->get(),
                               nConsidered->get(),
                               bFindHoles->get(),
                               bUseApproximation->get());	// find holes
    
    grayBg = grayImage;

}