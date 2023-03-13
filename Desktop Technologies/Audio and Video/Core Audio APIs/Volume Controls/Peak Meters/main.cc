#include "com_init.h"
#include <mmdeviceapi.h>    // MMDevice
#include <endpointvolume.h> // IAudioMeterInforamtion
#include <iostream>


using namespace std;


// Reference : https://learn.microsoft.com/en-us/windows/win32/api/endpointvolume/nn-endpointvolume-iaudiometerinformation
int main()
{

    ComInit comInit;
    // setp1 : get default audio endpoint
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

    IAudioMeterInformation* meterInfo = nullptr;
    hr                                = device->Activate(__uuidof(IAudioMeterInformation), CLSCTX_ALL, nullptr, reinterpret_cast<void**>(&meterInfo));
    if (FAILED(hr))
    {
        return 3;
    }

    // query hardware support
    DWORD hardwareSupportMask = 0;
    meterInfo->QueryHardwareSupport(&hardwareSupportMask);
    if (hardwareSupportMask & ENDPOINT_HARDWARE_SUPPORT_METER)
    {
        cout << "Hardware supported." << '\n';
    }
    else
    {
        cout << "Hardware not supported." << '\n';
    }

    uint32_t channelCount = 0;
    hr                    = meterInfo->GetMeteringChannelCount(&channelCount);
    if (FAILED(hr) || channelCount == 0)
    {
        return 4;
    }
    cout << "Metering channle Count : " << channelCount << '\n';    

    while (true)
    {
        // get the peak sample values for all the channels in the audio stream
        auto peakValues = std::make_unique<float[]>(channelCount);
        hr              = meterInfo->GetChannelsPeakValues(channelCount, peakValues.get());
        if (FAILED(hr))
        {
            return 5;
        }
        for (auto index = 0u; index < channelCount; ++index)
        {
            cout << "[" << index << "]" << peakValues[index] << "\n";
        }

        // retrive the peak sample value recorded across all of the channels in the audio stream
        float peakValue = 0;
        hr              = meterInfo->GetPeakValue(&peakValue);
        if (FAILED(hr))
        {
            return 6;
        }

        cout << "Peak value : " << peakValue << '\n';

        ::Sleep(1000); // 1 second
    }





}