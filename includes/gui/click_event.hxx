#pragma once

#include "board/board.hxx"

#include <SDL_events.h>

#include <optional>

struct click_event
{
  square::coordinate position;
  const square &clicked_square;
};

[[nodiscard]] auto get_clicked_square (SDL_Event &event,
                                       const board &board) noexcept
    -> std::optional<click_event>;
