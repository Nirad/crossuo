// Copyright (C) December 2019 Nirad

#include "GumpVisualUo.h"
#include "GumpSelectColor.h"
#include "GumpSelectFont.h"
#include "../Config.h"
#include "../ToolTip.h"
#include "../PressedObject.h"
#include "../SelectedObject.h"
#include "../GameWindow.h"
#include "../TextEngine/GameConsole.h"
#include "../Managers/ConfigManager.h"
#include "../Managers/GumpManager.h"
#include "../Managers/ColorManager.h"

#define KeyName(x) SDL_GetKeyName(x)

const uint16_t g_OptionsTextColor = 0;

enum
{
    ID_GO_NONE,

    ID_GO_PAGE_1,
    ID_GO_PAGE_2,
    ID_GO_CANCEL,
    ID_GO_APPLY,
    ID_GO_DEFAULT,
    ID_GO_OKAY,

    // Config
    ID_GO_P1_SOUND_ON_OFF,
    ID_GO_P1_MUSIC_ON_OFF,
    ID_GO_P1_PLAY_FOOTSTEP_SOUNDS,
    ID_GO_P1_PLAY_COMBAT_MUSIC,
    ID_GO_P1_SOUND_VOLUME,
    ID_GO_P1_MUSIC_VOLUME,
    ID_GO_P1_BACKGROUND_SOUND,

    // VisualUo Explorer
    ID_GO_P2_DEV_MODE_1,
    ID_GO_P2_DEV_MODE_2,
    ID_GO_P2_DEV_MODE_3,
};

CGumpVisualUo::CGumpVisualUo(short x, short y)
    : CGump(GT_NONE, 0, x, y)
{
    Page = 2;
}

CGumpVisualUo::~CGumpVisualUo()
{
}

void CGumpVisualUo::CalculateGumpState()
{
    CGump::CalculateGumpState();

    if (g_GumpPressed)
    {
        if (g_PressedObject.LeftObject != nullptr &&
            ((CBaseGUI *)g_PressedObject.LeftObject)->Type == GOT_COMBOBOX)
        {
            g_GumpMovingOffset.Reset();

            if (Minimized)
            {
                g_GumpTranslate.X = (float)MinimizedX;
                g_GumpTranslate.Y = (float)MinimizedY;
            }
            else
            {
                g_GumpTranslate.X = (float)m_X;
                g_GumpTranslate.Y = (float)m_Y;
            }
        }
        else
        {
            WantRedraw = true;
        }
    }
}

void CGumpVisualUo::PrepareContent()
{
    WantRedraw = true;
}

void CGumpVisualUo::UpdateContent()
{
    Clear();

    //Body
    Add(new CGUIResizepic(0, 0x0A28, 40, 0, 550, 450));

    //Left page buttons

    //Sound and Music
    CGUIButton *button =
        (CGUIButton *)Add(new CGUIButton(ID_GO_PAGE_1, 0x00DA, 0x00DA, 0x00DA, 0, 45));
    button->ToPage = 1;

    //VisualUo Explorer
    button = (CGUIButton *)Add(new CGUIButton(
        ID_GO_PAGE_2, 0x00E3, 0x00E3, 0x00E3, 0, 111)); // Fixme: no other side for this icon
    button->ToPage = 2;

    Add(new CGUIButton(ID_GO_CANCEL, 0x00F3, 0x00F2, 0x00F1, 154, 405));
    Add(new CGUIButton(ID_GO_APPLY, 0x00EF, 0x00EE, 0x00F0, 248, 405));
    Add(new CGUIButton(ID_GO_DEFAULT, 0x00F6, 0x00F5, 0x00F4, 346, 405));
    Add(new CGUIButton(ID_GO_OKAY, 0x00F9, 0x00F7, 0x00F8, 443, 405));

    DrawPage1(); //VisualUo config
    DrawPage2(); //VisualUo Explorer
}

void CGumpVisualUo::Init()
{
    WantUpdateContent = true;
}

void CGumpVisualUo::InitToolTip()
{
    uint32_t id = g_SelectedObject.Serial;

    switch (id)
    {
        case ID_GO_PAGE_1:
        {
            g_ToolTip.Set(L"See the 'Config of VisualUo'");
            break;
        }
        case ID_GO_PAGE_2:
        {
            g_ToolTip.Set(L"See the 'VisualUo' explorer page");
            break;
        }
        case ID_GO_CANCEL:
        {
            g_ToolTip.Set(L"Cancel changes and close the configuration gump");
            break;
        }
        case ID_GO_APPLY:
        {
            g_ToolTip.Set(L"Apply changes on current page");
            break;
        }
        case ID_GO_DEFAULT:
        {
            g_ToolTip.Set(L"Set default values on current page");
            break;
        }
        case ID_GO_OKAY:
        {
            g_ToolTip.Set(L"Apply changes on current page and close the configuration gump");
            break;
        }
        default:
            break;
    }
}

void CGumpVisualUo::DrawPage1()
{
    //VisualUo config
    Add(new CGUIPage(1));

    Add(new CGUIGumppic(0x00D9, 0, 45));

    CGUIText *text = (CGUIText *)Add(new CGUIText(g_OptionsTextColor, 84, 22));
    text->CreateTextureW(0, L"Config", 30, 460, TS_CENTER);
}

void CGumpVisualUo::DrawPage2()
{
    //VisualUo Explorer
    Add(new CGUIPage(2));

    Add(new CGUIGumppic(0x00E3, 0, 111));

    CGUIText *text = (CGUIText *)Add(new CGUIText(g_OptionsTextColor, 84, 22));
    text->CreateTextureW(0, L"VisualUo Explorer", 30, 460, TS_CENTER);

    text = (CGUIText *)Add(new CGUIText(g_OptionsTextColor, 64, 44));
    text->CreateTextureW(0, L"Chose file to explore :");

    CGUIRadio *devRadio =
        (CGUIRadio *)Add(new CGUIRadio(ID_GO_P2_DEV_MODE_1, 0x00D0, 0x00D1, 0x00D2, 64, 64));
    devRadio->Checked = true; //(g_DeveloperMode == DM_NO_DEBUG);
    devRadio->SetTextParameters(0, L"Gump", g_OptionsTextColor);

    devRadio =
        (CGUIRadio *)Add(new CGUIRadio(ID_GO_P2_DEV_MODE_2, 0x00D0, 0x00D1, 0x00D2, 164, 64));
    devRadio->Checked = false; //(g_DeveloperMode == DM_SHOW_FPS_ONLY);
    devRadio->SetTextParameters(0, L"Anim", g_OptionsTextColor);

    devRadio =
        (CGUIRadio *)Add(new CGUIRadio(ID_GO_P2_DEV_MODE_3, 0x00D0, 0x00D1, 0x00D2, 264, 64));
    devRadio->Checked = false; //(g_DeveloperMode == DM_DEBUGGING);
    devRadio->SetTextParameters(0, L"Sound", g_OptionsTextColor);

    //Explorer Win
    CGUIHTMLGump *html =
        (CGUIHTMLGump *)Add(new CGUIHTMLGump(0, 0x0A3C, 75, 85, 400, 300, true, true));

    // Display Button
    Add(new CGUIButton(0, 0x098B, 0x098B, 0x098B, 495, 95));

    CGUITextEntry *entry = (CGUITextEntry *)Add(
        new CGUITextEntry(0, 0, 0x0036, 0x0036, 503, 96, 50, true, 1, TS_CENTER));
    entry->m_Entry.SetTextW(L"Display");
    entry->CheckOnSerial = true;
    entry->ReadOnly = true;
    entry->FocusedOffsetY = 2;

    //TODO : For loop and include in the html gump

    //TEST
    html->Add(new CGUIGumppic(0x0001, 10, 10));

    //TEST
    html->Add(new CGUIGumppic(0x0002, 10, 55));

    //TEST
    html->Add(new CGUIGumppic(0x0003, 10, 55 + 45));
}

void CGumpVisualUo::GUMP_BUTTON_EVENT_C
{
    if (serial == ID_GO_CANCEL) //Cancel
    {
        RemoveMark = true;
    }
    else if (serial == ID_GO_APPLY) //Apply
    {
        ApplyPageChanges();
    }
    else if (serial == ID_GO_DEFAULT) //Default
    {
        switch (Page)
        {
            case 1:
            {
                break;
            }
            case 2:
            {
                break;
            }
            default:
                break;
        }

        WantUpdateContent = true;
    }
    else if (serial == ID_GO_OKAY) //Okay
    {
        ApplyPageChanges();
        RemoveMark = true;
    }
    else
    {
        switch (Page)
        {
            case 1: //VisualUo Config
            {
                break;
            }
            case 2: //VisualUo Explorer
            {
                break;
            }
            default:
                break;
        }
    }
}

void CGumpVisualUo::GUMP_CHECKBOX_EVENT_C
{
    switch (Page)
    {
        case 1: //Some config
        {
            // TODO:
            break;
        }
        case 2: //VisualUo Explorer
        {
            // TODO:
            break;
        }
        default:
            break;
    }
}

void CGumpVisualUo::GUMP_RADIO_EVENT_C
{
    if (!state)
    {
        return;
    }

    switch (Page)
    {
        case 1: //Sound and Music
        {
            break;
        }
        case 2: //CrossUO's configuration
        {
            if (serial == ID_GO_P2_DEV_MODE_1)
            {
            }
            else if (serial == ID_GO_P2_DEV_MODE_2)
            {
            }
            else if (serial == ID_GO_P2_DEV_MODE_3)
            {
            }
            break;
        }
        default:
            break;
    }
}

void CGumpVisualUo::GUMP_SLIDER_CLICK_EVENT_C
{
    OnSliderMove(serial);
}

void CGumpVisualUo::GUMP_SLIDER_MOVE_EVENT_C
{
    switch (Page)
    {
        case 1: //Sound and Music
        {
            break;
        }
        case 2: //VisualUo Explorer
        {
            break;
        }
        default:
            break;
    }
}

void CGumpVisualUo::GUMP_COMBOBOX_SELECTION_EVENT_C
{
}

void CGumpVisualUo::OnTextInput(const TextEvent &ev)
{
    const auto ch = EvChar(ev);
    if (g_EntryPointer == &m_GameWindowWidth->m_Entry ||
        g_EntryPointer == &m_GameWindowHeight->m_Entry)
    {
        if (ch >= '0' && ch <= '9')
        {
            g_EntryPointer->Insert(ch);

            int val = atoi(g_EntryPointer->c_str());
            if (val > 2000)
            {
                g_EntryPointer->Remove(true);
            }
            else
            {
                WantRedraw = true;
            }
        }
    }
}

void CGumpVisualUo::OnKeyDown(const KeyEvent &ev)
{
    const auto key = EvKey(ev);
    if (key == KEY_RETURN || key == KEY_RETURN2)
    {
        if (g_ConfigManager.GetConsoleNeedEnter())
        {
            g_EntryPointer = nullptr;
        }
        else
        {
            g_EntryPointer = &g_GameConsole;
        }
        WantRedraw = true;
    }
}

void CGumpVisualUo::ApplyPageChanges()
{
    switch (Page)
    {
        case 1: //VisualUo Config
        {
            break;
        }
        case 2: //VisualUo Explorer option
        {
            break;
        }
        default:
            break;
    }
}
