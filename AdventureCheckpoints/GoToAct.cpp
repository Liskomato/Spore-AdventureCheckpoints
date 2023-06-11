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
	
	if (Simulator::IsScenarioMode() && ScenarioMode.GetMode() == App::cScenarioMode::Mode::PlayMode) {

			auto arg = line.GetArgumentAt(1);

			uint32_t newAct;
			auto oldAct = ScenarioMode.GetPlayMode()->mCurrentActIndex;
			auto actAmount = ScenarioMode.GetResource()->mActs.size();

			try {
				newAct = mpFormatParser->ParseInt(arg);
			}
			catch (...) {
				App::ConsolePrintF("Please input a number between 1-8.");
				return;
			}
			if (newAct <= 8 && newAct >= 1) {
				if(newAct > actAmount) {
					App::ConsolePrintF("The given act index doesn't exist in this adventure!");
				}
				else {
					SetNewAct(newAct, oldAct);
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
		return "GoToAct: Go to the act in Adventure Play Mode specified by your input. Choose a number from 1 to 8.";
	}
}

void GoToAct::SetNewAct(int newAct, int currentAct) 
{
	newAct--;
	ScenarioMode.GetPlayMode()->JumpToAct(newAct);
//	CALL(Address(ModAPI::ChooseAddress(0xf1f7b0, 0xf1f3c0)), void, Args(Simulator::cScenarioPlayMode*, int), Args(ScenarioMode.GetPlayMode(), newAct));
//	CALL(Address(0xF462B0), void, Args(Simulator::cScenarioData*, int, int, int), Args(ScenarioMode.GetData(), 2, currentAct, newAct));
//	ScenarioMode.GetPlayMode()->SetCurrentAct(newAct);
}
