#pragma once

#include "SlotData1_8.h"

class SlotData1_9 : public SlotData1_8
{
public:
    explicit SlotData1_9(nlohmann::json slot_data) : SlotData1_8(std::move(slot_data)) {}
    
    std::string version() override
    {
        return "1.9";
    }
    
    bool requires_replanted() override
    {
        return slot_data["requires_replanted"].get<int>();
    }
};