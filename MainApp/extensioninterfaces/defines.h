/*
 * Copyright (c) 2012 Jesper Räftegård <jesper@huggpunkt.org>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef EXTENSIONINTERFACES_DEFINES_H
#define EXTENSIONINTERFACES_DEFINES_H

#define HVHS_HOOKS_INTERFACE                    "org.huggpunkt.VHS.Hooks/1.0"
#define HVHS_STREAMS_INTERFACE                  "org.huggpunkt.VHS.Streams/1.0"

// Extension::pluginHook() hooks
#define EXT_HOOK_INIT_EXTENSION_PERSISTENT      10
#define EXT_HOOK_INIT_EXTENSION                 20
#define EXT_HOOK_BEFORE_KILL_EXT                30
#define EXT_HOOK_WEBVIEW_LOADSTARTED            40
#define EXT_HOOK_WEBVIEW_LOADFINISHED           50
#define EXT_HOOK_AFTER_APP_INITIALISED          60

// Extension::pluginHook() return values (is NOOP or any combination of the other)
#define EXT_RETVAL_NOOP                         0
#define EXT_RETVAL_DONE                         1
#define EXT_RETVAL_BLOCK                        2
#define EXT_RETVAL_DATA_MODIFIED                4

#endif // EXTENSIONINTERFACES_DEFINES_H
