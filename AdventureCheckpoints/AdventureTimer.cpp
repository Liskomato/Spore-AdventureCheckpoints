#include "stdafx.h"
#include "AdventureTimer.h"

AdventureTimer::AdventureTimer()
{
}


AdventureTimer::~AdventureTimer()
{
}

// For internal use, do not modify.
int AdventureTimer::AddRef()
{
	return DefaultRefCounted::AddRef();
}

// For internal use, do not modify.
int AdventureTimer::Release()
{
	return DefaultRefCounted::Release();
}

void AdventureTimer::Update() 
{
	if (Simulator::IsScenarioMode() 
		&& ScenarioMode.GetMode() == App::cScenarioMode::Mode::PlayMode
		&& WindowManager.GetMainWindow()->FindWindowByID(0x055EBC68) != nullptr)
	{
		IWindowPtr anchor = WindowManager.GetMainWindow()->FindWindowByID(0x055EBC68);
		if (anchor->FindWindowByID(id("Text")) != nullptr) {
			IWindowPtr timer = anchor->FindWindowByID(id("Text"));
			string16 timestr;
			timestr.assign_convert(to_string(ScenarioMode.GetPlayMode()->field_98.GetElapsed()));
			timestr = u"field_98: " + timestr;
			timer->SetCaption(timestr.c_str());
		}
	}
}

// You can extend this function to return any other types your class implements.
void* AdventureTimer::Cast(uint32_t type) const
{
	CLASS_CAST(Object);
	CLASS_CAST(AdventureTimer);
	return nullptr;
}
