//
// Created by qdesmettre on 20/12/23.
//

#ifndef R_TYPE_BUTTONSYSTEM_HPP
#define R_TYPE_BUTTONSYSTEM_HPP

#include "aecs/SystemBase.hpp"

namespace rtype
{

    /**
     * @brief Sends inputs to server
     */
    class ButtonSystem : public aecs::ALogicSystem
    {
      public:
        ButtonSystem(aecs::World &world, const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities);

        ~ButtonSystem() override = default;

        /*
         * send inputs to server, via UDP
         */
        aecs::EntityChanges update(aecs::UpdateParams &updateParams) override;
    };
} // namespace rtype

#endif // R_TYPE_BUTTONSYSTEM_HPP
