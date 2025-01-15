#pragma once

#include "board/square.hxx"

namespace board_tests
{
constexpr auto b = initial_board ();

static_assert (b[0][0].get_file () == 0 && b[0][0].get_rank () == 0);
static_assert (b[7][7].get_file () == 7 && b[7][7].get_rank () == 7);

static_assert (b[1][0].get_piece ()->get_type () == piece::type::pawn
               && b[1][0].get_piece ()->is_black ());
static_assert (b[1][7].get_piece ()->get_type () == piece::type::pawn
               && b[1][7].get_piece ()->is_black ());
static_assert (b[6][0].get_piece ()->get_type () == piece::type::pawn
               && !b[6][0].get_piece ()->is_black ());
static_assert (b[6][7].get_piece ()->get_type () == piece::type::pawn
               && !b[6][7].get_piece ()->is_black ());

static_assert (!b[2][0].is_occupied ());
static_assert (!b[2][7].is_occupied ());
static_assert (!b[5][0].is_occupied ());
static_assert (!b[5][7].is_occupied ());

static_assert (b[7][0].get_piece ()->get_type () == piece::type::rook
               && !b[7][0].get_piece ()->is_black ());
static_assert (b[7][1].get_piece ()->get_type () == piece::type::knight
               && !b[7][1].get_piece ()->is_black ());
static_assert (b[7][2].get_piece ()->get_type () == piece::type::bishop
               && !b[7][2].get_piece ()->is_black ());
static_assert (b[7][3].get_piece ()->get_type () == piece::type::queen
               && !b[7][3].get_piece ()->is_black ());
static_assert (b[7][4].get_piece ()->get_type () == piece::type::king
               && !b[7][4].get_piece ()->is_black ());
static_assert (b[7][5].get_piece ()->get_type () == piece::type::bishop
               && !b[7][5].get_piece ()->is_black ());
static_assert (b[7][6].get_piece ()->get_type () == piece::type::knight
               && !b[7][6].get_piece ()->is_black ());
static_assert (b[7][7].get_piece ()->get_type () == piece::type::rook
               && !b[7][7].get_piece ()->is_black ());

static_assert (b[0][0].get_piece ()->get_type () == piece::type::rook
               && b[0][0].get_piece ()->is_black ());
static_assert (b[0][1].get_piece ()->get_type () == piece::type::knight
               && b[0][1].get_piece ()->is_black ());
static_assert (b[0][2].get_piece ()->get_type () == piece::type::bishop
               && b[0][2].get_piece ()->is_black ());
static_assert (b[0][3].get_piece ()->get_type () == piece::type::king
               && b[0][3].get_piece ()->is_black ()); // Swapped
static_assert (b[0][4].get_piece ()->get_type () == piece::type::queen
               && b[0][4].get_piece ()->is_black ()); // Swapped
static_assert (b[0][5].get_piece ()->get_type () == piece::type::bishop
               && b[0][5].get_piece ()->is_black ());
static_assert (b[0][6].get_piece ()->get_type () == piece::type::knight
               && b[0][6].get_piece ()->is_black ());
static_assert (b[0][7].get_piece ()->get_type () == piece::type::rook
               && b[0][7].get_piece ()->is_black ());

static_assert (b[0][0].get_piece ()->is_black ());
static_assert (!b[7][0].get_piece ()->is_black ());
}
