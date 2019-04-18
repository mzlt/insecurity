#include "phoneApp.hpp"


void phoneApp::setup(){
    
    // triggers
    isOn = false;
    isTalking = false;
    timer = false;
    isRinging = false;
    isMono = false;
    
    // time variables
    m = 0;
    s = 0;
    monolength = 141500;
    
    // app position
    callX = 100;
    callY = 100;
    
    // sounds
    ring.load("call.mp3");
    ring.setLoop(true);
    ring.setVolume(0.3f);
    mono.load("rant2.mp3");
    
    // fonts & images
    latoR.load("Lato-Regular.ttf", 10);
    latoB.load("Lato-Bold.ttf", 12);
    phone.load("phone.png");
    phone.resize(50, 50);
    user.load("user.png");
    user.resize(70, 70);
    
}

void phoneApp::update(){
    ofSoundUpdate();
    
}

void phoneApp::draw(){
    
    // reset timer 0.5 second before the end
    if(mono.getPositionMS() >= monolength - 1000) {
        timer = true;
        //        isOn = false;
        //        isMono = false;
        //        isTalking = false;
    }
    
    //     close app when finished talking
    if(mono.getPositionMS() >= monolength - 500) {
        isOn = false;
        isMono = false;
        isTalking = false;
    }
    
    elTime = ofGetElapsedTimeMillis();
    
    if(isOn) {
        // app box
        ofSetColor(0, 0, 0, 200);
        ofDrawRectRounded(callX, callY, 300, 200, 10);
        
        // close app button
        ofSetColor(200,200,200);
        latoB.drawString("x", callX + 280, callY + 20);
        if(hasClicked&&(ofGetMouseX()>callX + 275 && ofGetMouseX()<callX + 295)&&(ofGetMouseY()>callY+5 && ofGetMouseY()<callY+25)){
            hasClicked = false;
            ring.stop();
            mono.stop();
            ofResetElapsedTimeCounter();
//            timer = true;
            isOn = false;
            isTalking = false;
            isRinging = false;
            isMono = false;
        }
        
        // ongoing call
        if (isTalking){
            if(!isMono){
                mono.play();
                isMono = true;
            }
            // end call button active
            if((ofGetMouseX()>callX + 160 && ofGetMouseX()<callX + 260)&&(ofGetMouseY()>callY+140 && ofGetMouseY()<callY+160)){
                ofPushStyle();
                ofSetColor(150, 9, 0);
                ofDrawRectRounded(callX + 160, callY+140, 100, 20, 5);
                ofPopStyle();
                if(hasClicked){
                    hasClicked = false;
//                    timer = true;
                    ofResetElapsedTimeCounter();
                    isOn = false;
                    isTalking = false;
                    isMono = false;
                    mono.stop();
                }
            // end call button passive
            }else{
                ofPushStyle();
                ofSetColor(226,13,13);
                ofDrawRectRounded(callX + 160, callY+140, 100, 20, 5);
                ofPopStyle();
            }
            // end call button text
            ofPushStyle();
            ofSetColor(255, 255, 255);
            latoB.drawString("End Call", callX + 180, callY + 155);
            // ongoing call icon
            user.draw(callX + 40, callY + 60);
            // timer

            m = mono.getPositionMS()/60000;
            s = mono.getPositionMS() /1000;
            latoB.drawString("Unknown Number", callX + 140, callY + 85);
            if(s % 60 < 10){
                latoR.drawString("00:0"+ofToString(m)+":0"+ofToString(s%60), callX + 140, callY + 110);
            } else {
                latoR.drawString("00:0"+ofToString(m)+":"+ofToString(s%60), callX + 140, callY + 110);
            }
            ofPopStyle();

        // phone ringing
        } else {
            if(!isRinging){
                ring.play();
                isRinging = true;
            }
            // answear button active
            if((ofGetMouseX()>callX + 40 && ofGetMouseX()<callX + 140)&&(ofGetMouseY()>callY+150 && ofGetMouseY()<callY+170)){
                ofPushStyle();
                ofSetColor(37,106,5);
                ofDrawRectRounded(callX + 40, callY+150, 100, 20, 5);
                ofPopStyle();
                if(hasClicked){
                    hasClicked = false;
                    isTalking = true;
                    isRinging = false;
                    ring.stop();
                }
            // answear button passive
            }else{
                ofPushStyle();
                ofSetColor(62,179,8);
                ofDrawRectRounded(callX + 40, callY+150, 100, 20, 5);
                ofPopStyle();
            }
            // decline button active
            if((ofGetMouseX()>callX + 160 && ofGetMouseX()<callX + 260)&&(ofGetMouseY()>callY+150 && ofGetMouseY()<callY+170)){
                ofPushStyle();
                ofSetColor(150, 9, 0);
                ofDrawRectRounded(callX + 160, callY+150, 100, 20, 5);
                ofPopStyle();
                if(hasClicked){
                    hasClicked = false;
                    isOn = false;
                    ofResetElapsedTimeCounter();
//                    timer = true;
                    isRinging = false;
                    ring.stop();
                }
            // decline button passive
            }else{
                ofPushStyle();
                ofSetColor(226,13,13);
                ofDrawRectRounded(callX + 160, callY+150, 100, 20, 5);
                ofPopStyle();
            }
            ofPushStyle();
            ofSetColor(255, 255, 255);
            // app text
            latoB.drawString("INCOMING CALL", callX + 125, callY + 75);
            latoR.drawString("Unknown Number", callX + 125, callY + 100);
            // buttons text
            latoB.drawString("Answer", callX + 60, callY + 165);
            latoB.drawString("Decline", callX + 180, callY + 165);
            // ringing icon
            phone.draw(callX + 40, callY + 60);
            ofPopStyle();
        }
    }
    
    // reset timer
    if (timer){
        ofResetElapsedTimeCounter();
        timer = false;
    }
}

//--------------------------------------------------------------
void phoneApp::mousePressedSecondary(int x, int y, int button){
    
    hasClicked = true;
    
}









