#include <iostream>
#include "hardware_information.h"


int main()
{

    {
        const auto info = system_service::system_information::HardwareInformation::Get();

        std::cout << info.GetOEMId() << ", " << info.GetPageSize() << ", " << info.GetActiveProcessorMask() << ", " << info.GetNumberOfProcessors()
                  << ", " << info.GetProcessorType() << ", " << info.GetAllocationGranularity() << ", " << info.GetProcessorLevel() << ", "
                  << info.GetProcessorRevision() << "\n";
    }

    return 0;
}