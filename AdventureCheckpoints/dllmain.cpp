// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "ScenarioRewardScreen.h"
#include "GoToAct.h"
#include "AdventureEndScreenListener.h"
#include "ContinueCheckpointButton.h"

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
	CheatManager.AddCheat("GoToAct", new GoToAct());

	screenListener = new AdventureEndScreenListener();

	MessageManager.AddUnmanagedListener(screenListener.get(), id("StartCheckpointProc"));
	MessageManager.AddUnmanagedListener(screenListener.get(), id("EndCheckpointProc"));
}

/* 
virtual_detour(HandleUIMessage_Detour,UTFWin::IWinProc,UTFWin::IWinProc, bool(UTFWin::IWindow*, const UTFWin::Message&)) {	

	bool detoured(UTFWin::IWindow* window, const UTFWin::Message& message) 
	{ 
		
	}
};
*/
/*
member_detour(ScenarioRewardScreen_detour,ScenarioRewardScreen,void*(void*,void*)) {
	void* detoured(void* a1, void* a2) {

		auto func = original_function(this,a1,a2);

		IWindowPtr window = this->mpLayout->GetContainerWindow();

		UTFWin::UILayout button, button2;

		button.LoadByName(u"CheckpointButton");
		button.SetParentWindow(window->FindWindowByID(0x07C796D0));

		button2.LoadByName(u"CheckpointButton");
		button2.SetParentWindow(window->FindWindowByID(0x07C79820));

		button.GetContainerWindow()->AddWinProc(new ContinueCheckpointButton());
		button2.GetContainerWindow()->AddWinProc(new ContinueCheckpointButton());

		return func;
	}
};
*/
member_detour(UILayoutLoad_detour, UILayout, bool(const ResourceKey&, bool, uint32_t)) 
{

	bool detoured(const ResourceKey & resourceKey, bool boolean, uint32_t parameter) {

		bool func = original_function(this, resourceKey, boolean, parameter);
		
		if (resourceKey.instanceID == 0x3098258b) {

	//		IWindowPtr parentWindow = this->GetContainerWindow();

			/*
			IWindowPtr child1 = new Window();
			IWindowPtr child2 = new Window();

			parent1->SetControlID(0xFFFFFFFF);
			parent2->SetControlID(0xFFFFFFFF);

			parent1->SetFillColor(0x7f7f7f);
			parent2->SetFillColor(0x7f7f7f);

			parent1->SetEnabled(true);
			parent2->SetEnabled(true);

			parentWindow->FindWindowByID(0x07C796D0)->AddWindow(parent1.get());
			parentWindow->FindWindowByID(0x07C79820)->AddWindow(parent2.get());

			parent1->SetLayoutArea(Math::Rectangle(-542,-60,-392,-23));
			parent2->SetLayoutArea(Math::Rectangle(-542, -61, -392, -24));

			*/
			
			
	//		IWindowPtr parent1 = this->FindWindowByID(0x07C796D0);
	//		IWindowPtr parent2 = this->FindWindowByID(0x07C79820);
	//		/*
			Button = new UTFWin::UILayout();
			Button2 = new UTFWin::UILayout();

			if (Button->LoadByName(u"CheckpointButtonUI"))
			Button->SetParentWindow(this->FindWindowByID(0x07C796D0));

			if (Button2->LoadByName(u"CheckpointButtonUI"))
			Button2->SetParentWindow(this->FindWindowByID(0x07C79820));

			IWindowPtr winBtn;
			if (Button->FindWindowByID(id("CheckpointButton")) != nullptr) {

				winBtn = Button->FindWindowByID(id("CheckpointButton"));
				winBtn->SetLocation(-231, 520);
				winBtn->GetParent()->BringToFront(winBtn.get());
				winBtn->FindWindowByID(0x07C79940)->AddWinProc(new ContinueCheckpointButton());

			}
			if (Button2->FindWindowByID(id("CheckpointButton")) != nullptr) {
				winBtn = Button2->FindWindowByID(id("CheckpointButton"));
				winBtn->SetLocation(-231, 206);
				winBtn->GetParent()->BringToFront(winBtn.get());
				winBtn->FindWindowByID(0x07C79940)->AddWinProc(new ContinueCheckpointButton());
			}
	//		*/

	//		this->FindWindowByID(0x07C79820)->FindWindowByID(id("CheckpointButton"))->AddWinProc(new ContinueCheckpointButton());
	//		this->FindWindowByID(0x07C796D0)->FindWindowByID(id("CheckpointButton"))->AddWinProc(new ContinueCheckpointButton());

	//		this->FindWindowByID(0x07C79820)->FindWindowByID(0x07C79940)->AddWinProc(new ContinueCheckpointButton());
	//		this->FindWindowByID(0x07C796D0)->FindWindowByID(0x07C79940)->AddWinProc(new ContinueCheckpointButton());

	//		parent1->BringToFront(button.FindWindowByID(id("CheckpointButton")));
	//		parent2->BringToFront(button2.FindWindowByID(id("CheckpointButton")));


	//		parent1->SetVisible(true);
	//		parent2->SetVisible(true);

	//		parentWindow->FindWindowByID(0x07C796D0)->BringToFront(parent1.get());
	//		parentWindow->FindWindowByID(0x07C79820)->BringToFront(parent2.get());

		}

		return func;
	}

};

member_detour(cScenarioPlayMode_Initialize_detour, Simulator::cScenarioPlayMode, void(void)) {

	void detoured()
	{
		original_function(this);
		if (screenListener->IsCheckpointActivated() && screenListener->GetStoredAdventureIndex() != 0)
		{
			int lastAct = screenListener->GetStoredAdventureIndex();
			int previousAct = lastAct-1;

			//	CALL(Address(ModAPI::ChooseAddress(0xf1f7b0, 0xf1f3c0)), void, Args(Simulator::cScenarioPlayMode*, int), Args(ScenarioMode.GetPlayMode(), lastAct));
			CALL(Address(0xF462B0), void, Args(Simulator::cScenarioData*, int, int, int), Args(ScenarioMode.GetData(), 2, 0, lastAct));
			ScenarioMode.GetPlayMode()->SetCurrentAct(lastAct);

			Simulator::cScenarioAct& previousActClass = ScenarioMode.GetResource()->mActs[previousAct];
			Simulator::cScenarioGoal& goal = previousActClass.mGoals[previousActClass.mGoals.size()-1];
			
			Simulator::cScenarioMarker destination;
			eastl::vector_map<int, Simulator::cScenarioMarker>& markers = ScenarioMode.GetResource()->mMarkers;
			
			for (auto& marker : markers) 
			{
				if (marker.second.mClassIndex == goal.mTargetClassIndex)
				{
					destination = marker.second;
					break;
				}
			}
			if (destination.mPosition != Vector3(0,0,0) && destination.mOrientation != Quaternion(0,0,0,1))
			GameNounManager.GetAvatar()->Teleport(destination.mPosition,destination.mOrientation);
		}
		MessageManager.PostMSG(id("EndCheckpointProc"), nullptr);
	}

};

void Dispose()
{
	// This method is called when the game is closing
	// 
	// Button pointers
	Button = nullptr;
	Button2 = nullptr;

	// Message listener
	screenListener = nullptr;
}

void AttachDetours()
{
//	ScenarioRewardScreen_detour::attach(Address(ModAPI::ChooseAddress(0xf18c40,0xf18850)));
	cScenarioPlayMode_Initialize_detour::attach(Address(ModAPI::ChooseAddress(0xf1f450, 0xf1f060)));
	UILayoutLoad_detour::attach(GetAddress(UTFWin::UILayout,Load));
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

