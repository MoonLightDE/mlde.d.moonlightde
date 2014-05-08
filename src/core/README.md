Moonlight Desktop Environment Core 
=======================================

The core is the main component of the desktop environment. Its duty is to load a list of modules, which is know as a profile, and handle the interaction between then, as services.
For this propose it's used the "C++ MicroServices" libruary a dynamic OSGi-like C++ service registry. In this way the whole desktop environment could be loaded into a single 
application or splited in several applications whitout to modify the code.

