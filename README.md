# Commutator-Display-Manager

## Installation

### Dependencies

This program can connect with commutator as widget, and uses Boost libraries and sdbus-c++ bindings.

Supported widgets:

	- commutator-sytadin
	- commutator-vigicrues
	- commutator-idfm-line-reports

For more informations about Boost: http://www.boost.org/
For more informations about sdbus-c++: https://github.com/Kistler-Group/sdbus-cpp
	
### Build

	meson setup build

	meson compile -C build

