#include "hardware_information.h"
#include <Windows.h>

namespace system_service
{
namespace system_information
{
HardwareInformation HardwareInformation::Get()
{
    SYSTEM_INFO si;
    ::GetSystemInfo(&si);

    return HardwareInformation(si.dwOemId, si.dwPageSize, si.lpMinimumApplicationAddress, si.lpMaximumApplicationAddress, si.dwActiveProcessorMask,
                               si.dwNumberOfProcessors, si.dwProcessorType, si.dwAllocationGranularity, si.wProcessorLevel, si.wProcessorRevision);
}

HardwareInformation::HardwareInformation(unsigned long oemId, unsigned long pageSize, void* minimumAppAddress, void* maximumAppAddress,
                                         unsigned long activeProcessorMask, unsigned long numberOfProcessors, unsigned long processorType,
                                         unsigned long allocationGranularity, unsigned long processorLevel, unsigned long processorRevision)

    : _oemId(oemId)
    , _pageSize(pageSize)
    , _minimumApplicationAddress(minimumAppAddress)
    , _maximumApplicationAddress(maximumAppAddress)
    , _activeProcessorMask(activeProcessorMask)
    , _numberOfProcessors(numberOfProcessors)
    , _processorsType(processorType)
    , _allocationGranularity(allocationGranularity)
    , _processorLevel(processorLevel)
    , _processorRevision(processorRevision)
{
}
}
}
