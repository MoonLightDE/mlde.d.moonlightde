MLDE: MoonLight Desktop Environment - Install/build
=======================================

Now archived, the modular-concept **M**oonlight its a **L**ightweight **D**esktop **E**nvironment for GNU/Linux systems, mai
nly for those that runs in low performance devices but with modern qt5 tecnologies.

![MoonlightDE](screenshots/moonlightde-large.png)

Return to main: [README.md](README.md)

-----------------------------------------------------

The build process its take sources and produce the programs by hand, for Moonlight desktop environment 
its recommended try to search if there a ready to use package made for your distribution.

## Requirements and Dependencies
1. C++ compiler with C++11 support:
   - Gcc >> 4.7
   - Clang >> 3.9
2. Buiild tool:
   - CMake 2.8 or superior
3. QT framework:
   - Qt >> 5.2 with property set paths
4. External extension libs from LXQT project (deprecated version due project was staled)
   - 1. [libqtxdg](lxde/libqtxdg@cbc22cc1808c526fdb877b2a5bb158f868ed49be) lxde/libqtxdg@cbc22cc1808c526fdb877b2a5bb158f868ed49be
   - 2. [liblxqt](lxde/liblxqt@534c7cd9945fef25e59393366da21875fe799f02) lxde/liblxqt@534c7cd9945fef25e59393366da21875fe799f02

Due makes use of C++11 features, the GCC and QT5, any version of OS distribution since 2016 have it, 
for olders you must use backported packages or try to find a ready to use packages for your linux, 
either must be build/backported and install by yourself.

## Build process

All the desktop unless others, have a base in C++ Micro Services, event usage of Dbus. 
In this way the comunication with each desktop component are fast taking in consideration the QT5 heaviweight.

### **1** Build CppMicroServices library

Moonlight use a modified version of The C++ Micro Services library, version series 2.X (today C++ Micro services are in 3.X), 
provides a dynamic service registry and module system, partially based the OSGi Core Release 5 specifications, and 
a gcc with C++11 features its mandatory:

  1. Download the sources: `git clone https://github.com/MoonLightDE/mlde.l.cppmicroservices.git`
  2. Create a folder to compile: `mkdir build;cd build`
  3. Configure the building of LibC++MicroServices: `cmake .. -DLIBRARY_INSTALL_PREFIX=/usr
  4. Build: `make`
  5. Install: `sudo make install`

More detailed instructions can be foun in [https://github.com/MoonLightDE/mlde.l.cppmicroservices/blob/master/README.md#requirements]

### **2** Build Moonlight core

The core Moonlight software produced the start point to use the desktop, and coordinates the communication 
between modules and the CppMicroServices have the registration of that modules

   1. Download the sources: `git clone https://github.com/MoonLightDE/mlde.d.moonlightde`
   2. Create a "build" folder inside the folder containing the sources: `mkdir build; cd build`.
   3. Configure it, the CppMicroServices library must be preinstalled: `cmake ..`
   4. Build: `make`
   5. Install: `make install`
   6. Now the 'moonligth-core' program are installed to lauch

### **3** Complementary software

Moonlight Desktop are based and inspired in the *pre LXQt*, the *RazorQT* desktop and the simplicity of LXDE, 
so when get fissioned many modules comes usefully to MoonlightDE.

So event reinventing the well, lest get reuse it: (information used are deprecated due current staus of MLDE project)

3. Lxqt-session (see the lxqt build instructions)
4. Lxqt-common (see the lxqt build instructions)
5. qps ( https://github.com/QtDesktop/qps.git ) qt5 process manager
6. qterminal (see the lxqt build instructions )

To execute moonlight write the command: `moonlightDEcore`.

-----------------------------------------------------

Return to main: [README.md](README.md)
