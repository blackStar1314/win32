#include "com_init.h"
#include <mmdeviceapi.h> // IMMDeviceEnumerator
#include <atlbase.h>     // CComPtr
#include <Functiondiscoverykeys_devpkey.h> // PKEY_Device_FriendlyName
#include <iostream>

using namespace std;

// Notifyer IMMNotificationClient
class NotificationClient : public IMMNotificationClient
{
public:
    NotificationClient(CComPtr<IMMDeviceEnumerator> enumerator) : _ref(1), _enumerator(enumerator) {}
    // Implement IUnknown
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void **ppvObject) override 
    {
        if (IID_IUnknown == riid)
        {
            AddRef();
            *ppvObject = (IUnknown *) this;
        }
        else if (__uuidof(IMMNotificationClient) == riid)
        {
            AddRef();
            *ppvObject = (IMMNotificationClient *) this;
        }
        else
        {
            *ppvObject = NULL;
            return E_NOINTERFACE;
        }
        return S_OK;
    }

    ULONG STDMETHODCALLTYPE AddRef(void) override { return InterlockedIncrement(&_ref); }

    ULONG STDMETHODCALLTYPE Release(void) override
    {
        const auto ref = InterlockedDecrement(&_ref);
        if (0 == ref)
        {
            delete this;
        }

        return ref;
    }

    // IMMNotificationClient
    HRESULT STDMETHODCALLTYPE OnDeviceStateChanged(LPCWSTR pwstrDeviceId, DWORD dwNewState) override 
    {
        wcout << "NotificationClient::OnDeviceSateChanged." << pwstrDeviceId << ", " << dwNewState << '\n';
        return S_OK;
        std::string state = "?????";

        //PrintDeviceName(pwstrDeviceId);

        switch (dwNewState)
        {
        case DEVICE_STATE_ACTIVE:
            state = "ACTIVE";
            break;
        case DEVICE_STATE_DISABLED:
            state = "DISABLED";
            break;
        case DEVICE_STATE_NOTPRESENT:
            state = "NOTPRESENT";
            break;
        case DEVICE_STATE_UNPLUGGED:
            state = "UNPLUGGED";
            break;
        }

        printf("  -->New device state is DEVICE_STATE_%s (0x%8.8x)\n", state.c_str(), dwNewState);

        return S_OK;
    }

    HRESULT STDMETHODCALLTYPE OnDeviceAdded(LPCWSTR pwstrDeviceId) override 
    {
        wcout << "NotificationClient::OnDeviceAdd." << pwstrDeviceId << '\n';
        //PrintDeviceName(pwstrDeviceId);
        return S_OK;
    }

    HRESULT STDMETHODCALLTYPE OnDeviceRemoved(LPCWSTR pwstrDeviceId) override 
    {
        wcout << "NotificationClient::OnDeviceRemoved." << pwstrDeviceId << '\n';
        //PrintDeviceName(pwstrDeviceId);
        return S_OK;
    }

    HRESULT STDMETHODCALLTYPE OnDefaultDeviceChanged(EDataFlow flow, ERole role, LPCWSTR pwstrDefaultDeviceId) override 
    {
        return E_NOTIMPL;

        std::string flowStr = "?????";
        std::string roleStr = "?????";

        //PrintDeviceName(pwstrDefaultDeviceId);

        switch (flow)
        {
        case eRender:
            flowStr = "eRender";
            break;
        case eCapture:
            flowStr = "eCapture";
            break;
        }

        switch (role)
        {
        case eConsole:
            roleStr = "eConsole";
            break;
        case eMultimedia:
            roleStr = "eMultimedia";
            break;
        case eCommunications:
            roleStr = "eCommunications";
            break;
        }

        printf("  -->New default device: flow = %s, role = %s\n", flowStr.c_str(), roleStr.c_str());
        return S_OK;
    }

    HRESULT STDMETHODCALLTYPE OnPropertyValueChanged(LPCWSTR pwstrDeviceId, const PROPERTYKEY key) override 
    {
        //PrintDeviceName(pwstrDeviceId);
        //printf(
        //    "  -->Changed device property "
        //    "{%8.8x-%4.4x-%4.4x-%2.2x%2.2x-%2.2x%2.2x%2.2x%2.2x%2.2x%2.2x}#%d\n",
        //    key.fmtid.Data1, key.fmtid.Data2, key.fmtid.Data3, key.fmtid.Data4[0], key.fmtid.Data4[1], key.fmtid.Data4[2], key.fmtid.Data4[3],
        //    key.fmtid.Data4[4], key.fmtid.Data4[5], key.fmtid.Data4[6], key.fmtid.Data4[7], key.pid);
        return E_NOTIMPL;
    }

private:

    void PrintDeviceName(LPCWSTR pwstrDeviceId) 
    {
        CComPtr<IMMDevice>      device;
        CComPtr<IPropertyStore> props;
        PROPVARIANT     varString;

        PropVariantInit(&varString);

        if (_enumerator == nullptr)
        {
            abort();
        }
        auto hr = _enumerator->GetDevice(pwstrDeviceId, &device);
        if (hr == S_OK)
        {
            hr = device->OpenPropertyStore(STGM_READ, &props);
        }
        if (hr == S_OK)
        {
            // Get the endpoint device's friendly-name property.
            hr = props->GetValue(PKEY_Device_FriendlyName, &varString);
        }
        printf(
            "----------------------\nDevice name: \"%S\"\n"
            "  Endpoint ID string: \"%S\"\n",
            (hr == S_OK) ? varString.pwszVal : L"null device", (pwstrDeviceId != NULL) ? pwstrDeviceId : L"null ID");

        PropVariantClear(&varString);
    }

    ULONG                        _ref;
    CComPtr<IMMDeviceEnumerator> _enumerator;
};

int main()
{
    ComInit                      comInit;
    CComPtr<IMMDeviceEnumerator> enumerator;
    auto                         hr = ::CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr, CLSCTX_INPROC_SERVER, __uuidof(IMMDeviceEnumerator),
                                                         reinterpret_cast<void **>(&enumerator));
    if (FAILED(hr))
    {
        return 1;
    }

    NotificationClient client(enumerator);
    enumerator->RegisterEndpointNotificationCallback(&client);

    system("pause");
    system("pause");
    enumerator->UnregisterEndpointNotificationCallback(&client);
}