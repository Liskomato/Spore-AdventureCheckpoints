#include "stdafx.h"
#include "AdventureTimer.h"

AdventureTimer::AdventureTimer()
{
	listener = nullptr;
	debugEnabled = false;
	visible = true;
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
	if (Simulator::IsScenarioMode() && ScenarioMode.GetMode() == App::cScenarioMode::Mode::PlayMode)
		{
			if (WindowManager.GetMainWindow()->FindWindowByID(id("CheckpointButton")) != nullptr)
			{
				if (ScenarioMode.GetPlayMode()->field_90 == 6 && ScenarioMode.GetPlayMode()->mCurrentActIndex != 0)
				{
					WindowManager.GetMainWindow()->FindWindowByID(0x07C796D0)->FindWindowByID(id("CheckpointButton"))->SetVisible(true);
					WindowManager.GetMainWindow()->FindWindowByID(0x07C79820)->FindWindowByID(id("CheckpointButton"))->SetVisible(true);
				}
				else 
				{
					WindowManager.GetMainWindow()->FindWindowByID(0x07C796D0)->FindWindowByID(id("CheckpointButton"))->SetVisible(false);
					WindowManager.GetMainWindow()->FindWindowByID(0x07C79820)->FindWindowByID(id("CheckpointButton"))->SetVisible(false);
				}
			}
			if (WindowManager.GetMainWindow()->FindWindowByID(id("Text")) != nullptr)
			{
				IWindowPtr timer = WindowManager.GetMainWindow()->FindWindowByID(id("Text"));
				timer->SetVisible(visible);
				if (timer->IsVisible()) {
					string16 timestr;
					timestr.assign_convert(to_string(ScenarioMode.GetPlayMode()->mSummary.mNumDeaths));

					LocalizedString deaths;
					deaths.SetText(id("AdventureCheckpoints"), 0x2, u"Deaths");

					timestr = (string16)deaths.GetText() + u": " + timestr;

					// time is in milliseconds (ms)
					int time = ScenarioMode.GetPlayMode()->mCurrentTimeMS;
					/*
					string16 scenariotime;
					scenariotime.assign_convert(to_string(time));

					timestr += u"\nfield_C0: " + scenariotime;
					*/
					int hours, minutes, seconds, milliseconds;
					string16 clock;

					milliseconds = time;

					seconds = time / 1000;
					milliseconds %= 1000;

					minutes = seconds / 60;
					seconds %= 60;

					hours = minutes / 60;
					minutes %= 60;


					clock.append_sprintf(u"%02d:%02d:%02d.%03d", hours, minutes, seconds, milliseconds);

					LocalizedString time_local;
					time_local.SetText(id("EP1_Headers"), 0x07736317, u"Time");

					timestr += u"\n" + (string16)time_local.GetText() + u": " + clock;

					timer->SetCaption(timestr.c_str());
				}
			}
	
			if (WindowManager.GetMainWindow()->FindWindowByID(id("TextDebug")) != nullptr) 
			{

				IWindowPtr debug = WindowManager.GetMainWindow()->FindWindowByID(id("TextDebug"));
				debug->SetVisible(debugEnabled);
				if (debug->IsVisible()) 
				{
					string16 debugtext = u"cScenarioPlayMode fields:\n", debug2;
					cScenarioPlayModePtr play = ScenarioMode.GetPlayMode();
					int f_78 = play->field_78,
						f_90 = play->field_90,
						f_94 = play->field_94,
						cinDelay = play->mCinematicDelay.GetElapsedTime(),
						f_b0 = play->field_B0,
						f_b4 = play->field_B4,
						f_b8 = play->mCurrentActIndex,
						f_c0 = play->mCurrentTimeMS,
						f_c4 = play->mAdventurePoints,
						f_c8 = play->field_C8,
						f_cc = play->field_CC,
						f_d0 = play->field_D0,
						f_d4 = play->field_D4,
						f_d8 = play->field_D8,
						f_100 = play->field_100;
					float f_dc = play->field_DC,
						f_e0_x = play->field_E0.x,
						f_e0_y = play->field_E0.y,
						f_e0_z = play->field_E0.z,
						f_f8 = play->field_F8;
					bool f_ec = play->field_EC,
						f_fc = play->field_FC,
						f_fd = play->field_FD;
					uint32_t musicID = play->mCurrentMusicId;
					long f_bc = play->mTimeLimitMS;

					debug2.append_sprintf(u"field_78: %d\nfield_90: %d\nfield_94: %d\nmCinematicDelay: %d\nfield_B0: %d\nfield_B4: %d\nmCurrentActIndex: %d\nmTimeLimitMS: %ld\nmCurrentTimeMS: %d\nmAdventurePoints: %d\nfield_C8: %d\nfield_CC: %d\nfield_D0: %d\nfield_D4: %d\nfield_D8: %d\nfield_DC: %f\nfield_E0: (%f, %f, %f)\nfield_EC: %s\nmCurrentMusicId: %#x\nfield_F8: %f\nfield_FC: %s\nfield_FD: %s\nfield_100: %d\n", f_78, f_90, f_94, cinDelay, f_b0, f_b4, f_b8, f_bc, f_c0, f_c4, f_c8, f_cc, f_d0, f_d4, f_d8, f_dc, f_e0_x, f_e0_y, f_e0_z, f_ec ? u"true" : u"false", musicID, f_f8, f_fc ? u"true" : u"false", f_fd ? u"true" : u"false", f_100);
					debugtext += debug2;
					debug->SetCaption(debugtext.c_str());
				}
			}
	
			
	}
	else if (!Simulator::IsScenarioMode() || ScenarioMode.GetMode() != App::cScenarioMode::Mode::PlayMode)
	{
		if (WindowManager.GetMainWindow()->FindWindowByID(id("Text")) != nullptr) {
			WindowManager.GetMainWindow()->RemoveWindow(WindowManager.GetMainWindow()->FindWindowByID(id("Text")));
			if (listener != nullptr) {
				WindowManager.GetMainWindow()->RemoveWinProc(listener.get());
				listener = nullptr;
			}
		}
		if (WindowManager.GetMainWindow()->FindWindowByID(id("TextDebug")) != nullptr) {
			WindowManager.GetMainWindow()->RemoveWindow(WindowManager.GetMainWindow()->FindWindowByID(id("TextDebug")));
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

void AdventureTimer::InitializeListener() {
	listener = new TimerListener();
}
AdventureTimerPtr AdventureTimer::Get() {
	if (timer != nullptr) return timer;
	else return nullptr;
}

AdventureTimerPtr AdventureTimer::Initialize() {
	if (timer == nullptr)
	timer = new AdventureTimer();
	return timer;
}

void AdventureTimer::Dispose() {
	timer = nullptr;
}
