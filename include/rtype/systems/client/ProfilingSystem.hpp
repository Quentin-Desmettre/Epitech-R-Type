//
// Created by qdesmettre on 06/01/24.
//

#ifndef R_TYPE_PROFILINGSYSTEM_HPP
#define R_TYPE_PROFILINGSYSTEM_HPP

#include "aecs/SystemBase.hpp"
#include <functional>

namespace rtype
{

    class ProfilingSystem : public aecs::ALogicSystem
    {
      public:
        ProfilingSystem(aecs::World &world, const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities,
                        float timeBetweenUpdate = 0.5f);
        ~ProfilingSystem() override = default;

        aecs::EntityChanges update(aecs::UpdateParams &updateParams) override;

        typedef std::string (*ProfilingFunction)(aecs::World &);
        ProfilingSystem &addProfiling(ProfilingFunction function, const std::string &name);
        ProfilingSystem &clear();

      private:
        std::map<aecs::Entity *, std::pair<std::string, ProfilingFunction>> _profilers;
        float _timeBetweenUpdate;
        float _elapsedTime;
    };
} // namespace rtype

#endif // R_TYPE_PROFILINGSYSTEM_HPP
