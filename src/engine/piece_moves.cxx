#include "engine/piece_moves.hxx"

namespace
{
[[nodiscard]] constexpr auto
get_diagonal_moves (const board &b, const square &sq) noexcept
    -> std::vector<square::coordinate>
{
  std::vector<square::coordinate> moves;
  const auto rank = sq.get_rank ();
  const auto file = sq.get_file ();
  const bool is_black = sq.get_piece ()->is_black ();

  for (size_t f = file + 1, r = rank - 1; f < BOARD_SIZE && r < BOARD_SIZE;
       ++f, --r)
    {
      const auto &target = b.at (r).at (f);
      moves.push_back (square::coordinate{
          .file = static_cast<square::coordinate_type> (f),
          .rank = static_cast<square::coordinate_type> (r) });
      if (target.is_occupied ())
        {
          if (target.get_piece ()->is_black () == is_black)
            {
              moves.pop_back ();
            }
          break;
        }
    }

  // Up-left
  for (size_t f = file - 1, r = rank - 1; f < BOARD_SIZE && r < BOARD_SIZE;
       --f, --r)
    {
      const auto &target = b.at (r).at (f);
      moves.push_back (square::coordinate{
          .file = static_cast<square::coordinate_type> (f),
          .rank = static_cast<square::coordinate_type> (r) });
      if (target.is_occupied ())
        {
          if (target.get_piece ()->is_black () == is_black)
            {
              moves.pop_back ();
            }
          break;
        }
    }

  // Down-right
  for (size_t f = file + 1, r = rank + 1; f < BOARD_SIZE && r < BOARD_SIZE;
       ++f, ++r)
    {
      const auto &target = b.at (r).at (f);
      moves.push_back (square::coordinate{
          .file = static_cast<square::coordinate_type> (f),
          .rank = static_cast<square::coordinate_type> (r) });
      if (target.is_occupied ())
        {
          if (target.get_piece ()->is_black () == is_black)
            {
              moves.pop_back ();
            }
          break;
        }
    }

  for (size_t f = file - 1, r = rank + 1; f < BOARD_SIZE && r < BOARD_SIZE;
       --f, ++r)
    {
      const auto &target = b.at (r).at (f);
      moves.push_back (square::coordinate{
          .file = static_cast<square::coordinate_type> (f),
          .rank = static_cast<square::coordinate_type> (r) });
      if (target.is_occupied ())
        {
          if (target.get_piece ()->is_black () == is_black)
            {
              moves.pop_back ();
            }
          break;
        }
    }

  return moves;
}

[[nodiscard]] constexpr auto
get_straight_moves (const board &b, const square &sq) noexcept
    -> std::vector<square::coordinate>
{
  std::vector<square::coordinate> moves;
  const auto rank = sq.get_rank ();
  const auto file = sq.get_file ();
  const bool is_black = sq.get_piece ()->is_black ();

  for (size_t f = file + 1; f < BOARD_SIZE; ++f)
    {
      const auto &target = b.at (rank).at (f);
      moves.push_back (square::coordinate{
          .file = static_cast<square::coordinate_type> (f), .rank = rank });
      if (target.is_occupied ())
        {
          if (target.get_piece ()->is_black () == is_black)
            {
              moves.pop_back ();
            }
          break;
        }
    }

  for (size_t f = file; f > 0; --f)
    {
      const auto &target = b.at (rank).at (f - 1);
      moves.push_back (square::coordinate{
          .file = static_cast<square::coordinate_type> (f - 1),
          .rank = rank });
      if (target.is_occupied ())
        {
          if (target.get_piece ()->is_black () == is_black)
            {
              moves.pop_back ();
            }
          break;
        }
    }

  for (size_t r = rank; r > 0; --r)
    {
      const auto &target = b.at (r - 1).at (file);
      moves.push_back (square::coordinate{
          .file = file,
          .rank = static_cast<square::coordinate_type> (r - 1) });
      if (target.is_occupied ())
        {
          if (target.get_piece ()->is_black () == is_black)
            {
              moves.pop_back ();
            }
          break;
        }
    }

  for (size_t r = rank + 1; r < BOARD_SIZE; ++r)
    {
      const auto &target = b.at (r).at (file);
      moves.push_back (square::coordinate{
          .file = file, .rank = static_cast<square::coordinate_type> (r) });
      if (target.is_occupied ())
        {
          if (target.get_piece ()->is_black () == is_black)
            {
              moves.pop_back ();
            }
          break;
        }
    }

  return moves;
}

[[nodiscard]] constexpr auto
pawn_moves (const board &b, const square &sq) noexcept
    -> std::vector<square::coordinate>
{
  std::vector<square::coordinate> moves;
  const auto rank = sq.get_rank ();
  const auto file = sq.get_file ();
  const bool is_black = sq.get_piece ()->is_black ();
  const auto direction = is_black ? 1 : -1;
  const auto new_rank
      = static_cast<square::coordinate_type> (rank + direction);

  if (new_rank < BOARD_SIZE)
    {
      if (!b.at (new_rank).at (file).is_occupied ())
        {
          moves.push_back (
              square::coordinate{ .file = file, .rank = new_rank });

          if ((is_black && rank == 1) || (!is_black && rank == 6))
            {
              const auto two_squares = static_cast<square::coordinate_type> (
                  rank + (2 * direction));
              if (!b.at (static_cast<size_t> (two_squares))
                       .at (file)
                       .is_occupied ())
                {
                  moves.push_back (
                      square::coordinate{ .file = file, .rank = two_squares });
                }
            }
        }

      if (file > 0)
        {
          const auto &left_target = b.at (new_rank).at (file - 1);
          if (left_target.is_occupied ()
              && left_target.get_piece ()->is_black () != is_black)
            {
              moves.push_back (square::coordinate{
                  .file = static_cast<square::coordinate_type> (file - 1),
                  .rank = new_rank });
            }
        }

      if (file < BOARD_SIZE - 1)
        {
          const auto &right_target = b.at (new_rank).at (file + 1);
          if (right_target.is_occupied ()
              && right_target.get_piece ()->is_black () != is_black)
            {
              moves.push_back (square::coordinate{
                  .file = static_cast<square::coordinate_type> (file + 1),
                  .rank = new_rank });
            }
        }
    }

  return moves;
}

[[nodiscard]] constexpr auto
knight_moves (const board &b, const square &sq) noexcept
    -> std::vector<square::coordinate>
{
  std::vector<square::coordinate> moves;
  const auto rank = sq.get_rank ();
  const auto file = sq.get_file ();
  const bool is_black = sq.get_piece ()->is_black ();

  constexpr std::array<std::pair<int, int>, 8> offsets{ { { -2, -1 },
                                                          { -2, 1 },
                                                          { 2, -1 },
                                                          { 2, 1 },
                                                          { -1, -2 },
                                                          { 1, -2 },
                                                          { -1, 2 },
                                                          { 1, 2 } } };

  for (const auto &[rank_offset, file_offset] : offsets)
    {
      const auto new_rank = static_cast<int> (rank) + rank_offset;
      const auto new_file = static_cast<int> (file) + file_offset;

      if (new_rank >= 0 && new_rank < static_cast<int> (BOARD_SIZE)
          && new_file >= 0 && new_file < static_cast<int> (BOARD_SIZE))
        {
          const auto &target = b.at (static_cast<size_t> (new_rank))
                                   .at (static_cast<size_t> (new_file));
          if (!target.is_occupied ()
              || target.get_piece ()->is_black () != is_black)
            {
              moves.push_back (square::coordinate{
                  .file = static_cast<square::coordinate_type> (new_file),
                  .rank = static_cast<square::coordinate_type> (new_rank) });
            }
        }
    }

  return moves;
}

[[nodiscard]] constexpr auto
bishop_moves (const board &b, const square &sq) noexcept
    -> std::vector<square::coordinate>
{
  return get_diagonal_moves (b, sq);
}

[[nodiscard]] constexpr auto
rook_moves (const board &b, const square &sq) noexcept
    -> std::vector<square::coordinate>
{
  return get_straight_moves (b, sq);
}

[[nodiscard]] constexpr auto
queen_moves (const board &b, const square &sq) noexcept
    -> std::vector<square::coordinate>
{
  auto moves = get_straight_moves (b, sq);
  auto diagonals = get_diagonal_moves (b, sq);
  moves.insert (moves.end (), diagonals.begin (), diagonals.end ());
  return moves;
}

[[nodiscard]] constexpr auto
king_moves (const board &b, const square &sq) noexcept
    -> std::vector<square::coordinate>
{
  std::vector<square::coordinate> moves;
  const auto rank = sq.get_rank ();
  const auto file = sq.get_file ();
  const bool is_black = sq.get_piece ()->is_black ();

  constexpr std::array<std::pair<size_t, size_t>, 8> king_offsets{
    { { 1, 0 },
      { 1, 1 },
      { 0, 1 },
      { SIZE_MAX, 1 },
      { SIZE_MAX, 0 },
      { SIZE_MAX, SIZE_MAX },
      { 0, SIZE_MAX },
      { 1, SIZE_MAX } }
  };

  for (const auto &[rank_offset, file_offset] : king_offsets)
    {
      const auto new_rank = rank + rank_offset;
      const auto new_file = file + file_offset;

      if (new_rank < BOARD_SIZE && new_file < BOARD_SIZE)
        {
          const auto &target = b.at (new_rank).at (new_file);
          if (!target.is_occupied ()
              || target.get_piece ()->is_black () != is_black)
            {
              moves.push_back (square::coordinate{
                  .file = static_cast<square::coordinate_type> (new_file),
                  .rank = static_cast<square::coordinate_type> (new_rank) });
            }
        }
    }

  return moves;
}

using move_function
    = std::vector<square::coordinate> (*) (const board &, const square &);
constexpr std::array<move_function, 6> move_functions{
  pawn_moves, knight_moves, bishop_moves, rook_moves, queen_moves, king_moves
};

} // namespace

[[nodiscard]] auto
get_moves (const board &b, const square &sq) noexcept
    -> std::vector<square::coordinate>
{
  if (!sq.is_occupied ())
    {
      return {};
    }
  return move_functions.at (
      static_cast<size_t> (sq.get_piece ().value ().get_type ())) (b, sq);
}
