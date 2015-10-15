
extern "C" {

#include <lua.h>

}

extern "C" int luaopen_oda (lua_State *L) {
  lua_pushboolean(L, 1);
  return 1;
}

