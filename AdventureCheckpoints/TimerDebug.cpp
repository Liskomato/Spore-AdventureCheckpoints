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
		if (time->debugEnabled) App::ConsolePrintF("Adventure Checkpoints debug mode enabled.");
		else App::ConsolePrintF("Adventure Checkpoints debug mode disabled.");
	}
	else App::ConsolePrintF("Adventure Checkpoints: CheckpointDebug failed!");

}

const char* TimerDebug::GetDescription(ArgScript::DescriptionMode mode) const
{
	if (mode == ArgScript::DescriptionMode::Basic) {
		return "Enables Adventure Checkpoints debug mode.";
	}
	else {
		return "CheckpointDebug: Enables debug information to be displayed in the Adventure Play Mode. Part of the Adventure Checkpoints mod.";
	}
}
