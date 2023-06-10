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
			timestr.assign_convert(to_string(ScenarioMode.GetPlayMode()->field_98.GetElapsedTime()));
			timestr = u"field_98: " + timestr;
			
			// time is in milliseconds (ms)
			int time = ScenarioMode.GetPlayMode()->field_C0;

			string16 scenariotime;
			scenariotime.assign_convert(to_string(time));

			timestr += u"\nfield_C0: " + scenariotime;

			int hours, minutes, seconds, milliseconds;
			string16 clock;

			milliseconds = time;

			seconds = time / 1000;
			milliseconds %= 1000;
			
			minutes = seconds / 60;
			seconds %= 60;

			hours = minutes / 60;
			minutes %= 60;


			clock.append_sprintf(u"%02d:%02d:%02d.%03d",hours,minutes,seconds,milliseconds);

			timestr += u"\nTime: " + clock;

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
