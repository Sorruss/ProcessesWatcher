#include "ProcessesWatcher.hpp"

namespace FG
{
	ProcessesWatcher::ProcessesWatcher()
		: m_refcounter(0), subscribers()
	{}

	ProcessesWatcher::~ProcessesWatcher()
	{
		Release();
	}

	void ProcessesWatcher::Cleanup(IWbemServices*& services)
	{
		if (!services)
			return;

		services->CancelAsyncCall(this);
	}

	// ------------------------------
	// OBSERVER RELATED FUNCTIONALITY

	void ProcessesWatcher::Subscribe(IWatcherSubscriber* subscriber)
	{
		subscribers.push_back(subscriber);
	}

	void ProcessesWatcher::Unsubscribe(IWatcherSubscriber* subscriber)
	{
		subscribers.erase(std::remove(subscribers.begin(), subscribers.end(), subscriber), subscribers.end());
	}

	void ProcessesWatcher::NotifyOnEventStarted(const EventData& data)
	{
		for (int i = 0; i < subscribers.size(); i++)
		{
			subscribers[i]->OnEventStarted(data);
		}
	}

	void ProcessesWatcher::NotifyOnEventEnded(const EventData& data)
	{
		for (int i = 0; i < subscribers.size(); i++)
		{
			subscribers[i]->OnEventEnded(data);
		}
	}

	// -----------------------------
	// INHERITED VIA IWbemObjectSink

	HRESULT __stdcall ProcessesWatcher::QueryInterface(REFIID riid, void** ppvObject)
	{
		if (riid == IID_IUnknown || riid == IID_IWbemObjectSink)
		{
			*ppvObject = static_cast<IWbemObjectSink*>(this);
			AddRef();
			return WBEM_S_NO_ERROR;
		}

		return E_NOINTERFACE;
	}

	ULONG __stdcall ProcessesWatcher::AddRef(void)
	{
		return InterlockedIncrement(&m_refcounter);
	}

	ULONG __stdcall ProcessesWatcher::Release(void)
	{
		LONG newRefcounter = InterlockedDecrement(&m_refcounter);
		if (newRefcounter == 0)
			delete this;
		return newRefcounter;
	}

	HRESULT __stdcall ProcessesWatcher::Indicate(long lObjectCount, IWbemClassObject** apObjArray)
	{
		for (long i = 0; i < lObjectCount; i++)
		{
			IWbemClassObject* object = apObjArray[i];
			VARIANT variant;
			VariantInit(&variant);

			if (FAILED(object->Get(L"__CLASS", 0, &variant, 0, 0)))
			{
				VariantClear(&variant);
				continue;
			}
			std::wstring objectClass = variant.bstrVal;
			VariantClear(&variant);
		
			if (objectClass == L"Win32_ProcessStartTrace")
			{
				EventData data;

				// ADD STARTED PROCESS' NAME
				if (SUCCEEDED(object->Get(L"ProcessName", 0, &variant, 0, 0)))
				{
					data.name = variant.bstrVal;
					VariantClear(&variant);
				}

				// ADD STARTED PROCESS' ID
				if (SUCCEEDED(object->Get(L"ProcessId", 0, &variant, 0, 0)))
				{
					data.id = variant.ulVal;
					VariantClear(&variant);
				}

				NotifyOnEventStarted(data);
			}
			else if (objectClass == L"Win32_ProcessStopTrace")
			{
				EventData data;

				// ADD ENDED PROCESS' NAME
				if (SUCCEEDED(object->Get(L"ProcessName", 0, &variant, 0, 0)))
				{
					data.name = variant.bstrVal;
					VariantClear(&variant);
				}

				NotifyOnEventEnded(data);
			}
		}

		return WBEM_S_NO_ERROR;
	}

	HRESULT __stdcall ProcessesWatcher::SetStatus(long lFlags, HRESULT hResult, BSTR strParam, IWbemClassObject* pObjParam)
	{
		std::cout << "[QuerySink::SetStatus] hResult = " << hResult << '\n';
		return WBEM_S_NO_ERROR;
	}
}
