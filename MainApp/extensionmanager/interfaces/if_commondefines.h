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

// Cross-platform cleverness is not allowed here. Every value
// must be defined on all or no platforms, at build time, due
// to this file being parsed and recreated as a .qs file.

#ifndef EXTENSIONINTERFACES_IF_COMMONDEFINES_H
#define EXTENSIONINTERFACES_IF_COMMONDEFINES_H

#define EXT_INTERFACE_NOT_SUPPORTED            -1
#define HVHS_INTERFACE_COMMON                   "org.huggpunkt.VHS.Common/1.0"
#define HVHS_INTERFACE_HOOKS                    "org.huggpunkt.VHS.Hooks/1.0"
#define HVHS_INTERFACE_STREAMS                  "org.huggpunkt.VHS.Streams/1.0"

// Extension::suggestHookPriority()
#define EXT_NO_HOOK_PRIORITY_SUGGESTION        -1


/*
 *  Extension::pluginHook() definitions
 */

// Extension::pluginHook() return values. Obviously, these
// are flags, and not mutually exclusive.
#define EXT_RETVAL_NOOP                         0
#define EXT_RETVAL_DONE                         1
#define EXT_RETVAL_BLOCK                        2
#define EXT_RETVAL_DATA_MODIFIED                4

// Extension control hooks. Cannot be overridden from extensions
#define EXT_HOOK_INIT_EXTENSION_PERSISTENT      10
#define EXT_HOOK_INIT_EXTENSION                 20
#define EXT_HOOK_BEFORE_KILL_EXT                30

// WebWindow hooks
#define EXT_HOOK_AFTER_WEBWINDOW_CONSTRUCTOR    100
#define EXT_HOOK_WEBVIEW_LOADSTARTED            400
#define EXT_HOOK_WEBVIEW_LOADREQUESTED          410
#define EXT_HOOK_WEBVIEW_LOADFINISHED           420


#endif // EXTENSIONINTERFACES_IF_COMMONDEFINES_H
