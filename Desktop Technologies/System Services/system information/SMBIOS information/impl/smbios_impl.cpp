#include "smbios_impl.h"
#include <Windows.h>
#include <memory>
#include <vector>
#include <map>

namespace system_service
{
namespace system_information
{
#pragma pack(push)
#pragma pack(1)
struct RawSMBIOSData
{
    uint8_t  used20CallingMethod;
    uint8_t  majorVersion;
    uint8_t  minorVersion;
    uint8_t  dmiRevision;
    uint32_t length;
    uint8_t  tableData[];
};


// BIOS 结构订阅参考文档:DSP0134_3.5.0.pdf
struct SMBIOSHeader
{
    uint8_t  type;
    uint8_t  length;
    uint16_t handle;
};

typedef struct BIOSInfo
{
    SMBIOSHeader header;
    uint8_t      vendor;                                // string
    uint8_t      version;                               // string
    uint16_t     startingAddressSegment;                // varies
    uint8_t      releaseDate;                           // string
    uint8_t      ROMSize;                               // varies(n)
    uint64_t     characteristics;                       // bit field
    uint8_t      characteristicsExtension[2];           // bit field. reserved for future supported
    uint8_t      systemMajorRelease;                    // varies
    uint8_t      systemMinorRelease;                    // varies
    uint8_t      embededControllerFirmWareMajorRelease; // varies
    uint8_t      embededControllerFirmwareMinorRelease; // varies
    uint16_t     extenedROMSize;                        // bit field
};

struct SystemInfo
{
    SMBIOSHeader header;
    uint8_t      manufacturer; // string
    uint8_t      productName;  // string
    uint8_t      version;      // string
    uint8_t      sn;           // string; serial number
    uint8_t      uuid[16];     // varies
    uint8_t      wakeupType;   // enum
    uint8_t      SKUNumber;    // string
    uint8_t      family;       //string
};

struct BaseboardInfo
{
    SMBIOSHeader header;
    uint8_t      manufacturer;         // string
    uint8_t      product;              // string
    uint8_t      version;              // string
    uint8_t      sn;                   // string
    uint8_t      assetTag;             // string
    uint8_t      featureFlags;         // bit field
    uint8_t      locationInChassis;    // string
    uint16_t     chassisHandle;        // varies
    uint8_t      boardType;            // enum
    uint8_t      numberOfObjectHandle; // varies
    uint16_t     objHanles[];          // varies
};
#pragma pack(pop)

std::string LocateString(const SMBIOSHeader* biosHeader, uint8_t offset)
{
    auto headerPointer = reinterpret_cast<char*>(const_cast<SMBIOSHeader*>(biosHeader));
    if (offset == 0)
    {
        return "Not Specified";
    }

    auto rawStr = headerPointer + biosHeader->length;
    while (offset > 1 && *rawStr)
    {
        rawStr += strlen(rawStr);
        rawStr++;
        offset--;
    }

    if (!*rawStr)
    {
        return "";
    }

    /* ASCII filtering */
    const auto len = strlen(rawStr);
    for (size_t i = 0; i < len; ++i)
    {
        if (rawStr[i] < 32 || rawStr[i] == 127)
        {
            rawStr[i] = '.';
        }
    }

    std::string str(rawStr, len);
    str.append("\0");

    return str;
}

SMBIOSImpl::SMBIOSImpl() : _majorVersion(0), _minorVersion(0), _dmiRevision(0)
{
}

SMBIOSImpl::~SMBIOSImpl()
{
}

bool SMBIOSImpl::Initialize()
{
    unsigned long requiredSize = 0u;
    requiredSize               = ::GetSystemFirmwareTable('RSMB', 0, nullptr, 0);
    if (requiredSize <= 0)
    {
        return false;
    }

    auto       firmwareTableBuf = std::make_unique<char[]>(requiredSize);
    const auto ret              = ::GetSystemFirmwareTable('RSMB', 0, firmwareTableBuf.get(), requiredSize);
    if (ret != requiredSize)
    {
        return false;
    }

    const auto smbios = reinterpret_cast<RawSMBIOSData*>(firmwareTableBuf.get());
    if (smbios == nullptr)
    {
        return false;
    }

    _majorVersion = static_cast<uint32_t>(smbios->majorVersion);
    _minorVersion = static_cast<uint32_t>(smbios->minorVersion);
    _dmiRevision  = static_cast<uint32_t>(smbios->dmiRevision);

    ParseSBMIOS(smbios);

    return true;
}

void SMBIOSImpl::ParseSBMIOS(void* data)
{
    const auto smbios         = reinterpret_cast<RawSMBIOSData*>(data);
    auto       tableDataStart = smbios->tableData;
    const auto tableDataEnd   = tableDataStart + smbios->length;
    for (;;)
    {
        // smbios header
        const auto smbiosHeader = reinterpret_cast<SMBIOSHeader*>(tableDataStart);
        const auto type         = smbiosHeader->type;
        const auto len          = smbiosHeader->length;
        if (type == 127 && len == 4)
        {
            break;
        }

        DispatchType(smbiosHeader);

        tableDataStart += smbiosHeader->length;
        while (0 != (*tableDataStart | *(tableDataStart + 1)))
        {
            tableDataStart++;
        }
        tableDataStart += 2;
        if (tableDataStart >= tableDataEnd)
            break;
    }
}

void SMBIOSImpl::DispatchType(void* header)
{
    const auto smbiosHeader = reinterpret_cast<SMBIOSHeader*>(header);
    const auto type         = smbiosHeader->type;
    const auto len          = smbiosHeader->length;

    struct ProcFunc
    {
        uint8_t type;
        void (*Proc)(SMBIOSImpl* const, void*);
    };

    static std::vector<ProcFunc> procFunc = {
        {0,      SMBIOSImpl::ProcessBIOSInfo},
        {1,    SMBIOSImpl::ProcessSystemInfo},
        {2, SMBIOSImpl::ProcessBaseboardInfo},
    };

    auto iter = std::find_if(procFunc.begin(), procFunc.end(), [type](const ProcFunc& fn) { return fn.type == type; });
    if (iter != procFunc.end())
    {
        iter->Proc(this, smbiosHeader);
    }
}

void SMBIOSImpl::ProcessBIOSInfo(SMBIOSImpl* const self, void* header)
{
    auto biosInfo          = reinterpret_cast<BIOSInfo*>(header);
    self->_biosVendor      = LocateString(&biosInfo->header, biosInfo->vendor);
    self->_biosVersion     = LocateString(&biosInfo->header, biosInfo->version);
    self->_biosReleaseDate = LocateString(&biosInfo->header, biosInfo->releaseDate);
}

void SMBIOSImpl::ProcessSystemInfo(SMBIOSImpl* const self, void* header)
{
    auto systemInfo           = reinterpret_cast<SystemInfo*>(header);
    self->_systemManufacturer = LocateString(&systemInfo->header, systemInfo->manufacturer);
    self->_systemProductName  = LocateString(&systemInfo->header, systemInfo->productName);
    self->_systemVersion      = LocateString(&systemInfo->header, systemInfo->version);
    self->_systemSN           = LocateString(&systemInfo->header, systemInfo->sn);
    self->_systemSKUNumber    = LocateString(&systemInfo->header, systemInfo->SKUNumber);
    self->_systemFamily       = LocateString(&systemInfo->header, systemInfo->family);
}

void SMBIOSImpl::ProcessBaseboardInfo(SMBIOSImpl* const self, void* header)
{
    auto baseboardInfo           = reinterpret_cast<BaseboardInfo*>(header);
    self->_baseboardManufacturer = LocateString(&baseboardInfo->header, baseboardInfo->manufacturer);
    self->_baseboardProduct      = LocateString(&baseboardInfo->header, baseboardInfo->product);
    self->_baseboardVersion      = LocateString(&baseboardInfo->header, baseboardInfo->version);
    self->_baseboardSN           = LocateString(&baseboardInfo->header, baseboardInfo->sn);
}

} // namespace system_information
} // namespace system_service
