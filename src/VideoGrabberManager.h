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
#include "ofxCvCoordWarpingGui.h"
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
//using ofx::Video::SharedIPVideoGrabber;

class VideoGrabberManager : public ofBaseDraws
{
public:
    void setup();
    void update();
    ~VideoGrabberManager();
    //baseDraw
    void draw(float x, float y) const
    {
        draw(x,y,VIDEO_WIDTH , VIDEO_HEIGHT);
    }
    void draw(float x, float y, float w, float h)  const;
    void draw(const ofPoint & point)  const {
        draw(point.x, point.y);
    }
    void draw(const ofRectangle & rect)  const{
        draw(rect.x, rect.y, rect.width, rect.height);
    }
    void draw(const ofPoint & point, float w, float h)  const{
        draw(point.x, point.y, w, h);
    }
    
    float getHeight() const
    {
        return fbo.getHeight();
    }
    float getWidth() const
    {
        return fbo.getWidth();
    }
    
    void setAnchorPercent(float xPct, float yPct){};
    void setAnchorPoint(float x, float y){};
    void resetAnchor(){};

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
        std::vector< ofPtr<ofxCvColorImage > > outputImages;
    
//    std::vector< ofPtr<ofTexture > > textures;
    
    int nextCamera;
    ofFbo fbo;
    vector<ofPtr<ofxCvCoordWarpingGui> >quadGuis;
    void setupQuadGui (int i, int cameraWidth, int cameraHeight );
    void drawQuadGui();
    void drawQuadGui(int );
    void drawQuadGui(int i, int x, int y, int width, int height );
    void getQuadSubImage(ofxCvColorImage* inputImage, ofxCvColorImage* outputImage, vector <ofPoint>& quad, ofImageType imageType);
    int cameraWidth;
    int cameraHeight;
    void saveQuad();
    void enableDrawQuad(bool &b);
};

#endif /* defined(__wlbankinteractive__VideoGrabberManager__) */
