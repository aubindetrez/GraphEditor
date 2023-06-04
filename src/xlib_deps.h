/*!
 * \file xlib_deps.h
 * \brief Functions depending on Xlib
 *
 * Required because of namespace  collision with Xlib and other libraries
 */
#ifndef XLIB_DEPS_H
#define XLIB_DEPS_H

// List fonts in the terminal, using XLib
void xlib_list_fonts();

#endif // XLIB_DEPS_H
