#pragma once

#include "ofMain.h"
#include "ofxGui.h"

#define NUM_MAGNETS 3

struct Magnet {
	glm::vec2 location;
	glm::vec2 velocity;
	glm::vec3 color;
	float size;
};

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();


		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void reset();
		void updateMagnets();
		
private:
	Magnet magnets[NUM_MAGNETS];
	ofShader pixelShader;
	ofFbo renderFBOSmall;
	GLuint buf;

	//----------GUI
	ofxPanel gui;
	ofxFloatSlider resolution;
	ofxIntSlider maxCycles;
	ofxFloatSlider maxSpeed;
	ofxFloatSlider sizes[NUM_MAGNETS];
	ofxColorSlider colors[NUM_MAGNETS];
	
	
};
