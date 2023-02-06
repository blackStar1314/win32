#include <iostream>
#include "firmware_table_enumerator.h"

using namespace system_service::system_information;

int main()
{
    const auto enumerator = FirmwareTableEnumerator::Get('RSMB');
    std::cerr << "enumerator ec : " << enumerator.GetErrorCode() << "\n";
    if (enumerator.IsVaild())
    {
        std::cerr << "enumerator size : " << enumerator.GetFirmwareTableId().size() << "\n";
    }

    return 0;
}