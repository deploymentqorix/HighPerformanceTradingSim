#pragma once
#include <string>
#include <memory>
#include <unordered_map>

// Forward declaration
struct lua_State;

// Requirement Mapping
// QCSIDM_SRS_058 - Embed scripting engine
// QCSIDM_SRS_059 - Allow bots to place orders
// QCSIDM_SRS_060 - Sandbox execution
// QCSIDM_SRS_061 - Dynamic script loading
// QCSIDM_SRS_062 - Market data access in scripts

class ScriptingEngine {
public:
    ScriptingEngine();
    ~ScriptingEngine();

    bool loadScript(const std::string& name, const std::string& path);  // SRS_061
    void runFunction(const std::string& scriptName, const std::string& funcName); 

    // API exposed to Lua
    static int placeOrder(lua_State* L);    // SRS_059
    static int getMarketData(lua_State* L); // SRS_062

private:
    std::unordered_map<std::string, lua_State*> scripts;
};
