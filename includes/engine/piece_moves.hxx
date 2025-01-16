#pragma once
#include "board/board.hxx"

#include <vector>

[[nodiscard]] auto get_moves (const board &b, const square &sq) noexcept
    -> std::vector<square::coordinate>;
