
#include "ofApp.h"


int main()
{
    ofGLWindowSettings settings;
    settings.width = 320 * 4;
    settings.height = 240 * 4;
    ofCreateWindow(settings);
    ofRunApp(std::make_shared<ofApp>());
}
