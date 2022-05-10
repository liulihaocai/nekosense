

#include "ScriptExec.h"
#include "../config.h"
#include "ScriptApi.h"
#include "lua/lauxlib.h"
#include "lua/lua.h"
#include "lua/lualib.h"
#include <SDL2/SDL_messagebox.h>
#include <dirent.h>
#include <sstream>

void ScriptExec::Execute(const char *filename) {

  SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Notify", LUA_VERSION,
                           NULL);
  SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Notify",
                           "LuaL_newstate PRE", NULL);
  lua_State *state;
  SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Notify",
                           "LuaL_newstate POST", NULL);
  state = luaL_newstate();

  if (state == nullptr || state == NULL) {
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error",
                             "Failed to create lua state!", NULL);
    return;
  }

  // Make standard libraries available in the Lua object
  luaL_openlibs(state);
  SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Notify", "Luapush!",
                           NULL);

  ScriptApi::PushToLua(state);

  luaL_dostring(state, "client.notify('Hello, world!')");
  return;

  int result = luaL_loadfile(state, filename);
  if (result) {
    // pop a alert message with sdl
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error_Lua_Loadfile",
                             lua_tostring(state, -1), NULL);
    return;
  }
  // Finally, execute the program by calling into it.
  // Change the arguments if you're not running vanilla Lua code.
  result = lua_pcall(state, 0, 0, 0);
  if (result) {
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error_Lua_Pcall",
                             lua_tostring(state, -1), NULL);
    return;
  }
}

void ScriptExec::LoadScripts() {
  auto directory = GetScriptDirectory().c_str();
  // list all files in the directory
  DIR *dir;
  dirent *pdir;

  dir = opendir(directory);

  while ((pdir = readdir(dir))) {
    if (pdir->d_type != DT_DIR || strcmp(pdir->d_name, ".") == 0 ||
        strcmp(pdir->d_name, "..") == 0 || // skip files not ending in .lua
        strcmp(pdir->d_name + strlen(pdir->d_name) - 4, ".lua") != 0)
      continue;

    std::ostringstream path;
    path << directory << pdir->d_name;

    Execute(path.str().c_str());
  }
}