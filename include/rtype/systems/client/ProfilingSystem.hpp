//
// Created by qdesmettre on 06/01/24.
//

#ifndef R_TYPE_PROFILINGSYSTEM_HPP
#define R_TYPE_PROFILINGSYSTEM_HPP

#include "aecs/SystemBase.hpp"
#include <functional>

namespace rtype {

    class ProfilingSystem: public aecs::ALogicSystem {
    public:
        ProfilingSystem(aecs::World &world, const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities);
        ~ProfilingSystem() override = default;

        aecs::EntityChanges update(aecs::UpdateParams &updateParams) override;

        typedef std::function<std::pair<std::string, std::string>(aecs::World &)> ProfilingFunction;
        void addProfiling(const ProfilingFunction &function);

    private:
        std::map<ProfilingFunction, aecs::EntityPtr> _profilers;
    };
}


#endif //R_TYPE_PROFILINGSYSTEM_HPP
