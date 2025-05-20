// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
// Timer
#include "AdventureTimer.h"
#include <Spore/Input.h>

static UILayoutPtr Text1 = nullptr;

void Initialize()
{
	// This method is executed when the game starts, before the user interface is shown
	// Here you can do things such as:
	//  - Add new cheats
	//  - Add new simulator classes
	//  - Add new game modes
	//  - Add new space tools
	//  - Change materials

	App::AddUpdateFunction(AdventureTimer::Get());
}

member_detour(cScenarioPlayMode_Initialize_detour, Simulator::cScenarioPlayMode, void(void)) {

	void detoured()
	{
		
		// We want the original function to run first, so we can do our things afterwards.
		original_function(this);


		// Setting up text layout for timer.
		Text1 = new UTFWin::UILayout(); // Timer
		
		if (Text1->LoadByName(u"Timer") && WindowManager.GetMainWindow()->FindWindowByID(0xec2fd2c3) != nullptr) {
			Text1->SetParentWindow(WindowManager.GetMainWindow()->FindWindowByID(0xec2fd2c3));
		}
		IWindowPtr text;
		if (Text1->FindWindowByID(id("Text")) != nullptr) {
			text = Text1->FindWindowByID(id("Text"));
			text->SetLocation(10, 350);
			Math::Rectangle area = text->GetArea();
			text->SetSize(area.GetWidth(),40);
			text->SetVisible(Timer.visible);

		}

	}

};


member_detour(GameInput_OnKeyUp_GA_detour, GameInput, void(int, KeyModifiers)) {
	void detoured(int vkCode, KeyModifiers modifiers) {

		if (Simulator::IsScenarioMode() && ScenarioMode.GetMode() == App::cScenarioMode::Mode::PlayMode) {
			if (vkCode == 't' || vkCode == 'T') {
				Timer.visible = !Timer.visible;
			}
		}
		original_function(this, vkCode, modifiers);
	}
};


void Dispose()
{
	// Cleaning up our pointers.
	Text1 = nullptr;
	Timer.Dispose();
	// This method is called when the game is closing
}

void AttachDetours()
{
	cScenarioPlayMode_Initialize_detour::attach(GetAddress(Simulator::cScenarioPlayMode, Initialize));
	GameInput_OnKeyUp_GA_detour::attach(GetAddress(GameInput, OnKeyDown));

	// Call the attach() method on any detours you want to add
	// For example: cViewer_SetRenderType_detour::attach(GetAddress(cViewer, SetRenderType));
}


// Generally, you don't need to touch any code here
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		ModAPI::AddPostInitFunction(Initialize);
		ModAPI::AddDisposeFunction(Dispose);

		PrepareDetours(hModule);
		AttachDetours();
		CommitDetours();
		break;

	case DLL_PROCESS_DETACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	}
	return TRUE;
}

