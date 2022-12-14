#include "stdafx.h"
#include "ContinueCheckpointButton.h"
#include "GoToAct.h"

ContinueCheckpointButton::ContinueCheckpointButton()
{
}


ContinueCheckpointButton::~ContinueCheckpointButton()
{
}

// For internal use, do not modify.
int ContinueCheckpointButton::AddRef()
{
	return DefaultRefCounted::AddRef();
}

// For internal use, do not modify.
int ContinueCheckpointButton::Release()
{
	return DefaultRefCounted::Release();
}

// You can extend this function to return any other types your class implements.
void* ContinueCheckpointButton::Cast(uint32_t type) const
{
	CLASS_CAST(Object);
	CLASS_CAST(IWinProc);
	CLASS_CAST(ContinueCheckpointButton);
	return nullptr;
}

// This method returns a combinations of values in UTFWin::EventFlags.
// The combination determines what types of events (messages) this window procedure receives.
// By default, it receives mouse/keyboard input and advanced messages.
int ContinueCheckpointButton::GetEventFlags() const
{
	return kEventFlagBasicInput | kEventFlagAdvanced;
}

// The method that receives the message. The first thing you should do is probably
// checking what kind of message was sent...
bool ContinueCheckpointButton::HandleUIMessage(IWindow* window, const Message& message)
{
	
	/*if (message.IsSource(WindowManager.GetMainWindow()->FindWindowByID(0x07C796D0)->FindWindowByID(0x07C79940)) && !message.IsType(0x1001)) {
	//	App::ConsolePrintF("0x%x", message.eventType);
	}*/
	
	
	// */
	if (message.IsType(kMsgComponentActivated))
	{
		App::ConsolePrintF("Button hit. Something should happen.");
	//	App::ConsolePrintF("Restarting from the current act...");
			
	//	auto act = ScenarioMode.GetPlayMode()->mCurrentActIndex;
		IWindowPtr MainWindow = WindowManager.GetMainWindow();
		IWindowPtr RestartButton = WindowManager.GetMainWindow()->FindWindowByID(0x07C79940);
		
		Message msg = message;

		msg.source = RestartButton.get();

	//	msg.Mouse.mouseX = 640;
		
		bool ret = false;
		/*

		if (RestartButton->GetParent() != nullptr) {
			
			IWindowPtr rewardScreen = RestartButton->GetParent();
			
			if (rewardScreen->GetParent() != nullptr) {

				IWindowPtr rewardScreenParent = rewardScreen->GetParent();
				ret = rewardScreenParent->SendMsg(msg);
				//	std::async(std::launch::async, [&] {StartFromCheckpoint(ScenarioMode.GetPlayMode()->mCurrentActIndex); });


				//	MessageManager.PostMSG(id("StartCheckpointProc"),nullptr);


				//	ScenarioMode.GetPlayMode()->SetCurrentAct(act-1);
			}
			else return false;
			

		}
		else return false;
		
		*/
	

		

		// We did handled the message, return true
		return ret;
	}
//	*/
	// Return true if the message was handled, and therefore no other window procedure should receive it.
	return false;
}

void ContinueCheckpointButton::SendAsyncMessage(IWindow* pSource, IWindow* pDest, const Message& message, bool inheritable) {
	WindowManager.SendMsg(pSource, pDest, message, inheritable);
}

