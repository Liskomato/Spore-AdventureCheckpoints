#include "stdafx.h"
#include "stdio.h"
#include "shlobj.h"
#include <string>
#include "AdventureTimer.h"

AdventureTimer* AdventureTimer::timer = nullptr;

AdventureTimer::AdventureTimer()
{
	
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
			if (WindowManager.GetMainWindow()->FindWindowByID(id("Text")) != nullptr)
			{
				IWindowPtr timer = WindowManager.GetMainWindow()->FindWindowByID(id("Text"));
				timer->SetVisible(visible);
				if (timer->IsVisible()) {
					string16 timestr;

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

					timestr += (string16)time_local.GetText() + u": " + clock;

					timer->SetCaption(timestr.c_str());
				}
			}
			
	}
	else if (!Simulator::IsScenarioMode() || ScenarioMode.GetMode() != App::cScenarioMode::Mode::PlayMode)
	{
		if (WindowManager.GetMainWindow()->FindWindowByID(id("Text")) != nullptr) {
			WindowManager.GetMainWindow()->RemoveWindow(WindowManager.GetMainWindow()->FindWindowByID(id("Text")));
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

AdventureTimer* AdventureTimer::Get() {
	if (timer == nullptr) {
		timer = new AdventureTimer();
	}
	return timer;
}


void AdventureTimer::Dispose() {

	if (!SaveData()) {
		MessageBoxW(NULL,L"AdventureTimer::SaveData has failed!\n\nSaving data to property file has failed. If you get this error message, please contact Liskomato on GitHub or Discord and tell them what you were doing at the time.",L"Adventure Checkpoints", MB_ICONERROR | MB_OK);
	}

	
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

bool AdventureTimer::SaveData() {
	PropertyListPtr propList = new App::PropertyList();
	FileStreamPtr stream = new IO::FileStream(GetFilePath().c_str());

	propList->SetProperty(id("debugEnabled"),&App::Property().SetValueBool(debugEnabled));
	propList->SetProperty(id("isTimerVisible"), &App::Property().SetValueBool(visible));
	propList->SetProperty(id("checkpointsExtended"), &App::Property().SetValueBool(checkpointsExtended));
	stream->Open(IO::AccessFlags::ReadWrite,IO::CD::CreateAlways);
	bool check = propList->Write(stream.get());
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
		bool check = SaveData();
		if (check) App::ConsolePrintF("Adventure Checkpoints: Settings data didn't exist, so it was created to %ls", GetFilePath().c_str());
		else App::ConsolePrintF("Adventure Checkpoints: SaveData failed! Settings file will be attempted to be created to %ls once the game is closed.", GetFilePath().c_str());
		return check;
	}
}

void AdventureTimer::PrintStatus() {
	
	App::ConsolePrintF("\n--- Adventure Timer: Settings status ---");

	if (visible) App::ConsolePrintF("- Timer UI is visible in adventure play mode.");
	else App::ConsolePrintF("- Timer UI is hidden. You can enable it by toggling 'T' key.");

	App::ConsolePrintF("----------------------------------------------\n");
}
