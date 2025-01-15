#pragma once

#include "board/board.hxx"
#include "gui/texture_cache.hxx"

#include <SDL2/SDL.h>

#include <optional>

void draw (SDL_Renderer &renderer, const board &b, const texture_cache &cache,
           const std::optional<square::coordinate> &highlighted_square
           = std::nullopt) noexcept;
