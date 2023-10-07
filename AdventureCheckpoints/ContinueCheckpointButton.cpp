#include "stdafx.h"
#include "ContinueCheckpointButton.h"
#include "GoToAct.h"
#include "AdventureTimer.h"

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
	
	if (message.IsType(kMsgComponentActivated))
	{
	//	App::ConsolePrintF("Button hit. Something should happen.");
		if (Timer.debugEnabled) 
		{ 
			App::ConsolePrintF("Beginning checkpoint procedure.");
		}
	
	//	Send message to the listener knowing that the checkpoint button was pressed.
		MessageManager.MessageSend(id("StartCheckpointProc"),nullptr);

		

		// We did handled the message, return true
		return false;
	}
//	*/
	// Return true if the message was handled, and therefore no other window procedure should receive it.
	return false;
}

void ContinueCheckpointButton::SendAsyncMessage(IWindow* pSource, IWindow* pDest, const Message& message, bool inheritable) {
	WindowManager.SendMsg(pSource, pDest, message, inheritable);
}

