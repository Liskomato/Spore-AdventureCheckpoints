#include "stdafx.h"
#include "AdventureEndScreenListener.h"
#include "GoToAct.h"
#include <chrono>
#include <thread>

// Listener class functions

AdventureEndScreenListener::AdventureEndScreenListener()
{
	checkpointEnabled = false;
	storedActIndex = 0;
	storedSummary = Simulator::cScenarioPlaySummary();
	storedTime = Clock();
}


AdventureEndScreenListener::~AdventureEndScreenListener()
{
}

// For internal use, do not modify. 
/*
int AdventureEndScreenListener::AddRef()
{
	return DefaultRefCounted::AddRef();
}

// For internal use, do not modify.
int AdventureEndScreenListener::Release()
{
	return DefaultRefCounted::Release();
}
*/

// The method that receives the message. The first thing you should do is checking what ID sent this message...
bool AdventureEndScreenListener::HandleMessage(uint32_t messageID, void* message)
{
	if (messageID == id("StartCheckpointProc")) 
	{

		checkpointEnabled = true;
		storedActIndex = ScenarioMode.GetPlayMode()->mCurrentActIndex;
		storedSummary = ScenarioMode.GetPlayMode()->mSummary;
		storedTime = ScenarioMode.GetPlayMode()->field_98;

	}
	else if (messageID == id("EndCheckpointProc"))
	{
		checkpointEnabled = false;
		storedActIndex = 0;
		storedSummary = Simulator::cScenarioPlaySummary();
		storedTime = Clock();
		
	}
	// Return true if the message has been handled. Other listeners will receive the message regardless of the return value.
	return true;
}

void AdventureEndScreenListener::StartFromCheckpoint(int previousAct) {
	

//	GoToAct::SetNewAct(previousAct, ScenarioMode.GetPlayMode()->mCurrentActIndex);
}

bool AdventureEndScreenListener::IsCheckpointActivated() {
	return checkpointEnabled;
}

int AdventureEndScreenListener::GetStoredAdventureIndex() {
	return storedActIndex;
}

Clock AdventureEndScreenListener::RestoreTime() {
	
	ClockExt& ext = (ClockExt&)storedTime;
	
	ext.SetStartTime(ext.GetAccumulatedTime());

	return storedTime;
}

Simulator::cScenarioPlaySummary AdventureEndScreenListener::RestoreSummary() {
	return storedSummary;
}

// class ClockExt functions

LARGE_INTEGER ClockExt::GetStartTime() 
{
	return mStartTime;
}

LARGE_INTEGER ClockExt::GetAccumulatedTime()
{
	return mAccumulatedTime;
}

void ClockExt::SetStartTime(LARGE_INTEGER time) 
{
	mStartTime = time;
}

void ClockExt::SetAccumulatedTime(LARGE_INTEGER time) 
{
	mAccumulatedTime = time;
}