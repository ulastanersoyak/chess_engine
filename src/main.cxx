#include "board/board.hxx"
#include "engine/piece_moves.hxx"
#include "gui/background.hxx"
#include "gui/click_event.hxx"
#include "gui/draw.hxx"
#include "gui/raii_wrapper.hxx"
#include "gui/texture_cache.hxx"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <algorithm>
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

  background bg{ *renderer.get () };
  if (!bg.load_texture ((assets_path / "background.png").c_str ()))
    {
      SDL_Log ("failed to load background\n");
      return -1;
    }

  bool running = true;
  std::optional<square::coordinate> selected_square;
  std::vector<square::coordinate> valid_moves;
  bool white_turn = true;

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
                      if (selected_square)
                        {
                          if (std::ranges::find_if (
                                  valid_moves,
                                  [&position] (const auto &move) {
                                    return move.rank == position.rank
                                           && move.file == position.file;
                                  })
                              != valid_moves.end ())
                            {
                              auto piece = b.at (selected_square->rank)
                                               .at (selected_square->file)
                                               .remove_piece ();
                              b.at (position.rank)
                                  .at (position.file)
                                  .place_piece (piece.value ());

                              white_turn = !white_turn;
                              selected_square.reset ();
                              valid_moves.clear ();
                            }
                          else
                            {
                              selected_square.reset ();
                              valid_moves.clear ();
                            }
                        }
                      else if (square.is_occupied ())
                        {
                          if (square.get_piece ()->is_black () != white_turn)
                            {
                              selected_square = position;
                              valid_moves = get_moves (b, square);
                            }
                        }
                    }
                }
            }
        }

      draw (*renderer.get (), b, cache, bg, selected_square, valid_moves);
    }

  IMG_Quit ();
  SDL_Quit ();
  return 0;
}
