-- call C++ function from Lua --
CallMyCppFunction(42,'From Lua to C++')

-- define Lua function --
function MyLuaFunction(num, str)
    print("Hello from Lua!")
    print("num = "..tostring(num)..", str = "..str)
end
