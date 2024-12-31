#include "LegacyParticleAPI.h"
#include "ll/api/memory/Hook.h"
#include "mc/resources/ResourcePackRepository.h"

#include "ll/api/mod/RegisterHelper.h"

LL_AUTO_TYPE_INSTANCE_HOOK(
    ResourceInitHook,
    ll::memory::HookPriority::Normal,
    ResourcePackRepository,
    &ResourcePackRepository::_initialize,
    void
) {
    this->addCustomResourcePackPath(
        legacy_particleapi::LegacyParticleAPI::getInstance().getSelf().getModDir() / "ResourcePacks",
        PackType::Resources
    );
    origin();
}

namespace legacy_particleapi {

LegacyParticleAPI& LegacyParticleAPI::getInstance() {
    static LegacyParticleAPI instance;
    return instance;
}

bool LegacyParticleAPI::load() {
    ResourceInitHook::hook();
    return true;
}

bool LegacyParticleAPI::enable() { return true; }

bool LegacyParticleAPI::disable() { return true; }

} // namespace legacy_particleapi

LL_REGISTER_MOD(legacy_particleapi::LegacyParticleAPI, legacy_particleapi::LegacyParticleAPI::getInstance());
