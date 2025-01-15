#pragma once

#include "piece.hxx"

#include <algorithm>
#include <array>
#include <cstdint>
#include <optional>
#include <ranges>

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

private:
  std::optional<piece> occupying_piece{};
  coordinate coord{};
};
