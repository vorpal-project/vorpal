
#include <oda/oda.h>

extern "C" {

#include <lua.h>
#include <lauxlib.h>

}

#include <iostream>

namespace {
using std::string;
} // unnamed namespace

namespace oda {
namespace wrap {
namespace {

int start(lua_State *L) {
  Status status = Engine().start();
  if (!status.ok())
    return luaL_error(L, "%s\n", status.description().c_str());
  return 0;
};

int finish(lua_State *L) {
  Engine().finish();
  return 0;
}

int registerPath(lua_State *L) {
  lua_settop(L, 1);
  if (lua_isstring(L, 1)) {
    Engine().registerPath(lua_tostring(L, 1));
    return 0;
  }
  return luaL_error(L, "%s\n", "Bad argument, expected (string)");
}

int tick(lua_State *L) {
  Engine().tick(0.1);
  return 0;
}

luaL_Reg module[] = {
  { "start", &start },
  { "finish", &finish },
  { "registerPath", &registerPath },
  { "tick", &tick },
  { nullptr, nullptr }
};

constexpr size_t size() {
  return sizeof(module)/sizeof(luaL_Reg) - 1;
}

} // unnamed namespace
} // namespace wrap
} // namespace oda

extern "C" int luaopen_oda (lua_State *L) {
  std::cout << "Module has " << oda::wrap::size()
            << " functions" << std::endl;
  lua_createtable(L, 0, oda::wrap::size());
  luaL_register(L, nullptr, oda::wrap::module);
  return 1;
}

