#include "board/square.hxx"
#include "board_init.hxx"
#include "gui/raii_wrapper.hxx"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <cstddef>
#include <filesystem>
#include <print>

int
main (int argc, char *argv[])
{
  if (argc != 2)
    {
      std::println ("usage: {} <path-to-assets-folder>", argv[0]);
      return 1;
    }

  std::filesystem::path assets_path = argv[1];

  static constinit auto b = initial_board ();

  if (SDL_Init (SDL_INIT_VIDEO) < 0)
    {
      SDL_Log ("SDL initialization failed: %s\n", SDL_GetError ());
      return -1;
    }

  if (!(IMG_Init (IMG_INIT_PNG) & IMG_INIT_PNG))
    {
      SDL_Log ("SDL_image initialization failed: %s\n", IMG_GetError ());
      SDL_Quit ();
      return -1;
    }

  SDL_wrapper window (SDL_CreateWindow ("Chess", SDL_WINDOWPOS_CENTERED,
                                        SDL_WINDOWPOS_CENTERED, 800, 800,
                                        SDL_WINDOW_SHOWN),
                      SDL_DestroyWindow);

  SDL_wrapper renderer (SDL_CreateRenderer (window.get (), -1,
                                            SDL_RENDERER_ACCELERATED
                                                | SDL_RENDERER_PRESENTVSYNC),
                        SDL_DestroyRenderer);

  constexpr auto BOARD_SIZE = 8;
  constexpr auto SQUARE_SIZE = 80;
  constexpr auto BOARD_OFFSET_X = (800 - (BOARD_SIZE * SQUARE_SIZE)) / 2;
  constexpr auto BOARD_OFFSET_Y = (800 - (BOARD_SIZE * SQUARE_SIZE)) / 2;

  bool running = true;
  while (running)
    {
      SDL_Event event;
      while (SDL_PollEvent (&event) != 0)
        {
          if (event.type == SDL_QUIT)
            {
              running = false;
            }
        }

      SDL_SetRenderDrawColor (renderer.get (), 40, 40, 40, 255);
      SDL_RenderClear (renderer.get ());

      for (std::size_t row = 0; row < BOARD_SIZE; ++row)
        {
          for (std::size_t col = 0; col < BOARD_SIZE; ++col)
            {
              SDL_Rect square = { static_cast<std::int32_t> (
                                      BOARD_OFFSET_X + (col * SQUARE_SIZE)),
                                  static_cast<std::int32_t> (
                                      BOARD_OFFSET_Y + (row * SQUARE_SIZE)),
                                  SQUARE_SIZE, SQUARE_SIZE };

              if ((row + col) % 2 == 0)
                {
                  SDL_SetRenderDrawColor (renderer.get (), 238, 238, 210, 255);
                }
              else
                {
                  SDL_SetRenderDrawColor (renderer.get (), 118, 150, 86, 255);
                }
              SDL_RenderFillRect (renderer.get (), &square);

              const auto piece = b.at (row).at (col);
              if (piece.is_occupied ())
                {
                  const auto piece_path
                      = (assets_path
                         / (piece.get_piece ().value ().is_black () ? "black"
                                                                    : "white")
                         / piece.get_piece ().value ().get_type_as_str ())
                            .concat (".png");

                  if (!std::filesystem::exists (piece_path))
                    {
                      std::println ("could not find texture at: {}",
                                    piece_path.string ());
                      // return -1;
                      continue;
                    }

                  SDL_wrapper temp_surface (IMG_Load (piece_path.c_str ()),
                                            SDL_FreeSurface);

                  if (!temp_surface.get ())
                    {
                      SDL_Log ("failed to load texture: %s\n",
                               IMG_GetError ());
                      return -1;
                    }

                  SDL_wrapper pawn_texture (
                      SDL_CreateTextureFromSurface (renderer.get (),
                                                    temp_surface.get ()),
                      SDL_DestroyTexture);

                  SDL_RenderCopy (renderer.get (), pawn_texture.get (),
                                  nullptr, &square);
                }
            }
        }

      SDL_RenderPresent (renderer.get ());
    }

  IMG_Quit ();
  SDL_Quit ();
  return 0;
}
