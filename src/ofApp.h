#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
    ofImage img;
    ofMesh mesh;
    ofEasyCam easyCam;
    
    vector<ofVec3f> vertices, newVertices;
    vector<ofFloatColor> colors, newColors;
    
    int col;
    int row;
    int step;
    float b_threshold;
    float c_threshold;
    float z_range;
    float degree;
    bool change;
    float max_distance;
    float min_distance;
    
};
