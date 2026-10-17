#include "../hook.hpp"

CCNode* (*TRAM_GJUserScore_create)(CCDictionary* dict);
CCNode* GJUserScore_create(CCDictionary* dict) {
    auto score = TRAM_GJUserScore_create(dict);

    if (dict) {
        auto udidVal = dict->valueForKey("7");
        const char* udid = udidVal ? udidVal->getCString() : nullptr;

        if (udid && strcmp(udid, getPlayerUDID().c_str()) == 0) {
            auto idVal = dict->valueForKey("2");
            int userID = idVal ? idVal->intValue() : 0;

            if (userID != 0 && userID != HaxManager::sharedState().savedUserID) {
                HaxManager::sharedState().savedUserID = userID;
            }
        }
    }

    return score;
}

void NP4_GJUserScore_om() {
    Omni::hook("_ZN11GJUserScore6createEPN7cocos2d12CCDictionaryE",
        reinterpret_cast<void*>(GJUserScore_create),
        reinterpret_cast<void**>(&TRAM_GJUserScore_create));
}