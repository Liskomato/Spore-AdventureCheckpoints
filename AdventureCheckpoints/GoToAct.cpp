#include "stdafx.h"
#include "GoToAct.h"
#include "Spore\App\ScenarioMode.h"

GoToAct::GoToAct()
{
}


GoToAct::~GoToAct()
{
}


void GoToAct::ParseLine(const ArgScript::Line& line)
{
	// This method is called when your cheat is invoked.
	// Put your cheat code here.
	
	// Making sure we're first in the correct game mode.
	if (Simulator::IsScenarioMode() && ScenarioMode.GetMode() == App::cScenarioMode::Mode::PlayMode) {
			// Get the argument.
			auto arg = line.GetArgumentAt(1);

			// Retrieve the relevant data from the adventure.
			uint32_t newAct;
			auto oldAct = ScenarioMode.GetPlayMode()->mCurrentActIndex;
			auto actAmount = ScenarioMode.GetResource()->mActs.size();

			// Trying to parse the argument with the cheat.
			try {
				newAct = mpFormatParser->ParseInt(arg);
			}
			catch (...) {
				App::ConsolePrintF("Please input a number between 1-8.");
				return;
			}
			// Making sure the given index is within range and that it exists within adventure data.
			if (newAct <= 8 && newAct >= 1) {
				if(newAct > actAmount) {
					App::ConsolePrintF("The given act index doesn't exist in this adventure!");
				}
				else {
					SetNewAct(newAct, oldAct); // If everything goes right, move to the next part.
				}
				return;
			}
			else {
				App::ConsolePrintF("Please input a number between 1-8.");
				return;
			}

		
	}
	App::ConsolePrintF("You have to be in adventure play mode to use this cheat.");
	return;
	

}

const char* GoToAct::GetDescription(ArgScript::DescriptionMode mode) const
{
	if (mode == ArgScript::DescriptionMode::Basic) {
		return "Jump to an adventure act of your choosing.";
	}
	else {
		return "GoToAct: Go to the act in Adventure Play Mode specified by your input. Choose a number from 1 to 8. \nWARNING!\nGoing forward in acts will add a penalty time of 30 minutes per act skipped to the clock!";
	}
}

void GoToAct::SetNewAct(int newAct, int currentAct) 
{
	// Jump to the act with the designated index.
	newAct--;
	ScenarioMode.GetPlayMode()->JumpToAct(newAct);
	
	// Penalty for using the cheat if going forward in acts: Added time to clock.
	if (newAct > currentAct) {
		int penalty = 1800000, totalPenalty = 0, i = currentAct;
		while (i < newAct) {
			ScenarioMode.GetPlayMode()->mCurrentTimeMS += penalty;
			totalPenalty += penalty;
			i++;
		}
		App::ConsolePrintF("Added %d seconds to the adventure playtime.",totalPenalty/1000);
	}

	/// Old methods used by the cheat.
//	CALL(Address(ModAPI::ChooseAddress(0xf1f7b0, 0xf1f3c0)), void, Args(Simulator::cScenarioPlayMode*, int), Args(ScenarioMode.GetPlayMode(), newAct));
//	CALL(Address(0xF462B0), void, Args(Simulator::cScenarioData*, int, int, int), Args(ScenarioMode.GetData(), 2, currentAct, newAct));
//	ScenarioMode.GetPlayMode()->SetCurrentAct(newAct);
}
