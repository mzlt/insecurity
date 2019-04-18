#pragma once

#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "IPVideoGrabber.h"
#include "ofxCcv.h"
#include "phoneApp.hpp"

#define NUM_CAMERAS 4
#define NUM_ROWS 2
#define NUM_COLS 2

// IP CAMERA GRABBER CODE

class IPCameraDef
{
public:
    enum AuthType {
        NONE,
        BASIC,
        COOKIE
    };
    
    IPCameraDef()
    {
    }
    
    IPCameraDef(const std::string& url): _url(url)
    {
    }
    
    IPCameraDef(const std::string& name,
                const std::string& url,
                const std::string& username,
                const std::string& password,
                const AuthType authType):
    _name(name),
    _url(url),
    _username(username),
    _password(password),
    _authType(authType)
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
    
    void setAuthType(AuthType authType) { _authType = authType; }
    AuthType getAuthType() const { return _authType; }
    
private:
    std::string _name;
    std::string _url;
    std::string _username;
    std::string _password;
    AuthType _authType = NONE;
};

// OF APP

using namespace ofx;

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    void mousePressed(int x, int y, int button);
    
    // IP CAMERA GRABBER
    std::vector<std::shared_ptr<Video::IPVideoGrabber>> grabbers;
    void loadCameras();
    IPCameraDef& getNextCamera();
    std::vector<IPCameraDef> ipcams;
    std::size_t nextCamera;
    
    // CCV
    ofxCcv ccv;
    vector<ofRectangle> results;
    
    // WEBCAM
    ofVideoGrabber cam;
    int camWidth;
    int camHeight;
    
    // TEXTURES
    vector<ofPixels> transitions;
    vector<ofTexture> textures;
    
    // HEIGHT, WIDTH AND FRAMECOUNT
    int w;
    int h;
    int frame;

    // FONTS
    ofTrueTypeFont droid;
    
    // PHONE APP
    phoneApp phoneA;
    
   
};


    
    

