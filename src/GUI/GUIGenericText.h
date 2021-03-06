﻿// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2016 Hotride

#pragma once

#include <common/str.h>
#include "GUIText.h"

struct CGUIGenericText : public CGUIText
{
    uint32_t TextID = 0;
    int MaxWidth = 0;

    void Create(const wstr_t &str);

    CGUIGenericText(int index, uint16_t color, int x, int y, int maxWidth = 0)
        : CGUIText(color, x, y)
        , TextID(index)
        , MaxWidth(maxWidth)
    {
    }

    virtual ~CGUIGenericText() = default;
};
