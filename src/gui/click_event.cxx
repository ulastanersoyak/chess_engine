#include "gui/click_event.hxx"

[[nodiscard]] static auto
get_board_position (int mouse_x, int mouse_y) noexcept
    -> std::optional<square::coordinate>
{
  const int board_x = mouse_x - BOARD_OFFSET_X;
  const int board_y = mouse_y - BOARD_OFFSET_Y;

  if (board_x < 0 || board_x >= BOARD_SIZE * SQUARE_SIZE || board_y < 0
      || board_y >= BOARD_SIZE * SQUARE_SIZE)
    {
      return std::nullopt;
    }

  const auto file
      = static_cast<square::coordinate_type> (board_x / SQUARE_SIZE);
  const auto rank
      = static_cast<square::coordinate_type> (board_y / SQUARE_SIZE);

  return square::coordinate{ .file = file, .rank = rank };
}

[[nodiscard]] auto
get_clicked_square (SDL_Event &event, const board &b) noexcept
    -> std::optional<click_event>
{
  if (event.type != SDL_MOUSEBUTTONDOWN
      || event.button.button != SDL_BUTTON_LEFT)
    {
      return std::nullopt;
    }

  if (auto board_pos = get_board_position (event.button.x, event.button.y))
    {
      const auto &[file, rank] = *board_pos;
      return click_event{ .position = *board_pos,
                          .clicked_square = b.at (rank).at (file) };
    }

  return std::nullopt;
}
