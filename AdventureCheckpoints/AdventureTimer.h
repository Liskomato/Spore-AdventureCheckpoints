#pragma once

#include <Spore\BasicIncludes.h>
#include "TimerListener.h"

#define AdventureTimerPtr intrusive_ptr<AdventureTimer>
#define Timer (*AdventureTimer::Get())

class AdventureTimer 
	: public Object
	, public DefaultRefCounted
	, public App::IUpdatable
{
public:
	static const uint32_t TYPE = id("AdventureTimer");
	
	
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
	static AdventureTimer* Get();
	void Dispose();

	string16 GetFilePath();
	bool LoadData();
	bool SaveData();
	void PrintStatus();
private:
	AdventureTimer();
	static AdventureTimer* timer;
};
