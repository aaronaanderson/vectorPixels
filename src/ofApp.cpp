#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	
	
	pixelShader.load(ofToDataPath("vectorPixel"));

	glGenBuffers(1, &buf);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, buf);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(Magnet) * NUM_MAGNETS, magnets, GL_DYNAMIC_COPY);
 
	//glBufferData()
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, buf);
	

	

	//---------GUI----------------------------
	gui.setup();
	gui.add(maxSpeed.setup("Max Speed", 5, 1, 300));
	gui.add(maxCycles.setup("Max Cycles", 500, 3, 2000));
	gui.add(resolution.setup("Resolution", 0.25, 0.01, 1.0));
	for (int i = 0; i < NUM_MAGNETS; i++) {
		gui.add(colors[i].setup("color " + ofToString(i), ofColor(ofRandom(255), ofRandom(255), ofRandom(255)), ofColor(0, 0), ofColor(255, 255)));
		gui.add(sizes[i].setup("size " + ofToString(i), ofRandom(2, 10), 0.1, 50.0));
	}

	reset();
	pixelShader.setUniform1i("maxIterations", maxCycles);
	pixelShader.setUniform1f("maxSpeed", maxSpeed);
}

//--------------------------------------------------------------
void ofApp::update(){
	updateMagnets();
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(255);
	renderFBOSmall.begin();
	ofSetColor(255);

	pixelShader.begin();
	pixelShader.setUniform1i("maxIterations", maxCycles);
	pixelShader.setUniform1f("maxSpeed", maxSpeed);

	ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
	pixelShader.end();
	//ofSaveFrame();
	renderFBOSmall.end();

	renderFBOSmall.draw(0, 0, ofGetWidth(), ofGetHeight());
	ofSaveFrame();
	//for (int i = 0; i < NUM_MAGNETS; i++) {
	//	
	//	ofSetColor(ofColor(magnets[i].color.r * 255, magnets[i].color.g * 255, magnets[i].color.g * 255));
	//	ofDrawCircle(magnets[i].location.x,ofGetHeight() - magnets[i].location.y, magnets[i].size);
	//}
	gui.draw();
	//send stuff to the shader
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, buf);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(Magnet) * NUM_MAGNETS, magnets, GL_DYNAMIC_COPY);
}

//--------------------------------------------------------------
void ofApp::reset() {
	for (int i = 0; i < NUM_MAGNETS; i++) {
		magnets[i].location = glm::vec2(ofRandom(ofGetWidth() * resolution), ofRandom(ofGetHeight() * resolution));
		magnets[i].velocity = glm::vec2(ofRandom(-1.0, 1.0), ofRandom(-1.0, 1.0));
		magnets[i].color = glm::vec3(ofRandom(1.0), ofRandom(1.0), ofRandom(1.0));
		magnets[i].size = ofRandom(2, 8);
	}

	renderFBOSmall.allocate(int(ofGetWidth() * resolution), int(ofGetHeight() * resolution), GL_RGBA);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, buf);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(Magnet) * NUM_MAGNETS, magnets, GL_DYNAMIC_COPY);

}
//--------------------------------------------------------------
void ofApp::updateMagnets() {
	for (int i = 0; i < NUM_MAGNETS; i++) {
		magnets[i].location += magnets[i].velocity;
		if (magnets[i].location.x < 0 || magnets[i].location.x > ofGetWidth()) {
			magnets[i].velocity.x *= -1;
		}
		if (magnets[i].location.y < 0 || magnets[i].location.y > ofGetHeight()) {
			magnets[i].velocity.y *= -1;
		}

		ofColor temp = colors[i];
		magnets[i].color = glm::vec3(temp.r/255.0f , temp.g/255.0f , temp.b/255.0f );
		magnets[i].size = sizes[i];
	}

	

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, buf);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(Magnet) * NUM_MAGNETS, magnets, GL_DYNAMIC_COPY);
}

//-------------------------------------------------------------
void ofApp::windowResized(int w, int h) {
	renderFBOSmall.allocate(int(w * resolution),int( h * resolution), GL_RGBA);

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	if (button == 0) {
		reset();
	}
	else if (button == 1) {
		for (int i = 0; i < NUM_MAGNETS; i++) {
			magnets[i].location = glm::vec2(ofRandom(ofGetWidth()), ofRandom(ofGetHeight()));
		}
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, buf);
		glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(Magnet) * NUM_MAGNETS, magnets, GL_DYNAMIC_COPY);
	}

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

