#pragma once

#include <Spore\BasicIncludes.h>

class GoToAct 
	: public ArgScript::ICommand
{
public:
	GoToAct();
	~GoToAct();

	// Called when the cheat is invoked
	void ParseLine(const ArgScript::Line& line) override;

	
	// Returns a string containing the description. If mode != DescriptionMode::Basic, return a more elaborated description
	const char* GetDescription(ArgScript::DescriptionMode mode) const override;

	static void SetNewAct(int newAct, int currentAct);
};

