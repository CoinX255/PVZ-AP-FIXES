#pragma once

#include "SlotData1_7.h"

class SlotData1_8 : public SlotData1_7
{
public:
    explicit SlotData1_8(nlohmann::json slot_data) : SlotData1_7(std::move(slot_data)) {}
    
    std::string version() override
    {
        return "1.8";
    }
};