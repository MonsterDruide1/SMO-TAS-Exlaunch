#include "game/StageScene/StageScene.h"
#include "game/Player/PlayerTrigger.h"
#include "game/Player/PlayerActorHakoniwa.h"
#include "lib.hpp"
#include "logger/Logger.hpp"
#include "rs/util.hpp"

static StageScene* s_scene = nullptr;

void damagePlayer() {
    if(s_scene != nullptr) {
        auto player = reinterpret_cast<PlayerActorHakoniwa*>(rs::getPlayerActor(s_scene));
        player->mDamageKeeper->damage(1);
    }
}

HOOK_DEFINE_TRAMPOLINE(ControlHook) {
    static void Callback(StageScene* scene) {
        Orig(scene);
        s_scene = scene;
    }
};

HOOK_DEFINE_TRAMPOLINE(PlayerTriggerSetCollisionHook) {
    static void Callback(PlayerTrigger* trigger, PlayerTrigger::ECollisionTrigger type) {
        Orig(trigger, type);
        Logger::log("PlayerTrigger::set(CollisionTrigger=%u)\n", type);
    }
};
HOOK_DEFINE_TRAMPOLINE(PlayerTriggerSetActionHook) {
    static void Callback(PlayerTrigger* trigger, PlayerTrigger::EActionTrigger type) {
        Orig(trigger, type);
        if(u8(type) == 10) {
            damagePlayer();
        }
    }
};

extern "C" void exl_main(void* x0, void* x1) {
    /* Setup hooking enviroment. */
    envSetOwnProcessHandle(exl::util::proc_handle::Get());
    exl::hook::Initialize();

    R_ABORT_UNLESS(Logger::instance().init(LOGGER_IP, 3080).value);
    exl::patch::CodePatcher(0x95C498).WriteInst(exl::armv8::inst::Nop());

    ControlHook::InstallAtSymbol("_ZN10StageScene7controlEv");
    PlayerTriggerSetCollisionHook::InstallAtSymbol("_ZN13PlayerTrigger3setENS_17ECollisionTriggerE");
    PlayerTriggerSetActionHook::InstallAtSymbol("_ZN13PlayerTrigger3setENS_14EActionTriggerE");
}

extern "C" NORETURN void exl_exception_entry() {
    /* TODO: exception handling */
    EXL_ABORT(0x420);
}
