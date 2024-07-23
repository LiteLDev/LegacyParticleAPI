#include "Plugin.h"
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
    this->addCustomResourcePackPath("./plugins/LegacyParticleAPI/ResourcePacks", PackType::Resources);
    origin();
}

namespace legacy_particleapi {

static std::unique_ptr<LegacyParticleAPI> instance;

LegacyParticleAPI& LegacyParticleAPI::getInstance() { return *instance; }

bool LegacyParticleAPI::load() {
    getSelf().getLogger().info("Loading...");
    // Code for loading the plugin goes here.
    ResourceInitHook::hook();
    return true;
}

bool LegacyParticleAPI::enable() {
    getSelf().getLogger().info("Enabling...");
    // Code for enabling the plugin goes here.
    return true;
}

bool LegacyParticleAPI::disable() {
    getSelf().getLogger().info("Disabling...");
    // Code for disabling the plugin goes here.
    return true;
}

} // namespace legacy_particleapi

LL_REGISTER_MOD(legacy_particleapi::LegacyParticleAPI, legacy_particleapi::instance);
