
#include <vorpal/vorpal.h>

extern "C" {

#include <lua.h>
#include <lauxlib.h>

}

#include <iostream>
#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

namespace {
using std::cout;
using std::endl;
using std::shared_ptr;
using std::string;
using std::unordered_set;
using std::vector;
} // unnamed namespace

namespace vorpal {
namespace wrap {
namespace {

// Engine methods

unordered_set<shared_ptr<SoundtrackEvent>*> events;

int start(lua_State *L) {
  lua_settop(L, 1);
  vector<string> paths;
  if (lua_istable(L, 1)) {
    for (size_t i = 0; i < lua_objlen(L, 1); ++i) {
      lua_pushinteger(L, i+1);
      lua_gettable(L, 1);
      paths.push_back(lua_tostring(L, 2));
      lua_settop(L, 1);
    }
  }
  Status status = Engine().start(paths);
  if (!status.ok())
    return luaL_error(L, "%s\n", status.description().c_str());
  return 0;
};

int finish(lua_State *L) {
  lua_settop(L, 0);
  for (shared_ptr<SoundtrackEvent> *event : events)
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
  lua_settop(L, 1);
  if (lua_isnumber(L, 1)) {
    Engine().tick(static_cast<double>(lua_tonumber(L, 1)));
    return 0;
  }
  return luaL_argerror(L, 1, "number expected");;
}

int eventInstance(lua_State *L) {
  lua_settop(L, 1);
  if (lua_isstring(L, 1)) {
    shared_ptr<SoundtrackEvent> *event = new shared_ptr<SoundtrackEvent>;
    Status status = Engine().eventInstance(lua_tostring(L, 1), event);
    if (status.ok()) {
      events.insert(event);
      shared_ptr<SoundtrackEvent> **data =
        static_cast<shared_ptr<SoundtrackEvent>**>(
            lua_newuserdata(L, sizeof(event))
        );
      *data = event;
      luaL_getmetatable(L, "event");
      lua_setmetatable(L, -2);
      return 1;
    } else {
      delete event;
      return luaL_error(L, "[vorpal binding] %s\n", status.description().c_str());
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

// SoundtrackEvent methods

int event_gc(lua_State *L) {
  lua_settop(L, 1);
  if (lua_isuserdata(L, 1)) {
    cout << "[WRAP] event collected" << endl;
    shared_ptr<SoundtrackEvent> *event =
      *static_cast<shared_ptr<SoundtrackEvent>**>(lua_touserdata(L, 1));
    if (events.find(event) != events.end()) {
      events.erase(event);
      delete event;
    }
  }
  return luaL_argerror(L, 1, "userdata:SoundtrackEvent expected");
}

int event_pushCommand(lua_State *L) {
  if (lua_gettop(L) < 2)
    return luaL_error(L, "at least 2 arguments expected (got %d)",
                      lua_gettop(L));
  if (!lua_isuserdata(L, 1))
    return luaL_argerror(L, 1, "userdata:SoundtrackEvent expected");
  if (!lua_isstring(L, 2))
    return luaL_argerror(L, 2, "string expected");
  shared_ptr<SoundtrackEvent> *event =
    *static_cast<shared_ptr<SoundtrackEvent>**>(lua_touserdata(L, 1));
  string            identifier = lua_tostring(L, 2);
  vector<Parameter> parameters;
  for (int i = 3; i <= lua_gettop(L); ++i) {
    if (lua_isnumber(L, i))
      parameters.emplace_back(static_cast<float>(lua_tonumber(L, i)));
    else if (lua_isstring(L, i))
      parameters.emplace_back(lua_tostring(L, i));
    else luaL_argerror(L, i, "string or number expected");
  }
  (*event)->pushCommand(identifier, parameters);
  return 0;
}

int event_setAudioSource(lua_State *L) {
  if (lua_gettop(L) < 3)
    return luaL_error(L, "at least 3 arguments expected (got %d)",
                      lua_gettop(L));
  if (!lua_isuserdata(L, 1))
    return luaL_argerror(L, 1, "userdata:SoundtrackEvent expected");
  shared_ptr<SoundtrackEvent> *event =
    *static_cast<shared_ptr<SoundtrackEvent>**>(lua_touserdata(L, 1));
  float pos[3] = { 0.0f, 0.0f, 0.0f };
  for (int i = 0; i < 3; ++i)
    if (!lua_isnumber(L, 2 + i))
      return luaL_argerror(L, 2 + i, "number expected");
    else
      pos[i] = static_cast<float>(lua_tonumber(L, 2 + i));
  (*event)->setAudioSource(pos[0], pos[1], pos[2]);
  return 0;
}

luaL_Reg event_meta[] = {
  { "__gc", &event_gc },
  { "pushCommand", &event_pushCommand },
  { "setAudioSource", &event_setAudioSource },
  { nullptr, nullptr }
};

constexpr size_t meta_size() {
  return sizeof(event_meta)/sizeof(luaL_Reg) - 1;
}

} // unnamed namespace
} // namespace wrap
} // namespace vorpal

extern "C" int luaopen_vorpal (lua_State *L) {
  luaL_newmetatable(L, "event");
  luaL_register(L, nullptr, vorpal::wrap::event_meta);
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");
  lua_createtable(L, 0, vorpal::wrap::size());
  luaL_register(L, nullptr, vorpal::wrap::module);
  return 1;
}
