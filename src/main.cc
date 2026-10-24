#include "hooks.cpp"
#include "JniHook.cpp"
#include <type_traits>

#if GDPS == GDPS_NEOPOINTFOUR && GAME_VERSION != GV_1_4
    #error "Building for Neopointfour while GAME_VERSION is not 1.4"
#endif
#if GDPS == GDPS_1_7 && GAME_VERSION != GV_1_7
    #error "Building for 1.7 GDPS while GAME_VERSION is not 1.7"
#endif

#if defined(EXTRA_COLORS) && GAME_VERSION != GV_1_3 && GAME_VERSION != GV_1_4
    #error "Building with EXTRA_COLORS while GAME_VERSION is not 1.3 or 1.4"
#endif

void load_debugger()
{
    void* handle = dlopen("libfrida-gadget.so", RTLD_NOW);
    if (!handle) 
        dlopen("/data/app/com.antimatter.gj181menu/lib/arm/libfrida-gadget.so", RTLD_NOW);
}

[[gnu::constructor]]
int main() {
    initialize_hooks();
#if GAME_VERSION < GV_1_7
    DobbyCodePatch(
        reinterpret_cast<void*>(get_address(search_query_length_fix)),
        std::vector<uint8_t>(
        #if GAME_VERSION > GV_1_5
            {0x00, 0xe0}
        #else
            {0x05, 0xe0}
        #endif
        ).data(), 2
    );
#endif

#if GAME_VERSION > GV_1_7
    DobbyCodePatch(
        reinterpret_cast<void*>(get_address(lel_unschedule_update)),
        std::vector<uint8_t>(
        {0x00, 0xbf, 0x00, 0xbf}
        ).data(), 4
    );

    DobbyCodePatch(
        reinterpret_cast<void*>(get_address(lel_unschedule_update2)),
        std::vector<uint8_t>(
        {0x00, 0xbf, 0x00, 0xbf}
        ).data(), 4
    );
#endif
}
