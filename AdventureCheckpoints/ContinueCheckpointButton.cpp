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
	if (message.IsType(kMsgButtonClick)) {
		if (message.source->GetControlID() == 0x3DAF6E43)
		{
			App::ConsolePrintF("Restarting from the current act...");
			
			auto act = ScenarioMode.GetPlayMode()->mCurrentActIndex;

			IWindow* RestartButton = WindowManager.GetMainWindow()->FindWindowByID(0x07737870);

			WindowManager.SendMsg(window,RestartButton,message,false);

			GoToAct::SetNewAct(act,0);

			// We did handled the message, return true
			return true;
		}
	}
	// Return true if the message was handled, and therefore no other window procedure should receive it.
	return false;
}
