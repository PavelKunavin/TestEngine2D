#pragma once
#include <te2d_types.hpp>
#include <glad/glad.h>
#include <stb_image.h>
#include <core/te2d_log.hpp>

struct te2d_texture
{
 u32 id = 0;
 i32 width = 0;
 i32 height = 0;
 i32 channels = 0; // всегда 4 (RGBA)

 // Фабрика: загружает PNG, заливает в GPU
static te2d_texture load(const char* path) {
    te2d_texture result = {};

    i32 w, h, c;
    u8* data = stbi_load(path, &w, &h, &c, 4);
    if (!data) {
        TE2D_ERROR("TEXTURE", "Failed to load: %s", path);
        return result;
    }

    TE2D_INFO("TEXTURE", "Loaded: %s | id=1 size=%dx%d", path, w, h);

    GLuint gl_id = 0;
    glGenTextures(1, &gl_id);
    glBindTexture(GL_TEXTURE_2D, gl_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(data);

    result.id = gl_id;
    result.width = w;
    result.height = h;
    result.channels = 4;

    return result;
}

 // Выгружает текстуру из GPU
 void unload()
 {
  if (id)
  {
   glDeleteTextures(1, &id);
   id = 0;
   width = 0;
   height = 0;
   channels = 0;
  }
 }

 // Привязывает текстуру к текстурному блоку
 void bind(u32 unit) const
 {
  if (id)
  {
   glActiveTexture(GL_TEXTURE0 + unit);
   glBindTexture(GL_TEXTURE_2D, id);
  }
 }
};