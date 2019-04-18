#include "ofApp.h"
#include "ofxPS3EyeGrabber.h"

//--------------------------------------------------------------
void ofApp::setup() {
    
    // LOAD CAMERAS
    ofSetLogLevel(OF_LOG_VERBOSE);
    ofSetFrameRate(200);
    ofSetVerticalSync(false);
    loadCameras();
    
    // START CAMERAS CONNECTION
    for (std::size_t i = 0; i < NUM_CAMERAS; i++)
    {
        IPCameraDef& cam = getNextCamera();
        auto grabber = std::make_shared<Video::IPVideoGrabber>();
        if (!cam.getUsername().empty() || !cam.getPassword().empty())
        {
            grabber->setUsername(cam.getUsername());
            grabber->setPassword(cam.getPassword());
        }
        grabber->setCameraName(cam.getName());
        grabber->setURI(cam.getURL());
        grabber->connect(); // connect immediately
        grabbers.push_back(grabber);
    }
    
//    // WEBCAM
    cam.setGrabber(std::make_shared<ofxPS3EyeGrabber>());
    cam.setup(640, 480);
    
//     mac inbuilt cam:
//    camWidth = 640;
//    camHeight = 480;
//    cam.initGrabber(camWidth/2, camHeight/2);
    
    //TEXTURES
    for (std::size_t i = 0; i < NUM_CAMERAS; i++){
        ofPixels transition;
        ofTexture texture;
        transitions.push_back(transition);
        textures.push_back(texture);
        transitions[i].allocate(320, 240, OF_PIXELS_RGB);
        textures[i].allocate(transitions[i]);
    }
    
    
    // SET UP CCV
    ccv.setupFace("face.sqlite3");
    
    // PHONE APP SETUP
    phoneA.setup();
    
    // FONTS
    droid.load("DroidSansMono.ttf", 20);
    
    ofBackground(0,0,0);
    
    w = ofGetWidth() / NUM_COLS;
    h = ofGetHeight() / NUM_ROWS;

}

//--------------------------------------------------------------
IPCameraDef& ofApp::getNextCamera()
{
    nextCamera = (nextCamera + 1) % ipcams.size();
    return ipcams[nextCamera];
}

//--------------------------------------------------------------
void ofApp::loadCameras()
{
    // CODE FROM THE IP CAMERA ADDON
    ofxXmlSettings XML;
    if (XML.loadFile("streams.xml")) {
        XML.pushTag("streams");
        std::string tag = "stream";
        std::size_t nCams = static_cast<std::size_t>(XML.getNumTags(tag));
        for (std::size_t n = 0; n < nCams; ++n) {
            std::string username = XML.getAttribute(tag, "username", "", n);
            std::string password = XML.getAttribute(tag, "password", "", n);
            std::string auth = XML.getAttribute(tag, "auth-type", "NONE", n);
            IPCameraDef::AuthType authType = IPCameraDef::AuthType::NONE;
            if (auth.compare("NONE") == 0) {
                authType = IPCameraDef::AuthType::NONE;
            } else if (auth.compare("BASIC") == 0) {
                authType = IPCameraDef::AuthType::BASIC;
            } else if (auth.compare("COOKIE") == 0) {
                authType = IPCameraDef::AuthType::COOKIE;
            }
            IPCameraDef def(XML.getAttribute(tag, "name", "", n),
                            XML.getAttribute(tag, "url", "", n),
                            username,
                            password,
                            authType);
            ipcams.push_back(def);
        }
        XML.popTag();
    }
    nextCamera = ipcams.size();
}

//--------------------------------------------------------------
void ofApp::update(){
    

    
    // UPDATE CAMERAS
    for (std::size_t i = 0; i < grabbers.size(); ++i) {
        grabbers[i]->update();
        if (grabbers[i]->isFrameNew()) {
            ofPixels & pixels = grabbers[i]->getPixels();
            for (int y=0; y<grabbers[i]->getHeight(); y++) {
                for (int x=0; x<grabbers[i]->getWidth(); x++) {
                    // NORMAL
                    transitions[i][(grabbers[i]->getWidth()*y+x)*3] = pixels[(grabbers[i]->getWidth()*y+x)* 3];
                    transitions[i][(grabbers[i]->getWidth()*y+x)*3 + 1] = pixels[(grabbers[i]->getWidth()*y+x)* 3 + 1];
                    transitions[i][(grabbers[i]->getWidth()*y+x)*3 + 2] = pixels[(grabbers[i]->getWidth()*y+x)* 3 + 2];
                    
                    
                    if (phoneA.isMono) {
                        
                        int level = ofMap(phoneA.mono.getPositionMS(),0,phoneA.monolength,0,100);
                        int offset = pow(ofMap(phoneA.mono.getPositionMS(),0,phoneA.monolength,0,5),2);
                        
                        // distortion
                        int off = x + (320 - offset) + offset * ofNoise(ofMap(y, 0, grabbers[i]->getHeight(), 0, level), frame );
                        if(off>=320) off -= 320;
                        
                        // channel shift
                        int offY = y + (240 - level) + ((ofNoise(frame)+1)/2) * level;
                        if(offY>=240) offY -= 240;
                        int offX = off + (320 - level) + ((ofNoise(frame)+1)/2) * level;
                        if(offX>=320) offX -= 320;
                        
                        // final glitch
                        int off1 = x;
                        if (phoneA.mono.getPositionMS() >= phoneA.monolength - 10000) off1 = x + (50 * ofNoise(ofMap(y, 0, grabbers[i]->getHeight(), 0,10)));
                        
                        /// pixel operations
                        transitions[i][(grabbers[i]->getWidth()* offY + off)*3] = transitions[i][(grabbers[i]->getWidth()* y + off1)*3];
                        transitions[i][(grabbers[i]->getWidth()* y + off)*3 + 1] = transitions[i][(grabbers[i]->getWidth()* y + off1)*3 + 1];
                        transitions[i][(grabbers[i]->getWidth()* y + offX)*3 + 2] = transitions[i][(grabbers[i]->getWidth()* y + off1)*3 + 2];
                    
                    }
                }
            }
            textures[i].loadData(transitions[i]);
        }
    }
    
    
    
    // UPDATE WEBCAM
    cam.update();
    
    // UPDATE CCV
    if(cam.isFrameNew()) {
        results = ccv.classifyFace(cam);
    }
    
    // CHECK IF WE HAVE A PERSON IN THE CHAIR
    for(int i = 0; i < results.size(); i++) {
        if(results[i].width > 110 && phoneA.isOn == false && phoneA.elTime > 15000) {
            cout << "Showing as person is in chair: " << phoneA.elTime << endl;
            phoneA.isOn = true;
        }
    }
    
    // UPDATE PHONE
    phoneA.update();
    
    frame++;
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    int row = 0;
    int col = 0;
    int x = 0;
    int y = 0;
    
    // DRAW TEXTURES + TIMESTAMPS
    for (std::size_t i = 0; i < grabbers.size(); ++i){
        x = col * w;
        y = row * h;
        row = (row + 1) % NUM_ROWS;
        if (row == 0) col = (col + 1) % NUM_COLS;
        ofPushMatrix();
        ofTranslate(x,y);
        textures[i].draw(0,30,w,h);
        ofSetColor(0,0,0,150);
        ofDrawRectangle(25, 45, 90, 30);
        ofDrawRectangle(155, h - 20, 315, 30);
        ofSetColor(255,255,255,255);
        droid.drawString("CAM_" + ofToString(i + 1), 30, 70);
        droid.drawString(ofGetTimestampString("%Y-%m-%d-%H-%M-%S"), 160, h + 5);
        ofPopMatrix();
        }

    // DRAW PHONE APP
    phoneA.draw();
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    phoneA.mousePressedSecondary(x, y, button);
}
