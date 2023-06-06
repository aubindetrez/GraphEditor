import freetype, sys

stdout = open(1, mode="w", encoding="utf8")
face = freetype.Face("/usr/share/fonts/google-noto-emoji/NotoEmoji-Regular.ttf")

position=0
# Iterate over all unicode char codes and glyphs
for c, g in face.get_chars():
    if c:
        index = face.get_char_index(chr(c))
        name = face.get_glyph_name(index)
        stdout.write("Position:"+str(position)+" Index:"+str(index)
                     +" char:"+str(g)+' glyph:'+chr(c)+" name:"+str(name)+'\n')
    position+=1

# Example: How to get the format:
# In [14]: face.get_format()
# Out[14]: b'TrueType'
