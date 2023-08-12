#include "stdafx.h"
#include "TimerListener.h"
#include "AdventureTimer.h"

TimerListener::TimerListener()
{
}


TimerListener::~TimerListener()
{
}

// For internal use, do not modify.
int TimerListener::AddRef()
{
	return DefaultRefCounted::AddRef();
}

// For internal use, do not modify.
int TimerListener::Release()
{
	return DefaultRefCounted::Release();
}

// You can extend this function to return any other types your class implements.
void* TimerListener::Cast(uint32_t type) const
{
	CLASS_CAST(TimerListener);
	return nullptr;
}

// This method returns a combinations of values in UTFWin::EventFlags.
// The combination determines what types of events (messages) this window procedure receives.
// By default, it receives mouse/keyboard input and advanced messages.
int TimerListener::GetEventFlags() const
{
	return kEventFlagBasicInput | kEventFlagAdvanced;
}

// The method that receives the message. The first thing you should do is probably
// checking what kind of message was sent...
bool TimerListener::HandleUIMessage(IWindow* window, const Message& message)
{
	if (AdventureTimer::Get() != nullptr) {
		timer = AdventureTimer::Get();
		if (message.IsType(kMsgKeyDown) && (message.Key.vkey == 't' || message.Key.vkey == 'T')) 
		{
			if (message.Key.modifiers == kModifierCtrlDown + kModifierShiftDown) 
			{
				timer->debugEnabled = !timer->debugEnabled;
				return true;
			}
			else 
			{
				timer->visible = !timer->visible;
				return true;
			}
		}
		else if (message.IsType(kMsgKeyDown) && (message.Key.vkey == 'r' || message.Key.vkey == 'R') && message.Key.modifiers == kModifierCtrlDown
			&& Simulator::IsScenarioMode() && ScenarioMode.GetMode() == App::cScenarioMode::Mode::PlayMode) {
			ScenarioMode.GetPlayMode()->JumpToAct(ScenarioMode.GetPlayMode()->mCurrentActIndex);
			return true;
		}
	}
	// Return true if the message was handled, and therefore no other window procedure should receive it.
	return false;
}
