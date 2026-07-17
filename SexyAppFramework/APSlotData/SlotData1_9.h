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
    
    bool lock_conveyor() override
    {
        return slot_data["lock_conveyor_plants"].get<int>();
    }
    
    bool lock_vasebreaker() override
    {
        return slot_data["lock_vasebreaker_plants"].get<int>();
    }
    
    bool lock_izombie() override
    {
        return slot_data["lock_izombie_zombies"].get<int>();
    }
    
    bool lawnlink_enabled() override
    {
        return slot_data["lawnlink_enabled"].get<int>();
    }
    
    bool seedlink_enabled() override
    {
        return slot_data["seedlink_enabled"].get<int>();
    }
};