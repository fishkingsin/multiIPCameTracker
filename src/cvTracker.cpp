//
//  cvTracker.cpp
//  multiIPCameTracker
//
//  Created by James Kong on 9/1/15.
//
//

#include "ofxXmlSettings.h"
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
    
    trackerControl.setName("CV opticalFlow Tracking");
    trackerControl.add( bUseFlow.set("USE_FLOW",true));
    trackerControl.add( threshold.set("threashold",80,1,255));
    trackerControl.add( minArea.set("minArea",20,20,width*height*0.5));
    trackerControl.add( maxArea.set("maxArea",80,20,width*height));
    trackerControl.add( nConsidered.set("nConsidered",10,1,100));
    trackerControl.add( bFindHoles.set("bFindHoles",true));
    trackerControl.add( bUseApproximation.set("bUseApproximation",true));
    trackerControl.add( bBlur.set("bBlur",true));
    trackerControl.add(     bTrackDiff.set("trackDiff",true));
    ofxXmlSettings XML;
    
    if(XML.loadFile("streams.xml")){
        sender.setup(XML.getValue("host", HOST), XML.getValue("port", PORT));
    }else{
        sender.setup(HOST, PORT);
    }
}
void cvTracker::update(ofFbo &infbo, int width, int height)
{
    
    if(!incoming_pixels.isAllocated())
    {
        incoming_pixels.allocate(infbo.getWidth(), infbo.getHeight(), OF_IMAGE_COLOR);
    }
    infbo.readToPixels(incoming_pixels);
    if(bUseFlow.get())
    {
        try
        {
            opticalFlow.update(incoming_pixels.getData() , incoming_pixels.getWidth(), incoming_pixels.getHeight() , OF_IMAGE_COLOR);
//            opticalFlow.update(incoming_pixels.getPixels());
        }
        catch(exception e)
        {
            ofLogVerbose() << e.what();
        }
        
        fbo.begin();
        ofPushStyle();
        ofSetColor(0);
        ofFill();
        ofRect(0, 0, width,height);
        ofPopStyle();
        ofSetColor(255);
        //    infbo.draw(0,0,width,height);
        opticalFlow.draw(width,height,5,4);
        fbo.end();
        fbo.readToPixels(pixels);
        cvImage.setFromPixels(pixels);
        
        grayImage = cvImage;
    }
    else{
        grayBg = grayImage;
        cvImage.setFromPixels(incoming_pixels.getPixels(), width,height);
        grayImage = cvImage;
    }
    if(bBlur)grayImage.blur();
    
    
    
    
    // take the abs value of the difference between background and incoming and then threshold:
    if(bTrackDiff){
        grayDiff.absDiff(grayBg, grayImage);
    }else{
        grayDiff = grayImage;
    }
    grayDiff.threshold(threshold.get());
    
    // find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
    // also, find holes is set to true so we will get interior contours as well....
    contourFinder.findContours(grayDiff,
                               minArea.get(),
                               maxArea.get(),
                               nConsidered.get(),
                               bFindHoles.get(),
                               bUseApproximation.get());	// find holes
    
    if(bTrackDiff)grayBg = grayImage;
    //    ofxOscBundle bundle;
    //    ofxOscMessage m;
    if(contourFinder.nBlobs>0)
    {
        ofParameterGroup pGroup;
        pGroup.setName("cvtracker");
        ofParameter<int> nBlobs;
        //        ofxOscBundle bundle;
        pGroup.add(nBlobs.set("nBlobs",contourFinder.nBlobs));
        for (int i = 0; i < contourFinder.nBlobs; i++){
            //            ofxOscMessage m;
            //            m.setAddress("/contour");
            
            //            m.addFloatArg(contourFinder.blobs[i].boundingRect.getCenter().x);
            //            m.addFloatArg(contourFinder.blobs[i].boundingRect.getCenter().y);
            //            sender.sendMessage(m);
            //            bundle.addMessage(m);
            
            //            ofxOscMessage m_hue;
            //            m_hue.setAddress("/hue");
            //            m_hue.addIntArg(c.getHue());
            //            bundle.addMessage(m_hue);
//            ofParameter<ofColor>color;
            ofParameter<ofVec2f>v2;
            pGroup.add(v2.set("contour", ofVec2f(contourFinder.blobs[i].boundingRect.getCenter().x/contourFinder.getWidth() , contourFinder.blobs[i].boundingRect.getCenter().y / contourFinder.getHeight())));
//            ofColor c = incoming_pixels.getColor(contourFinder.blobs[i].boundingRect.getCenter().x, contourFinder.blobs[i].boundingRect.getCenter().x);
//            ofLogVerbose ("color") << c;
//            pGroup.add(color.set("color",c) );
            
        }
        //         sender.sendBundle(bundle);
        sender.sendParameter(pGroup);
    }
    
    
}
