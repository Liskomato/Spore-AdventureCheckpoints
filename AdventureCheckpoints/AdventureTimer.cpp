#include "stdafx.h"
#include "stdio.h"
#include "shlobj.h"
#include <string>
#include "AdventureTimer.h"

AdventureTimer::AdventureTimer()
{
	listener = nullptr;
	debugEnabled = false;
	visible = true;
	checkpointsExtended = false;

	if (LoadData()) {
		PrintStatus();
	}
	else {
		App::ConsolePrintF("Adventure Checkpoints: LoadData has failed. Settings have been kept at their defaults.");
	}
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
				/// All the needed pointers.
				/// p1, p2: Parents
				/// c1, c2: Container windows
				/// b1, b2: Buttons
				/// bo1, bo2: Original "Replay" buttons, where we copy our sizes from.
				IWindowPtr p1, p2, c1, c2, b1, b2, bo1, bo2;
				p1 = WindowManager.GetMainWindow()->FindWindowByID(0x07C796D0),
				bo1 = p1->FindWindowByID(0x07C79940, false),
				c1 = p1->FindWindowByID(id("CheckpointButton")),

				p2 = WindowManager.GetMainWindow()->FindWindowByID(0x07C79820),
				bo2 = p2->FindWindowByID(0x07C79940, false),
				c2 = p2->FindWindowByID(id("CheckpointButton")),

				b1 = c1->FindWindowByID(0x07C79940), b2 = c2->FindWindowByID(0x07C79940);
				
				b1->SetSize(bo1->GetArea().GetWidth()+10,bo1->GetArea().GetHeight());
				b2->SetSize(bo2->GetArea().GetWidth()+10,bo2->GetArea().GetHeight());

				/// Setting visibility state based on conditions.
				if ((ScenarioMode.GetPlayMode()->mCurrentPlayModeState == Simulator::ScenarioPlayModeState::Failed && ScenarioMode.GetPlayMode()->mCurrentActIndex != 0) || checkpointsExtended)
				{
					c1->SetVisible(true);
					c2->SetVisible(true);
				}
				else 
				{
					c1->SetVisible(false);
					c2->SetVisible(false);
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
	
	PropertyListPtr propList = new App::PropertyList();
	FileStreamPtr stream = new IO::FileStream(GetFilePath().c_str());
	if (!SaveData(propList.get(), stream.get(), debugEnabled, visible,checkpointsExtended)) {
		MessageBoxW(NULL,L"AdventureTimer::SaveData has failed!\n\nSaving data to property file has failed. If you get this error message, please contact Liskomato on GitHub or Discord and tell them what you were doing at the time.",L"Adventure Checkpoints", MB_ICONERROR | MB_OK);
	}

	listener = nullptr;
	timer = nullptr;
}

string16 AdventureTimer::GetFilePath() {
	// Get %APPDATA%
	PWSTR appdata;
	/*	size_t len;
		errno_t err = getenv_s(&appdata, &len, u"APPDATA");*/

	if (SHGetKnownFolderPath(FOLDERID_RoamingAppData, KF_FLAG_CREATE, NULL, &appdata) != S_OK) {
		// This shouldn't happen.
		return NULL;
	}


	// Add rest of the filepath.

	string16 path = (char16_t*)appdata;

	path += u"\\Spore\\Preferences\\AdventureCheckpoints_Properties.prop";

	return path;
}

bool AdventureTimer::SaveData(App::PropertyList* propList, IO::FileStream* stream,bool debug, bool visible, bool extended) {
	propList->SetProperty(id("debugEnabled"),&App::Property().SetValueBool(debug));
	propList->SetProperty(id("isTimerVisible"), &App::Property().SetValueBool(visible));
	propList->SetProperty(id("checkpointsExtended"), &App::Property().SetValueBool(extended));
	stream->Open(IO::AccessFlags::ReadWrite,IO::CD::CreateAlways);
	bool check = propList->Write(stream);
	stream->Close();
	return check;
}

bool AdventureTimer::LoadData() {
	
	PropertyListPtr propList = new App::PropertyList();
	FileStreamPtr stream = new IO::FileStream(GetFilePath().c_str());
	if (stream->Open(IO::AccessFlags::Read,IO::CD::OpenExisting)) {
		propList->Read(stream.get());
		stream->Close();
	}
	if (propList->HasProperty(id("debugEnabled")) && propList->HasProperty(id("isTimerVisible")) && propList->HasProperty(id("checkpointsExtended"))) {
		App::Property::GetBool(propList.get(), id("debugEnabled"),debugEnabled);
		App::Property::GetBool(propList.get(), id("isTimerVisible"), visible);
		App::Property::GetBool(propList.get(), id("checkpointsExtended"), checkpointsExtended);
		return true;
	}
	else {
		bool check = SaveData(propList.get(),stream.get(),debugEnabled,visible,checkpointsExtended);
		if (check) App::ConsolePrintF("Adventure Checkpoints: Settings data didn't exist, so it was created to %ls", GetFilePath().c_str());
		else App::ConsolePrintF("Adventure Checkpoints: SaveData failed! Settings file will be attempted to be created to %ls once the game is closed.", GetFilePath().c_str());
		return check;
	}
}

void AdventureTimer::PrintStatus() {
	
	App::ConsolePrintF("\n--- Adventure Checkpoints: Settings status ---");

	if (visible) App::ConsolePrintF("- Timer UI is visible in adventure play mode.");
	else App::ConsolePrintF("- Timer UI is hidden. You can enable it by toggling 'T' key.");

	if (checkpointsExtended) App::ConsolePrintF("- Checkpoints appear every time the adventure ends.");
	else App::ConsolePrintF("- Checkpoints appear only when failing an adventure past act 1.");

	if (debugEnabled) App::ConsolePrintF("- Debug mode is enabled.");
	App::ConsolePrintF("----------------------------------------------\n");
}
