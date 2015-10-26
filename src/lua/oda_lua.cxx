
#include <oda/oda.h>

extern "C" {

#include <lua.h>
#include <lauxlib.h>

}

#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

namespace {
using std::cout;
using std::endl;
using std::string;
using std::unordered_set;
using std::vector;
} // unnamed namespace

namespace oda {
namespace wrap {
namespace {

// Engine methods

unordered_set<Event*> events;

int start(lua_State *L) {
  Status status = Engine().start();
  if (!status.ok())
    return luaL_error(L, "%s\n", status.description().c_str());
  return 0;
};

int finish(lua_State *L) {
  lua_settop(L, 0);
  for (Event *event : events)
    delete event;
  events.clear();
  Engine().finish();
  return 0;
}

int registerPath(lua_State *L) {
  lua_settop(L, 1);
  if (lua_isstring(L, 1)) {
    Engine().registerPath(lua_tostring(L, 1));
    return 0;
  }
  return luaL_argerror(L, 1, "string expected");
}

int tick(lua_State *L) {
  lua_settop(L, 0);
  Engine().tick(0.1);
  return 0;
}

int eventInstance(lua_State *L) {
  lua_settop(L, 1);
  if (lua_isstring(L, 1)) {
    Event *event = new Event;
    Status status = Engine().eventInstance(lua_tostring(L, 1), event);
    if (status.ok()) {
      events.insert(event);
      Event **data = static_cast<Event**>(lua_newuserdata(L, sizeof(event)));
      *data = event;
      luaL_getmetatable(L, "event");
      lua_setmetatable(L, -2);
      return 1;
    } else {
      delete event;
      return luaL_error(L, "%s\n", status.description().c_str());
    }
  }
  return luaL_argerror(L, 1, "string expected");
}

luaL_Reg module[] = {
  { "start", &start },
  { "finish", &finish },
  { "registerPath", &registerPath },
  { "tick", &tick },
  { "eventInstance", &eventInstance },
  { nullptr, nullptr }
};

constexpr size_t size() {
  return sizeof(module)/sizeof(luaL_Reg) - 1;
}

// Event methods

int event_gc(lua_State *L) {
  lua_settop(L, 1);
  if (lua_isuserdata(L, 1)) {
    cout << "[WRAP] event collected" << endl;
    Event *event = *static_cast<Event**>(lua_touserdata(L, 1));
    if (events.find(event) != events.end()) {
      events.erase(event);
      delete event;
    }
  }
  return luaL_argerror(L, 1, "userdata:Event expected");
}

int event_pushCommand(lua_State *L) {
  if (lua_gettop(L) < 2)
    return luaL_error(L, "at least 2 arguments expected (got %d)",
                      lua_gettop(L));
  if (!lua_isuserdata(L, 1))
    return luaL_argerror(L, 1, "userdata:Event expected");
  if (!lua_isstring(L, 2))
    return luaL_argerror(L, 2, "string expected");
  Event             *event = *static_cast<Event**>(lua_touserdata(L, 1));
  string            identifier = lua_tostring(L, 2);
  vector<Parameter> parameters;
  for (int i = 3; i <= lua_gettop(L); ++i) {
    if (lua_isnumber(L, i))
      parameters.emplace_back(static_cast<float>(lua_tonumber(L, i)));
    else if (lua_isstring(L, i))
      parameters.emplace_back(lua_tostring(L, i));
    else luaL_argerror(L, i, "string or number expected");
  }
  event->pushCommand(identifier, parameters);
  return 0;
}

luaL_Reg event_meta[] = {
  { "__gc", &event_gc },
  { "pushCommand", &event_pushCommand },
  { nullptr, nullptr }
};

constexpr size_t meta_size() {
  return sizeof(event_meta)/sizeof(luaL_Reg) - 1;
}

} // unnamed namespace
} // namespace wrap
} // namespace oda

extern "C" int luaopen_oda (lua_State *L) {
  luaL_newmetatable(L, "event");
  luaL_register(L, nullptr, oda::wrap::event_meta);
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");
  lua_createtable(L, 0, oda::wrap::size());
  luaL_register(L, nullptr, oda::wrap::module);
  return 1;
}

