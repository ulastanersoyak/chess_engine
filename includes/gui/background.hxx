#pragma once

#include "gui/raii_wrapper.hxx"

#include <SDL2/SDL.h>
#include <SDL_image.h>

class background
{
public:
  explicit background (SDL_Renderer &rend) noexcept
      : renderer{ rend }, texture{ nullptr, SDL_DestroyTexture }
  {
  }

  [[nodiscard]] auto
  load_texture (const char *path) noexcept -> bool
  {
    SDL_wrapper surface{ IMG_Load (path), SDL_FreeSurface };
    if (surface.get () == nullptr)
      {
        return false;
      }

    width = surface.get ()->w;
    height = surface.get ()->h;

    texture = SDL_wrapper{ SDL_CreateTextureFromSurface (&renderer,
                                                         surface.get ()),
                           SDL_DestroyTexture };

    return texture.get () != nullptr;
  }

  void
  render (SDL_Renderer &rend) const noexcept
  {
    if (texture.get () == nullptr)
      {
        return;
      }

    const float window_ratio = 800.0F / 800.0F;
    const float image_ratio
        = static_cast<float> (width) / static_cast<float> (height);

    SDL_Rect dest;
    if (window_ratio > image_ratio)
      {
        dest.w = 800;
        dest.h = static_cast<int> (800 / image_ratio);
        dest.x = 0;
        dest.y = (800 - dest.h) / 2;
      }
    else
      {
        dest.h = 800;
        dest.w = static_cast<int> (800 * image_ratio);
        dest.x = (800 - dest.w) / 2;
        dest.y = 0;
      }

    SDL_SetTextureAlphaMod (texture.get (), 180);
    SDL_RenderCopy (&rend, texture.get (), nullptr, &dest);
  }

private:
  SDL_Renderer &renderer;
  SDL_wrapper<SDL_Texture *, decltype (&SDL_DestroyTexture)> texture;
  int width{ 0 };
  int height{ 0 };
};
