#include "com_init.h"
#include <mmdeviceapi.h>    // IMMDevice
#include <endpointvolume.h> // IAudioEndpointVolume
#include <iostream>

using namespace std;

// Implement Audio Endpoint volume callback
class AudioEndpointVolumeCallbacker : public IAudioEndpointVolumeCallback
{
public:

    AudioEndpointVolumeCallbacker() : _ref(1) {}
    ~AudioEndpointVolumeCallbacker() {}

    ULONG STDMETHODCALLTYPE AddRef() override { return InterlockedIncrement(&_ref); }
    ULONG STDMETHODCALLTYPE Release() override
    {
        const auto ref = InterlockedDecrement(&_ref);
        if (ref == 0)
        {
            delete this;
        }
        return ref;
    }

    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, VOID** ppvInterface) override
    {
        if (IID_IUnknown == riid)
        {
            AddRef();
            *ppvInterface = reinterpret_cast<IUnknown*>(this);
        }
        else if (__uuidof(IAudioEndpointVolumeCallback) == riid)
        {
            AddRef();
            *ppvInterface = reinterpret_cast<IAudioEndpointVolumeCallback*>(this);
        }
        else
        {
            *ppvInterface = nullptr;
            return E_NOINTERFACE;
        }
        return S_OK;
    }

    HRESULT STDMETHODCALLTYPE OnNotify(PAUDIO_VOLUME_NOTIFICATION_DATA pNotify) override
    {
        if (pNotify == nullptr)
        {
            return E_INVALIDARG;
        }

        cout << "muted : " << pNotify->bMuted << ", master volume : " << pNotify->fMasterVolume << ", channels : " << pNotify->nChannels << '\n';
        return S_OK;
    }

private:

    long _ref;
};

int main()
{
    ComInit              comInit;
    IMMDeviceEnumerator* enumerator = nullptr;
    auto                 hr         = ::CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr, CLSCTX_INPROC_SERVER, __uuidof(IMMDeviceEnumerator),
                                                         reinterpret_cast<void**>(&enumerator));
    if (FAILED(hr))
    {
        return 1;
    }

    IMMDevice* device = nullptr;
    hr                = enumerator->GetDefaultAudioEndpoint(eRender, eConsole, &device);
    if (FAILED(hr))
    {
        return 2;
    }

    IAudioEndpointVolume* audioEndpointVolume = nullptr;
    hr = device->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_ALL, nullptr, reinterpret_cast<void**>(&audioEndpointVolume));
    if (FAILED(hr))
    {
        return 3;
    }

    uint32_t channelCount = 0;
    hr                    = audioEndpointVolume->GetChannelCount(&channelCount);
    if (FAILED(hr))
    {
        return 4;
    }
    cout << "Count of the channles in the audio stream : " << channelCount << '\n';

    for (auto index = 0u; index < channelCount; ++index)
    {
        float levelDB = 0;
        hr            = audioEndpointVolume->GetChannelVolumeLevel(index, &levelDB);
        if (FAILED(hr))
        {
            return 5;
        }
        cout << "The volume level : " << levelDB << '\n';
        float level = 0;
        hr          = audioEndpointVolume->GetChannelVolumeLevelScalar(index, &level);
        cout << "The normalized volume level : " << level << '\n';
    }

    float masterLevelDB = 0;
    hr                  = audioEndpointVolume->GetMasterVolumeLevel(&masterLevelDB);
    if (FAILED(hr))
    {
        return 6;
    }
    cout << "The master volume level : " << masterLevelDB << '\n';

    float masterLevel = 0;
    hr                = audioEndpointVolume->GetMasterVolumeLevelScalar(&masterLevel);
    if (FAILED(hr))
    {
        return 7;
    }
    cout << "The master normalized volume level : " << masterLevelDB << '\n';

    BOOL mute = FALSE;
    hr        = audioEndpointVolume->GetMute(&mute);
    if (FAILED(hr))
    {
        return 8;
    }
    cout << "The muting state of the audio stream : " << mute << '\n';

    float volumeMindB       = 0;
    float volumeMaxdB       = 0;
    float volumeIncrementdB = 0;
    hr                      = audioEndpointVolume->GetVolumeRange(&volumeMindB, &volumeMaxdB, &volumeIncrementdB);
    if (FAILED(hr))
    {
        return 9;
    }
    cout << "Volume min db : " << volumeMindB << ", maxMaxdB : " << volumeMaxdB << ", increment dB : " << volumeIncrementdB << '\n';

    uint32_t step      = 0u;
    uint32_t stepCount = 0u;
    hr                 = audioEndpointVolume->GetVolumeStepInfo(&step, &stepCount);
    cout << "Step : " << step << ", step count : " << stepCount << '\n';
    if (FAILED(hr))
    {
        return 10;
    }

    DWORD hardwareSupportMask = 0;
    hr                        = audioEndpointVolume->QueryHardwareSupport(&hardwareSupportMask);
    if (FAILED(hr))
    {
        return 11;
    }
    cout << "Hardware supported ? : " << (hardwareSupportMask & ENDPOINT_HARDWARE_SUPPORT_VOLUME) << '\n';

    AudioEndpointVolumeCallbacker callbacker;
    hr = audioEndpointVolume->RegisterControlChangeNotify(&callbacker);
    if (FAILED(hr))
    {
        return 11;
    }

    getchar();

    audioEndpointVolume->UnregisterControlChangeNotify(&callbacker);

    return 0;
}