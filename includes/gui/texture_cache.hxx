#pragma once
#include "board/piece.hxx"
#include "gui/raii_wrapper.hxx"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <array>
#include <filesystem>
#include <string_view>

class texture_cache
{
public:
  struct texture_entry
  {
    std::string_view key{};
    SDL_wrapper<SDL_Texture *, decltype (&SDL_DestroyTexture)> texture;

    constexpr texture_entry () noexcept
        : key{}, texture{ nullptr, SDL_DestroyTexture }
    {
    }

    constexpr texture_entry (
        std::string_view k,
        SDL_wrapper<SDL_Texture *, decltype (&SDL_DestroyTexture)> t) noexcept
        : key{ k }, texture{ std::move (t) }
    {
    }
  };

  explicit constexpr texture_cache (SDL_Renderer *rend) noexcept
      : renderer{ rend }
  {
  }

  texture_cache (const texture_cache &) = delete;
  texture_cache &operator= (const texture_cache &) = delete;
  texture_cache (texture_cache &&) = default;
  texture_cache &operator= (texture_cache &&) = default;
  ~texture_cache () = default;

  [[nodiscard]] auto constexpr load_textures (
      const std::filesystem::path &assets_path) noexcept -> bool
  {
    constexpr std::array<piece::colour, 2> colours{ piece::colour::black,
                                                    piece::colour::white };
    constexpr std::array<piece::type, 6> types{
      piece::type::pawn,   piece::type::rook,  piece::type::knight,
      piece::type::bishop, piece::type::queen, piece::type::king
    };

    std::size_t texture_index = 0;
    for (const auto colour : colours)
      {
        const auto colour_path
            = assets_path
              / (colour == piece::colour::black ? "black" : "white");

        for (const auto type : types)
          {
            const piece current_piece{ type, colour };
            const auto png_path
                = (colour_path / current_piece.get_type_as_str ().data ())
                      .concat (".png");

            if (!std::filesystem::exists (png_path))
              {
                return false;
              }

            SDL_wrapper surface{ IMG_Load (png_path.c_str ()),
                                 SDL_FreeSurface };

            if (surface.get () == nullptr)
              {
                return false;
              }

            SDL_wrapper texture{ SDL_CreateTextureFromSurface (renderer,
                                                               surface.get ()),
                                 SDL_DestroyTexture };

            if (texture.get () == nullptr)
              {
                return false;
              }

            textures.at (texture_index)
                = texture_entry{ piece_to_key (current_piece),
                                 std::move (texture) };
            ++texture_index;
          }
      }
    return true;
  }

  [[nodiscard]] auto constexpr get_texture (const piece &p) const noexcept
      -> SDL_Texture *
  {
    const auto key = piece_to_key (p);
    for (const auto &entry : textures)
      {
        if (entry.key == key)
          {
            return entry.texture.get ();
          }
      }
    return nullptr;
  }

private:
  [[nodiscard]] static constexpr auto
  piece_to_key (const piece &p) noexcept -> std::string_view
  {
    constexpr std::array<std::string_view, 12> keys{
      "black_pawn",   "black_rook",   "black_knight", "black_bishop",
      "black_queen",  "black_king",   "white_pawn",   "white_rook",
      "white_knight", "white_bishop", "white_queen",  "white_king"
    };
    const auto index
        = (p.is_black () ? 0 : 6) + static_cast<std::size_t> (p.get_type ());
    return keys.at (index);
  }

  SDL_Renderer *renderer;
  std::array<texture_entry, 12> textures{};
};
