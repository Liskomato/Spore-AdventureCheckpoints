#pragma once

#include <Spore\BasicIncludes.h>

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
	

	int AddRef() override;
	int Release() override;
	void Update() override;
	void* Cast(uint32_t type) const override;
};
