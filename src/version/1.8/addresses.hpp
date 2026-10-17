/////////////////////////////////////////////////////////
//                         1.8                         //
/*******************************************************/
/*/                      OFFSETS                      /*/
/*******************************************************/
/**/ #define BoomScrollLayer__m_currentScreen  0x198 /**/ // BoomScrollLayer::getCurrentScreen
/**/ #define BoomScrollLayer__m_dynamic        0x130 /**/ // BoomScrollLayer::getTotalPages
/**/ #define BoomScrollLayer__m_dynamicPages   0x12c /**/ // BoomScrollLayer::getTotalPages
/**/ #define BoomScrollLayer__m_pages          0x140 /**/ // BoomScrollLayer::getTotalPages
/**/ #define CCMenuItemSpriteExtra__m_ogScale  0x118 /**/ // CCMenuItemSpriteExtra::setOriginalScale
/**/ #define CCTextInputNode__m_charLimit      0x14c /**/ // CCTextInputNode::getCharLimit
/**/ #define CCTextInputNode__m_inputDelegate  0x148 /**/ // CCTextInputNode::getDelegate
/**/ #define CCTextInputNode__m_textField      0x144 /**/ // CCTextInputNode::getTextField
/**/ #define CheckpointObject__m_isFlipped      0xf5 /**/ // CheckpointObject::getIsFlipped
/**/ #define CheckpointObject__m_playerPos     0x12c /**/ // CheckpointObject::getPlayerPos

/**/ #define CheckpointObject__m_playerCheck01  0xec /**/ // CheckpointObject::getPlayerCheck01
/**/ #define CheckpointObject__m_playerCheck02  0xf0 /**/ // CheckpointObject::getPlayerCheck02

/**/ #define ColorPickerPopup__m_colorWheel    0x1b8 /**/ // ColorPickerPopup::selectColor
/**/ #define ColorSelectPopup__m_colorWheel    0x1b0 /**/ // ColorSelectPopup::selectColor
/**/ #define ColorSelectPopup__m_duration      0x1d0 /**/ // ColorSelectPopup::sliderChanged
/**/ #define ColorSelectPopup__m_durLabel      0x1b8 /**/ // ColorSelectPopup::updateDurLabel
/**/ #define ColorSelectPopup__m_slider        0x1bc /**/ // ColorSelectPopup::init

/**/ #define DrawGridLayer__m_speedObjects     0x13c /**/ // DrawGridLayer::addToSpeedObjects

/**/ #define EditButtonBar__m_pages             0xec /**/ // EditButtonBar::init
/**/ #define EditLevelLayer__m_level           0x124 /**/ // EditLevelLayer::init
/**/ #define EditorPauseLayer__m_editorLayer   0x1a8 /**/ // EditorPauseLayer::init
/**/ #define EditorUI__m_buttonMenu            0x188 /**/ // EditorUI::setupDeleteMenu
/**/ #define EditorUI__m_createButtonBar       0x154 /**/ // EditorUI::setupCreateMenu (at the bottom)
/**/ #define EditorUI__m_createButtonBars      0x1cc /**/ // EditorUI::setupCreateMenu (at the bottom)
/**/ #define EditorUI__m_createButtons         0x204 /**/ // EditorUI::getCreateBtn (at the bottom)
/**/ #define EditorUI__m_editButtonBar         0x158 /**/ // EditorUI::createMoveMenu (at the bottom)
/**/ #define EditorUI__m_editorLayer           0x20c /**/ // EditorUI::init
/**/ #define EditorUI__m_selectedObject        0x238 /**/ // EditorUI::selectObject
/**/ #define EditorUI__m_selectedObjects       0x184 /**/ // EditorUI::deselectAll 
/**/ #define EditorUI__m_touch                 0x230 /**/ // EditorUI::onDelete
/**/ #define EditorUI__m_unkFloat              0x24c /**/ // EditorUI::setupCreateMenu

// /**/ #define EditorUI__m_zoomOutButton         0x158 /**/ // EditorUI::init

/**/ #define GameManager__m_autoRetry          0x170 /**/ // GameManager::getAutoRetryLevel
/**/ #define GameManager__m_playLayer          0x118 /**/ // GameManager::getPlayLayer
/**/ #define GameManager__m_playerName         0x130 /**/ // GameManager::getPlayerName
/**/ #define GameManager__m_playerUDID         0x12c /**/ // GameManager::getPlayerUDID

/**/ #define GameManager__m_showProgressBar    0x171 /**/ // GameManager::getShowProgressBar

/**/ #define GameObject__m_objectKey           0x290 /**/ // GameObject::getObjectKey
/**/ #define GameObject__m_particles           0x1e6 /**/ // GameObject::setOpacity

/**/ #define GameObject__m_radius              0x230 /**/ // GameObject::getRadius

/**/ #define GameObject__m_realPosition        0x250 /**/ // GameObject::getRealPosition
/**/ #define GameObject__m_sectionIdx          0x248 /**/ // GameObject::getSectionIdx
/**/ #define GameObject__m_shouldSpawn         0x272 /**/ // GameObject::getShouldSpawn
/**/ #define GameObject__m_type                0x244 /**/ // GameObject::getType
/**/ #define GameObject__m_useAudioScale       0x260 /**/ // GameObject::getUseAudioScale
/**/ #define GJComment__m_commentID             0xf0 /**/ // GJComment::getCommentID

/**/ #define GJSearchObject__m_page             0xf8 /**/ // GJSearchObject::getPage
/**/ #define GJSearchObject__m_type             0xe8 /**/ // GJSearchObject::getType
/**/ #define LeaderboardsLayer__m_boardType    0x124 /**/ // LeaderboardsLayer::selectLeaderboard
/**/ #define LevelBrowserLayer__m_searchObject 0x130 /**/ // LevelBrowserLayer::setSearchObject
/**/ #define LevelBrowserLayer__m_total        0x138 /**/ // LevelBrowserLayer::updateLevelsLabel

/**/ #define LevelCell__m_level                0x15c /**/ // LevelCell::loadFromLevel
/**/ #define LevelEditorLayer__m_batchNode     0x158 /**/ // LevelEditorLayer::init (search for CCSpriteBatchNode)
/**/ #define LevelEditorLayer__m_gameLayer     0x178 /**/ // LevelEditorLayer::getGameLayer
/**/ #define LevelEditorLayer__m_gridLayer     0x174 /**/ // LevelEditorLayer::getGridLayer
/**/ #define LevelEditorLayer__m_level         0x17c /**/ // LevelEditorLayer::getLevel
/**/ #define LevelEditorLayer__m_objectCount   0x170 /**/ // LevelEditorLayer::getObjectCount
/**/ #define LevelEditorLayer__m_sections      0x15c /**/ // LevelEditorLayer::getSectionCount
/**/ #define LevelEditorLayer__m_settings      0x180 /**/ // LevelEditorLayer::getLevelSettings
/**/ #define LevelEditorLayer__m_redoArray     0x164 /**/ // LevelEditorLayer::addToRedoList

/**/ #define LevelSettingsObject__m_startSpeed 0x110 /**/ // LevelSettingsObject::getStartSpeed

/**/ #define LevelSettingsObject__m_audioTrack 0x118 /**/ // LevelSettingsObject::getAudioTrack
/**/ #define LevelInfoLayer__m_level           0x13c /**/ // LevelInfoLayer::init
/**/ #define LevelSelectLayer__m_scrollLayer   0x128 /**/ // LevelSelectLayer::onNext (param in BoomScrollLayer::quickUpdate)
/**/ #define LocalLevelManager__m_localLevels   0xf4 /**/ // LocalLevelManager::getLocalLevels
/**/ #define PlayLayer__m_attempts             0x2a0 /**/ // PlayLayer::getAttempts
/**/ #define PlayLayer__m_attemptLabel         0x1c4 /**/ // PlayLayer::updateAttempts (assigned to a var right after the if statement)
/**/ #define PlayLayer__m_cameraPos            0x274 /**/ // PlayLayer::getCameraPos
/**/ #define PlayLayer__m_checkpoints          0x140 /**/ // PlayLayer::removeLastCheckpoint
/**/ #define PlayLayer__m_clkTimer             0x2ac /**/ // PlayLayer::getClkTimer
/**/ #define PlayLayer__m_hazards              0x160 /**/ // PlayLayer::checkCollisions (the for loop at the bottom which checks for rect intersections and calls destroyPlayer)
/**/ #define PlayLayer__m_isFlipped            0x258 /**/ // PlayLayer::getIsFlipped
/**/ #define PlayLayer__m_isPractice           0x27d /**/ // PlayLayer::getPracticeMode
/**/ #define PlayLayer__m_jumps                0x2a4 /**/ // PlayLayer::getJumps
/**/ #define PlayLayer__m_lastX                0x1bc /**/ // PlayLayer::destroyPlayer (find the multiplication by 100, and the denominator in the division right before it is this field)
/**/ #define PlayLayer__m_level                0x270 /**/ // PlayLayer::getLevel
/**/ #define PlayLayer__m_pGlitter             0x178 /**/ // PlayLayer::toggleGlitter
/**/ #define PlayLayer__m_playerObject         0x268 /**/ // PlayLayer::getPlayer
/**/ #define PlayLayer__m_progressBar          0x200 /**/ // PlayLayer::init - slidergroove2.png
/**/ #define PlayLayer__m_sections             0x15c /**/ // PlayLayer::removeObjectFromSection
/**/ #define PlayLayer__m_startPos             0x298 /**/ // PlayLayer::getStartPos
/**/ #define PlayLayer__m_uiLayer              0x264 /**/ // PlayLayer::getUILayer
/**/ #define PlayLayer__m_unkPrac              0x294 /**/ // PlayLayer::togglePracticeMode (set to 1 at the bottom)

/**/ #define PlayerCheckpoint__m_playerPos      0xe8 /**/ // PlayerCheckpoint::getPlayerPos

/**/ #define PlayerObject__m_gravity           0x358 /**/ // PlayerObject::gravityUp
/**/ #define PlayerObject__m_onGround          0x40d /**/ // PlayerObject::pushButton (if statement that updateJump is called in)
/**/ #define PlayerObject__m_pGround           0x3ac /**/ // PlayerObject::deactivateParticle
/**/ #define PlayerObject__m_pShipGround       0x3b8 /**/ // PlayerObject::playerDestroyed (third stopSystem)
/**/ #define PlayerObject__m_pShipFire         0x3b0 /**/ // PlayerObject::playerDestroyed (first stopSystem)
/**/ #define PlayerObject__m_pShipLift         0x3b4 /**/ // PlayerObject::playerDestroyed (second stopSystem)
/**/ #define PlayerObject__m_pLanding          0x3c4 /**/ // PlayerObject::hitGround (second if statement)
/**/ #define PlayerObject__m_pLanding2         0x3c8 /**/ // PlayerObject::hitGround (second if statement)
/**/ #define PlayerObject__m_unk1              0x370 /**/ // PlayerObject::playerDestroyed (at the top)
/**/ #define PlayerObject__m_unk2              0x40c /**/ // PlayerObject::playerDestroyed (slightly below, set to 1)
/**/ #define PlayerObject__m_xVelocity         0x348 /**/ // PlayerObject::speedUp
/**/ #define PlayerObject__m_yStart            0x350 /**/ // PlayerObject::yStartUp
/**/ #define UILayer__m_checkpointMenu         0x19c /**/ // UILayer::toggleCheckpointsMenu
/*******************************************************/
/*/                     ADDRESSES                     /*/
/*******************************************************/
/**/ #define _0_1f                          0x46bf95 /**/ // string search: %0.1f, the "1" base addr + 3
/**/ #define basicstring_assign             0x412e50 /**/ // differs between versions :/
/**/ #define global_order_of_arrival        0x535b4c /**/ // CCNode::reorderChild
/**/ #define __CCTextInputNode_setCharLimit 0x1a3f1e /**/
/**/ #define __CCTextInputNode_setProfanity 0x1a3f30 /**/ // CCTextInputNode::setProfanityFilter
/**/ #define __GameManager_isColorUnlocked  0x1b7d0c /**/ // fuck dobbyhook fuck this stupid game fuck everyone kill 7 billion people
/**/ #define __GameManager_isIconUnlocked   0x1b7c0c /**/ // fuck dobbyhook fuck this stupid game fuck everyone kill 7 billion people
/**/ #define __GJGameLevel_setIsVerified    0x1ebbf2 /**/
/**/ #define clubstep_bypass                0x1faa16 /**/ // LevelPage::onPlay
// /**/ #define __LevelEditorLayer_update_v    0x486664 /**/ // LevelEditorLayer::~
/**/ #define move_x_min                     0x1d704c /**/ // EditorUI::moveObject - 00 80 96 43
/**/ #define move_x_max                     0x1d7050 /**/ // EditorUI::moveObject - 00 C8 2F 47
/**/ #define move_y_max                     0x1d7058 /**/ // EditorUI::moveObject - 00 C0 C6 44
/**/ #define move_y_min                     0x1d7054 /**/ // EditorUI::moveObject - 00 00 B6 42
/**/ #define object_limit                   0x1ddab4 /**/ // EditorUI::onCreate DAB
/**/ #define object_limit_duplicate         0x1de484 /**/ // EditorUI::onDuplicate
/**/ #define pause_edit_button              0x1df5ba /**/ // PauseLayer::customSetup
/**/ #define pause_restart_button           0x1df6f8 /**/ // PauseLayer::customSetup
/**/ #define place_x_max                    0x1dda60 /**/ // EditorUI::onCreateObject - 00 B9 2F 47
/**/ #define place_y_max                    0x1dda64 /**/ // EditorUI::onCreateObject - 00 E0 C4 44
/**/ #define place_x_min                    0x1dda58 /**/ // EditorUI::onCreateObject - 00 80 9D 43
/**/ #define place_y_min                    0x1dda5c /**/ // EditorUI::onCreateObject - 00 00 D2 42
// /**/ #define zoom_bypass_max                0x19BE9E /**/ // EditorUI::zoomGameLayer
// /**/ #define zoom_bypass_min                0x19BEB0 /**/ // EditorUI::zoomGameLayer
/*******************************************************/