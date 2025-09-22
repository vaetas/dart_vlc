// This file is a part of dart_vlc (https://github.com/alexmercerind/dart_vlc)
//
// Copyright (C) 2021-2022 Hitesh Kumar Saini <saini123hitesh@gmail.com>
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

#ifndef VLC_VIDEO_OUTLET_H_
#define VLC_VIDEO_OUTLET_H_

#include <flutter_linux/flutter_linux.h>
#include <gtk/gtk.h>
#include <mutex>

#include "dart_vlc_plugin.h"

struct _VlcVideoOutletClass {
  FlPixelBufferTextureClass parent_class;
};

struct VlcVideoOutletPrivate {
  int64_t texture_id = 0;
  uint8_t* buffer = nullptr;
  int32_t video_width = 0;
  int32_t video_height = 0;
  std::mutex mutex;
};

G_DECLARE_DERIVABLE_TYPE(VlcVideoOutlet, vlc_video_outlet, DART_VLC, VLC_VIDEO_OUTLET,
                         FlPixelBufferTexture)

G_DEFINE_TYPE_WITH_CODE(VlcVideoOutlet, vlc_video_outlet,
                        fl_pixel_buffer_texture_get_type(),
                        G_ADD_PRIVATE(VlcVideoOutlet))

static gboolean vlc_video_outlet_copy_pixels(FlPixelBufferTexture* texture,
                                         const uint8_t** out_buffer,
                                         uint32_t* width, uint32_t* height,
                                         GError** error) {
  auto vlc_video_outlet_private =
      (VlcVideoOutletPrivate*)vlc_video_outlet_get_instance_private(
          DART_VLC_VLC_VIDEO_OUTLET(texture));
  std::lock_guard<std::mutex> lock(vlc_video_outlet_private->mutex);
  *out_buffer = vlc_video_outlet_private->buffer;
  *width = vlc_video_outlet_private->video_width;
  *height = vlc_video_outlet_private->video_height;
  return TRUE;
}

static VlcVideoOutlet* vlc_video_outlet_new() {
  return DART_VLC_VLC_VIDEO_OUTLET(g_object_new(vlc_video_outlet_get_type(), nullptr));
}

static void vlc_video_outlet_class_init(VlcVideoOutletClass* klass) {
  FL_PIXEL_BUFFER_TEXTURE_CLASS(klass)->copy_pixels = vlc_video_outlet_copy_pixels;
}

static void vlc_video_outlet_init(VlcVideoOutlet* self) {}

#endif
