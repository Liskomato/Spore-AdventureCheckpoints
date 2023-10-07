#include "stdafx.h"
#include "CheckpointsExtended.h"
#include "AdventureTimer.h"

CheckpointsExtended::CheckpointsExtended()
{
}


CheckpointsExtended::~CheckpointsExtended()
{
}


void CheckpointsExtended::ParseLine(const ArgScript::Line& line)
{
	// This method is called when your cheat is invoked.
	// Put your cheat code here.
	auto time = AdventureTimer::Get();
	if (time != nullptr) {
		time->checkpointsExtended = !time->checkpointsExtended;
		if (time->checkpointsExtended) App::ConsolePrintF("Adventure Checkpoints: Checkpoints now appear every time the adventure ends.");
		else App::ConsolePrintF("Adventure Checkpoints: Checkpoints now only appear after failing the adventure past act 1.");
		if (!time->SaveData()) {
			App::ConsolePrintF("Adventure Checkpoints: SaveData failed! Settings file will be attempted to be created to %ls once the game is closed.", time->GetFilePath().c_str());
		}
	}
	else App::ConsolePrintF("Adventure Checkpoints: CheckpointsExtended failed!");
}

const char* CheckpointsExtended::GetDescription(ArgScript::DescriptionMode mode) const
{
	if (mode == ArgScript::DescriptionMode::Basic) {
		return "Enables old behaviour for Adventure Checkpoints.";
	}
	else {
		return "CheckpointsExtended: Enables the Adventure Checkpoints to be visible in act 1 and while successfully clearing an adventure.";
	}
}
