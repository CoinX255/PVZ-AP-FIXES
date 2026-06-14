#pragma once

#include "SlotData1_5.h"

class SlotData1_6 : public SlotData1_5
{
public:
    explicit SlotData1_6(nlohmann::json slot_data) : SlotData1_5(std::move(slot_data)) {}
    
    std::string version() override
    {
        return "1.6";
    }
    
    PVZRAPData::SlotData::GoalProgress goal_requirements() override
    {
        PVZRAPData::SlotData::GoalProgress gp;
	
        gp.adventure_levels_goal = slot_data["adventure_levels_goal"];
        gp.adventure_areas_goal = slot_data["adventure_areas_goal"];
        gp.minigame_levels_goal = slot_data["minigame_levels_goal"];
        gp.puzzle_levels_goal = slot_data["puzzle_levels_goal"];
        gp.survival_levels_goal = slot_data["survival_levels_goal"];
        gp.overall_levels_goal = slot_data["overall_levels_goal"];
        gp.taco_goal = slot_data["taco_goal"];
        
        return gp;
    }
    
    std::optional<std::map<SeedType, int>> conveyor_seeds_for_level(int level) override
    {
        auto conveyor_map = slot_data["conveyor_map"];
        auto level_data = conveyor_map[std::to_string(level)];
        if (level_data.is_null())
        {
            return {};
        }
        
        auto available_plants_for_level = level_data["weights"];

        std::map<SeedType, int> result;
        for (const auto& [plant_json, weight_json] : available_plants_for_level.items())
        {
            auto seed = static_cast<SeedType>(std::stoi(plant_json));
            result.insert_or_assign(seed, weight_json.get<int>());
        }
        return result;
    }
    
    std::optional<std::vector<SeedType>> conveyor_order_for_level(int level) override
    {
        auto conveyor_map = slot_data["conveyor_map"];
        auto level_data = conveyor_map[std::to_string(level)];
        if (level_data.is_null())
        {
            return {};
        }
        
        auto available_plants_for_level = level_data["default"];
        
        std::vector<SeedType> result;
        for (const auto& plant_id : available_plants_for_level)
        {
            auto seed = static_cast<SeedType>(plant_id.get<int>());
            result.push_back(seed);
        }
        return result;
    }
    
    bool energylink_enabled() override
    {
        return slot_data["energylink_enabled"].get<int>() == 1;
    }
};