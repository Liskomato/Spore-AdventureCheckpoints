#include "stdafx.h"
#include "PrintCheckpointStatus.h"
#include "AdventureTimer.h"

PrintCheckpointStatus::PrintCheckpointStatus()
{
}


PrintCheckpointStatus::~PrintCheckpointStatus()
{
}


void PrintCheckpointStatus::ParseLine(const ArgScript::Line& line)
{
	// This method is called when your cheat is invoked.
	// Put your cheat code here.

	auto time = AdventureTimer::Get();
	if (time != nullptr) {
		time->PrintStatus();
	}
	else App::ConsolePrintF("Adventure Checkpoints: CheckpointsStatus failed!");

}

const char* PrintCheckpointStatus::GetDescription(ArgScript::DescriptionMode mode) const
{
	if (mode == ArgScript::DescriptionMode::Basic) {
		return "Prints out Adventure Checkpoints prop data.";
	}
	else {
		return "CheckpointsStatus: Prints out current state of Adventure Checkpoints properties.";
	}
}
