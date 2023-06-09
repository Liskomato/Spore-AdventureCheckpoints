#pragma once

#include <Spore\BasicIncludes.h>
#include <Spore\Simulator\cScenarioPlayMode.h>

#define AdventureEndScreenListenerPtr intrusive_ptr<AdventureEndScreenListener>

class AdventureEndScreenListener 
	: public App::IUnmanagedMessageListener
	, public DefaultRefCounted
{
public:
	static const uint32_t TYPE = id("AdventureEndScreenListener");

	AdventureEndScreenListener();
	~AdventureEndScreenListener();

//	int AddRef() override;
//	int Release() override;
	
	// This is the function you have to implement, called when a message you registered to is sent.
	bool HandleMessage(uint32_t messageID, void* message) override;
	void StartFromCheckpoint(int previousAct);
	bool IsCheckpointActivated();
	int GetStoredAdventureIndex();
	Clock RestoreTime();
	Simulator::cScenarioPlaySummary RestoreSummary();
private:
	bool checkpointEnabled;
	int storedActIndex;
	Clock storedTime;
	Simulator::cScenarioPlaySummary storedSummary;
};
