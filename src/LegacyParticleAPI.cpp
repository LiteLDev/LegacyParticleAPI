#include "LegacyParticleAPI.h"
#include "ll/api/memory/Hook.h"
#include "mc/resources/IRepositoryFactory.h"
#include "mc/resources/ResourcePackRepository.h"

#include "ll/api/mod/RegisterHelper.h"

LL_AUTO_TYPE_INSTANCE_HOOK(
    ResourceInitHook,
    ll::memory::HookPriority::Normal,
    ResourcePackRepository,
    &ResourcePackRepository::$ctor,
    void*,
    ::gsl::not_null<::std::shared_ptr<::RepositoryPacks>>                 repositoryPacks,
    ::PackManifestFactory&                                                manifestFactory,
    ::Bedrock::NotNullNonOwnerPtr<::IContentAccessibilityProvider> const& contentAccessibility,
    ::Bedrock::NotNullNonOwnerPtr<::Core::FilePathManager> const&         pathManager,
    ::Bedrock::NonOwnerPointer<::PackCommand::IPackCommandPipeline>       commands,
    ::PackSourceFactory&                                                  packSourceFactory,
    bool                                                                  initAsync,
    ::std::unique_ptr<::IRepositoryFactory>                               factory
) {
    void* ori = origin(
        repositoryPacks,
        manifestFactory,
        contentAccessibility,
        pathManager,
        commands,
        packSourceFactory,
        initAsync,
        std::move(factory)
    );
    this->addCustomResourcePackPath(
        legacy_particleapi::LegacyParticleAPI::getInstance().getSelf().getModDir() / "ResourcePacks",
        PackType::Resources
    );
    return ori;
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
