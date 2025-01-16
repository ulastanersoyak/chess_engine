#pragma once

#include "board/board.hxx"
#include "gui/background.hxx"
#include "gui/texture_cache.hxx"

#include <SDL2/SDL.h>

#include <optional>
#include <vector>

void draw (SDL_Renderer &renderer, const board &b, const texture_cache &cache,
           const background &bg,
           const std::optional<square::coordinate> &selected_square,
           const std::vector<square::coordinate> &valid_moves) noexcept;
