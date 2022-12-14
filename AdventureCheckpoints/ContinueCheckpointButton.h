#pragma once

#include <Spore\BasicIncludes.h>

#define ContinueCheckpointButtonPtr intrusive_ptr<ContinueCheckpointButton>

// To avoid repeating UTFWin:: all the time.
using namespace UTFWin;

static UILayoutPtr Button = nullptr, Button2 = nullptr;

class ContinueCheckpointButton 
	: public IWinProc
	, public DefaultRefCounted
{
public:
	static const uint32_t TYPE = id("ContinueCheckpointButton");
	
	ContinueCheckpointButton();
	~ContinueCheckpointButton();

	int AddRef() override;
	int Release() override;
	void* Cast(uint32_t type) const override;
	
	int GetEventFlags() const override;
	// This is the function you have to implement, called when a window you added this winproc to received an event
	bool HandleUIMessage(IWindow* pWindow, const Message& message) override;
	void SendAsyncMessage(IWindow* pSource, IWindow* pDest, const Message& message, bool inheritable);
	
};
