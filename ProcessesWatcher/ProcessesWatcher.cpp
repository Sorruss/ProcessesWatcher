#include "ProcessesWatcher.hpp"

namespace FG
{
	ProcessesWatcher::ProcessesWatcher()
		: m_refcounter(0), subscribers()
	{}

	ProcessesWatcher::~ProcessesWatcher()
	{
		
	}

	// ------------------------------
	// OBSERVER RELATED FUNCTIONALITY

	void ProcessesWatcher::Subscribe(std::shared_ptr<IWatcherSubscriber> subscriber)
	{
		subscribers.push_back(subscriber);
	}

	void ProcessesWatcher::NotifyOnEventStarted(const EventData& data)
	{
		CleanupExpiredSubscribers();

		for (int i = 0; i < subscribers.size(); i++)
		{
			auto subscriber = subscribers[i].lock();
			if (subscriber == nullptr)
				continue;

			try
			{
				subscriber->OnEventStarted(data);
			}
			catch (const std::runtime_error& error)
			{
				std::cout << "[ProcessesWatcher::NotifyOnEventStarted] " << error.what() << '\n';
				continue;
			}
		}
	}

	void ProcessesWatcher::NotifyOnEventEnded(const EventData& data)
	{
		CleanupExpiredSubscribers();

		for (int i = 0; i < subscribers.size(); i++)
		{
			auto subscriber = subscribers[i].lock();
			if (subscriber == nullptr)
				continue;

			try
			{
				subscriber->OnEventEnded(data);
			}
			catch (const std::runtime_error& error)
			{
				std::cout << "[ProcessesWatcher::NotifyOnEventStarted] " << error.what() << '\n';
				continue;
			}
		}
	}

	void ProcessesWatcher::CleanupExpiredSubscribers()
	{
		subscribers.erase(std::remove_if(subscribers.begin(), subscribers.end(), 
			[](const std::weak_ptr<IWatcherSubscriber>& weak) {
				return weak.expired();
			}),
			subscribers.end()
		);
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
		if (newRefcounter <= 0)
			delete this;
		return newRefcounter;
	}

	HRESULT __stdcall ProcessesWatcher::Indicate(long lObjectCount, IWbemClassObject** apObjArray)
	{
		for (long i = 0; i < lObjectCount; i++)
		{
			IWbemClassObject* object = apObjArray[i];
			_variant_t variant;

			if (FAILED(object->Get(L"__CLASS", 0, &variant, 0, 0)))
				continue;

			std::wstring objectClass = variant.bstrVal;
			if (objectClass == L"Win32_ProcessStartTrace")
			{
				EventData data;

				// ADD STARTED PROCESS' NAME
				if (SUCCEEDED(object->Get(L"ProcessName", 0, &variant, 0, 0)))
					data.name = variant.bstrVal;

				// ADD STARTED PROCESS' ID
				if (SUCCEEDED(object->Get(L"ProcessId", 0, &variant, 0, 0)))
					data.id = variant.ulVal;

				NotifyOnEventStarted(data);
			}
			else if (objectClass == L"Win32_ProcessStopTrace")
			{
				EventData data;

				// ADD ENDED PROCESS' NAME
				if (SUCCEEDED(object->Get(L"ProcessName", 0, &variant, 0, 0)))
					data.name = variant.bstrVal;

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
