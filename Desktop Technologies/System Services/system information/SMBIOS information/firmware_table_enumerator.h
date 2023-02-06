#pragma once
#include <vector>

namespace system_service
{
// https://learn.microsoft.com/en-us/windows/win32/sysinfo/system-information
namespace system_information
{
// https://learn.microsoft.com/en-us/windows/win32/api/sysinfoapi/nf-sysinfoapi-enumsystemfirmwaretables
class FirmwareTableEnumerator
{
public:

    static FirmwareTableEnumerator Get(unsigned int firmwareTableProviderSignature);
    bool                           IsVaild() const { return _errorCode == 0; }
    unsigned int                   GetErrorCode() const { return _errorCode; }
    std::vector<unsigned int>      GetFirmwareTableId() const { return _firmwareTableId; }

private:

    FirmwareTableEnumerator(const unsigned int errorCode, const char* firmwareTableBuf, unsigned int bufSize);

private:

    const unsigned int        _errorCode;
    std::vector<unsigned int> _firmwareTableId;
};
}
}