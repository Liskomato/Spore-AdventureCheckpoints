#include "stdafx.h"
#include "TimerDebug.h"
#include "AdventureTimer.h"

TimerDebug::TimerDebug()
{
}


TimerDebug::~TimerDebug()
{
}


void TimerDebug::ParseLine(const ArgScript::Line& line)
{
	// This method is called when your cheat is invoked.
	// Put your cheat code here.
	auto time = AdventureTimer::Get();
	if (time != nullptr) {
		time->debugEnabled = !time->debugEnabled;
		if (time->debugEnabled) App::ConsolePrintF("Adventure Checkpoints: Debug mode enabled.");
		else App::ConsolePrintF("Adventure Checkpoints: Debug mode disabled.");
		if (!time->SaveData()) {
			App::ConsolePrintF("Adventure Checkpoints: SaveData failed! Settings file will be attempted to be created to %ls once the game is closed.", time->GetFilePath().c_str());
		}
	}
	else App::ConsolePrintF("Adventure Checkpoints: CheckpointsDebug failed!");

}

const char* TimerDebug::GetDescription(ArgScript::DescriptionMode mode) const
{
	if (mode == ArgScript::DescriptionMode::Basic) {
		return "Enables Adventure Checkpoints debug mode.";
	}
	else {
		return "CheckpointsDebug: Enables debug information to be displayed in the Adventure Play Mode. Part of the Adventure Checkpoints mod.";
	}
}
