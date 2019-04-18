#ifndef phoneApp_hpp
#define phoneApp_hpp

#include "ofMain.h"

class phoneApp {
public:
    void setup();
    void update();
    void draw();
    //--------------------------------------------------------------
    void mousePressedSecondary(int x, int y, int button);
    
    ofTrueTypeFont latoR;
    ofTrueTypeFont latoB;
    ofImage phone;
    ofImage user;
    ofSoundPlayer ring;
    ofSoundPlayer mono;
    
    int callX;
    int callY;
    
    bool isOn;
    bool isTalking;
    bool timer;
    bool isRinging;
    bool isMono;
    bool hasClicked;
    
    int m;
    int s;
    int monolength;
    long elTime;
    
};

#endif /* phoneApp_hpp */
