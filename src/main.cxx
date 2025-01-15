#include "board_init.hxx"
#include "square/square.hxx"

#include <print>

int
main ()
{
  static constinit auto b = initial_board ();
  for (const auto &row : b)
    {
      for (const auto &sq : row)
        {
          std::print ("{} ", sq.to_string ());
        }
      std::println ("");
    }
  return 0;
}
