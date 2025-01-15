#pragma once

#include "piece.hxx"
#include "square.hxx"

using board = std::array<std::array<square, 8>, 8>;

constexpr auto BOARD_SIZE = 8;
constexpr auto SQUARE_SIZE = 80;
constexpr auto BOARD_OFFSET_X = (800 - (BOARD_SIZE * SQUARE_SIZE)) / 2;
constexpr auto BOARD_OFFSET_Y = (800 - (BOARD_SIZE * SQUARE_SIZE)) / 2;

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
