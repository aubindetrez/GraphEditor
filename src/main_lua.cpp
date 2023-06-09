#include <iostream>

// include Lua headers
extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

static void stackDump (lua_State *L) {
    int i;
    int top = lua_gettop(L);
    std::cout << "Stack (Lua): ";
    for (i = 1; i <= top; i++) {  /* repeat for each level */
        int t = lua_type(L, i);
        switch (t) {

            case LUA_TSTRING:  /* strings */
                std::cout << lua_tostring(L, i);
                break;

            case LUA_TBOOLEAN:  /* booleans */
                std::cout << lua_toboolean(L, i) ? "true" : "false";
                break;

            case LUA_TNUMBER:  /* numbers */
                std::cout << lua_tonumber(L, i);
                break;

            default:  /* other values */
                std::cout << lua_typename(L, t);
                break;

        }
        std::cout << "\t";
    }
    std::cout << std::endl;
}

static int editorConfigInt(lua_State* L)
{
    const char* str = lua_tostring(L,1);
    if (!lua_isnumber(L, 2)) {
        std::cerr << "Error: editorConfigInt() expects an integer" << std::endl;
        return LUA_ERRERR;
    }
    const int num = (int)lua_tonumber(L,2);
    // lua_isnumber() to check the type
    std::cout << "editorConfigInt(): Register=" << str << " Value:" << num << std::endl;
    return LUA_OK;
}

static int editorConfigStr(lua_State* L)
{
    const char* str = lua_tostring(L,1);
    const char* value = lua_tostring(L,2);
    std::cout << "editorConfigStr(): Register=" << str << " Value:" << value << std::endl;
    return LUA_OK;
}


// Lua's manual: https://www.lua.org/manual/5.3/manual.html
int main(int argc, const char * argv[])
{
    int rc;
    const char* lua_file = "tests/test.lua";
    lua_State* L = luaL_newstate(); // create a new lua instance
    luaL_openlibs(L); // give lua access to basic libraries

    lua_pushinteger(L, 1);
    lua_setglobal(L, "editorVersion");

    lua_register(L, "editorConfigStr", editorConfigStr);
    lua_register(L, "editorConfigInt", editorConfigInt);

    rc = luaL_dofile(L, lua_file); // loads the Lua script
    if (rc != 0) {
        std::cerr << "Error: Cannot open lua file: " << lua_file << std::endl;
    }

    lua_getglobal(L, "MyLuaFunction");
    lua_pushnumber(L, 73); // First argument
    lua_pushstring(L, "From C++ to Lua"); // Seconds argument
    lua_pcall(L, /*nargs*/2, /*nresults*/0, /*msgh=return err to stack*/0);

    lua_getglobal(L, "GlobalVariable");
    if (lua_isnoneornil(L, -1)) {
        std::cout << "GlobalVariable is nil" << std::endl;
    }

    // lua_gettop(L) returns the number of elements on the stack
    // lua_pop(L, 1);

    stackDump(L);

    lua_close(L);

    return 0;
}
