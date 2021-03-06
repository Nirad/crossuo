// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2016 Hotride
// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2020 Danny Angelo Carminati Grein

#include "ConfigManager.h"
#include "GumpManager.h"
#include "SoundManager.h"
#include "ObjectPropertiesManager.h"
#include <xuocore/text_parser.h>
#include <common/fs.h>
#include <common/str.h>
#include <common/utils.h> // countof
#include "../Point.h"
#include "../Config.h"
#include "../CrossUO.h"
#include "../Party.h"
#include "../ServerList.h"
#include "../GameWindow.h"
#include "../Container.h"
#include "../CharacterList.h"
#include "../TextEngine/GameConsole.h"
#include "../Gumps/Gump.h"
#include "../Gumps/GumpSpell.h"
#include "../Gumps/GumpPropertyIcon.h"
#include "../GameObjects/GameWorld.h"
#include "../GameObjects/GamePlayer.h"
#include "../Network/Packets.h"
#include "../ScreenStages/GameScreen.h"
#include "../Renderer/RenderAPI.h"

CConfigManager g_ConfigManager;
CConfigManager g_OptionsConfig;

enum
{
    CMKC_NONE = -1,

    CMKC_SOUND = 0,
    CMKC_SOUND_VOLUME,
    CMKC_MUSIC,
    CMKC_MUSIC_VOLUME,
    CMKC_FOOTSTEPS_SOUND,
    CMKC_COMBAT_MUSIC,
    CMKC_BACKGROUND_SOUND,
    CMKC_CLIENT_FPS,
    CMKC_USE_SCALING,
    CMKC_REMOVE_TEXT_WITH_BLENDING,
    CMKC_DRAW_STATUS_STATE,
    CMKC_DRAW_STUMPS,
    CMKC_MARKING_CAVES,
    CMKC_NO_ANIMATE_FIELDS,
    CMKC_AUTO_OPEN_DOOR,
    CMKC_ALWAYS_DISPLAY_HUMANOIDS_NAME,
    CMKC_NO_VEGETATION,
    CMKC_HIDDEN_CHARACTERS_RENDER_MODE,
    CMKC_HIDDEN_ALPHA,
    CMKC_USE_HIDDEN_MODE_ONLY_FOR_SELF,
    CMKC_TRANSPARENT_SPELL_ICONS,
    CMKC_SPELL_ICON_ALPHA,
    CMKC_OLD_STYLE_STATUSBAR,
    CMKC_ORIGINAL_PARTY_STATUSBAR,
    CMKC_APPLY_STATE_COLOR_ON_CHARACTERS,
    CMKC_CHANGE_FIELDS_GRAPHIC,
    CMKC_PAPERDOLL_SLOTS,
    CMKC_DRAW_STATUS_CONDITION_STATE,
    CMKC_DRAW_STATUS_CONDITION_VALUE,
    CMKC_REMOVE_STATUSBARS_WITHOUT_OBJECTS,
    CMKC_SHOW_DEFAULT_CONSOLE_ENTRY_MODE,
    CMKC_DRAW_AURA_STATE,
    CMKC_DRAW_AURA_WITH_CTRL_PRESSED,
    CMKC_SCREENSHOT_FORMAT,
    CMKC_SCALE_IMAGES_IN_PAPERDOLL_SLOTS,
    CMKC_REMOVE_OR_CREATE_OBJECTS_WITH_BLENDING,
    CMKC_DRAW_HELMETS_ON_SHROUD,
    CMKC_USE_GLOBAL_MAP_LAYER,
    CMKC_NO_DRAW_ROOFS,
    CMKC_HIGHLIGHT_TARGET_BY_TYPE,
    CMKC_AUTO_DISPLAY_WORLD_MAP,
    CMKC_DISABLE_MACRO_IN_CHAT,
    CMKC_USE_TOOLTIPS,
    CMKC_TOOLTIPS_TEXT_COLOR,
    CMKC_TOOLTIPS_TEXT_FONT,
    CMKC_TOOLTIPS_DELAY,
    CMKC_CHAT_COLOR_INPUT_TEXT,
    CMKC_CHAT_COLOR_MENU_OPTION,
    CMKC_CHAT_COLOR_PLAYER_IN_MEMBER_LIST,
    CMKC_CHAT_COLOR_TEXT,
    CMKC_CHAT_COLOR_PLAYER_NAME_WITHOUT,
    CMKC_CHAT_COLOR_MUTED,
    CMKC_CHAT_COLOR_CHANNEL_MODERATOR_NAME,
    CMKC_CHAT_COLOR_CHANNEL_MODERATOR_TEXT,
    CMKC_CHAT_COLOR_MY_NAME,
    CMKC_CHAT_COLOR_MY_TEXT,
    CMKC_CHAT_COLOR_SYSTEM_MESSAGE,
    CMKC_CHAT_FONT,
    CMKC_CHAT_COLOR_BG_OUTPUT_TEXT,
    CMKC_CHAT_COLOR_BG_INPUT_TEXT,
    CMKC_CHAT_COLOR_BG_USER_LIST,
    CMKC_CHAT_COLOR_BG_CONF_LIST,
    CMKC_CHAT_COLOR_BG_COMMAND_LIST,
    CMKC_ENABLE_PATHFIND,
    CMKC_HOLD_TAB_FOR_COMBAT,
    CMKC_OFFSET_INTERFACE_WINDOWS,
    CMKC_AUTO_ARRANGE,
    CMKC_ALWAYS_RUN,
    CMKC_DISABLE_MENUBAR,
    CMKC_GRAY_OUT_OF_RANGE_OBJECTS,
    CMKC_DISABLE_NEW_TARGET_SYSTEM,
    CMKC_ITEMP_ROPERTIES_MODE,
    CMKC_ITEMP_ROPERTIES_ICON,
    CMKC_OBJECT_HANDLES,
    CMKC_OBJECT_HANDLES_NO_BODIES,
    CMKC_REDUCE_FPS_UNACTIVE_WINDOW,
    CMKC_HOLD_SHIFT_FOR_CONTEXT_MENUS,
    CMKC_HOLD_SHIFT_FOR_ENABLE_PATHFIND,
    CMKC_CONTAINER_DEFAULT_X,
    CMKC_CONTAINER_DEFAULT_Y,
    CMKC_GAME_WINDOW_WIDTH,
    CMKC_GAME_WINDOW_HEIGHT,
    CMKC_SPEECH_DELAY,
    CMKC_SCALE_SPEECH_DELAY,
    CMKC_SPEECH_COLOR,
    CMKC_EMOTE_COLOR,
    CMKC_PARTY_MESSAGE_COLOR,
    CMKC_GUILD_MESSAGE_COLOR,
    CMKC_ALLIANCE_MESSAGE_COLOR,
    CMKC_IGNORE_GUILD_MESSAGE,
    CMKC_IGNORE_ALLIANCE_MESSAGE,
    CMKC_DARK_NIGHTS,
    CMKC_COLORED_LIGHTING,
    CMKC_STANDART_CHARACTERS_ANIMATION_DELAY,
    CMKC_STANDART_ITEMS_ANIMATION_DELAY,
    CMKC_LOCK_RESIZING_GAME_WINDOW,
    CMKC_LOCK_GUMPS_MOVING,
    CMKC_INNOCENT_COLOR,
    CMKC_FRIENDLY_COLOR,
    CMKC_SOMEONE_COLOR,
    CMKC_CRIMINAL_COLOR,
    CMKC_ENEMY_COLOR,
    CMKC_MURDERER_COLOR,
    CMKC_CRIMINAL_ACTIONS_QUERY,
    CMKC_SHOW_INCOMING_NAMES,
    CMKC_USE_CIRCLE_TRANS,
    CMKC_STAT_REPORT,
    CMKC_CONSOLE_NEED_ENTER,
    CMKC_CIRCLE_TRANS_RADIUS,
    CMKC_SKILL_REPORT,
    CMKC_SPEECH_FONT,
    CMKC_GAME_WINDOW_X,
    CMKC_GAME_WINDOW_Y,
    CMKC_ZOOMED,
    CMKC_REAL_X,
    CMKC_REAL_Y,
    CMKC_REAL_WIDTH,
    CMKC_REAL_HEIGHT,
    CMKC_TOGGLE_BUFFICON_WINDOW,
    CMKC_DEVELOPER_MODE,
    CMKC_LAST_SERVER,
    CMKC_LAST_CHARACTER,
    CMKC_CHARACTER_BACKPACK_STYLE,
#if USE_PING
    CMKC_CHECK_PING,
    CMKC_PING_TIMER,
#endif // USE_PING
    CMKC_CANCEL_NEW_TARGET_SYSTEM_ON_SHIFT_ESC,
    CMKC_DRAW_STATUS_FOR_HUMANOIDS,
    CMKC_LARGEGUMPS,

    CMKC_COUNT
};

namespace cmkc
{
struct ConfigEntry
{
    uint32_t key;
    const char *key_name;
};

static const ConfigEntry s_Keys[] = {
    { CMKC_SOUND, "sound" },
    { CMKC_SOUND_VOLUME, "soundvolume" },
    { CMKC_MUSIC, "music" },
    { CMKC_MUSIC_VOLUME, "musicvolume" },
    { CMKC_FOOTSTEPS_SOUND, "footstepssound" },
    { CMKC_COMBAT_MUSIC, "combatmusic" },
    { CMKC_BACKGROUND_SOUND, "backgroundsound" },
    { CMKC_CLIENT_FPS, "clientfps" },
    { CMKC_USE_SCALING, "usescaling" },
    { CMKC_REMOVE_TEXT_WITH_BLENDING, "removetextwithblending" },
    { CMKC_DRAW_STATUS_STATE, "drawstatusstate" },
    { CMKC_DRAW_STUMPS, "drawstumps" },
    { CMKC_MARKING_CAVES, "markingcaves" },
    { CMKC_NO_ANIMATE_FIELDS, "noanimatefields" },
    { CMKC_NO_VEGETATION, "novegetation" },
    { CMKC_HIDDEN_CHARACTERS_RENDER_MODE, "hiddencharactersrendermode" },
    { CMKC_HIDDEN_ALPHA, "hiddenalpha" },
    { CMKC_USE_HIDDEN_MODE_ONLY_FOR_SELF, "usehiddenmodeonlyforself" },
    { CMKC_TRANSPARENT_SPELL_ICONS, "transparentspellicons" },
    { CMKC_SPELL_ICON_ALPHA, "spelliconalpha" },
    { CMKC_OLD_STYLE_STATUSBAR, "oldstylestatusbar" },
    { CMKC_ORIGINAL_PARTY_STATUSBAR, "originalpartystatusbar" },
    { CMKC_APPLY_STATE_COLOR_ON_CHARACTERS, "applystatecoloroncharacters" },
    { CMKC_CHANGE_FIELDS_GRAPHIC, "changefieldsgraphic" },
    { CMKC_AUTO_OPEN_DOOR, "autoopendoors" },
    { CMKC_ALWAYS_DISPLAY_HUMANOIDS_NAME, "alwaysdisplayhumanoidsname" },
    { CMKC_PAPERDOLL_SLOTS, "paperdollslots" },
    { CMKC_DRAW_STATUS_CONDITION_STATE, "drawstatusconditionstate" },
    { CMKC_DRAW_STATUS_CONDITION_VALUE, "drawstatusconditionvalue" },
    { CMKC_REMOVE_STATUSBARS_WITHOUT_OBJECTS, "removestatusbarswithoutobjects" },
    { CMKC_SHOW_DEFAULT_CONSOLE_ENTRY_MODE, "showdefaultconsoleentrymode" },
    { CMKC_DRAW_AURA_STATE, "drawaurastate" },
    { CMKC_DRAW_AURA_WITH_CTRL_PRESSED, "drawaurawithctrlpressed" },
    { CMKC_SCREENSHOT_FORMAT, "screenshotformat" },
    { CMKC_SCALE_IMAGES_IN_PAPERDOLL_SLOTS, "scaleimagesinpaperdollslots" },
    { CMKC_REMOVE_OR_CREATE_OBJECTS_WITH_BLENDING, "removeorcreateobjectswithblending" },
    { CMKC_DRAW_HELMETS_ON_SHROUD, "drawhelmetsonshroud" },
    { CMKC_USE_GLOBAL_MAP_LAYER, "useglobalmaplayer" },
    { CMKC_NO_DRAW_ROOFS, "nodrawroofs" },
    { CMKC_HIGHLIGHT_TARGET_BY_TYPE, "highlighttargetbytype" },
    { CMKC_AUTO_DISPLAY_WORLD_MAP, "autodisplayworldmap" },
    { CMKC_DISABLE_MACRO_IN_CHAT, "disablemacroinchat" },
    { CMKC_USE_TOOLTIPS, "usetooltips" },
    { CMKC_TOOLTIPS_TEXT_COLOR, "tooltipstextcolor" },
    { CMKC_TOOLTIPS_TEXT_FONT, "tooltipstextfont" },
    { CMKC_TOOLTIPS_DELAY, "tooltipsdelay" },
    { CMKC_CHAT_COLOR_INPUT_TEXT, "chatcolorinputtext" },
    { CMKC_CHAT_COLOR_MENU_OPTION, "chatcolormenuoption" },
    { CMKC_CHAT_COLOR_PLAYER_IN_MEMBER_LIST, "chatcolorplayerinmemberlist" },
    { CMKC_CHAT_COLOR_TEXT, "chatcolortext" },
    { CMKC_CHAT_COLOR_PLAYER_NAME_WITHOUT, "chatcolorplayernamewithout" },
    { CMKC_CHAT_COLOR_MUTED, "chatcolormuted" },
    { CMKC_CHAT_COLOR_CHANNEL_MODERATOR_NAME, "chatcolorchannelmoderatorname" },
    { CMKC_CHAT_COLOR_CHANNEL_MODERATOR_TEXT, "chatcolorchannelmoderatortext" },
    { CMKC_CHAT_COLOR_MY_NAME, "chatcolormyname" },
    { CMKC_CHAT_COLOR_MY_TEXT, "chatcolormytext" },
    { CMKC_CHAT_COLOR_SYSTEM_MESSAGE, "chatcolorsystemmessage" },
    { CMKC_CHAT_FONT, "chatfont" },
    { CMKC_CHAT_COLOR_BG_OUTPUT_TEXT, "chatcolorbgoutputtext" },
    { CMKC_CHAT_COLOR_BG_INPUT_TEXT, "chatcolorbginputtext" },
    { CMKC_CHAT_COLOR_BG_USER_LIST, "chatcolorbguserlist" },
    { CMKC_CHAT_COLOR_BG_CONF_LIST, "chatcolorbgconflist" },
    { CMKC_CHAT_COLOR_BG_COMMAND_LIST, "chatcolorbgcommandlist" },
    { CMKC_ENABLE_PATHFIND, "enablepathfind" },
    { CMKC_HOLD_TAB_FOR_COMBAT, "holdtabforcombat" },
    { CMKC_OFFSET_INTERFACE_WINDOWS, "offsetinterfacewindows" },
    { CMKC_AUTO_ARRANGE, "autoarrange" },
    { CMKC_ALWAYS_RUN, "alwaysrun" },
    { CMKC_DISABLE_MENUBAR, "disablemenubar" },
    { CMKC_GRAY_OUT_OF_RANGE_OBJECTS, "grayoutofrangeobjects" },
    { CMKC_DISABLE_NEW_TARGET_SYSTEM, "disablenewtargetsystem" },
    { CMKC_ITEMP_ROPERTIES_MODE, "itempropertiesmode" },
    { CMKC_ITEMP_ROPERTIES_ICON, "itempropertiesicon" },
    { CMKC_OBJECT_HANDLES, "objecthandles" },
    { CMKC_OBJECT_HANDLES_NO_BODIES, "objecthandlesnobodies" },
    { CMKC_REDUCE_FPS_UNACTIVE_WINDOW, "reducefpsunactivewindow" },
    { CMKC_HOLD_SHIFT_FOR_CONTEXT_MENUS, "holdshiftforcontextmenus" },
    { CMKC_HOLD_SHIFT_FOR_ENABLE_PATHFIND, "holdshiftforenablepathfind" },
    { CMKC_CONTAINER_DEFAULT_X, "containerdefaultx" },
    { CMKC_CONTAINER_DEFAULT_Y, "containerdefaulty" },
    { CMKC_GAME_WINDOW_WIDTH, "gamewindowwidth" },
    { CMKC_GAME_WINDOW_HEIGHT, "gamewindowheight" },
    { CMKC_SPEECH_DELAY, "speechdelay" },
    { CMKC_SCALE_SPEECH_DELAY, "scalespeechdelay" },
    { CMKC_SPEECH_COLOR, "speechcolor" },
    { CMKC_EMOTE_COLOR, "emotecolor" },
    { CMKC_PARTY_MESSAGE_COLOR, "partymessagecolor" },
    { CMKC_GUILD_MESSAGE_COLOR, "guildmessagecolor" },
    { CMKC_ALLIANCE_MESSAGE_COLOR, "alliancemessagecolor" },
    { CMKC_IGNORE_GUILD_MESSAGE, "ignoreguildmessage" },
    { CMKC_IGNORE_ALLIANCE_MESSAGE, "ignorealliancemessage" },
    { CMKC_DARK_NIGHTS, "darknights" },
    { CMKC_COLORED_LIGHTING, "coloredlighting" },
    { CMKC_STANDART_CHARACTERS_ANIMATION_DELAY, "standartcharactersanimationdelay" },
    { CMKC_STANDART_ITEMS_ANIMATION_DELAY, "standartitemsanimationdelay" },
    { CMKC_LOCK_RESIZING_GAME_WINDOW, "lockresizinggamewindow" },
    { CMKC_LOCK_GUMPS_MOVING, "lockgumpsmoving" },
    { CMKC_INNOCENT_COLOR, "innocentcolor" },
    { CMKC_FRIENDLY_COLOR, "friendlycolor" },
    { CMKC_SOMEONE_COLOR, "someonecolor" },
    { CMKC_CRIMINAL_COLOR, "criminalcolor" },
    { CMKC_ENEMY_COLOR, "enemycolor" },
    { CMKC_MURDERER_COLOR, "murderercolor" },
    { CMKC_CRIMINAL_ACTIONS_QUERY, "criminalactionsquery" },
    { CMKC_SHOW_INCOMING_NAMES, "showincomingnames" },
    { CMKC_USE_CIRCLE_TRANS, "usecircletrans" },
    { CMKC_STAT_REPORT, "statreport" },
    { CMKC_CONSOLE_NEED_ENTER, "consoleneedenter" },
    { CMKC_CIRCLE_TRANS_RADIUS, "circletransradius" },
    { CMKC_SKILL_REPORT, "skillreport" },
    { CMKC_SPEECH_FONT, "speechfont" },
    { CMKC_GAME_WINDOW_X, "gamewindowx" },
    { CMKC_GAME_WINDOW_Y, "gamewindowy" },
    { CMKC_ZOOMED, "zoomed" },
    { CMKC_REAL_X, "realx" },
    { CMKC_REAL_Y, "realy" },
    { CMKC_REAL_WIDTH, "realwidth" },
    { CMKC_REAL_HEIGHT, "realheight" },
    { CMKC_TOGGLE_BUFFICON_WINDOW, "togglebufficonwindow" },
    { CMKC_DEVELOPER_MODE, "developermode" },
    { CMKC_LAST_SERVER, "lastserver" },
    { CMKC_LAST_CHARACTER, "lastcharacter" },
    { CMKC_CHARACTER_BACKPACK_STYLE, "characterbackpackstyle" },
#if USE_PING
    { CMKC_CHECK_PING, "checkping" },
    { CMKC_PING_TIMER, "pingtimer" },
#endif // USE_PING
    { CMKC_CANCEL_NEW_TARGET_SYSTEM_ON_SHIFT_ESC, "cancelnewtargetsystemonshiftesc" },
    { CMKC_DRAW_STATUS_FOR_HUMANOIDS, "drawstatusforhumanoids" },
    { CMKC_LARGEGUMPS, "largegumps" },
    { CMKC_COUNT, nullptr },
};

static_assert(countof(s_Keys) == CMKC_COUNT + 1, "Missing key string for configuration option");

static uint32_t GetConfigKey(const astr_t &key)
{
    auto str = str_lower(key);
    for (int i = 0; s_Keys[i].key_name; i++)
    {
        if (str == s_Keys[i].key_name)
        {
            return s_Keys[i].key;
        }
    }
    return CMKC_NONE;
}

} // namespace cmkc

CConfigManager::CConfigManager()
{
}

void CConfigManager::Init()
{
    DefaultPage1();
    DefaultPage2();
    DefaultPage3();
    DefaultPage4();
    DefaultPage6();
    DefaultPage7();
    DefaultPage8();
    DefaultPage9();

    g_CircleOfTransparency.Create(CircleTransRadius);

    FilterPWOn = false;
    ObscenityFilter = false;
    FilterPassword = "";

    ToggleBufficonWindow = true;

    GameWindowX = 0;
    GameWindowY = 0;

    if (g_Config.ClientVersion >= CV_70331)
    {
        g_MaxViewRange = MAX_VIEW_RANGE_NEW;
    }
    else
    {
        g_MaxViewRange = MAX_VIEW_RANGE_OLD;
    }

    UpdateRange = g_MaxViewRange;
}

void CConfigManager::DefaultPage1()
{
    m_Sound = true;
    m_Music = true;
    FootstepsSound = true;
    CombatMusic = true;
    m_SoundVolume = 255;
    m_MusicVolume = 255;
    BackgroundSound = false;
}

void CConfigManager::DefaultPage2()
{
    m_ClientFPS = DEFAULT_FPS;
    m_ReduceFPSUnactiveWindow = true;
    StandartCharactersAnimationDelay = false;
    StandartItemsAnimationDelay = true;
    m_UseScaling = false;
    RemoveTextWithBlending = true;
    m_DrawStatusState = DCSS_NO_DRAW;
    DrawStatusConditionState = DCSCS_LOWER;
    DrawStatusConditionValue = 70;
    m_DrawStumps = false;
    m_MarkingCaves = false;
    m_NoVegetation = false;
    m_NoAnimateFields = false;
    LockGumpsMoving = true;
    m_ConsoleNeedEnter = false;
    HiddenCharactersRenderMode = 0;
    HiddenAlpha = 0x7F;
    UseHiddenModeOnlyForSelf = true;
    TransparentSpellIcons = false;
    m_SpellIconAlpha = 0x7F;
    m_OldStyleStatusbar = false;
    m_ApplyStateColorOnCharacters = false;
    m_OriginalPartyStatusbar = false;
    m_ChangeFieldsGraphic = false;
    m_PaperdollSlots = true;
    RemoveStatusbarsWithoutObjects = false;
    ShowDefaultConsoleEntryMode = true;
    m_DrawAuraState = DAS_NEVER;
    DrawAuraWithCtrlPressed = true;
    ScreenshotFormat = SF_PNG;
    m_ScaleImagesInPaperdollSlots = true;
    RemoveOrCreateObjectsWithBlending = true;
    DrawHelmetsOnShroud = false;
    m_UseGlobalMapLayer = false;
    m_NoDrawRoofs = false;
    HighlightTargetByType = true;
    AutoDisplayWorldMap = false;
    DisableMacroInChat = false;
#if USE_PING
    CheckPing = true;
    m_PingTimer = 10;
#endif // USE_PING
    CancelNewTargetSystemOnShiftEsc = false;
    DrawStatusForHumanoids = true;
}

void CConfigManager::DefaultPage3()
{
    UseToolTips = true;
    ToolTipsTextColor = 0xFFFF;
    ToolTipsTextFont = 1;
    ToolTipsDelay = 200;
}

void CConfigManager::DefaultPage4()
{
    ChatColorInputText = 0xFFFF;
    ChatColorMenuOption = 0xFFFF;
    ChatColorPlayerInMemberList = 0xFFFF;
    ChatColorText = 0xFFFF;
    ChatColorPlayerNameWithout = 0xFFFF;
    ChatColorMuted = 0xFFFF;
    ChatColorChannelModeratorName = 0xFFFF;
    ChatColorChannelModeratorText = 0xFFFF;
    ChatColorMyName = 0xFFFF;
    ChatColorMyText = 0xFFFF;
    ChatColorSystemMessage = 0xFFFF;
    ChatColorBGOutputText = 0xFFFF;
    ChatColorBGInputText = 0xFFFF;
    ChatColorBGUserList = 0xFFFF;
    ChatColorBGConfList = 0xFFFF;
    ChatColorBGCommandList = 0xFFFF;

    ChatFont = 0;
}

void CConfigManager::DefaultPage6()
{
    EnablePathfind = true;
    HoldTabForCombat = true;
    OffsetInterfaceWindows = true;
    AutoArrange = true;
    AlwaysRun = false;
    DisableMenubar = false;
    GrayOutOfRangeObjects = false;
    DisableNewTargetSystem = true;
    m_ItemPropertiesMode = OPM_FOLLOW_MOUSE;
    m_ItemPropertiesIcon = false;
    ObjectHandles = false;
    ObjectHandlesNoBodies = false;
    HoldShiftForContextMenus = false;
    HoldShiftForEnablePathfind = false;
    m_CharacterBackpackStyle = CBS_DEFAULT;
}

void CConfigManager::DefaultPage7()
{
    GameWindowWidth = 800;
    GameWindowHeight = 600;
    SpeechDelay = 500;
    ScaleSpeechDelay = true;
    SpeechColor = 0x02B2;
    EmoteColor = 0x0021;
    PartyMessageColor = 0x0044;
    GuildMessageColor = 0x0044;
    AllianceMessageColor = 0x0057;
    IgnoreGuildMessage = false;
    IgnoreAllianceMessage = false;
    DarkNights = false;
    ColoredLighting = true;
    LockResizingGameWindow = false;
}

void CConfigManager::DefaultPage8()
{
    InnocentColor = 0x005A;
    FriendlyColor = 0x0044;
    SomeoneColor = 0x03B2;
    CriminalColor = 0x03B2;
    EnemyColor = 0x0031;
    MurdererColor = 0x0023;
    CriminalActionsQuery = true;
}

void CConfigManager::DefaultPage9()
{
    ShowIncomingNames = true;
    UseCircleTrans = false;
    StatReport = true;
    CircleTransRadius = 64;
    SkillReport = 1;
    SpeechFont = 0;
    LargeGumps = false;
}

void CConfigManager::UpdateFeatures()
{
    SetDrawStatusState(m_DrawStatusState);
    SetDrawStumps(m_DrawStumps);
    SetMarkingCaves(m_MarkingCaves);
    SetNoVegetation(m_NoVegetation);
    SetNoAnimateFields(m_NoAnimateFields);
    SetApplyStateColorOnCharacters(m_ApplyStateColorOnCharacters);
    SetChangeFieldsGraphic(m_ChangeFieldsGraphic);
    SetDrawAuraState(m_DrawAuraState);
    SetNoDrawRoofs(m_NoDrawRoofs);
}

void CConfigManager::SetSound(bool val)
{
    m_Sound = val;
    if (this == &g_ConfigManager && !val)
    {
        g_Game.AdjustSoundEffects(g_Ticks + 100000);
    }
}

void CConfigManager::SetMusic(bool val)
{
    m_Music = val;
    if (this == &g_ConfigManager && !val)
    {
        g_SoundManager.StopMusic();
    }
}

void CConfigManager::SetSoundVolume(uint8_t val)
{
    if (this == &g_ConfigManager && m_SoundVolume != val)
    {
        g_Game.AdjustSoundEffects(g_Ticks + 100000, val);
    }

    m_SoundVolume = val;
}

void CConfigManager::SetMusicVolume(uint8_t val)
{
    if (this == &g_ConfigManager && m_MusicVolume != val)
    {
        m_MusicVolume = val;
        g_SoundManager.SetMusicVolume(m_MusicVolume);
    }
    else
    {
        m_MusicVolume = val;
    }
}

void CConfigManager::SetClientFPS(int val)
{
    m_ClientFPS = val;
    if (this == &g_ConfigManager)
    {
        if (m_ClientFPS < MIN_FPS_LIMIT)
        {
            m_ClientFPS = MIN_FPS_LIMIT;
        }
        else if (m_ClientFPS > MAX_FPS_LIMIT)
        {
            m_ClientFPS = MAX_FPS_LIMIT;
        }

        if (m_ClientFPS == MIN_FPS_LIMIT)
        {
            g_FrameDelay[WINDOW_ACTIVE] = 80;
        }
        else
        {
            g_FrameDelay[WINDOW_ACTIVE] = 1000 / m_ClientFPS;
        }

        if (!m_ReduceFPSUnactiveWindow)
        {
            g_FrameDelay[WINDOW_INACTIVE] = g_FrameDelay[WINDOW_ACTIVE];
        }
        g_GameWindow.SetRenderTimerDelay(g_FrameDelay[g_GameWindow.IsActive()]);
    }
}

void CConfigManager::SetUseScaling(bool val)
{
    m_UseScaling = val;
    if (!val && this == &g_ConfigManager)
    {
        g_GlobalScale = 1.0;
    }
}

void CConfigManager::SetDrawStatusState(uint8_t val)
{
    uint8_t state = val;
    if ((g_GameFeaturesFlags & OFF_DRAW_CHARACTERS_STATUS_IN_WORLD) == 0u)
    {
        state = DCSS_NO_DRAW;
    }

    if (this == &g_ConfigManager)
    {
        if ((state != 0u) && (m_DrawStatusState == 0u))
        {
            QFOR(item, g_World->m_Items, CGameObject *)
            {
                if (item->NPC)
                {
                    CPacketStatusRequest(item->Serial).Send();
                }
            }
        }
    }

    m_DrawStatusState = state;
}

void CConfigManager::SetDrawStumps(bool val)
{
    bool state = val;
    if ((g_GameFeaturesFlags & OFF_CHANGE_TREES_TO_STUMPS) == 0u)
    {
        state = false;
    }

    if (m_DrawStumps != state && this == &g_ConfigManager)
    {
        g_Game.ClearTreesTextures();
    }

    m_DrawStumps = state;
}

void CConfigManager::SetMarkingCaves(bool val)
{
    bool state = val;
    if ((g_GameFeaturesFlags & OFF_MARKING_CAVES) == 0u)
    {
        state = false;
    }

    if (m_MarkingCaves != state && this == &g_ConfigManager)
    {
        g_Game.ClearCaveTextures();
    }

    m_MarkingCaves = state;
}

void CConfigManager::SetNoVegetation(bool val)
{
    bool state = val;
    if ((g_GameFeaturesFlags & OFF_NO_VEGETATION) == 0u)
    {
        state = false;
    }

    m_NoVegetation = state;
}

void CConfigManager::SetNoAnimateFields(bool val)
{
    bool state = val;
    if ((g_GameFeaturesFlags & OFF_NO_FIELDS_ANIMATION) == 0u)
    {
        state = false;
    }

    m_NoAnimateFields = state;
}

void CConfigManager::SetApplyStateColorOnCharacters(bool val)
{
    bool state = val;
    if ((g_GameFeaturesFlags & OFF_COLORED_CHARACTERS_STATE) == 0u)
    {
        state = false;
    }

    m_ApplyStateColorOnCharacters = state;
}

void CConfigManager::SetDrawAuraState(uint8_t val)
{
    uint8_t state = val;
    if ((g_GameFeaturesFlags & OFF_DRAW_AURA) == 0u)
    {
        state = DAS_NEVER;
    }

    m_DrawAuraState = state;
}

void CConfigManager::SetReduceFPSUnactiveWindow(bool val)
{
    if (this == &g_ConfigManager)
    {
        if (val)
        {
            g_FrameDelay[WINDOW_INACTIVE] = FRAME_DELAY_INACTIVE_WINDOW;
        }
        else
        {
            g_FrameDelay[WINDOW_INACTIVE] = g_FrameDelay[WINDOW_ACTIVE];
        }
        g_GameWindow.SetRenderTimerDelay(g_FrameDelay[g_GameWindow.IsActive()]);
    }

    m_ReduceFPSUnactiveWindow = val;
}

void CConfigManager::SetConsoleNeedEnter(bool val)
{
    if (this == &g_ConfigManager && val && g_EntryPointer == &g_GameConsole)
    {
        g_EntryPointer = nullptr;
    }

    m_ConsoleNeedEnter = val;
}

void CConfigManager::SetSpellIconAlpha(uint8_t val)
{
    if (this == &g_ConfigManager && val != m_SpellIconAlpha)
    {
        float alpha = val / 255.0f;
        const bool redraw = g_ConfigManager.TransparentSpellIcons;
        QFOR(gump, g_GumpManager.m_Items, CGump *)
        {
            if (gump->GumpType == GT_SPELL)
            {
                ((CGumpSpell *)gump)->m_Blender->Alpha = alpha;
                if (redraw)
                {
                    gump->WantRedraw = true;
                }
            }
        }
    }

    m_SpellIconAlpha = val;
}

void CConfigManager::SetOldStyleStatusbar(bool val)
{
    m_OldStyleStatusbar = val;
    if (this == &g_ConfigManager)
    {
        CGump *gump = g_GumpManager.UpdateGump(g_PlayerSerial, 0, GT_STATUSBAR);

        if (gump != nullptr && !gump->Minimized)
        {
            gump->WantUpdateContent = true;
        }
    }
}

void CConfigManager::SetOriginalPartyStatusbar(bool val)
{
    m_OriginalPartyStatusbar = val;
    if (this == &g_ConfigManager)
    {
        if (g_Party.Leader != 0)
        {
            QFOR(gump, g_GumpManager.m_Items, CGump *)
            {
                if (gump->GumpType == GT_STATUSBAR &&
                    (g_Party.Leader == gump->Serial || g_Party.Contains(gump->Serial)))
                {
                    gump->WantRedraw = true;
                    gump->WantUpdateContent = true;
                }
            }
        }
    }
}

void CConfigManager::SetChangeFieldsGraphic(bool val)
{
    m_ChangeFieldsGraphic = val;
    if ((g_GameFeaturesFlags & OFF_TILED_FIELDS) == 0u)
    {
        m_ChangeFieldsGraphic = false;
    }

    if (this == &g_ConfigManager && g_World != nullptr)
    {
        QFOR(item, g_World->m_Items, CGameObject *)
        {
            if (!item->NPC)
            {
                ((CGameItem *)item)->CalculateFieldColor();
            }
        }
    }
}

void CConfigManager::SetPaperdollSlots(bool val)
{
    m_PaperdollSlots = val;
    if (this == &g_ConfigManager && g_World != nullptr)
    {
        QFOR(gump, g_GumpManager.m_Items, CGump *)
        {
            if (gump->GumpType == GT_PAPERDOLL)
            {
                gump->WantRedraw = true;
                gump->WantUpdateContent = true;
            }
        }
    }
}

void CConfigManager::SetScaleImagesInPaperdollSlots(bool val)
{
    m_ScaleImagesInPaperdollSlots = val;
    if (this == &g_ConfigManager && g_World != nullptr)
    {
        QFOR(gump, g_GumpManager.m_Items, CGump *)
        {
            if (gump->GumpType == GT_PAPERDOLL)
            {
                gump->WantRedraw = true;
                gump->WantUpdateContent = true;
            }
        }
    }
}

void CConfigManager::SetUseGlobalMapLayer(bool val)
{
    m_UseGlobalMapLayer = val;
    if (this == &g_ConfigManager && val)
    {
        QFOR(gump, g_GumpManager.m_Items, CGump *)
        {
            if (gump->GumpType == GT_WORLD_MAP)
            {
                g_GumpManager.MoveToFront(gump);
            }
        }
    }
}

void CConfigManager::SetNoDrawRoofs(bool val)
{
    m_NoDrawRoofs = val;
    if ((g_GameFeaturesFlags & OFF_TILED_FIELDS) == 0u)
    {
        m_NoDrawRoofs = false;
    }

    if (this == &g_ConfigManager && g_Player != nullptr)
    {
        g_Player->OldX = 0;
        g_Player->OldY = 0;
    }
}

#if USE_PING
void CConfigManager::SetPingTimer(uint8_t val)
{
    m_PingTimer = std::max(std::min(val, uint8_t(120)), uint8_t(10));
    g_PingTimer = 0;
}
#endif // USE_PING

void CConfigManager::SetItemPropertiesMode(uint8_t val)
{
    m_ItemPropertiesMode = val;
    if (this == &g_ConfigManager && g_World != nullptr)
    {
        CGumpPropertyIcon *gump =
            (CGumpPropertyIcon *)g_GumpManager.UpdateContent(0, 0, GT_PROPERTY_ICON);

        if (gump != nullptr && (val == OPM_AT_ICON || val == OPM_ALWAYS_UP))
        {
            gump->SetTextW(gump->GetTextW());
        }

        g_ObjectPropertiesManager.Reset();
    }
}

void CConfigManager::SetItemPropertiesIcon(bool val)
{
    m_ItemPropertiesIcon = val;
    if (this == &g_ConfigManager && g_World != nullptr)
    {
        if (val)
        {
            CGump *gump = g_GumpManager.UpdateContent(0, 0, GT_PROPERTY_ICON);

            if (gump == nullptr)
            {
                CSize windowSize = g_GameWindow.GetSize();

                int x = GameWindowX + (int)(GameWindowWidth * 0.9f);
                int y = GameWindowY + GameWindowHeight;

                if (x + 100 >= windowSize.Width)
                {
                    x = windowSize.Width - 100;
                }

                if (y + 60 >= windowSize.Height)
                {
                    y = windowSize.Height - 60;
                }

                g_GumpManager.AddGump(new CGumpPropertyIcon(x, y));
            }
        }
        else
        {
            g_GumpManager.CloseGump(0, 0, GT_PROPERTY_ICON);
            g_ObjectPropertiesManager.Reset();
        }
    }
}

void CConfigManager::SetCharacterBackpackStyle(uint8_t val)
{
    m_CharacterBackpackStyle = val;
    if (this == &g_ConfigManager && g_World != nullptr)
    {
        g_GumpManager.UpdateContent(g_PlayerSerial, 0, GT_PAPERDOLL);
        CGameItem *backpack = g_Player->FindLayer(OL_BACKPACK);
        if (backpack != nullptr)
        {
            g_GumpManager.UpdateContent(backpack->Serial, 0, GT_CONTAINER);
        }
    }
}

uint16_t CConfigManager::GetColorByNotoriety(uint8_t notoriety)
{
    uint16_t color = 0;
    switch ((NOTORIETY_TYPE)notoriety)
    {
        case NT_INNOCENT: //Blue
        {
            color = InnocentColor;
            break;
        }
        case NT_FRIENDLY: //Green
        {
            color = FriendlyColor;
            break;
        }
        case NT_SOMEONE_GRAY: //Someone gray
        {
            color = SomeoneColor;
            break;
        }
        case NT_CRIMINAL: //Criminal
        {
            color = CriminalColor;
            break;
        }
        case NT_ENEMY: //Orange
        {
            color = EnemyColor;
            break;
        }
        case NT_MURDERER: //Red*
        {
            color = MurdererColor;
            break;
        }
        case NT_INVULNERABLE:
        {
            color = 0x0034;
            break;
        }
        default:
            break;
    }

    return color;
}

bool CConfigManager::Load(const fs_path &path)
{
    if (!fs_path_exists(path))
    {
        return false;
    }

    int screenX, screenY;
    GetDisplaySize(&screenX, &screenY);
    screenX -= 20;
    screenY -= 60;

    TextFileParser file(path, "=", "#;", "");

    bool zoomed = false;
    int windowX = -1;
    int windowY = -1;
    int windowWidth = -1;
    int windowHeight = -1;
    UpdateRange = g_MaxViewRange;
    auto contPos = ContainerDefaultPosition();

    while (!file.IsEOF())
    {
        auto strings = file.ReadTokens();
        if (strings.size() >= 2)
        {
            const int code = cmkc::GetConfigKey(strings[0]);
            if (code == CMKC_NONE)
            {
                continue;
            }

            switch (code)
            {
                //Page 1
                case CMKC_SOUND:
                    SetSound(str_to_bool(strings[1]));
                    break;
                case CMKC_SOUND_VOLUME:
                    m_SoundVolume = str_to_int(strings[1]);
                    break;
                case CMKC_MUSIC:
                    SetMusic(str_to_bool(strings[1]));
                    break;
                case CMKC_MUSIC_VOLUME:
                    m_MusicVolume = str_to_int(strings[1]);
                    break;
                case CMKC_FOOTSTEPS_SOUND:
                    FootstepsSound = str_to_bool(strings[1]);
                    break;
                case CMKC_COMBAT_MUSIC:
                    CombatMusic = str_to_bool(strings[1]);
                    break;
                case CMKC_BACKGROUND_SOUND:
                    BackgroundSound = str_to_bool(strings[1]);
                    break;

                //Page 2
                case CMKC_CLIENT_FPS:
                    SetClientFPS(str_to_int(strings[1]));
                    break;
                case CMKC_USE_SCALING:
                    m_UseScaling = str_to_bool(strings[1]);
                    break;
                case CMKC_REMOVE_TEXT_WITH_BLENDING:
                    RemoveTextWithBlending = str_to_bool(strings[1]);
                    break;
                case CMKC_DRAW_STATUS_STATE:
                    m_DrawStatusState = str_to_int(strings[1]);
                    break;
                case CMKC_DRAW_STUMPS:
                    SetDrawStumps(str_to_bool(strings[1]));
                    break;
                case CMKC_MARKING_CAVES:
                    SetMarkingCaves(str_to_bool(strings[1]));
                    break;
                case CMKC_NO_ANIMATE_FIELDS:
                    SetNoAnimateFields(str_to_bool(strings[1]));
                    break;
                case CMKC_AUTO_OPEN_DOOR:
                    AutoOpenDoors = str_to_bool(strings[1]);
                    break;
                case CMKC_ALWAYS_DISPLAY_HUMANOIDS_NAME:
                    AlwaysDisplayHumanoidsName = str_to_bool(strings[1]);
                    break;
                case CMKC_NO_VEGETATION:
                    SetNoVegetation(str_to_bool(strings[1]));
                    break;
                case CMKC_HIDDEN_CHARACTERS_RENDER_MODE:
                    HiddenCharactersRenderMode = str_to_int(strings[1]);
                    break;
                case CMKC_HIDDEN_ALPHA:
                    HiddenAlpha = str_to_int(strings[1]);
                    break;
                case CMKC_USE_HIDDEN_MODE_ONLY_FOR_SELF:
                    UseHiddenModeOnlyForSelf = str_to_bool(strings[1]);
                    break;
                case CMKC_TRANSPARENT_SPELL_ICONS:
                    TransparentSpellIcons = str_to_bool(strings[1]);
                    break;
                case CMKC_SPELL_ICON_ALPHA:
                    m_SpellIconAlpha = str_to_int(strings[1]);
                    break;
                case CMKC_OLD_STYLE_STATUSBAR:
                    m_OldStyleStatusbar = str_to_bool(strings[1]);
                    break;
                case CMKC_ORIGINAL_PARTY_STATUSBAR:
                    m_OriginalPartyStatusbar = str_to_bool(strings[1]);
                    break;
                case CMKC_APPLY_STATE_COLOR_ON_CHARACTERS:
                    SetApplyStateColorOnCharacters(str_to_bool(strings[1]));
                    break;
                case CMKC_CHANGE_FIELDS_GRAPHIC:
                    SetChangeFieldsGraphic(str_to_bool(strings[1]));
                    break;
                case CMKC_PAPERDOLL_SLOTS:
                    SetPaperdollSlots(str_to_bool(strings[1]));
                    break;
                case CMKC_DRAW_STATUS_CONDITION_STATE:
                    DrawStatusConditionState = str_to_int(strings[1]);
                    break;
                case CMKC_DRAW_STATUS_CONDITION_VALUE:
                    DrawStatusConditionValue = str_to_int(strings[1]);
                    break;
                case CMKC_REMOVE_STATUSBARS_WITHOUT_OBJECTS:
                    RemoveStatusbarsWithoutObjects = str_to_bool(strings[1]);
                    break;
                case CMKC_SHOW_DEFAULT_CONSOLE_ENTRY_MODE:
                    ShowDefaultConsoleEntryMode = str_to_bool(strings[1]);
                    break;
                case CMKC_DRAW_AURA_STATE:
                    SetDrawAuraState(str_to_int(strings[1]));
                    break;
                case CMKC_DRAW_AURA_WITH_CTRL_PRESSED:
                    DrawAuraWithCtrlPressed = str_to_bool(strings[1]);
                    break;
                case CMKC_SCREENSHOT_FORMAT:
                    ScreenshotFormat = str_to_int(strings[1]);
                    break;
                case CMKC_SCALE_IMAGES_IN_PAPERDOLL_SLOTS:
                    SetScaleImagesInPaperdollSlots(str_to_bool(strings[1]));
                    break;
                case CMKC_REMOVE_OR_CREATE_OBJECTS_WITH_BLENDING:
                    RemoveOrCreateObjectsWithBlending = str_to_bool(strings[1]);
                    break;
                case CMKC_DRAW_HELMETS_ON_SHROUD:
                    DrawHelmetsOnShroud = str_to_bool(strings[1]);
                    break;
                case CMKC_USE_GLOBAL_MAP_LAYER:
                    SetUseGlobalMapLayer(str_to_bool(strings[1]));
                    break;
                case CMKC_NO_DRAW_ROOFS:
                    SetNoDrawRoofs(str_to_bool(strings[1]));
                    break;
                case CMKC_HIGHLIGHT_TARGET_BY_TYPE:
                    HighlightTargetByType = str_to_bool(strings[1]);
                    break;
                case CMKC_AUTO_DISPLAY_WORLD_MAP:
                    AutoDisplayWorldMap = str_to_bool(strings[1]);
                    break;
                case CMKC_DISABLE_MACRO_IN_CHAT:
                    DisableMacroInChat = str_to_bool(strings[1]);
                    break;
#if USE_PING
                case CMKC_CHECK_PING:
                    CheckPing = str_to_bool(strings[1]);
                    break;
                case CMKC_PING_TIMER:
                    SetPingTimer(str_to_int(strings[1]));
                    break;
#endif // USE_PING
                case CMKC_CANCEL_NEW_TARGET_SYSTEM_ON_SHIFT_ESC:
                    CancelNewTargetSystemOnShiftEsc = str_to_bool(strings[1]);
                    break;
                case CMKC_DRAW_STATUS_FOR_HUMANOIDS:
                    DrawStatusForHumanoids = str_to_bool(strings[1]);
                    break;

                //Page 3
                case CMKC_USE_TOOLTIPS:
                    UseToolTips = str_to_bool(strings[1]);
                    break;
                case CMKC_TOOLTIPS_TEXT_COLOR:
                    ToolTipsTextColor = str_to_int(strings[1]);
                    break;
                case CMKC_TOOLTIPS_TEXT_FONT:
                    ToolTipsTextFont = str_to_int(strings[1]);
                    break;
                case CMKC_TOOLTIPS_DELAY:
                    ToolTipsDelay = str_to_int(strings[1]);
                    break;

                //Page 4
                case CMKC_CHAT_COLOR_INPUT_TEXT:
                    ChatColorInputText = str_to_int(strings[1]);
                    break;
                case CMKC_CHAT_COLOR_MENU_OPTION:
                    ChatColorMenuOption = str_to_int(strings[1]);
                    break;
                case CMKC_CHAT_COLOR_PLAYER_IN_MEMBER_LIST:
                    ChatColorPlayerInMemberList = str_to_int(strings[1]);
                    break;
                case CMKC_CHAT_COLOR_TEXT:
                    ChatColorText = str_to_int(strings[1]);
                    break;
                case CMKC_CHAT_COLOR_PLAYER_NAME_WITHOUT:
                    ChatColorPlayerNameWithout = str_to_int(strings[1]);
                    break;
                case CMKC_CHAT_COLOR_MUTED:
                    ChatColorMuted = str_to_int(strings[1]);
                    break;
                case CMKC_CHAT_COLOR_CHANNEL_MODERATOR_NAME:
                    ChatColorChannelModeratorName = str_to_int(strings[1]);
                    break;
                case CMKC_CHAT_COLOR_CHANNEL_MODERATOR_TEXT:
                    ChatColorChannelModeratorText = str_to_int(strings[1]);
                    break;
                case CMKC_CHAT_COLOR_MY_NAME:
                    ChatColorMyName = str_to_int(strings[1]);
                    break;
                case CMKC_CHAT_COLOR_MY_TEXT:
                    ChatColorMyText = str_to_int(strings[1]);
                    break;
                case CMKC_CHAT_COLOR_SYSTEM_MESSAGE:
                    ChatColorSystemMessage = str_to_int(strings[1]);
                    break;
                case CMKC_CHAT_FONT:
                    ChatFont = str_to_int(strings[1]);
                    break;
                case CMKC_CHAT_COLOR_BG_OUTPUT_TEXT:
                    ChatColorBGOutputText = str_to_int(strings[1]);
                    break;
                case CMKC_CHAT_COLOR_BG_INPUT_TEXT:
                    ChatColorBGInputText = str_to_int(strings[1]);
                    break;
                case CMKC_CHAT_COLOR_BG_USER_LIST:
                    ChatColorBGUserList = str_to_int(strings[1]);
                    break;
                case CMKC_CHAT_COLOR_BG_CONF_LIST:
                    ChatColorBGConfList = str_to_int(strings[1]);
                    break;
                case CMKC_CHAT_COLOR_BG_COMMAND_LIST:
                    ChatColorBGCommandList = str_to_int(strings[1]);
                    break;

                //Page 6
                case CMKC_ENABLE_PATHFIND:
                    EnablePathfind = str_to_bool(strings[1]);
                    break;
                case CMKC_HOLD_TAB_FOR_COMBAT:
                    HoldTabForCombat = str_to_bool(strings[1]);
                    break;
                case CMKC_OFFSET_INTERFACE_WINDOWS:
                    OffsetInterfaceWindows = str_to_bool(strings[1]);
                    break;
                case CMKC_AUTO_ARRANGE:
                    AutoArrange = str_to_bool(strings[1]);
                    break;
                case CMKC_ALWAYS_RUN:
                    AlwaysRun = str_to_bool(strings[1]);
                    break;
                case CMKC_DISABLE_MENUBAR:
                    DisableMenubar = str_to_bool(strings[1]);
                    break;
                case CMKC_GRAY_OUT_OF_RANGE_OBJECTS:
                    GrayOutOfRangeObjects = str_to_bool(strings[1]);
                    break;
                case CMKC_DISABLE_NEW_TARGET_SYSTEM:
                    DisableNewTargetSystem = str_to_bool(strings[1]);
                    break;
                case CMKC_ITEMP_ROPERTIES_MODE:
                    m_ItemPropertiesMode = str_to_int(strings[1]);
                    break;
                case CMKC_ITEMP_ROPERTIES_ICON:
                    m_ItemPropertiesIcon = str_to_bool(strings[1]);
                    break;
                case CMKC_OBJECT_HANDLES:
                    ObjectHandles = str_to_bool(strings[1]);
                    break;
                case CMKC_OBJECT_HANDLES_NO_BODIES:
                    ObjectHandlesNoBodies = str_to_bool(strings[1]);
                    break;
                case CMKC_REDUCE_FPS_UNACTIVE_WINDOW:
                    SetReduceFPSUnactiveWindow(str_to_bool(strings[1]));
                    break;
                case CMKC_HOLD_SHIFT_FOR_CONTEXT_MENUS:
                    HoldShiftForContextMenus = str_to_bool(strings[1]);
                    break;
                case CMKC_HOLD_SHIFT_FOR_ENABLE_PATHFIND:
                    HoldShiftForEnablePathfind = str_to_bool(strings[1]);
                    break;
                case CMKC_CONTAINER_DEFAULT_X:
                    contPos.X = str_to_int(strings[1]);
                    break;
                case CMKC_CONTAINER_DEFAULT_Y:
                    contPos.Y = str_to_int(strings[1]);
                    break;

                //Page 7
                case CMKC_GAME_WINDOW_WIDTH:
                    GameWindowWidth = str_to_int(strings[1]);
                    break;
                case CMKC_GAME_WINDOW_HEIGHT:
                    GameWindowHeight = str_to_int(strings[1]);
                    break;
                case CMKC_SPEECH_DELAY:
                    SpeechDelay = str_to_int(strings[1]);
                    break;
                case CMKC_SCALE_SPEECH_DELAY:
                    ScaleSpeechDelay = str_to_bool(strings[1]);
                    break;
                case CMKC_SPEECH_COLOR:
                    SpeechColor = str_to_int(strings[1]);
                    break;
                case CMKC_EMOTE_COLOR:
                    EmoteColor = str_to_int(strings[1]);
                    break;
                case CMKC_PARTY_MESSAGE_COLOR:
                    PartyMessageColor = str_to_int(strings[1]);
                    break;
                case CMKC_GUILD_MESSAGE_COLOR:
                    GuildMessageColor = str_to_int(strings[1]);
                    break;
                case CMKC_ALLIANCE_MESSAGE_COLOR:
                    AllianceMessageColor = str_to_int(strings[1]);
                    break;
                case CMKC_IGNORE_GUILD_MESSAGE:
                    IgnoreGuildMessage = str_to_bool(strings[1]);
                    break;
                case CMKC_IGNORE_ALLIANCE_MESSAGE:
                    IgnoreAllianceMessage = str_to_bool(strings[1]);
                    break;
                case CMKC_DARK_NIGHTS:
                    DarkNights = str_to_bool(strings[1]);
                    break;
                case CMKC_COLORED_LIGHTING:
                    ColoredLighting = str_to_bool(strings[1]);
                    break;
                case CMKC_STANDART_CHARACTERS_ANIMATION_DELAY:
                    StandartCharactersAnimationDelay = str_to_bool(strings[1]);
                    break;
                case CMKC_STANDART_ITEMS_ANIMATION_DELAY:
                    StandartItemsAnimationDelay = str_to_bool(strings[1]);
                    break;
                case CMKC_LOCK_RESIZING_GAME_WINDOW:
                    LockResizingGameWindow = str_to_bool(strings[1]);
                    break;
                case CMKC_LOCK_GUMPS_MOVING:
                    LockGumpsMoving = str_to_bool(strings[1]);
                    break;

                //Page 8
                case CMKC_INNOCENT_COLOR:
                    InnocentColor = str_to_int(strings[1]);
                    break;
                case CMKC_FRIENDLY_COLOR:
                    FriendlyColor = str_to_int(strings[1]);
                    break;
                case CMKC_SOMEONE_COLOR:
                    SomeoneColor = str_to_int(strings[1]);
                    break;
                case CMKC_CRIMINAL_COLOR:
                    CriminalColor = str_to_int(strings[1]);
                    break;
                case CMKC_ENEMY_COLOR:
                    EnemyColor = str_to_int(strings[1]);
                    break;
                case CMKC_MURDERER_COLOR:
                    MurdererColor = str_to_int(strings[1]);
                    break;
                case CMKC_CRIMINAL_ACTIONS_QUERY:
                    CriminalActionsQuery = str_to_bool(strings[1]);
                    break;

                //Page 9
                case CMKC_SHOW_INCOMING_NAMES:
                    ShowIncomingNames = str_to_bool(strings[1]);
                    break;
                case CMKC_USE_CIRCLE_TRANS:
                    UseCircleTrans = str_to_bool(strings[1]);
                    break;
                case CMKC_STAT_REPORT:
                    StatReport = str_to_bool(strings[1]);
                    break;
                case CMKC_CONSOLE_NEED_ENTER:
                    SetConsoleNeedEnter(str_to_bool(strings[1]));
                    break;
                case CMKC_CIRCLE_TRANS_RADIUS:
                    CircleTransRadius = str_to_int(strings[1]);
                    g_CircleOfTransparency.Create(CircleTransRadius);
                    break;
                case CMKC_SKILL_REPORT:
                    SkillReport = str_to_int(strings[1]);
                    break;
                case CMKC_SPEECH_FONT:
                    SpeechFont = str_to_int(strings[1]);
                    break;
                case CMKC_LARGEGUMPS:
                    LargeGumps = str_to_bool(strings[1]);
                    break;
                //No page
                case CMKC_GAME_WINDOW_X:
                    GameWindowX = str_to_int(strings[1]);
                    break;
                case CMKC_GAME_WINDOW_Y:
                    GameWindowY = str_to_int(strings[1]);
                    break;
                case CMKC_ZOOMED:
                    zoomed = str_to_bool(strings[1]);
                    break;
                case CMKC_REAL_X:
                    windowX = str_to_int(strings[1]);
                    break;
                case CMKC_REAL_Y:
                    windowY = str_to_int(strings[1]);
                    break;
                case CMKC_REAL_WIDTH:
                    windowWidth = str_to_int(strings[1]);
                    break;
                case CMKC_REAL_HEIGHT:
                    windowHeight = str_to_int(strings[1]);
                    break;
                case CMKC_TOGGLE_BUFFICON_WINDOW:
                    ToggleBufficonWindow = str_to_bool(strings[1]);
                    break;
                case CMKC_DEVELOPER_MODE:
                    g_DeveloperMode = (DEVELOPER_MODE)str_to_int(strings[1]);
                    break;
                case CMKC_LAST_SERVER:
                    if (g_World == nullptr)
                    {
                        g_ServerList.LastServerName = strings[1];
                    }
                    break;
                case CMKC_LAST_CHARACTER:
                    if (g_World == nullptr)
                    {
                        g_CharacterList.LastCharacterName = strings[1];
                    }
                    break;
                case CMKC_CHARACTER_BACKPACK_STYLE:
                    m_CharacterBackpackStyle = str_to_int(strings[1]);
                    break;
                default:
                    break;
            }
        }
    }

    if (GameWindowX < 0)
    {
        GameWindowX = 0;
    }

    if (GameWindowY < 0)
    {
        GameWindowY = 0;
    }

    if (windowX != -1 && windowY != -1 && windowWidth != -1 && windowHeight != -1)
    {
        if (windowX < 0)
        {
            windowX = 0;
        }

        if (windowY < 0)
        {
            windowY = 0;
        }

        if (g_GameState >= GS_GAME)
        {
            if (windowWidth < 640)
            {
                windowWidth = 640;
            }

            if (windowWidth >= screenX)
            {
                windowWidth = screenX;
            }

            if (windowHeight < 480)
            {
                windowHeight = 480;
            }

            if (windowHeight >= screenY)
            {
                windowHeight = screenY;
            }
        }
        else
        {
            windowWidth = 640;
            windowHeight = 480;
        }

        if (zoomed)
        {
            g_GameScreen.SetMaximized(true);
#ifdef NEW_RENDERER_ENABLED
            // FIXME this should be deferred until there's something to draw
            // and set once at the beginning of the renderer frame, where there's
            // a command list
            int sdlWindowWidth, sdlWindowHeight;
            SDL_GetWindowSize(g_GameWindow.m_window, &sdlWindowWidth, &sdlWindowHeight);

            HACKRender_SetViewParams(SetViewParamsCmd{ 0,
                                                       0,
                                                       sdlWindowWidth,
                                                       sdlWindowHeight,
                                                       sdlWindowWidth,
                                                       sdlWindowHeight,
                                                       -150,
                                                       150 });
#endif
        }
        else
        {
            g_GameWindow.SetPositionSize(windowX, windowY, windowWidth, windowHeight);
#ifdef NEW_RENDERER_ENABLED
            // FIXME this should be deferred until there's something to draw
            // and set once at the beginning of the renderer frame, where there's
            // a command list
            HACKRender_SetViewParams(SetViewParamsCmd{
                0, 0, windowWidth, windowHeight, windowWidth, windowHeight, -150, 150 });
#endif
        }

#ifndef NEW_RENDERER_ENABLED
        g_GL.UpdateRect();
#else
        g_GumpManager.RedrawAll();
#endif
    }
    else
    {
        g_GameScreen.SetMaximized(true);
    }

    return true;
}

struct TextFileWriter
{
    FILE *m_File{ nullptr };

    TextFileWriter(const fs_path &path) { m_File = fs_open(path, FS_WRITE); }

    ~TextFileWriter() { Close(); }

    bool Opened() { return (m_File != nullptr); };
    void Close()
    {
        if (m_File != nullptr)
        {
            fs_close(m_File);
            m_File = nullptr;
        }
    }

    void WriteString(const astr_t &key, const astr_t &value)
    {
        if (m_File != nullptr)
        {
            fputs(astr_t(key + "=" + value + "\n").c_str(), m_File);
        }
    }

    void WriteInt(const astr_t &key, int value)
    {
        if (m_File != nullptr)
        {
            fputs(astr_t(key + "=" + str_from(value) + "\n").c_str(), m_File);
        }
    }

    void WriteBool(const astr_t &key, bool value)
    {
        if (m_File != nullptr)
        {
            fputs(astr_t(key + "=" + (value ? "yes" : "no") + "\n").c_str(), m_File);
        }
    }
};

void CConfigManager::Save(const fs_path &path)
{
    TextFileWriter writer(path);
    if (writer.Opened())
    {
        //Page 1
        writer.WriteBool("Sound", m_Sound);
        writer.WriteInt("SoundVolume", m_SoundVolume);
        writer.WriteBool("Music", m_Music);
        writer.WriteInt("MusicVolume", m_MusicVolume);
        writer.WriteBool("FootstepsSound", FootstepsSound);
        writer.WriteBool("CombatMusic", CombatMusic);
        writer.WriteBool("BackgroundSound", BackgroundSound);

        //Page 2
        writer.WriteInt("ClientFPS", m_ClientFPS);
        writer.WriteBool("UseScaling", m_UseScaling);
        writer.WriteBool("RemoveTextWithBlending", RemoveTextWithBlending);
        writer.WriteInt("DrawStatusState", m_DrawStatusState);
        writer.WriteBool("DrawStumps", m_DrawStumps);
        writer.WriteBool("MarkingCaves", m_MarkingCaves);
        writer.WriteBool("NoAnimateFields", m_NoAnimateFields);
        writer.WriteBool("NoVegetation", m_NoVegetation);
        writer.WriteInt("HiddenCharactersRenderMode", HiddenCharactersRenderMode);
        writer.WriteInt("HiddenAlpha", HiddenAlpha);
        writer.WriteBool("UseHiddenModeOnlyForSelf", UseHiddenModeOnlyForSelf);
        writer.WriteBool("TransparentSpellIcons", TransparentSpellIcons);
        writer.WriteInt("SpellIconAlpha", m_SpellIconAlpha);
        writer.WriteBool("OldStyleStatusbar", m_OldStyleStatusbar);
        writer.WriteBool("OriginalPartyStatusbar", m_OriginalPartyStatusbar);
        writer.WriteBool("ApplyStateColorOnCharacters", m_ApplyStateColorOnCharacters);
        writer.WriteBool("ChangeFieldsGraphic", m_ChangeFieldsGraphic);
        writer.WriteBool("PaperdollSlots", m_PaperdollSlots);
        writer.WriteInt("DrawStatusConditionState", DrawStatusConditionState);
        writer.WriteInt("DrawStatusConditionValue", DrawStatusConditionValue);
        writer.WriteBool("RemoveStatusbarsWithoutObjects", RemoveStatusbarsWithoutObjects);
        writer.WriteBool("ShowDefaultConsoleEntryMode", ShowDefaultConsoleEntryMode);
        writer.WriteInt("DrawAuraState", m_DrawAuraState);
        writer.WriteBool("DrawAuraWithCtrlPressed", DrawAuraWithCtrlPressed);
        writer.WriteInt("ScreenshotFormat", ScreenshotFormat);
        writer.WriteBool("ScaleImagesInPaperdollSlots", m_ScaleImagesInPaperdollSlots);
        writer.WriteBool("RemoveOrCreateObjectsWithBlending", RemoveOrCreateObjectsWithBlending);
        writer.WriteBool("DrawHelmetsOnShroud", DrawHelmetsOnShroud);
        writer.WriteBool("UseGlobalMapLayer", m_UseGlobalMapLayer);
        writer.WriteBool("NoDrawRoofs", m_NoDrawRoofs);
        writer.WriteBool("AutoOpenDoors", AutoOpenDoors);
        writer.WriteBool("HighlightTargetByType", HighlightTargetByType);
        writer.WriteBool("AutoDisplayWorldMap", AutoDisplayWorldMap);
        writer.WriteBool("DisableMacroInChat", DisableMacroInChat);
#if USE_PING
        writer.WriteBool("CheckPing", CheckPing);
        writer.WriteInt("PingTimer", m_PingTimer);
#endif // USE_PING
        writer.WriteBool("CancelNewTargetSystemOnShiftEsc", CancelNewTargetSystemOnShiftEsc);
        writer.WriteBool("DrawStatusForHumanoids", DrawStatusForHumanoids);
        writer.WriteBool("AlwaysDisplayHumanoidsName", AlwaysDisplayHumanoidsName);

        //Page 3
        writer.WriteBool("UseToolTips", UseToolTips);
        writer.WriteInt("ToolTipsTextColor", ToolTipsTextColor);
        writer.WriteInt("ToolTipsTextFont", ToolTipsTextFont);
        writer.WriteInt("ToolTipsDelay", ToolTipsDelay);

        //Page 4
        writer.WriteInt("ChatColorInputText", ChatColorInputText);
        writer.WriteInt("ChatColorMenuOption", ChatColorMenuOption);
        writer.WriteInt("ChatColorPlayerInMemberList", ChatColorPlayerInMemberList);
        writer.WriteInt("ChatColorText", ChatColorText);
        writer.WriteInt("ChatColorPlayerNameWithout", ChatColorPlayerNameWithout);
        writer.WriteInt("ChatColorMuted", ChatColorMuted);
        writer.WriteInt("ChatColorChannelModeratorName", ChatColorChannelModeratorName);
        writer.WriteInt("ChatColorChannelModeratorText", ChatColorChannelModeratorText);
        writer.WriteInt("ChatColorMyName", ChatColorMyName);
        writer.WriteInt("ChatColorMyText", ChatColorMyText);
        writer.WriteInt("ChatColorSystemMessage", ChatColorSystemMessage);
        writer.WriteInt("ChatFont", ChatFont);
        writer.WriteInt("ChatColorBGOutputText", ChatColorBGOutputText);
        writer.WriteInt("ChatColorBGInputText", ChatColorBGInputText);
        writer.WriteInt("ChatColorBGUserList", ChatColorBGUserList);
        writer.WriteInt("ChatColorBGConfList", ChatColorBGConfList);
        writer.WriteInt("ChatColorBGCommandList", ChatColorBGCommandList);

        //Page 6
        writer.WriteBool("EnablePathfind", EnablePathfind);
        writer.WriteBool("HoldTabForCombat", HoldTabForCombat);
        writer.WriteBool("OffsetInterfaceWindows", OffsetInterfaceWindows);
        writer.WriteBool("AutoArrange", AutoArrange);
        writer.WriteBool("AlwaysRun", AlwaysRun);
        writer.WriteBool("DisableMenubar", DisableMenubar);
        writer.WriteBool("GrayOutOfRangeObjects", GrayOutOfRangeObjects);
        writer.WriteBool("DisableNewTargetSystem", DisableNewTargetSystem);
        writer.WriteInt("ItemPropertiesMode", m_ItemPropertiesMode);
        writer.WriteBool("ItemPropertiesIcon", m_ItemPropertiesIcon);
        writer.WriteBool("ObjectHandles", ObjectHandles);
        writer.WriteBool("ObjectHandlesNoBodies", ObjectHandlesNoBodies);
        writer.WriteBool("ReduceFPSUnactiveWindow", m_ReduceFPSUnactiveWindow);
        writer.WriteBool("HoldShiftForContextMenus", HoldShiftForContextMenus);
        writer.WriteBool("HoldShiftForEnablePathfind", HoldShiftForEnablePathfind);
        const auto contPos = ContainerDefaultPosition();
        writer.WriteInt("ContainerDefaultX", contPos.X);
        writer.WriteInt("ContainerDefaultY", contPos.Y);
        writer.WriteInt("CharacterBackpackStyle", GetCharacterBackpackStyle());

        //Page 7
        writer.WriteInt("GameWindowWidth", GameWindowWidth);
        writer.WriteInt("GameWindowHeight", GameWindowHeight);
        writer.WriteInt("SpeechDelay", SpeechDelay);
        writer.WriteBool("ScaleSpeechDelay", ScaleSpeechDelay);
        writer.WriteInt("SpeechColor", SpeechColor);
        writer.WriteInt("EmoteColor", EmoteColor);
        writer.WriteInt("PartyMessageColor", PartyMessageColor);
        writer.WriteInt("GuildMessageColor", GuildMessageColor);
        writer.WriteInt("AllianceMessageColor", AllianceMessageColor);
        writer.WriteBool("IgnoreGuildMessage", IgnoreGuildMessage);
        writer.WriteBool("IgnoreAllianceMessage", IgnoreAllianceMessage);
        writer.WriteBool("DarkNights", DarkNights);
        writer.WriteBool("ColoredLighting", ColoredLighting);
        writer.WriteBool("StandartCharactersAnimationDelay", StandartCharactersAnimationDelay);
        writer.WriteBool("StandartItemsAnimationDelay", StandartItemsAnimationDelay);
        writer.WriteBool("LockResizingGameWindow", LockResizingGameWindow);
        writer.WriteBool("LockGumpsMoving", LockGumpsMoving);

        //Page 8
        writer.WriteInt("InnocentColor", InnocentColor);
        writer.WriteInt("FriendlyColor", FriendlyColor);
        writer.WriteInt("SomeoneColor", SomeoneColor);
        writer.WriteInt("CriminalColor", CriminalColor);
        writer.WriteInt("EnemyColor", EnemyColor);
        writer.WriteInt("MurdererColor", MurdererColor);
        writer.WriteBool("CriminalActionsQuery", CriminalActionsQuery);

        //Page 9
        writer.WriteBool("ShowIncomingNames", ShowIncomingNames);
        writer.WriteBool("UseCircleTrans", UseCircleTrans);
        writer.WriteBool("StatReport", StatReport);
        writer.WriteBool("ConsoleNeedEnter", m_ConsoleNeedEnter);
        writer.WriteInt("CircleTransRadius", CircleTransRadius);
        writer.WriteInt("SkillReport", SkillReport);
        writer.WriteInt("SpeechFont", SpeechFont);
        writer.WriteBool("LargeGumps", LargeGumps);

        //No page
        writer.WriteInt("GameWindowX", GameWindowX);
        writer.WriteInt("GameWindowY", GameWindowY);

        writer.WriteBool("Zoomed", g_GameWindow.IsMaximizedWindow());

        int x, y, w, h;
        g_GameWindow.GetPositionSize(&x, &y, &w, &h);

        writer.WriteInt("RealX", x);
        writer.WriteInt("RealY", y);
        writer.WriteInt("RealWidth", w);
        writer.WriteInt("RealHeight", h);

        writer.WriteBool("ToggleBufficonWindow", ToggleBufficonWindow);
        writer.WriteInt("DeveloperMode", g_DeveloperMode);

        writer.WriteString("LastServer", g_ServerList.LastServerName);
        writer.WriteString("LastCharacter", g_CharacterList.LastCharacterName);

        writer.Close();
    }
}
