#pragma once

#include "piece/piece.hxx"

#include <algorithm>
#include <array>
#include <cstdint>
#include <format>
#include <optional>
#include <ranges>
#include <string>

class square
{
public:
  using coordinate_type = std::uint8_t;

  struct coordinate
  {
    coordinate_type file{ 0 };
    coordinate_type rank{ 0 };
  };

  constexpr square () noexcept = default;

  explicit constexpr square (coordinate c) noexcept : coord{ c } {}

  constexpr auto
  operator= (const piece &p) noexcept -> square &
  {
    occupying_piece = p;
    return *this;
  }

  [[nodiscard]] constexpr auto
  get_file () const noexcept -> coordinate_type
  {
    return coord.file;
  }

  [[nodiscard]] constexpr auto
  get_rank () const noexcept -> coordinate_type
  {
    return coord.rank;
  }

  [[nodiscard]] constexpr auto
  is_occupied () const noexcept -> bool
  {
    return occupying_piece.has_value ();
  }

  [[nodiscard]] constexpr auto
  get_piece () const noexcept -> const std::optional<piece> &
  {
    return occupying_piece;
  }

  constexpr auto
  remove_piece () noexcept -> std::optional<piece>
  {
    auto removed = occupying_piece;
    occupying_piece.reset ();
    return removed;
  }

  constexpr void
  place_piece (const piece &p) noexcept
  {
    occupying_piece = p;
  }

  [[nodiscard]] constexpr auto
  to_string () const noexcept -> std::string
  {
    return std::format ("{}{} {}", coord.file, coord.rank,
                        occupying_piece.has_value ()
                            ? occupying_piece.value ().to_string ()
                            : " ");
  }

private:
  std::optional<piece> occupying_piece{};
  coordinate coord{};
};

using board = std::array<std::array<square, 8>, 8>;

[[nodiscard]] constexpr auto
initial_board () noexcept -> board
{
  board b{};

  for (square::coordinate_type r = 0; r < 8; ++r)
    {
      for (square::coordinate_type f = 0; f < 8; ++f)
        {
          b.at (r).at (f)
              = square{ square::coordinate{ .file = f, .rank = r } };
        }
    }

  std::ranges::fill (b.at (1),
                     piece{ piece::type::pawn, piece::colour::black });

  std::ranges::fill (b.at (6),
                     piece{ piece::type::pawn, piece::colour::white });

  constexpr std::array<piece::type, 8> back_rank{
    piece::type::rook,   piece::type::knight, piece::type::bishop,
    piece::type::queen,  piece::type::king,   piece::type::bishop,
    piece::type::knight, piece::type::rook
  };

  std::ranges::for_each (
      std::views::zip (b.at (0), back_rank), [] (auto &&tup) {
        auto &[square, piece_type] = tup;
        square.place_piece (piece{ piece_type, piece::colour::black });
      });

  std::ranges::for_each (
      std::views::zip (b.at (7), back_rank), [] (auto &&tup) {
        auto &[square, piece_type] = tup;
        square.place_piece (piece{ piece_type, piece::colour::white });
      });

  const auto black_queen = b.at (0).at (3).remove_piece ();
  const auto black_king = b.at (0).at (4).remove_piece ();
  b.at (0).at (3).place_piece (black_king.value ());
  b.at (0).at (4).place_piece (black_queen.value ());

  return b;
}
