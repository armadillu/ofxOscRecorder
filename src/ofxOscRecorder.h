//
//  ofxOscRecorder.h
//  BaseApp
//
//  Created by Oriol Ferrer Mesià on 14/9/16.
//
//

#pragma once
#include "ofMain.h"
#include "ofxJSON.h"
#include "ofxOsc.h"

class ofxOscRecorder{

public:
	
	ofxOscRecorder();

	void startRecording(); //clears current recording
	void addMessage(ofxOscMessage & m);
	ofxJSON getRecordingSoFar();
	int getNumRecordingMessages(){return numRecMessages;}
	float getRecordingDuration(){return ofGetElapsedTimef() - recStartTime;}

	void update();

	void playback(ofxJSON & recording, bool loop);
	void stopPlayback();

	ofEvent<ofxOscMessage> eventOscPlayback;
	ofEvent<ofxOscRecorder> eventPlaybackFinished;

protected:

	ofxJSON toJson(ofxOscMessage & m);
	ofxOscMessage toOSC(const ofxJSON & json);

	float recStartTime;
	ofxJSON recording;
	int numRecMessages;

	//playback
	bool isPlayback = false;
	float playbackStartTime;
	bool loopPlayback;
	vector<float> times;
	vector<ofxOscMessage> messages;
	int lastPlayedIndex;
};

