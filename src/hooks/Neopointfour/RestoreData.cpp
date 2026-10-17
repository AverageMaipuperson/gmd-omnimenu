#ifndef NP4_RESTOREDATA_CPP
#define NP4_RESTOREDATA_CPP

// #include "../hook.hpp"
#include "../../version/VersionUtils.hpp"

int getCompletedLevelAmount() {
    auto GSM = GameStatsManager::sharedState();
    auto completedDict = getCompletedDict(GSM);
    
    cocos2d::CCArray* keys = completedDict->allKeys();
    if (!keys) return 0;
    
    int count = 0;
    for (unsigned int i = 0; i < keys->count(); i++) {
        cocos2d::CCString* key = static_cast<cocos2d::CCString*>(keys->objectAtIndex(i));
        if (!key) continue;
        
        const char* keyStr = key->getCString();
        if (keyStr[0] == 'c' && keyStr[1] == '_') {
            int levelID = atoi(keyStr + 2);
            if (levelID > 0) {
                count++;
            }
        }
    }
    
    return count;
}

struct RestoreData {
    std::pair<int,int> mainLevels[12];
    int stars;
    int demons;
    int totalJumps;
    int totalAttempts;
    std::string completedLvls;
    std::string userName;
    int icon;
    int color1;
    int color2;
    int ship;
    int lastUpdated;
    int userID;
};

RestoreData parseRestoreResponse(const std::string& response) {
    RestoreData data = {};
    std::map<int, std::string> fields;
    std::istringstream ss(response);
    std::string token;
    std::vector<std::string> tokens;
    while (std::getline(ss, token, ':')) {
        tokens.push_back(token);
    }
    
    for (size_t i = 0; i + 1 < tokens.size(); i += 2) {
        int key = stoi(tokens[i]);
        fields[key] = tokens[i + 1];
    }
    
    if (fields.count(1)) {
        std::istringstream lvlss(fields[1]);
        std::string pair;
        int idx = 0;
        while (std::getline(lvlss, pair, ';') && idx < 12) {
            auto comma = pair.find(',');
            if (comma != std::string::npos) {
                data.mainLevels[idx].first = stoi(pair.substr(0, comma));
                data.mainLevels[idx].second = stoi(pair.substr(comma + 1));
            }
            idx++;
        }
    }
    
    if (fields.count(2)) {
        data.stars = stoi(fields[2]);
    }
    if (fields.count(3)) {
        data.demons = stoi(fields[3]);
    }
    if (fields.count(4)) {
        data.totalJumps = stoi(fields[4]);
    }
    if (fields.count(5)) {
        data.totalAttempts = stoi(fields[5]);
    }
    if (fields.count(6)) {
        data.completedLvls = fields[6];
    }
    if (fields.count(7)) {
        data.userName = fields[7];
    }
    if (fields.count(8)) {
        data.icon = stoi(fields[8]);
    }
    if (fields.count(9)) {
        data.color1 = stoi(fields[9]);
    }
    if (fields.count(10)) {
        data.color2 = stoi(fields[10]);
    }
    if (fields.count(11)) {
        data.ship = stoi(fields[11]);
    }
    if (fields.count(12)) {
        data.lastUpdated = stoi(fields[12]);
    }
    if (fields.count(13)) {
        data.userID = stoi(fields[13]);
    }
    
    return data;
}

static std::string timeToString(time_t input) {
    struct tm timeInfo;
    localtime_r(&input, &timeInfo);
    char buffer[64];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M", &timeInfo);
    return std::string(buffer);
}

static void restoreMainLevels(const RestoreData& backup) {
    auto glm = GameLevelManager::sharedState();
    auto GSM = GameStatsManager::sharedState();
    auto gam = GameManager::sharedState();
    auto completedDict = getCompletedDict(GSM);

    for (int i = 1; i < 13; i++) {
        GJGameLevel* level = glm->getMainLevel(i);
        if (!level) continue;
        
        if (backup.mainLevels[i - 1].first == 1) {
            level->m_nNormalPercent = 100;
            level->m_bIsVerified = true;

            char keyStr[64];
            sprintf(keyStr, "n_%i", i);
            completedDict->setObject(cocos2d::CCString::createWithFormat("%i", 1), std::string(keyStr));

            sprintf(keyStr, "star_%i", i);
            completedDict->setObject(cocos2d::CCString::createWithFormat("%i", 1), std::string(keyStr));
            
            auto achID = cocos2d::CCString::createWithFormat("geometry.ach.level%02db", i)->getCString();
            gam->reportAchievementWithID(achID, 100, false);

        }
        if (backup.mainLevels[i - 1].second == 1) {
            level->m_nPracticePercent = 100;
            auto achID = cocos2d::CCString::createWithFormat("geometry.ach.level%02da", i)->getCString();
            gam->reportAchievementWithID(achID, 100, false);
        }
    }
}

static void restoreOnlineLevelsSaved(const std::vector<int>& backupIDs) {
    auto glm = GameLevelManager::sharedState();
    auto savedLevels = getSavedLevels(glm);
    if (!savedLevels) return;
    
    cocos2d::CCArray* keys = savedLevels->allKeys();
    if (!keys) return;
    
    for (unsigned int i = 0; i < keys->count(); i++) {
        cocos2d::CCString* key = static_cast<cocos2d::CCString*>(keys->objectAtIndex(i));
        if (!key) continue;
        
        int levelID = atoi(key->getCString());
        
        bool inBackup = false;
        for (int id : backupIDs) {
            if (id == levelID) {
                inBackup = true;
                break;
            }
        }
        if (!inBackup) continue;
        
        cocos2d::CCObject* obj = savedLevels->objectForKey(key->getCString());
        GJGameLevel* level = dynamic_cast<GJGameLevel*>(obj);
        if (!level) continue;
        
        level->m_nNormalPercent = 100;
        level->m_bIsVerified = true;
    }
}

static void restoreCompletedDict(const std::vector<int>& backupIDs) {
    auto GSM = GameStatsManager::sharedState();
    auto completedDict = getCompletedDict(GSM);
    
    for (int id : backupIDs) {
        char keyStr[64];
        sprintf(keyStr, "c_%i", id);
        auto val = cocos2d::CCString::createWithFormat("%i", 1);
        completedDict->setObject(val, std::string(keyStr));
    }
}

static void restoreAchievements() {
    auto gam = GameManager::sharedState();
    gam->reportAchievementWithID("geometry.ach.moreGames", 100, false);
    gam->reportAchievementWithID("geometry.ach.facebook", 100, false);
    gam->reportAchievementWithID("geometry.ach.rate", 100, false);
    gam->reportAchievementWithID("geometry.ach.like", 100, false);
    gam->reportAchievementWithID("geometry.ach.submit", 100, false);
    gam->reportAchievementWithID("geometry.ach.rateDiff", 100, false);
}

static void saveAndQuit() {
    auto gman = GameManager::sharedState();
    gman->save();
    cocos2d::CCDirector::sharedDirector()->end();
}

static void restoreIcons(const RestoreData& backup) {
    auto gman = GameManager::sharedState();
    
    gman->setPlayerFrame(backup.icon);
    gman->setPlayerShip(backup.ship);
    gman->setPlayerColor(backup.color1);
    gman->setPlayerColor2(backup.color2);
    gman->setPlayerName(backup.userName);
    
    gman->setClickedGarage(true);
    gman->setClickedEditor(true);
    
    if(backup.userName != "Player"){
        gman->setClickedName(true);
    }
}

static void awardProgress(const char* achID, int current, int target) {
    int pct = (int)(((float)current / target) * 100.f);
    if (pct > 100) pct = 100;
    if (pct > 0) GameManager::sharedState()->reportAchievementWithID(achID, pct, false);
}

static void restoreStatCounts(const RestoreData& backup) {
    auto GSM = GameStatsManager::sharedState();
    
    int completedMainCount = 0;
    auto glm = GameLevelManager::sharedState();
    for (int i = 0; i < 12; i++) {
        GJGameLevel* level = glm->getMainLevel(i + 1);
        if (!level) continue;
        
        if (level->m_nNormalPercent == 100) {
            completedMainCount++;
        }
    }
    
    auto statsDict = getStatsDict(GSM);
    statsDict->setObject(cocos2d::CCString::createWithFormat("%i", completedMainCount), "3");
    
    int completedOnlineCount = getCompletedLevelAmount();
    statsDict->setObject(cocos2d::CCString::createWithFormat("%i", completedOnlineCount), "4");

    // these get overwritten by statsRecalc, but they're set here for the sake of achievements
    int demonCount = backup.demons;
    statsDict->setObject(cocos2d::CCString::createWithFormat("%i", demonCount), "5");

    awardProgress("geometry.ach.demon01", demonCount, 1);
    awardProgress("geometry.ach.demon02", demonCount, 2);
    awardProgress("geometry.ach.demon03", demonCount, 3);
    awardProgress("geometry.ach.demon04", demonCount, 4);
    awardProgress("geometry.ach.demon05", demonCount, 5);

    int starCount = backup.stars;
    statsDict->setObject(cocos2d::CCString::createWithFormat("%i", starCount), "6");

    awardProgress("geometry.ach.stars01", starCount, 100);
    awardProgress("geometry.ach.stars02", starCount, 200);
    awardProgress("geometry.ach.stars03", starCount, 300);
    awardProgress("geometry.ach.stars04", starCount, 400);
    awardProgress("geometry.ach.stars05", starCount, 500);
    awardProgress("geometry.ach.stars06", starCount, 600);
    
    awardProgress("geometry.ach.custom01", completedOnlineCount, 1);
    awardProgress("geometry.ach.custom02", completedOnlineCount, 10);
}

void restore(const RestoreData& backup) {
    std::vector<int> backupIDs;
    {
        std::istringstream ss(backup.completedLvls);
        std::string token;
        while (std::getline(ss, token, ',')) {
            if (!token.empty()) {
                int id = atoi(token.c_str());
                if (id > 0) backupIDs.push_back(id);
            }
        }
    }
    
    auto GSM = GameStatsManager::sharedState();
    auto statsDict = getStatsDict(GSM);
    auto currentJumps = statsDict->valueForKey("1")->intValue();
    auto currentAttempts = statsDict->valueForKey("2")->intValue();
    int totalJumps = currentJumps;
    int totalAttempts = currentAttempts;
    
    if (backup.totalJumps > currentJumps) {
        totalJumps += backup.totalJumps;
        auto s = cocos2d::CCString::createWithFormat("%i", totalJumps);
        statsDict->setObject(s, "1");
    }
    if (backup.totalAttempts > currentAttempts) {
        totalAttempts += backup.totalAttempts;
        auto s = cocos2d::CCString::createWithFormat("%i", totalAttempts);
        statsDict->setObject(s, "2");
    }

    auto gam = GameManager::sharedState();
    awardProgress("geometry.ach.jump01", totalJumps, 1000);
    awardProgress("geometry.ach.jump02", totalJumps, 10000);
    awardProgress("geometry.ach.jump03", totalJumps, 20000);
    awardProgress("geometry.ach.attempt01", totalAttempts, 100);
    awardProgress("geometry.ach.attempt02", totalAttempts, 500);
    
    restoreMainLevels(backup);
    restoreOnlineLevelsSaved(backupIDs);
    restoreCompletedDict(backupIDs);
    restoreStatCounts(backup);
    restoreAchievements();
    restoreIcons(backup);
}

// we love handlers

struct QuitHandler : public cocos2d::CCObject, public FLAlertLayerProtocol {
    void FLAlert_Clicked(FLAlertLayer*, bool) override {
        saveAndQuit();
        release();
    }
};

struct RestoreHandler : public cocos2d::CCObject, public FLAlertLayerProtocol {
    RestoreData backupData;
    
    void FLAlert_Clicked(FLAlertLayer* layer, bool btn2) override {
        if (!btn2) {
            release();
            return;
        }
        
        restore(backupData);
        
        auto* quitHandler = new QuitHandler();
        quitHandler->retain();
        
        FLAlertLayer::create(
            quitHandler,
            "Data Restored",
            "Your data has been <cg>restored</c>.\nThe game needs to <cl>restart</c> for the changes to take effect.",
            "OK",
            nullptr,
            280.f
        )->show();
        
        release();
    }
};

#endif