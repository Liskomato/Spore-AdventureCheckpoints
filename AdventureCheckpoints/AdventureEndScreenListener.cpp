#include "stdafx.h"
#include "AdventureEndScreenListener.h"
#include "GoToAct.h"
#include <chrono>
#include <thread>

AdventureEndScreenListener::AdventureEndScreenListener()
{
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

		StartFromCheckpoint(ScenarioMode.GetPlayMode()->mCurrentActIndex);

	}
	/*
	if (messageID == UTFWin::kMsgWinProcAdded) {
		auto window = (const UTFWin::Message&)message;

		if (window.IsSource(WindowManager.GetMainWindow()->FindWindowByID(id("CheckpointButton")))) {
			App::ConsolePrintF("Button spawned.");
		}
	}
	*/
	// Return true if the message has been handled. Other listeners will receive the message regardless of the return value.
	return true;
}

void AdventureEndScreenListener::StartFromCheckpoint(int previousAct) {
	while (GameTimeManager.IsPaused()) {
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	};

	GoToAct::SetNewAct(previousAct, ScenarioMode.GetPlayMode()->mCurrentActIndex);
}
