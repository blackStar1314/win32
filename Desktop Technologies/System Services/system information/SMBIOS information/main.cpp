#include <iostream>
#include "firmware_table_enumerator.h"
#include "smbios.h"

using namespace system_service::system_information;


int main()
{
    const auto enumerator = FirmwareTableEnumerator::Get('RSMB');
    std::cerr << "enumerator ec : " << enumerator.GetErrorCode() << "\n";
    if (enumerator.IsVaild())
    {
        std::cerr << "enumerator size : " << enumerator.GetFirmwareTableId().size() << "\n";
    }

    SMBIOS smbios;
    const auto init = smbios.Initialize();

    if (init)
    {
        // smbios info
        std::cerr << "SMBIOS info :\n major version : " <<  smbios.GetMajorVersion() << ", minor version : " << smbios.GetMinorVersion() << ", dmi revision : " << smbios.GetDMIRevision() << "\n"
                  << " bios info :\n BIOS vendor :  " << smbios.GetBIOSVendor() << ", BIOS version : " << smbios.GetBIOSVersion() << ", BIOS release date : " << smbios.GetBIOSReleaseDate() << "\n"
                  << " system info :\n manufacturer : " << smbios.GetSystemManufacturer() << ", product name : " << smbios.GetSystemProductName() << ", version : " << smbios.GetSystemVersion()
                  << ", SN : " << smbios.GetSystemSN() << ", SKU number : " << smbios.GetSystemSKUNumber() << ", family : " << smbios.GetSystemFamily() << "\n"
                  << " baseboard info :\n manufacturer : " << smbios.GetBaseboardManufacturer() << ", product : " << smbios.GetBaseboardProduct() << ", version : "
                  << smbios.GetBaseboardVersion() << ", SN : " << smbios.GetBaseboardSN() << "\n";
    }



    return 0;
}