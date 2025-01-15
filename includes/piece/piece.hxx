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
  to_string () const noexcept -> std::string_view
  {
    if (piece_colour == colour::black)
      {
        switch (piece_type)
          {
          case piece::type::pawn:
            return "♟";
          case piece::type::knight:
            return "♞";
          case piece::type::bishop:
            return "♝";
          case piece::type::rook:
            return "♜";
          case piece::type::queen:
            return "♛";
          case piece::type::king:
            return "♚";
          }
      }
    else
      {
        switch (piece_type)
          {
          case piece::type::pawn:
            return "♙";
          case piece::type::knight:
            return "♘";
          case piece::type::bishop:
            return "♗";
          case piece::type::rook:
            return "♖";
          case piece::type::queen:
            return "♕";
          case piece::type::king:
            return "♔";
          }
      }
    std::unreachable ();
  }

private:
  type piece_type{ type::pawn };
  colour piece_colour{ colour::black };
};
