#include <ll/api/plugin/NativePlugin.h>

#include "Plugin.h"
#include "ll/api/memory/Hook.h"
#include "mc/resources/ResourcePackRepository.h"

namespace plugin {

LL_AUTO_TYPE_INSTANCE_HOOK(
    ResourceInitHook,
    ll::memory::HookPriority::Normal,
    ResourcePackRepository,
    &ResourcePackRepository::_initialize,
    void
) {
    this->setCustomResourcePackPath("./plugins/LegacyParticleAPI/ResourcePacks", PackType::Resources);
    origin();
}

Plugin::Plugin(ll::plugin::NativePlugin& self) : mSelf(self) {
    ResourceInitHook::hook();
    mSelf.getLogger().info("Loaded");
}

bool Plugin::enable() { return true; }

bool Plugin::disable() { return true; }

} // namespace plugin
