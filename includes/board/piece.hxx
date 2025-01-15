#pragma once

#include <cstdint>
#include <string_view>
#include <utility>

class piece
{
public:
  enum class colour : bool
  {
    black = false,
    white = true
  };

  enum class type : std::uint8_t
  {
    pawn,
    knight,
    bishop,
    rook,
    queen,
    king
  };

  constexpr piece (type t, colour c) noexcept
      : piece_type{ t }, piece_colour{ c }
  {
  }

  [[nodiscard]] constexpr auto
  is_black () const noexcept -> bool
  {
    return piece_colour == colour::black;
  }

  [[nodiscard]] constexpr auto
  get_type () const noexcept -> piece::type
  {
    return piece_type;
  }

  [[nodiscard]] constexpr auto
  get_type_as_str () const noexcept -> std::string_view
  {
    switch (piece_type)
      {
      case piece::type::pawn:
        return "pawn";
      case piece::type::knight:
        return "knight";
      case piece::type::bishop:
        return "bishop";
      case piece::type::rook:
        return "rook";
      case piece::type::queen:
        return "queen";
      case piece::type::king:
        return "king";
      }
    std::unreachable ();
  }

private:
  type piece_type{ type::pawn };
  colour piece_colour{ colour::black };
};
