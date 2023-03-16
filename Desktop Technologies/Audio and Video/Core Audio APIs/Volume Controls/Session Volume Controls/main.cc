#include "com_init.h"
#define NOBITMAP
#include <audiopolicy.h> // IAudioSessionEvents
#include <mmdeviceapi.h> // IMMDeviceEnumerator
#include <atlbase.h>     // CComPtr
#include <iostream>

using namespace std;

// https://learn.microsoft.com/en-us/windows/win32/coreaudio/audio-session-events
class AudioSessionEvent : public IAudioSessionEvents
{
public:

    AudioSessionEvent() : _ref(1) {}
    ~AudioSessionEvent() {}

    ULONG STDMETHODCALLTYPE AddRef() { return InterlockedIncrement(&_ref); }

    ULONG STDMETHODCALLTYPE Release()
    {
        ULONG ulRef = InterlockedDecrement(&_ref);
        if (0 == ulRef)
        {
            delete this;
        }
        return ulRef;
    }

    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, VOID **ppvInterface)
    {
        if (IID_IUnknown == riid)
        {
            AddRef();
            *ppvInterface = (IUnknown *) this;
        }
        else if (__uuidof(IAudioSessionEvents) == riid)
        {
            AddRef();
            *ppvInterface = (IAudioSessionEvents *) this;
        }
        else
        {
            *ppvInterface = NULL;
            return E_NOINTERFACE;
        }
        return S_OK;
    }

    // Notification methods for audio session events

    HRESULT STDMETHODCALLTYPE OnDisplayNameChanged(LPCWSTR NewDisplayName, LPCGUID EventContext) { return S_OK; }

    HRESULT STDMETHODCALLTYPE OnIconPathChanged(LPCWSTR NewIconPath, LPCGUID EventContext) { return S_OK; }

    HRESULT STDMETHODCALLTYPE OnSimpleVolumeChanged(float NewVolume, BOOL NewMute, LPCGUID EventContext)
    {
        cout << "New mute : " << NewMute << ", volume percent : " << static_cast<uint32_t>(100 * NewVolume + 0.5) << '\n';

        return S_OK;
    }

    HRESULT STDMETHODCALLTYPE OnChannelVolumeChanged(DWORD ChannelCount, float NewChannelVolumeArray[], DWORD ChangedChannel, LPCGUID EventContext)
    {
        return S_OK;
    }

    HRESULT STDMETHODCALLTYPE OnGroupingParamChanged(LPCGUID NewGroupingParam, LPCGUID EventContext) { return S_OK; }

    HRESULT STDMETHODCALLTYPE OnStateChanged(AudioSessionState NewState)
    {
        std::string state;

        switch (NewState)
        {
        case AudioSessionStateActive:
            state = "active";
            break;
        case AudioSessionStateInactive:
            state = "inactive";
            break;
        }
        cout << "New session state : " << state << '\n';

        return S_OK;
    }

    HRESULT STDMETHODCALLTYPE OnSessionDisconnected(AudioSessionDisconnectReason DisconnectReason)
    {
        std::string reason;

        switch (DisconnectReason)
        {
        case DisconnectReasonDeviceRemoval:
            reason = "device removed";
            break;
        case DisconnectReasonServerShutdown:
            reason = "server shut down";
            break;
        case DisconnectReasonFormatChanged:
            reason = "format changed";
            break;
        case DisconnectReasonSessionLogoff:
            reason = "user logged off";
            break;
        case DisconnectReasonSessionDisconnected:
            reason = "session disconnected";
            break;
        case DisconnectReasonExclusiveModeOverride:
            reason = "exclusive-mode override";
            break;
        }

        cout << "Audio session disconnected reason : " << reason << '\n';

        return S_OK;
    }

private:

    long _ref;
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

    CComPtr<IMMDevice> device;
    hr = enumerator->GetDefaultAudioEndpoint(eRender, eConsole, &device);
    if (FAILED(hr))
    {
        return 2;
    }

    CComPtr<IAudioSessionManager2> audioSessionManager2;
    hr = device->Activate(__uuidof(IAudioSessionManager2), CLSCTX_INPROC_SERVER, nullptr, reinterpret_cast<void **>(&audioSessionManager2));
    if (FAILED(hr))
    {
        return 3;
    }

    CComPtr<ISimpleAudioVolume> simpleAduioVolume;
    hr = audioSessionManager2->GetSimpleAudioVolume(nullptr, 0, &simpleAduioVolume);
    if (FAILED(hr))
    {
        return 4;
    }

    simpleAduioVolume->SetMute(true, nullptr);

    CComPtr<IAudioSessionControl> audioSessionControl = nullptr;
    hr                                                = audioSessionManager2->GetAudioSessionControl(nullptr, 0, &audioSessionControl);
    if (FAILED(hr))
    {
        return 5;
    }
    AudioSessionEvent sessionEvent;
    hr = audioSessionControl->RegisterAudioSessionNotification(&sessionEvent);
    if (FAILED(hr))
    {
        return 6;
    }

    getchar();

    audioSessionControl->UnregisterAudioSessionNotification(&sessionEvent);

    return 0;
}