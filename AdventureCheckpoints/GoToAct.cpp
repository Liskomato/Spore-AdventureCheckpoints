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

			int newAct;
			auto oldAct = ScenarioMode.GetPlayMode()->mCurrentActIndex;

			try {
				newAct = mpFormatParser->ParseInt(arg);
			}
			catch (...) {
				App::ConsolePrintF("Please input a number between 1-8.");
				return;
			}
			if (newAct <= 8 && newAct >= 1) {
			//	if() {
				SetNewAct(newAct, oldAct);
			//	}
			//	else {
			//		App::ConsolePrintF("The given act index doesn't exist in this adventure!");
			//	}
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
	CALL(Address(0xF462B0), void, Args(Simulator::cScenarioData*, int, int, int), Args(ScenarioMode.GetData(), 2, currentAct, newAct));
	ScenarioMode.GetPlayMode()->SetCurrentAct(newAct);
}
