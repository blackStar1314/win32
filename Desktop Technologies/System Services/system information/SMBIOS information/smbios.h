#pragma once
#include <string>

namespace system_service
{
namespace system_information
{
class SMBIOSImpl;
class SMBIOS
{
public:

    SMBIOS();
    ~SMBIOS();

    bool              Initialize();
    uint32_t          GetMajorVersion() const; 
    uint32_t          GetMinorVersion() const;
    uint32_t          GetDMIRevision() const; 
    const std::string GetBIOSVendor() const; 
    const std::string GetBIOSVersion() const; 
    const std::string GetBIOSReleaseDate() const; 
    const std::string GetSystemManufacturer() const;
    const std::string GetSystemProductName() const;
    const std::string GetSystemVersion() const;
    const std::string GetSystemSN() const;
    const std::string GetSystemSKUNumber() const;
    const std::string GetSystemFamily() const;
    const std::string GetBaseboardManufacturer() const;
    const std::string GetBaseboardProduct() const;
    const std::string GetBaseboardVersion() const;
    const std::string GetBaseboardSN() const;
private:

    SMBIOSImpl* _impl;
};
}
}
