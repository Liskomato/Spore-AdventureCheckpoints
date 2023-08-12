// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
// Cheat includes
#include "GoToAct.h"
#include "TimerDebug.h"
#include "CheckpointsExtended.h"
#include "PrintCheckpointStatus.h"
// Timer
#include "AdventureTimer.h"
// Listeners
#include "AdventureEndScreenListener.h"
#include "ContinueCheckpointButton.h"
#include "TimerListener.h"


AdventureEndScreenListenerPtr screenListener = nullptr;



void Initialize()
{
	// This method is executed when the game starts, before the user interface is shown
	// Here you can do things such as:
	//  - Add new cheats
	//  - Add new simulator classes
	//  - Add new game modes
	//  - Add new space tools
	//  - Change materials

	// Cheats
	CheatManager.AddCheat("GoToAct", new GoToAct());
	CheatManager.AddCheat("CheckpointsDebug", new TimerDebug());
	CheatManager.AddCheat("CheckpointsExtended", new CheckpointsExtended());
	CheatManager.AddCheat("CheckpointsStatus", new PrintCheckpointStatus());

	// "Static" pointers
	screenListener = new AdventureEndScreenListener();
	timer = AdventureTimer::Initialize();

	// Listeners
	MessageManager.AddUnmanagedListener(screenListener.get(), id("StartCheckpointProc"));
	MessageManager.AddUnmanagedListener(screenListener.get(), id("EndCheckpointProc"));
//	MessageManager.AddUnmanagedListener(screenListener.get(), id("TimeRestored"));

	// Update functions
	App::AddUpdateFunction(timer.get());

	// Check base address (Mod research)
//	App::ConsolePrintF("Base address: 0x%x",baseAddress);
}

member_detour(UILayoutLoad_detour, UILayout, bool(const ResourceKey&, bool, uint32_t)) 
{

	bool detoured(const ResourceKey & resourceKey, bool boolean, uint32_t parameter) {

		// Original function gets called first.
		bool func = original_function(this, resourceKey, boolean, parameter);
		
		// Checkpoint buttons are loaded with this instance ID.
		if (resourceKey.instanceID == 0x3098258b) {

			Button = new UTFWin::UILayout();
			Button2 = new UTFWin::UILayout();
			// Load the buttons and set them on each variant of the reward screen (if we're able).
			if (Button->LoadByName(u"CheckpointButtonUI"))
			Button->SetParentWindow(this->FindWindowByID(0x07C796D0));

			if (Button2->LoadByName(u"CheckpointButtonUI"))
			Button2->SetParentWindow(this->FindWindowByID(0x07C79820));

			IWindowPtr winBtn, parent;
			if (Button->FindWindowByID(id("CheckpointButton")) != nullptr) {

				winBtn = Button->FindWindowByID(id("CheckpointButton"));
				parent = winBtn->GetParent();
				parent->BringToFront(winBtn.get());
				parent->BringToFront(parent->FindWindowByID(0x07C79940,false));
				winBtn->SetLocation(168, 520);
				winBtn->FindWindowByID(0x07C79940)->AddWinProc(new ContinueCheckpointButton());
				winBtn->SetVisible(false);

			}
			if (Button2->FindWindowByID(id("CheckpointButton")) != nullptr) {
				winBtn = Button2->FindWindowByID(id("CheckpointButton"));
				parent = winBtn->GetParent();
				parent->BringToFront(winBtn.get());
				parent->BringToFront(parent->FindWindowByID(0x07C79940, false));
				winBtn->SetLocation(168, 206);
				winBtn->FindWindowByID(0x07C79940)->AddWinProc(new ContinueCheckpointButton());
				winBtn->SetVisible(false);
			}

		}
		// Text layout for the timer + death counter (can also house other debug information due to its nature)
		/*
		if (resourceKey.instanceID == 0xf8d70d51) {
			Text1 = new UILayout();
			if (Text1->LoadByName(u"Timer")) {
				Text1->SetParentWindow(this->FindWindowByID(0x055EBC68));
			}
			IWindowPtr text;
			if (Text1->FindWindowByID(id("Text")) != nullptr) {
				text = Text1->FindWindowByID(id("Text"));
				text->SetLocation(10, 350);
			}

		}
		*/
		return func;
	}

};

member_detour(cScenarioPlayMode_Initialize_detour, Simulator::cScenarioPlayMode, void(void)) {

	void detoured()
	{
		// We want the original function to run first, so we can do our things afterwards.
		original_function(this);

		// Check if the checkpoint button was pressed.
		if (screenListener->IsCheckpointActivated())
		{
			// Restore playtime, summary, and furthest act reached from the previous adventure run, if "Continue from Previous Act" button was pressed.

			screenListener->RestoreSummary();
			
			ScenarioMode.GetPlayMode()->mCurrentTimeMS = screenListener->RestoreTime();

			int lastAct = screenListener->GetStoredAdventureIndex();

			// Check if we need to go ahead in acts.
			if (lastAct != 0) {

				// field_90 needs to be set to 3, otherwise JumpToAct will fail.
				ScenarioMode.GetPlayMode()->field_90 = 3;

				// Skip to the act we recorded. Because this function adds duplicate information to the summary that we may not need, we restore the summary yet again.
				ScenarioMode.GetPlayMode()->JumpToAct(lastAct);
				screenListener->RestoreSummary();
			}

		}
		// Send a message afterwards to the listener to clean up.	
		MessageManager.MessageSend(id("EndCheckpointProc"), nullptr);

		//	MessageManager.MessageSend(id("TimeRestored"), nullptr);
		
		// Setting up text layout for timer + death counter.
		Text1 = new UILayout(); // Timer + Death counter
		Text2 = new UILayout(); // Debug information, visibility controlled by CheckpointDebug cheat.
		if (Text1->LoadByName(u"Timer") && WindowManager.GetMainWindow()->FindWindowByID(0xec2fd2c3) != nullptr) {
			Text1->SetParentWindow(WindowManager.GetMainWindow()->FindWindowByID(0xec2fd2c3));
		}
		IWindowPtr text;
		if (Text1->FindWindowByID(id("Text")) != nullptr) {
			text = Text1->FindWindowByID(id("Text"));
			text->SetLocation(10, 350);
			if (timer != nullptr) {
				timer->InitializeListener();
				WindowManager.GetMainWindow()->AddWinProc(timer->listener.get());
				text->SetVisible(timer->visible);
			}
		}

		// Debug text layout
		if (Text2->LoadByName(u"TimerDebug")) {
			Text2->SetParentWindow(WindowManager.GetMainWindow());
			if (Text2->FindWindowByID(id("TextDebug")) != nullptr) {
				text = Text2->FindWindowByID(id("TextDebug"));
				text->SetLocation(10, 400);
				text->SetVisible(timer->debugEnabled);
			}
		}


	}

};


void Dispose()
{
	// This method is called when the game is closing
	// 
	// UILayout pointers
	Button = nullptr;
	Button2 = nullptr;
	Text1 = nullptr;
	Text2 = nullptr;

	// Message listener
	screenListener = nullptr;

	// Timer object
	timer->Dispose();
	timer = nullptr;
}

void AttachDetours()
{

	cScenarioPlayMode_Initialize_detour::attach(GetAddress(Simulator::cScenarioPlayMode, Initialize));
	UILayoutLoad_detour::attach(GetAddress(UTFWin::UILayout,Load));

	/// Unused detours
	//	ScenarioRewardScreen_detour::attach(Address(ModAPI::ChooseAddress(0xf18c40,0xf18850)));
	//	HandleUIMessage_Detour::attach(GetAddress(UTFWin::IWinProc, HandleUIMessage));


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

