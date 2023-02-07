#pragma once
#include <string>

namespace system_service
{
// https://learn.microsoft.com/en-us/windows/win32/sysinfo/system-information
namespace system_information
{
// https://learn.microsoft.com/en-us/windows/win32/api/sysinfoapi/nf-sysinfoapi-getsystemfirmwaretable
class SMBIOSImpl
{
public:
    SMBIOSImpl();
    ~SMBIOSImpl();

    bool              Initialize();
    uint32_t          GetMajorVersion() const { return _majorVersion; }
    uint32_t          GetMinorVersion() const { return _minorVersion; }
    uint32_t          GetDMIRevision() const { return _dmiRevision; }
    const std::string GetBIOSVendor() const { return _biosVendor; }
    const std::string GetBIOSVersion() const { return _biosVersion; }
    const std::string GetBIOSReleaseDate() const { return _biosReleaseDate; }
    const std::string GetSystemManufacturer() const { return _systemManufacturer; }
    const std::string GetSystemProductName() const { return _systemProductName; }
    const std::string GetSystemVersion() const { return _systemVersion; }
    const std::string GetSystemSN() const { return _systemSN; }
    const std::string GetSystemSKUNumber() const { return _systemSKUNumber; }
    const std::string GetSystemFamily() const { return _systemFamily; }
    const std::string GetBaseboardManufacturer() const { return _baseboardManufacturer; }
    const std::string GetBaseboardProduct() const { return _baseboardProduct; }
    const std::string GetBaseboardVersion() const { return _baseboardVersion; }
    const std::string GetBaseboardSN() const { return _baseboardSN; }


private:
    void        ParseSBMIOS(void* data);
    void        DispatchType(void* header);
    static void ProcessBIOSInfo(SMBIOSImpl* const self, void* header);
    static void ProcessSystemInfo(SMBIOSImpl* const self, void* header);
    static void ProcessBaseboardInfo(SMBIOSImpl* const self, void* header);
private:

    // SMBIOS version info
    uint32_t    _majorVersion;
    uint32_t    _minorVersion;
    uint32_t    _dmiRevision;
    // BIOS info
    std::string _biosVendor;
    std::string _biosVersion;
    std::string _biosReleaseDate;
    // system info
    std::string _systemManufacturer;
    std::string _systemProductName;
    std::string _systemVersion;
    std::string _systemSN;
    std::string _systemSKUNumber;
    std::string _systemFamily;
    // baseboard info
    std::string _baseboardManufacturer;
    std::string _baseboardProduct;
    std::string _baseboardVersion;
    std::string _baseboardSN;
};
} // namespace system_information
} // namespace system_service
