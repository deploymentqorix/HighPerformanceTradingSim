#include "scripting_engine.h"
#include <lua.hpp>
#include <iostream>

ScriptingEngine::ScriptingEngine() {}

ScriptingEngine::~ScriptingEngine() {
    for (auto& kv : scripts) {
        lua_close(kv.second);
    }
}

bool ScriptingEngine::loadScript(const std::string& name, const std::string& path) {
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);

    // Register API functions
    lua_register(L, "placeOrder", ScriptingEngine::placeOrder);
    lua_register(L, "getMarketData", ScriptingEngine::getMarketData);

    if (luaL_dofile(L, path.c_str())) {
        std::cerr << "[ScriptingEngine] Failed to load script " << name 
                  << ": " << lua_tostring(L, -1) << std::endl;
        lua_close(L);
        return false;
    }

    scripts[name] = L;
    std::cout << "[ScriptingEngine] Loaded script: " << name << std::endl;
    return true; // SRS_061
}

void ScriptingEngine::runFunction(const std::string& scriptName, const std::string& funcName) {
    auto it = scripts.find(scriptName);
    if (it == scripts.end()) return;

    lua_State* L = it->second;
    lua_getglobal(L, funcName.c_str());

    if (lua_pcall(L, 0, 0, 0) != LUA_OK) {
        std::cerr << "[ScriptingEngine] Error running function " << funcName 
                  << ": " << lua_tostring(L, -1) << std::endl;
    }
}

// --- Lua exposed API ---

int ScriptingEngine::placeOrder(lua_State* L) {
    std::string side = lua_tostring(L, 1);
    int qty = lua_tointeger(L, 2);
    double price = lua_tonumber(L, 3);

    std::cout << "[Bot] placeOrder called: " << side 
              << " " << qty << " @ " << price << std::endl;
    return 0; // SRS_059
}

int ScriptingEngine::getMarketData(lua_State* L) {
    lua_pushnumber(L, 101.25); // Fake best bid
    lua_pushnumber(L, 101.30); // Fake best ask
    return 2; // return two values (bid, ask) → SRS_062
}
