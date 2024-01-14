//
// Created by qdesmettre on 14/01/24.
//

#ifndef R_TYPE_XPCOMPONENT_HPP
#define R_TYPE_XPCOMPONENT_HPP

#include "aecs/AbstractComponent.hpp"

namespace rtype {

    class XPComponent: public aecs::AbstractComponent {
    public:
        XPComponent() = default;
        ~XPComponent() override = default;

        std::vector<std::byte> encode() const override;
        void decode(const std::vector<std::byte> &data) override;

        const char *getName() const override {
            return "XPComponent";
        }

        [[nodiscard]] int getXp() const {
            return _xp;
        }
        [[nodiscard]] int getLevel() const {
            return _level;
        }
        [[nodiscard]] int getXpForLevelup() const {
            return _xpForLevelup;
        }

        void increaseXp(float xp) {
            _xp += xp;
            while (_xp >= _xpForLevelup) {
                _xp -= _xpForLevelup;
                _xpForLevelup += 10;
                _level++;
            }
        }

    private:
        float _xp = 0;
        float _xpForLevelup = 10;
        int _level = 1;
    };

}


#endif //R_TYPE_XPCOMPONENT_HPP
