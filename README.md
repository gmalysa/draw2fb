# draw2fb

Simple library for drawing stuff to a framebuffer-type structure, usually for embedded devices. The
current application is text rendering without needing to incorporate a heavyweight library like Qt.
See the companion program at (todo) for a simple driver that uses this library to provide a
scriptable program for drawing text.

Features will be added as needed.

Currently:
- Draw a string using FreeType and any installed string

TODO:
- Manage a single FT instance instead of creating a new one for each call?
- Other drawing primitives? Shapes, lines, etc.?
