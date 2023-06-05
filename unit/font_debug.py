import freetype, sys

stdout = open(1, mode="w", encoding="utf8")
face = freetype.Face("/usr/share/fonts/google-noto-emoji/NotoEmoji-Regular.ttf")

# All unicode char code and glyph
for c, g in face.get_chars():
    if c:
        stdout.write(str(g)+' : '+chr(c)+'\n')
# 888 chars

# Return glyph index of a character code
# In [12]: face.get_char_index('↩')
# Out[12]: 29
# In [16]: face.get_glyph_name(29)
# Out[16]: b'uni21A9'

# In [14]: face.get_format()
# Out[14]: b'TrueType'
