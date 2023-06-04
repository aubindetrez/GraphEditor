#include <cstdio>
#include <X11/Xlib.h>

#include "xlib_deps.h"

void xlib_list_fonts() {
    int count = 0; 
    XFontStruct  *info;
    Display *dpy = XOpenDisplay(NULL);
    char **names = XListFontsWithInfo(dpy, "*ono*", /*maxnames*/64, &count, &info);
    if (count == 0) {
        puts("xlib_list_fonts(): No font returned by XListFontsWithInfo");
    } else {
        puts("List of font names returned by XListFontsWithInfo:");
        for (size_t i = 0; i < (size_t)count; i++)
            printf(" - %s\n", names[i]);
    }

    // Just a test, print the properties
    if (count > 0) {
        // info[0].n_properties
        puts("Properties for the first font");
        for (size_t i = 0; i < (size_t)info[0].n_properties; i++) {
            printf(" - %ld: %ld\n", info[0].properties[i].name, info[0].properties[i].card32);
        }
    }

    XFreeFontInfo(names, info, count);
}
