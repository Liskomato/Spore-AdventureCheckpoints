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
	storedTime = 0;
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
		storedTime = ScenarioMode.GetPlayMode()->mCurrentTimeMS;

	}
	else if (messageID == id("EndCheckpointProc"))
	{
		checkpointEnabled = false;
		storedActIndex = 0;
		storedSummary = Simulator::cScenarioPlaySummary();
		storedTime = 0;
		
	}
	/*	Older attempt
	else if (messageID == id("TimeRestored"))
	{
		checkpointEnabled = false;
		storedTime = 0;

	}
	*/
	// Return true if the message has been handled. Other listeners will receive the message regardless of the return value.
	return true;
}

void AdventureEndScreenListener::StartFromCheckpoint(int previousAct) {
	
	ScenarioMode.GetPlayMode()->JumpToAct(previousAct-1);
}

bool AdventureEndScreenListener::IsCheckpointActivated() {
	return checkpointEnabled;
}

int AdventureEndScreenListener::GetStoredAdventureIndex() {
	return storedActIndex;
}

int AdventureEndScreenListener::RestoreTime() {
	return storedTime;
}

Simulator::cScenarioPlaySummary AdventureEndScreenListener::RestoreSummary() {
	return storedSummary;
}
/*
void AdventureEndScreenListener::SetTime(int time) {
	storedTime = time;
}
*/
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