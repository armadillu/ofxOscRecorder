#include "ofApp.h"


void ofApp::setup(){

	oscIN.setup(12345);
	oscOUT.setup("127.0.0.1", 6884);

	recorder.startRecording();
	recording = false;

	ofAddListener(recorder.eventOscPlayback, this, &ofApp::onOscPlayback);

}

void ofApp::onOscPlayback(ofxOscMessage&m){
	oscOUT.sendMessage(m);
}

void ofApp::update(){

	float dt = 1./60.;

	while(oscIN.hasWaitingMessages()){

		ofxOscMessage m;
		oscIN.getNextMessage(m);

		if(recording){
			recorder.addMessage(m);
		}
	}

	recorder.update();
}

void ofApp::draw(){

	string status = "Recording: " + string(recording ? "true" : "false") + " Msgs: " +
					ofToString(recorder.getNumRecordingMessages()) +
					"\nPlayback: " + string(playback? "true" : "false");

	status += "\n'r' to record, 'p' to playback;\n's' to save rec to disk, 'l' to load last saved rec.";

	ofDrawBitmapStringHighlight(status, 20, 20);
}


void ofApp::keyPressed(int key){


	if(key == 'r'){
		recording = true;
		playback = false;
		recorder.startRecording();
	}

	if(key == 'p'){
		recordingData = recorder.getRecordingSoFar();
		recording = false;
		playback = true;
		recorder.playback(recordingData, true);
	}


	if(key == 's'){ //save recording to disk
		recorder.getRecordingSoFar().save("recording.json");
	}

	if(key == 'l'){ //load recording to disk
		ofxJSON loadedRec;
		loadedRec.openLocal("recording.json");
		recording = false;
		playback = true;
		recorder.playback(loadedRec, true);
	}
}


void ofApp::keyReleased(int key){

}


void ofApp::mouseMoved(int x, int y ){

}


void ofApp::mouseDragged(int x, int y, int button){

}


void ofApp::mousePressed(int x, int y, int button){

}


void ofApp::mouseReleased(int x, int y, int button){

}


void ofApp::windowResized(int w, int h){

}


void ofApp::gotMessage(ofMessage msg){

}


void ofApp::dragEvent(ofDragInfo dragInfo){
	
}

