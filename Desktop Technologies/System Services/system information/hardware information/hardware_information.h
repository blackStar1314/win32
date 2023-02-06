#pragma once

namespace system_service
{
// https://learn.microsoft.com/en-us/windows/win32/sysinfo/system-information
namespace system_information
{
// https://learn.microsoft.com/en-us/windows/win32/sysinfo/getting-hardware-information
class HardwareInformation
{
public:
    static HardwareInformation Get();
    unsigned long              GetOEMId() const { return _oemId; }
    unsigned long              GetPageSize() const { return _pageSize; }
    void*                      GetMinimumApplicationAddress() const { return _minimumApplicationAddress; }
    void*                      GetMaximumApplicationAddress() const { return _maximumApplicationAddress; }
    unsigned long              GetActiveProcessorMask() const { return _activeProcessorMask; }
    unsigned long              GetNumberOfProcessors() const { return _numberOfProcessors; }
    unsigned long              GetProcessorType() const { return _processorsType; }
    unsigned long              GetAllocationGranularity() const { return _allocationGranularity; }
    unsigned short             GetProcessorLevel() const { return _processorLevel; }
    unsigned short             GetProcessorRevision() const { return _processorRevision; }

private:

    HardwareInformation(unsigned long oemId, unsigned long pageSize, void* minimumAppAddress, void* maximumAppAddress,
                        unsigned long activeProcessorMask, unsigned long numberOfProcessors, unsigned long processorType,
                        unsigned long allocationGranularity, unsigned long processorLevel, unsigned long processorRevision);

private:

    unsigned long  _oemId;
    unsigned long  _pageSize;
    void*          _minimumApplicationAddress;
    void*          _maximumApplicationAddress;
    unsigned long  _activeProcessorMask;
    unsigned long  _numberOfProcessors;
    unsigned long  _processorsType;
    unsigned long  _allocationGranularity;
    unsigned short _processorLevel;
    unsigned short _processorRevision;
};
}
} // namespace system_service