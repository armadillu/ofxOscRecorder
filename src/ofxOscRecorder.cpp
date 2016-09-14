//
//  ofxOscRecorder.cpp
//  BaseApp
//
//  Created by Oriol Ferrer Mesià on 14/9/16.
//
//

#include "ofxOscRecorder.h"

ofxOscRecorder::ofxOscRecorder(){

}

void ofxOscRecorder::startRecording(){
	recStartTime = ofGetElapsedTimef();
	recording = ofxJSON();
	numRecMessages = 0;
}

void ofxOscRecorder::addMessage(ofxOscMessage & m){

	float t = ofGetElapsedTimef() - recStartTime;
	recording["Times"].append(t);
	recording["Packets"].append(toJson(m));
	numRecMessages++;
}


ofxJSON ofxOscRecorder::getRecordingSoFar(){
	return recording;
}


void ofxOscRecorder::playback(ofxJSON & recording, bool loop){

	loopPlayback = loop;
	int nSamples = recording["Times"].size();

	times.clear();
	for(auto & t : recording["Times"]){
		times.push_back(t.asFloat());
	}

	messages.clear();
	for(auto & m : recording["Packets"]){
		messages.push_back(toOSC(m));
	}

	playbackStartTime = ofGetElapsedTimef();
	lastPlayedIndex = 0;
	isPlayback = true;
}


void ofxOscRecorder::update(){

	if(isPlayback){
		float timeNow = ofGetElapsedTimef();
		float playbackTime = timeNow - playbackStartTime;

		bool done = false;
		int index = lastPlayedIndex;
		while(!done){

			if(times[index] < playbackTime){
				ofNotifyEvent(eventOscPlayback, messages[index]);
			}else{
				done = true; continue;
			}
			index++;
			if(index >= times.size()){
				done = true; continue;
			}
		}

		lastPlayedIndex = index;

		if(lastPlayedIndex == times.size()){
			if(loopPlayback){
				playbackStartTime = ofGetElapsedTimef();
				lastPlayedIndex = 0;
				ofLogNotice("ofxOscRecorder") << "looping!";
			}else{
				isPlayback = false;
				ofNotifyEvent(eventPlaybackFinished, *this);
			}
		}
	}
}



ofxJSON ofxOscRecorder::toJson(ofxOscMessage & m){

	ofxJSON json;
	json["address"] = m.getAddress();
	json["remoteIP"] = m.getRemoteIp();
	json["remotePort"] = m.getRemotePort();
	int numArgs = m.getNumArgs();

	ofxJSON args;
	for(int i = 0; i < numArgs; i++){
		ofxJSON arg;
		int typeInt = m.getArgType(i);
		ofxOscArgType type = ofxOscArgType(typeInt);
		arg["type"] = (int)type;
		switch (type) {
			case OFXOSC_TYPE_INT32: arg["value"] = m.getArgAsInt(i); break;
			case OFXOSC_TYPE_INT64: arg["value"] = m.getArgAsInt64(i); break;
			case OFXOSC_TYPE_FLOAT: arg["value"] = m.getArgAsFloat(i); break;
			case OFXOSC_TYPE_DOUBLE: arg["value"] = m.getArgAsDouble(i); break;
			case OFXOSC_TYPE_STRING: arg["value"] = m.getArgAsString(i); break;
			case OFXOSC_TYPE_SYMBOL: arg["value"] = m.getArgAsSymbol(i); break;
			case OFXOSC_TYPE_CHAR: arg["value"] = char(m.getArgAsChar(i)); break;
			case OFXOSC_TYPE_MIDI_MESSAGE: arg["value"] = m.getArgAsMidiMessage(i); break;
			case OFXOSC_TYPE_TRUE: arg["value"] = m.getArgAsBool(i); break;
			case OFXOSC_TYPE_FALSE: arg["value"] = m.getArgAsBool(i); break;
			case OFXOSC_TYPE_TRIGGER: arg["value"] = m.getArgAsTrigger(i); break;
			case OFXOSC_TYPE_TIMETAG: arg["value"] = m.getArgAsTimetag(i); break;
			case OFXOSC_TYPE_RGBA_COLOR: arg["value"] = m.getArgAsRgbaColor(i); break;
			default: ofLogError("ofxOscRecorder") << "type not supported!";
		}
		args.append(arg);
	}

	json["args"] = args;
	return json;
}

ofxOscMessage ofxOscRecorder::toOSC(const ofxJSON & json){

	ofxOscMessage m;

	string address = json["address"].asString();
	string remoteIP = json["remoteIP"].asString();
	int remotePort = json["remotePort"].asInt();
	m.setAddress(address);
	m.setRemoteEndpoint(remoteIP, remotePort);

	for(auto & arg : json["args"]){
		int typeInt = arg["type"].asInt();
		ofxOscArgType type =ofxOscArgType(typeInt);

		switch (type) {
			case OFXOSC_TYPE_INT32: m.addIntArg(arg["value"].asInt()); break;
			case OFXOSC_TYPE_INT64: m.addInt64Arg(arg["value"].asInt64()); break;
			case OFXOSC_TYPE_FLOAT: m.addFloatArg(arg["value"].asFloat()); break;
			case OFXOSC_TYPE_DOUBLE: m.addDoubleArg(arg["value"].asDouble()); break;
			case OFXOSC_TYPE_STRING: m.addStringArg(arg["value"].asString()); break;
			case OFXOSC_TYPE_SYMBOL: m.addSymbolArg(arg["value"].asString()); break;
			case OFXOSC_TYPE_CHAR: m.addCharArg(char(arg["value"].asInt())); break;
			case OFXOSC_TYPE_MIDI_MESSAGE: m.addIntArg(arg["value"].asInt()); break;
			case OFXOSC_TYPE_TRUE: m.addBoolArg(arg["value"].asBool()); break;
			case OFXOSC_TYPE_FALSE: m.addBoolArg(arg["value"].asBool()); break;
			case OFXOSC_TYPE_TRIGGER: m.addBoolArg(arg["value"].asBool()); break;
			case OFXOSC_TYPE_TIMETAG:  m.addInt64Arg(arg["value"].asInt64()); break;
			case OFXOSC_TYPE_RGBA_COLOR: m.addIntArg(arg["value"].asInt()); break;
			default: ofLogError("ofxOscRecorder") << "type not supported!";
		}
	}
	return m;
}


