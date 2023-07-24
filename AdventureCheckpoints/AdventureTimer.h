#pragma once

#include <Spore\BasicIncludes.h>
#include "TimerListener.h"

#define AdventureTimerPtr intrusive_ptr<AdventureTimer>

class AdventureTimer 
	: public Object
	, public DefaultRefCounted
	, public App::IUpdatable
{
public:
	static const uint32_t TYPE = id("AdventureTimer");
	
	AdventureTimer();
	~AdventureTimer();

	TimerListenerPtr listener;
	bool visible;
	bool debugEnabled;
	bool checkpointsExtended;

	int AddRef() override;
	int Release() override;
	void Update() override;
	void* Cast(uint32_t type) const override;

	void InitializeListener();
	static AdventureTimerPtr Get();
	static AdventureTimerPtr Initialize();
	void Dispose();

	string16 GetFilePath();
	bool LoadData();
	bool SaveData(App::PropertyList* propList, IO::FileStream* stream, bool debug, bool visible, bool extended);
	void PrintStatus();
};

static AdventureTimerPtr timer = nullptr;
