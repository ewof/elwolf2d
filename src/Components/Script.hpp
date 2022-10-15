#pragma once

#include <sol/sol.hpp>

struct Script{
  sol::function func;

  Script(sol::function func = sol::lua_nil) { this->func = func; }
};
