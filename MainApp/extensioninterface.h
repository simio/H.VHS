/*
 * Copyright (c) 2012 Jesper Räftegård <jesper@huggpunkt.org>
 *
 * Permission to use, copy, modify, and distribute this software for any
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

#ifndef EXTENSIONINTERFACE_H
#define EXTENSIONINTERFACE_H

#include <QPointer>
#include <QDataStream>

#include "extensionhooks.h"
#include "cassette.h"

/*  H.VHS QPlugin Extension Interface, version 1.0
 *
 *  All extensions should use the herein declared d-pointer.
 *
 *  STREAM EXTENSIONS
 *  To the caller, this extension looks like a wrapper around a QDataStream.
 *  Every instance of the extension object exposes exactly one QDataStream.
 *  If, for example, a local file access extension is to be used to copy
 *  a file from one filename to another, two instances of that extension must
 *  be created; one object for reading from the source file, and another for
 *  writing to the target file.
 *
 *  The QDataStream must use QDataStream::setVersion(QDataStream::Qt_4_8).
 *
 *  When the stream is no longer needed, the instance will be deleted.
 *
 *  PLUGIN HOOK EXTENSIONS
 *  The application will create exactly one instance of each hooked extension,
 *  which will not be used as a stream extension. It will be called exactly
 *  once per plugin hook in the main application.
 *
 *  If, during this process, an extension instance suggests a hook call priority
 *  of zero, it might get deleted.
 *
 *  STREAM AND PLUGIN HOOK EXTENSIONS
 *  Extensions may provide both streams and hook handling. If they do, H.VHS
 *  will still create one instance of the extension to only handle hooks, plus
 *  additional instances for for stream handling, as specified above.
 *
 *  Instances not created for hook handling only won't get deleted if they
 *  suggest a hook priority of zero. This way, instances with active streams
 *  will get hook calls.
 */

enum ExtensionStreamControl {
    None,
    Start,
    Abort,
    Pause,
    Restart
};
enum ExtensionStreamErrorCode {
    Ok                  = 0,
    NotInitialised      = 1,
    UnspecifiedError    = 2,
    TransportFailure    = 4,
    FormatFailure       = 8,
    ControlFailure      = 16
};
enum ExtensionStreamDirection {
    Read,
    Write
};
enum ExtensionPluginHookHandling {
    Unhandled           = 0,
    Handled             = 1,
    Blocked             = 2,
    DataChanged         = 4
};

class ExtensionPrivate;
class ExtensionInterface
{
public:
    virtual ~ExtensionInterface();

    /*
     *      PLUGIN HOOK EXTENSION INTERFACE
     */

    /* A suggestion to what priority this extension should have
     * when constructing the pluginHook calling list.
     *
     *  0           means GO AWAY                           (Use if this instance will not handle hooks.)
     *  1           means F1RST PLZ!                        (Use only if blocking all other extensions.)
     *  2           means I FIX BROKEN THINGS               (Use only if fixing broken things.)
     *  3-9         are reserved for future use.            (Don't use them at all.)
     *
     *  100         means JUST CALL ME, THAT ALL            (Use if the extension doesn't care about ordering.)
     *
     * The main application (and user) may and quite possibly will
     * change the call order to their own preferences.
     */
    virtual int suggestedHookPriority() const;

    /* This member will get called exactly once each time the main application
     * hits a plugin hook. The return value should specify how this extension
     * handles the hook. Blocking results in no more extensions being called.
     * The order in which extensions are called is decided in the main application.
     *
     * The hook names are defined in extensionhooks.h
     */
    virtual qint64 pluginHook(const QString hookName, QPointer<QVariant> hookData);


    /*
     *      STREAM EXTENSION INTERFACE
     */

    virtual QList<qint64> availableStreamControls() const;          // Return a QList of the Control commands this object accepts.
    virtual bool streamControl(qint64 control);                     // Send a Control command. Return true for success.
    virtual bool streamReset();                                     // Reset extension to an empty, ready state. Returns true on success.
    virtual qint64 streamError() const;                             // Returns the current state(s) of the extension.
    virtual QString streamErrorStr() const;                         // Human readable description of current state. For prettyprinting only.

    // Initialise a connection. Returns false on failure, otherwise true.
    virtual bool streamInitialise(ExtensionStreamControl direction, QPointer<const Cassette> cassette);

    /* Returns a pointer to the QDataStream, or NULL if the internal
     * state (i.e. streamError()) is non-zero or this extension does not
     * supply data streams (in which case the extension XML definition
     * should not have any <inputs> or <outputs> elements, and this
     * member should not be called at all.
     */
    virtual QPointer<QDataStream> stream() const;


private:
    QPointer<ExtensionPrivate> *d;
    qint64 _state;
    QPointer<QDataStream> _stream;
};

Q_DECLARE_INTERFACE(ExtensionInterface,
                    "org.huggpunkt.VHS.ExtensionInterface/1.0")

#endif // EXTENSIONINTERFACE_H
