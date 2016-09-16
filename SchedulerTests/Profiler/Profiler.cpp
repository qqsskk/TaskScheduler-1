#include <MTScheduler.h>
#include "Profiler.h"


#ifdef MT_INSTRUMENTED_BUILD


void PushPerfMarker(const char* name, MT::Color::Type color)
{
	MT_UNUSED(name);
	MT_UNUSED(color);
}

void PopPerfMarker(const char* name)
{
	MT_UNUSED(name);
}



void PushPerfEvent(const char* name)
{
	PushPerfMarker(name, MT::Color::Red);
}

void PopPerfEvent(const char* name)
{
	PopPerfMarker(name);
}


class Microprofile : public MT::IProfilerEventListener
{
public:

	Microprofile()
	{
	}

	~Microprofile()
	{
	}

	virtual void OnThreadCreated(uint32 workerIndex) override 
	{
		MT_UNUSED(workerIndex);
	}

	virtual void OnThreadStarted(uint32 workerIndex) override 
	{
		MT_UNUSED(workerIndex);
	}

	virtual void OnThreadStoped(uint32 workerIndex) override 
	{
		MT_UNUSED(workerIndex);
	}

	virtual void OnThreadIdleStarted(uint32 workerIndex) override 
	{
		MT_UNUSED(workerIndex);
		PushPerfMarker("ThreadIdle", MT::Color::Red);
	}

	virtual void OnThreadIdleFinished(uint32 workerIndex) override 
	{
		MT_UNUSED(workerIndex);
		PopPerfMarker("ThreadIdle");
	}

	virtual void OnThreadWaitStarted() override 
	{
		PushPerfMarker("ThreadWait", MT::Color::Red);
	}

	virtual void OnThreadWaitFinished() override 
	{
		PopPerfMarker("ThreadWait");
	}

	virtual void NotifyTaskExecuteStateChanged(MT::Color::Type debugColor, const mt_char* debugID, MT::TaskExecuteState::Type type) override 
	{
		switch(type)
		{
		case MT::TaskExecuteState::START:
		case MT::TaskExecuteState::RESUME:
			PushPerfMarker(debugID, debugColor);
			break;
		case MT::TaskExecuteState::STOP:
		case MT::TaskExecuteState::SUSPEND:
			PopPerfMarker(debugID);
			break;
		}
	}
};


#endif


MT::IProfilerEventListener* GetProfiler()
{
#ifdef MT_INSTRUMENTED_BUILD
	static Microprofile profile;
	return &profile;
#else
	return nullptr;
#endif

	
}
