// Copyright (C) December 2019 Nirad

#pragma once

#include "Gump.h"
#include "../Platform.h"

class CGumpVisualUo : public CGump
{
private:
    void DrawPage1(); //Config
    void DrawPage2(); //VisualUo Explorer

    void ApplyPageChanges();

protected:
    virtual void CalculateGumpState() override;

public:
    CGumpVisualUo(short x, short y);
    virtual ~CGumpVisualUo();

    CGUITextEntry *m_GameWindowWidth{ nullptr };
    CGUITextEntry *m_GameWindowHeight{ nullptr };

    void UpdateColor(const SELECT_COLOR_GUMP_STATE &state, uint16_t color);
    virtual void InitToolTip() override;
    virtual void PrepareContent() override;
    virtual void UpdateContent() override;
    virtual void Init();

    GUMP_BUTTON_EVENT_H override;
    GUMP_CHECKBOX_EVENT_H override;
    GUMP_RADIO_EVENT_H override;
    GUMP_COMBOBOX_SELECTION_EVENT_H override;
    GUMP_SLIDER_CLICK_EVENT_H override;
    GUMP_SLIDER_MOVE_EVENT_H override;

    virtual void OnTextInput(const TextEvent &ev) override;
    virtual void OnKeyDown(const KeyEvent &ev) override;
};
