#ifndef ENTITY_ENTITY_ID_HPP
#define ENTITY_ENTITY_ID_HPP

# include "entity/fwd.hpp"
# include <string>

namespace chips
{
    /// This is just an example of the identifiers you may use
    enum class entity_id
    {
        hero, 
        monster,
        villager, 
        wall, 
        dummy, 
        BAD_ID
    };
    
    inline std::string to_string(entity_id id)
    {
        switch (id)
        {
            case entity_id::hero:
                return "hero";
            case entity_id::monster:
                return "monster";
            case entity_id::villager:
                return "villager";
            case entity_id::wall:
                return "wall";
            case entity_id::dummy:
                return "dummy";
            case entity_id::BAD_ID:
                return "BAD_ID";
        }
    }
}                                                           // namespace chips
#endif /* ENTITY_ENTITY_ID_HPP */