//
//  VideoGrabberManager.h
//  wlbankinteractive
//
//  Created by Kong king sin on 2/1/15.
//
//

#ifndef __wlbankinteractive__VideoGrabberManager__
#define __wlbankinteractive__VideoGrabberManager__
#include "ofMain.h"
#include "IPVideoGrabber.h"
#include "Constants.h"
#include "ofxXmlSettings.h"
#include "ofxOpenCv.h"
#include "ofxAutoReloadedShader.h"
class IPCameraDef
{
public:
    IPCameraDef()
    {
    }
    
    IPCameraDef(const std::string& url): _url(url)
    {
    }
    
    IPCameraDef(const std::string& name,
                const std::string& url,
                const std::string& username,
                const std::string& password):
    _name(name),
    _url(url),
    _username(username),
    _password(password)
    {
    }
    
    void setName(const std::string& name) { _name = name; }
    std::string getName() const { return _name; }
    
    void setURL(const std::string& url) { _url = url; }
    std::string getURL() const { return _url; }
    
    void setUsername(const std::string& username) { _username = username; }
    std::string getUsername() const { return _username; }
    
    void setPassword(const std::string& password) { _password = password; }
    std::string getPassword() const { return _password; }
    
    
private:
    std::string _name;
    std::string _url;
    std::string _username;
    std::string _password;
};
using ofx::Video::IPVideoGrabber;
using ofx::Video::SharedIPVideoGrabber;

class VideoGrabberManager
{
public:
    void setup();
    void update();
    void draw();
    std::vector< ofPtr<IPVideoGrabber> > grabbers;
    void loadCameras();
    IPCameraDef& getNextCamera();
    std::vector<IPCameraDef> ipcams; // a list of IPCameras
    std::vector< ofPtr<ofParameterGroup> > ipcamRectControls;
    std::vector< ofPtr<ofParameter<float> > > ipCamX;
    std::vector< ofPtr<ofParameter<float> > > ipCamY;
    std::vector< ofPtr<ofParameter<float> > > ipCamW;
    std::vector< ofPtr<ofParameter<float> > > ipCamH;
    
//    std::vector< ofPtr<ofxCvColorImage > > cvImages;
    
    std::vector< ofPtr<ofParameterGroup> > shaderControls;
    std::vector< ofPtr<ofParameter<float> > > alphaLs;
    std::vector< ofPtr<ofParameter<float> > > alphaRs;
    std::vector< ofPtr<ofParameter<float> > > sizeLs;
    std::vector< ofPtr<ofParameter<float> > > sizeRs;
    
    std::vector< ofPtr<ofParameterGroup> > undistortControls;
    std::vector< ofPtr<ofParameter<float> > > radialDistX;
    std::vector< ofPtr<ofParameter<float> > > radialDistY;
    std::vector< ofPtr<ofParameter<float> > > tangentDistX;
    std::vector< ofPtr<ofParameter<float> > > tangentDistY;
    std::vector< ofPtr<ofParameter<float> > > focalX;
    std::vector< ofPtr<ofParameter<float> > > focalY;
    std::vector< ofPtr<ofParameter<float> > > centerX;
    std::vector< ofPtr<ofParameter<float> > > centerY;
    std::vector< ofPtr<ofParameter<float> > > BarrelPowers;
    
    
    ofParameter<bool>enableShader;
    ofxAutoReloadedShader blendShader;
        std::vector< ofPtr<ofxCvColorImage > > cvImages;
    
//    std::vector< ofPtr<ofTexture > > textures;
    
    int nextCamera;
    ofFbo fbo;
};

#endif /* defined(__wlbankinteractive__VideoGrabberManager__) */
