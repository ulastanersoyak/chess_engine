#include "board/board.hxx"
#include "gui/background.hxx"
#include "gui/texture_cache.hxx"

#include <SDL_render.h>

#include <algorithm>
#include <optional>
#include <vector>

void
draw (SDL_Renderer &renderer, const board &b, const texture_cache &cache,
      const background &bg,
      const std::optional<square::coordinate> &selected_square,
      const std::vector<square::coordinate> &valid_moves) noexcept
{
  SDL_SetRenderDrawColor (&renderer, 38, 36, 33, 255);
  SDL_RenderClear (&renderer);

  bg.render (renderer);

  SDL_SetRenderDrawBlendMode (&renderer, SDL_BLENDMODE_BLEND);
  SDL_Rect overlay
      = { BOARD_OFFSET_X - 10, BOARD_OFFSET_Y - 10,
          (BOARD_SIZE * SQUARE_SIZE) + 20, (BOARD_SIZE * SQUARE_SIZE) + 20 };
  SDL_SetRenderDrawColor (&renderer, 0, 0, 0, 100);
  SDL_RenderFillRect (&renderer, &overlay);

  for (std::size_t row = 0; row < BOARD_SIZE; ++row)
    {
      for (std::size_t col = 0; col < BOARD_SIZE; ++col)
        {
          SDL_Rect square = {
            static_cast<std::int32_t> (BOARD_OFFSET_X + (col * SQUARE_SIZE)),
            static_cast<std::int32_t> (BOARD_OFFSET_Y + (row * SQUARE_SIZE)),
            SQUARE_SIZE, SQUARE_SIZE
          };

          const bool is_selected = selected_square.has_value ()
                                   && selected_square->file == col
                                   && selected_square->rank == row;

          const bool is_valid_move
              = std::ranges::find_if (valid_moves,
                                      [row, col] (const auto &move) {
                                        return move.rank == row
                                               && move.file == col;
                                      })
                != valid_moves.end ();

          if ((row + col) % 2 == 0)
            {
              SDL_SetRenderDrawColor (&renderer, 240, 217, 181, 230);
            }
          else
            {
              SDL_SetRenderDrawColor (&renderer, 181, 136, 99, 230);
            }
          SDL_RenderFillRect (&renderer, &square);

          if (is_selected)
            {
              SDL_SetRenderDrawColor (&renderer, 207, 171, 96, 100);
              SDL_RenderFillRect (&renderer, &square);
            }

          const auto piece = b.at (row).at (col);
          if (piece.is_occupied ())
            {
              if (SDL_Texture *texture
                  = cache.get_texture (piece.get_piece ().value ()))
                {
                  SDL_RenderCopy (&renderer, texture, nullptr, &square);
                }
            }

          if (is_valid_move)
            {
              if (piece.is_occupied ())
                {
                  SDL_SetRenderDrawColor (&renderer, 168, 50, 78, 255);
                  const int ring_thickness = 3;
                  const int ring_size = SQUARE_SIZE - 10;
                  SDL_Rect ring = { square.x + ((SQUARE_SIZE - ring_size) / 2),
                                    square.y + ((SQUARE_SIZE - ring_size) / 2),
                                    ring_size, ring_size };

                  for (int i = 0; i < ring_thickness; ++i)
                    {
                      SDL_RenderDrawRect (&renderer, &ring);
                      ring.x += 1;
                      ring.y += 1;
                      ring.w -= 2;
                      ring.h -= 2;
                    }
                }
              else
                {
                  SDL_SetRenderDrawColor (&renderer, 168, 50, 78, 255);
                  const int dot_size = SQUARE_SIZE / 4;
                  SDL_Rect dot = { square.x + ((SQUARE_SIZE - dot_size) / 2),
                                   square.y + ((SQUARE_SIZE - dot_size) / 2),
                                   dot_size, dot_size };
                  SDL_RenderFillRect (&renderer, &dot);
                }
            }
        }
    }
  SDL_SetRenderDrawBlendMode (&renderer, SDL_BLENDMODE_NONE);
  SDL_RenderPresent (&renderer);
}
