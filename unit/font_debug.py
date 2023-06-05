import freetype, sys

stdout = open(1, mode="w", encoding="utf8")
face = freetype.Face("/usr/share/fonts/google-noto-emoji/NotoEmoji-Regular.ttf")
for c, g in face.get_chars():
    if c:
        stdout.write(str(g)+' : '+chr(c)+'\n')
#stdout.write("".join(sorted([chr(c) for c, g in face.get_chars() if c]) + ["\n"]))
