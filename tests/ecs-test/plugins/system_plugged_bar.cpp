//
// Created by roman Sztergbaum on 10/06/2018.
//

#include <boost/dll.hpp>
#include "system_plugged_bar.hpp"

namespace shiva::testing::plugins
{
    void bar_system::update() noexcept
    {

    }

    std::unique_ptr<shiva::ecs::base_system>
    bar_system::system_creator(shiva::entt::dispatcher &dispatcher,
                               shiva::entt::entity_registry &registry,
                               const float& fixed_delta_time) noexcept
    {
        return std::make_unique<bar_system>(dispatcher, registry, fixed_delta_time);
    }
}

BOOST_DLL_ALIAS(
    shiva::testing::plugins::bar_system::system_creator, // <-- this function is exported with...
    create_plugin                               // <-- ...this alias name
)