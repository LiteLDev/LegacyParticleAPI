#pragma once

#include "ll/api/mod/NativeMod.h"

namespace legacy_particleapi {

class LegacyParticleAPI {

public:
    static LegacyParticleAPI& getInstance();

    LegacyParticleAPI() : mSelf(*ll::mod::NativeMod::current()) {}

    [[nodiscard]] ll::mod::NativeMod& getSelf() const { return mSelf; }

    bool load();

    bool enable();

    bool disable();

private:
    ll::mod::NativeMod& mSelf;
};

} // namespace legacy_particleapi
