print("Editor's LUA API version: "..tostring(editorVersion))

editorConfigInt('FontSize',14)
editorConfigStr("Font",'/home/phileas/.fonts/JetBrainsMono/ttf/JetBrainsMono-SemiBold.ttf')

-- define Lua function --
function MyLuaFunction(num, str)
    print("Hello from Lua!")
    print("num = "..tostring(num)..", str = "..str)
end

GlobalVariable = 42
