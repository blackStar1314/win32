#include "smbios.h"
#include "impl/smbios_impl.h"

namespace system_service
{
namespace system_information
{
SMBIOS::SMBIOS() : _impl(new SMBIOSImpl())
{
}
SMBIOS::~SMBIOS()
{
    if (_impl)
    {
        delete _impl;
        _impl = nullptr;
    }
}
bool SMBIOS::Initialize()
{
    return _impl->Initialize();
}
uint32_t SMBIOS::GetMajorVersion() const
{
    return _impl->GetMajorVersion();
}
uint32_t SMBIOS::GetMinorVersion() const
{
    return _impl->GetMinorVersion();
}
uint32_t SMBIOS::GetDMIRevision() const
{
    return _impl->GetDMIRevision();
}
const std::string SMBIOS::GetBIOSVendor() const
{
    return _impl->GetBIOSVendor();
}
const std::string SMBIOS::GetBIOSVersion() const
{
    return _impl->GetBIOSVersion();
}
const std::string SMBIOS::GetBIOSReleaseDate() const
{
    return _impl->GetBIOSReleaseDate();
}
const std::string SMBIOS::GetSystemManufacturer() const
{
    return _impl->GetSystemManufacturer();
}
const std::string SMBIOS::GetSystemProductName() const
{
    return _impl->GetSystemProductName();
}
const std::string SMBIOS::GetSystemVersion() const
{
    return _impl->GetSystemVersion();
}
const std::string SMBIOS::GetSystemSN() const
{
    return _impl->GetSystemSN();
}
const std::string SMBIOS::GetSystemSKUNumber() const
{
    return _impl->GetSystemSKUNumber();
}
const std::string SMBIOS::GetSystemFamily() const
{
    return _impl->GetSystemFamily();
}
const std::string SMBIOS::GetBaseboardManufacturer() const
{
    return _impl->GetBaseboardManufacturer();
}
const std::string SMBIOS::GetBaseboardProduct() const
{
    return _impl->GetBaseboardProduct();
}
const std::string SMBIOS::GetBaseboardVersion() const
{
    return _impl->GetBaseboardVersion();
}
const std::string SMBIOS::GetBaseboardSN() const
{
    return _impl->GetBaseboardVersion();
}
}
}