#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(0);
    img.load("image.png"); // load reference image (64 words)
    mesh.setMode(OF_PRIMITIVE_LINES); // set mesh mode

    
    col = 0; //word index, divide by 64 pixels
    row = 0; //word index, divide by 64 pixels
    b_threshold = 0.4; //threshold of brightness
    c_threshold = 0.2; //threshold of chance
    step = 16; // scale the point
    z_range = 200; // z value
    degree = 0; // initial rotation
    change = false;
    max_distance = 80; //the threshold of max distance
    min_distance = 48; //the threshold of min distance
    
    mesh.clear();
    
    //iterate every pixel of a reference word to generate vertices and colors
    for (int j = row; j < row+64; j++) {
        int _y = (j - row);
        for (int i = col; i < col+64; i++) {
            int _x = (i - col);
            ofFloatColor c = img.getColor(i, j); //pick color of the pixel
            float brightness = c.getBrightness(); //get the brightness of the pixel
            float chance = ofRandom(1); //instanciate a chance
            float z = ofRandom(-1, 1)*z_range; // randomize a z value in a range
            ofVec3f pos(_x*step, _y*step, z); // create a vector
            
            if(brightness > b_threshold && chance > c_threshold){ // define it's color
                vertices.push_back(pos);
                colors.push_back(c);
            } else {
                vertices.push_back(pos);
                colors.push_back(ofFloatColor(0, 0, 0, 0));
            }
        }
    }
    
    mesh.addVertices(vertices); // define the mesh vertices
    mesh.addColors(colors); //define the mesh colors
    
    newVertices = vertices; //new vertices are for position lerping
    newColors = colors; //new colors are for color lerping
    
    // define the line connection according to the threshold of distance
    int numVerts = mesh.getNumVertices();
    
    //compare all vertices to define mesh index
    for (int i = 0; i < numVerts - 1; i++) {
        ofVec3f vert_a = mesh.getVertex(i);
        ofFloatColor color_a = mesh.getColor(i);
        float brightness_a = color_a.getBrightness();
        
        for (int j = i+1; j < numVerts; j++) {
            ofVec3f vert_b = mesh.getVertex(j);
            ofFloatColor color_b = mesh.getColor(j);
            float brightness_b = color_b.getBrightness();

            float distance = vert_a.distance(vert_b); //calculate distance
            //the rule to connect two points
            if (brightness_a > b_threshold && brightness_b > b_threshold && distance < max_distance && distance > min_distance) {
                
                mesh.addIndex(i);
                mesh.addIndex(j);
            }
        }
    }

    
    
    
    

}

//--------------------------------------------------------------
void ofApp::update(){
    //show framerate
    std::stringstream strm;
    strm << "FPS:" << ofGetFrameRate();
    ofSetWindowTitle(strm.str());
    
    degree += 0.05; // for the global rotation
    
    // set an opportunity to update the new vertices, new colors and new refrence of word
    float feed = ofRandom(1);
    if (feed > 0.8 || change == true) {
        z_range = ofRandom(50, 250); //update the z_range
        
        for (int j = row; j < row+64; j++) {
            int _y = (j - row);
            for (int i = col; i < col+64; i++) {
                int _x = (i - col);
                ofFloatColor c = img.getColor(i, j);
                float brightness = c.getBrightness();
                float chance = ofRandom(1);
                float _z = ofRandom(-1, 1)*z_range;
                ofVec3f pos(_x*step, _y*step, _z);
                if(brightness > b_threshold && chance > c_threshold){
                    newVertices[_x+_y*64] = pos;
                    newColors[_x+_y*64] = c;
                } else {
                    newVertices[_x+_y*64] = pos;
                    newColors[_x+_y*64] = ofFloatColor(0, 0, 0, 0);
                }
            }
        }
        mesh.clearIndices();
        int numVerts = mesh.getNumVertices();
        
        //update the indices and redefine the line connections
        for (int i = 0; i < numVerts - 1; i++) {
            ofVec3f vert_a = mesh.getVertex(i);
            ofFloatColor color_a = mesh.getColor(i);
            float brightness_a = color_a.getBrightness();
            
            for (int j = i+1; j < numVerts; j++) {
                ofVec3f vert_b = mesh.getVertex(j);
                ofFloatColor color_b = mesh.getColor(j);
                float brightness_b = color_b.getBrightness();
                
                float distance = vert_a.distance(vert_b); //calculate distance
                //the rule to connect two points
                if (brightness_a > b_threshold && brightness_b > b_threshold && distance < max_distance && distance > min_distance) {
                    
                    mesh.addIndex(i);
                    mesh.addIndex(j);
                }
            }
        }

        change = false;
        
    }
    
    // lerp the position from old vertices to new vertices
    float speed = 0.05;
    vector<ofVec3f> &verts = mesh.getVertices();
    vector<ofFloatColor> &clrs = mesh.getColors();
    for (int i = 0; i < vertices.size(); i++) {
        verts[i].x = ofLerp(verts[i].x, newVertices[i].x, speed);
        verts[i].y = ofLerp(verts[i].y, newVertices[i].y, speed);
        verts[i].z = ofLerp(verts[i].z, newVertices[i].z, speed);
        clrs[i].r = ofLerp(clrs[i].r, newColors[i].r, speed);
        clrs[i].g = ofLerp(clrs[i].g, newColors[i].g, speed);
        clrs[i].b = ofLerp(clrs[i].b, newColors[i].b, speed);
    }
    
    
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0);
    easyCam.begin();
    ofPushMatrix();
    ofRotateY(degree);
    ofTranslate(-32*step, -32*step);
    mesh.draw();
    ofPopMatrix();
    easyCam.end();
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    // change word reference
    if (key == ' ') {
        change = true;
        col = (int)ofRandom(7)*64;
        row = (int)ofRandom(7)*64;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
