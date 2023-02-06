#include "firmware_table_enumerator.h"
#include <Windows.h>
#include <memory>


namespace system_service
{
namespace system_information
{
FirmwareTableEnumerator FirmwareTableEnumerator::Get(unsigned int firmwareTableProviderSignature)
{
    const auto              requiredBufferSize = ::EnumSystemFirmwareTables(firmwareTableProviderSignature, NULL, 0);
    unsigned int            errorCode          = 0;
    std::unique_ptr<char[]> firmwareTableEnumBuffer;
    if (requiredBufferSize == 0)
    {
        errorCode = ::GetLastError();
    }
    else
    {
        firmwareTableEnumBuffer = std::make_unique<char[]>(requiredBufferSize);
        const auto ret          = ::EnumSystemFirmwareTables(firmwareTableProviderSignature, firmwareTableEnumBuffer.get(), requiredBufferSize);
        if (ret != requiredBufferSize)
        {
            errorCode = ::GetLastError();
        }
    }
    return FirmwareTableEnumerator(errorCode, firmwareTableEnumBuffer.get(), requiredBufferSize);
}

FirmwareTableEnumerator::FirmwareTableEnumerator(const unsigned int errorCode, const char* firmwareTableBuf, unsigned int bufSize)
    : _errorCode(errorCode)
{

    if (errorCode == 0) 
    {
        for (unsigned int i = 0; i < bufSize / 4; ++i)
        {
            const auto firmwareTableId = firmwareTableBuf + i * 4;
            _firmwareTableId.push_back(*firmwareTableId);
        }
    }
}
}
}