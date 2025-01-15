#pragma once

#include <type_traits>

template <typename T>
concept SDL_destructor = std::is_pointer_v<T>;

template <typename Value, SDL_destructor Destructor> class SDL_wrapper
{
public:
  SDL_wrapper (Value val, Destructor des) : value (val), destructor (des) {}

  SDL_wrapper (const SDL_wrapper &) = delete;
  SDL_wrapper &operator= (const SDL_wrapper &) = delete;

  SDL_wrapper (SDL_wrapper &&other) noexcept
      : value (other.value), destructor (other.destructor)
  {
    other.value = nullptr;
  }

  auto
  operator= (SDL_wrapper &&other) noexcept -> SDL_wrapper &
  {
    if (this != &other)
      {
        if (destructor && value)
          {
            destructor (value);
          }
        value = other.value;
        destructor = other.destructor;
        other.value = nullptr;
      }
    return *this;
  }

  [[nodiscard]] constexpr auto
  get () const noexcept -> Value
  {
    return value;
  }

  ~SDL_wrapper ()
  {
    if (destructor)
      {
        destructor (value);
      }
  }

private:
  Value value;
  Destructor destructor;
};
;
