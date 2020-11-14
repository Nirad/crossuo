// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2020 Danny Angelo Carminati Grein

#pragma once

struct xuo_context;
struct xuo_release;

#include "revision.h"

#define XUOL_AGENT_NAME LAUNCHER_TITLE
#if defined(XUO_RELEASE)
#define XUOL_UPDATER_HOST "https://raw.githubusercontent.com/crossuo/update/release/"
// "http://update.crossuo.com/"
#else
#define XUOL_UPDATER_HOST "https://raw.githubusercontent.com/crossuo/update/release/"
//#define XUOL_UPDATER_HOST "http://localhost:12593/"
#endif

typedef void (*xuo_release_cb)(const char *release, const char *version, bool latest);

xuo_context *xuo_init(const char *path, bool beta = false);
const char *xuo_platform_name();
const char *xuo_changelog(xuo_context *ctx);
void xuo_releases_iterate(xuo_context *ctx, xuo_release_cb pFunc);
bool xuo_release_get(xuo_context *ctx, const char *name, const char *version = nullptr);
bool xuo_update_check(xuo_context *ctx);
bool xuo_update_apply(xuo_context *ctx);
void xuo_shutdown(xuo_context *ctx);
