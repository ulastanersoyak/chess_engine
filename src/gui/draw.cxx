#include "gui/draw.hxx"

void
draw (SDL_Renderer &renderer, const board &b, const texture_cache &cache,
      const std::optional<square::coordinate> &highlighted_square) noexcept
{
  SDL_SetRenderDrawColor (&renderer, 50, 46, 43, 255);
  SDL_RenderClear (&renderer);

  for (std::size_t row = 0; row < BOARD_SIZE; ++row)
    {
      for (std::size_t col = 0; col < BOARD_SIZE; ++col)
        {
          SDL_Rect square = {
            static_cast<std::int32_t> (BOARD_OFFSET_X + (col * SQUARE_SIZE)),
            static_cast<std::int32_t> (BOARD_OFFSET_Y + (row * SQUARE_SIZE)),
            SQUARE_SIZE, SQUARE_SIZE
          };

          const bool is_highlighted = highlighted_square.has_value ()
                                      && highlighted_square->file == col
                                      && highlighted_square->rank == row;

          if (is_highlighted)
            {
              SDL_SetRenderDrawColor (&renderer, 186, 202, 68,
                                      255); // Highlight color
            }
          else if ((row + col) % 2 == 0)
            {
              SDL_SetRenderDrawColor (&renderer, 241, 217, 181,
                                      255); // Light squares
            }
          else
            {
              SDL_SetRenderDrawColor (&renderer, 181, 136, 99,
                                      255); // Dark squares
            }

          SDL_RenderFillRect (&renderer, &square);

          const auto piece = b.at (row).at (col);
          if (piece.is_occupied ())
            {
              if (SDL_Texture *texture
                  = cache.get_texture (piece.get_piece ().value ()))
                {
                  SDL_RenderCopy (&renderer, texture, nullptr, &square);
                }
            }
        }
    }

  SDL_RenderPresent (&renderer);
}
