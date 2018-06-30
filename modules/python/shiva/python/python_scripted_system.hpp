//
// Created by roman Sztergbaum on 30/06/2018.
//

#pragma once

#include <shiva/ecs/system.hpp>
#include <shiva/filesystem/filesystem.hpp>
#include <pybind11/pybind11.h>
#include <shiva/event/destruct_callback_scripted_systems.hpp>

namespace shiva::ecs
{
    template <typename SystemType>
    class python_scripted_system : public system<python_scripted_system<SystemType>, SystemType>
    {
    public:
        using TSystem = system <python_scripted_system<SystemType>, SystemType>;

        python_scripted_system(shiva::entt::dispatcher &dispatcher,
                               shiva::entt::entity_registry &entity_registry,
                               const float &fixed_delta_time,
                               pybind11::module &module,
                               std::string table_name,
                               std::string class_name) noexcept :
            TSystem::system(dispatcher, entity_registry, fixed_delta_time),
            module_(module),
            table_name_(std::move(table_name))
        {
            this->dispatcher_.template sink<shiva::event::destruct_callback_scripted_systems>().connect(this);
            class_name_ = std::move(class_name);
            safe_function("on_construct");
        }

        ~python_scripted_system() noexcept override = default;

        void receive([[maybe_unused]] const shiva::event::destruct_callback_scripted_systems &evt)
        {
            safe_function("on_destruct");
        }

        void update() noexcept override
        {
            safe_function("update");
        }

        static const std::string &class_name() noexcept
        {
            return class_name_;
        }

        static constexpr auto reflected_functions() noexcept
        {
            return meta::makeMap();
        }

        static constexpr auto reflected_members() noexcept
        {
            return meta::makeMap();
        }

    private:
        template <typename ... Args>
        void safe_function(const std::string &function, Args &&... args)
        {
            try {
                module_.attr(table_name_.c_str()).attr(function.c_str())(std::forward<Args>(args)...);
            }
            catch (const std::exception &error) {
                std::cerr << error.what() << std::endl;
            }
        }

        pybind11::module &module_;
        std::string table_name_;
        static inline std::string class_name_{""};
    };

    using python_post_scripted_system = python_scripted_system<shiva::ecs::system_post_update>;
    using python_pre_scripted_system = python_scripted_system<shiva::ecs::system_pre_update>;
    using python_logic_scripted_system = python_scripted_system<shiva::ecs::system_logic_update>;
}