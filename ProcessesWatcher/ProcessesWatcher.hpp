#pragma once

#define _WIN32_DCOM

#include <iostream>
#include <comdef.h>
#include <Wbemidl.h>
#include <vector>

#include "Interfaces/IWatcherSubscriber.hpp"

#pragma comment(lib, "wbemuuid.lib")

namespace FG
{
	class ProcessesWatcher : public IWbemObjectSink
	{
	private:
		// For counting all external objects which use this class' object
		LONG m_refcounter;
		std::vector<IWatcherSubscriber*> subscribers;

	public:
		ProcessesWatcher();
		~ProcessesWatcher();

		// -------------
		// GENERAL STUFF

		void Cleanup(IWbemServices*& services);

		// ------------------------------
		// OBSERVER RELATED FUNCTIONALITY

		void Subscribe(IWatcherSubscriber* subscriber);
		void Unsubscribe(IWatcherSubscriber* subscriber);
		void NotifyOnEventStarted(const EventData& data);
		void NotifyOnEventEnded(const EventData& data);

		// -----------------------------
		// INHERITED VIA IWbemObjectSink
		
		// Checks if this object supports provided interface
		// riid -> ID of provided interface
		// ppvObject -> pointer to the interface if supported
		// This object supports only IWbemObjectSink (IUknown) interface
		HRESULT __stdcall QueryInterface(REFIID riid, void** ppvObject) override;

		// Increases reference counter whenever someone saves reference to this object
		// Similar to shared_pointer logic
		ULONG __stdcall AddRef(void) override;

		// Decreases reference counter whenever this object to owner is not needed anymore
		// If counter == 0, object deletes itself hence not needed anyone anymore
		// Similar to shared_pointer logic
		ULONG __stdcall Release(void) override;

		// Being called with every new event occuring
		// lObjectCount -> how many events are in the array
		// apObjArray -> array of events
		HRESULT __stdcall Indicate(long lObjectCount, IWbemClassObject** apObjArray) override;
		
		// Being called when subscribtion is ended or error occured
		HRESULT __stdcall SetStatus(long lFlags, HRESULT hResult, BSTR strParam, IWbemClassObject* pObjParam) override;
	};
}
