#include "board/board.hxx"
#include "gui/click_event.hxx"
#include "gui/draw.hxx"
#include "gui/raii_wrapper.hxx"
#include "gui/texture_cache.hxx"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
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

  if ((IMG_Init (IMG_INIT_PNG) & IMG_INIT_PNG) == 0)
    {
      SDL_Log ("SDL_image initialization failed: %s\n", IMG_GetError ());
      SDL_Quit ();
      return -1;
    }

  SDL_wrapper window (SDL_CreateWindow ("chess", SDL_WINDOWPOS_CENTERED,
                                        SDL_WINDOWPOS_CENTERED, 800, 800,
                                        SDL_WINDOW_SHOWN),
                      SDL_DestroyWindow);

  SDL_wrapper renderer (SDL_CreateRenderer (window.get (), -1,
                                            SDL_RENDERER_ACCELERATED
                                                | SDL_RENDERER_PRESENTVSYNC),
                        SDL_DestroyRenderer);

  texture_cache cache{ renderer.get () };
  if (!cache.load_textures (assets_path))
    {
      SDL_Log ("failed to load piece textures\n");
      return -1;
    }

  bool running = true;
  std::optional<square::coordinate> highlighted_square;

  while (running)
    {
      SDL_Event event;
      while (SDL_PollEvent (&event) != 0)
        {
          if (event.type == SDL_QUIT)
            {
              running = false;
            }
          else if (event.type == SDL_MOUSEBUTTONDOWN)
            {
              if (event.button.button == SDL_BUTTON_LEFT)
                {
                  if (auto click = get_clicked_square (event, b))
                    {
                      const auto &[position, square] = *click;

                      // Toggle highlight on clicked square
                      if (highlighted_square.has_value ()
                          && highlighted_square->file == position.file
                          && highlighted_square->rank == position.rank)
                        {
                          highlighted_square.reset ();
                        }
                      else
                        {
                          highlighted_square = position;
                        }

                      if (square.is_occupied ())
                        {
                          const auto &piece = square.get_piece ().value ();
                          std::println ("clicked {} {} at position ({}, {})",
                                        piece.is_black () ? "black" : "white",
                                        piece.get_type_as_str (),
                                        position.file, position.rank);
                        }
                      else
                        {
                          std::println (
                              "clicked empty square at position ({}, {})",
                              position.file, position.rank);
                        }
                    }
                }
            }
        }

      draw (*renderer.get (), b, cache, highlighted_square);
    }

  IMG_Quit ();
  SDL_Quit ();
  return 0;
}
