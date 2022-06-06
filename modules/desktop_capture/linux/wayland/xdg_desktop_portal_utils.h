/*
 *  Copyright 2022 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#ifndef MODULES_DESKTOP_CAPTURE_LINUX_WAYLAND_XDG_DESKTOP_PORTAL_UTILS_H_
#define MODULES_DESKTOP_CAPTURE_LINUX_WAYLAND_XDG_DESKTOP_PORTAL_UTILS_H_

#include <gio/gio.h>
#include <stdint.h>

#include <string>
#include <vector>

#include "modules/desktop_capture/linux/wayland/portal_request_response.h"
#include "modules/desktop_capture/linux/wayland/scoped_glib.h"
#include "modules/desktop_capture/linux/wayland/xdg_session_details.h"
#include "rtc_base/checks.h"
#include "rtc_base/logging.h"

namespace webrtc {
namespace xdg_portal {

constexpr char kDesktopBusName[] = "org.freedesktop.portal.Desktop";
constexpr char kDesktopObjectPath[] = "/org/freedesktop/portal/desktop";
constexpr char kDesktopRequestObjectPath[] =
    "/org/freedesktop/portal/desktop/request";
constexpr char kSessionInterfaceName[] = "org.freedesktop.portal.Session";
constexpr char kRequestInterfaceName[] = "org.freedesktop.portal.Request";
constexpr char kScreenCastInterfaceName[] = "org.freedesktop.portal.ScreenCast";

using ProxyRequestCallback = void (*)(GObject*, GAsyncResult*, gpointer);
using SessionRequestCallback = void (*)(GDBusProxy*, GAsyncResult*, gpointer);
using SessionRequestResponseSignalHandler = void (*)(GDBusConnection*,
                                                     const char*,
                                                     const char*,
                                                     const char*,
                                                     const char*,
                                                     GVariant*,
                                                     gpointer);
using SessionRequestResponseSignalCallback = void (*)(std::string);
using StartRequestResponseSignalHandler = void (*)(GDBusConnection*,
                                                   const char*,
                                                   const char*,
                                                   const char*,
                                                   const char*,
                                                   GVariant*,
                                                   gpointer);
using SessionStartRequestedHandler = void (*)(GDBusProxy*,
                                              GAsyncResult*,
                                              gpointer);

std::string RequestResponseToString(RequestResponse request);

// Returns a string path for signal handle based on the provided connection and
// token.
std::string PrepareSignalHandle(const char* token, GDBusConnection* connection);

// Sets up the callback to execute when a response signal is received for the
// given object.
uint32_t SetupRequestResponseSignal(const char* object_path,
                                    const GDBusSignalCallback callback,
                                    gpointer user_data,
                                    GDBusConnection* connection);

void RequestSessionProxy(const char* interface_name,
                         const ProxyRequestCallback proxy_request_callback,
                         GCancellable* cancellable,
                         gpointer user_data);

void SetupSessionRequestHandlers(
    const std::string& portal_prefix,
    const SessionRequestCallback session_request_callback,
    const SessionRequestResponseSignalHandler request_response_signale_handler,
    GDBusConnection* connection,
    GDBusProxy* proxy,
    GCancellable* cancellable,
    std::string& portal_handle,
    guint& session_request_signal_id,
    gpointer user_data);

void StartSessionRequest(
    const std::string& prefix,
    const std::string session_handle,
    const StartRequestResponseSignalHandler signal_handler,
    const SessionStartRequestedHandler session_started_handler,
    GDBusProxy* proxy,
    GDBusConnection* connection,
    GCancellable* cancellable,
    guint& start_request_signal_id,
    std::string& start_handle,
    gpointer user_data);

// Tears down the portal session and cleans up related objects.
void TearDownSession(std::string session_handle,
                     GDBusProxy* proxy,
                     GCancellable* cancellable,
                     GDBusConnection* connection);

}  // namespace xdg_portal
}  // namespace webrtc

#endif  // MODULES_DESKTOP_CAPTURE_LINUX_WAYLAND_XDG_DESKTOP_PORTAL_UTILS_H_