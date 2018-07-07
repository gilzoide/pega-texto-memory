pega-texto-memory
=================
Memory usage statistics for the [pega-texto](https://github.com/gilzoide/pega-texto)
PEG runtime engine.


Dependencies
------------
- [Judy](http://judy.sourceforge.net/)


Build
-----
Using [CMake](https://cmake.org/):

	$ mkdir build
	$ cd build
	$ cmake ..
	$ make


Install
-------
From build directory:

	$ make install


Package
-------
[CPack](https://gitlab.kitware.com/cmake/community/wikis/doc/cpack/Packaging-With-CPack)
is configured for packaging __pega-texto-memory__. From build directory, after
building:

	$ cpack

By default, it uses the TGZ, STGZ and TZ generators. You may override this with
other [available generators](https://gitlab.kitware.com/cmake/community/wikis/doc/cpack/PackageGenerators)
if you want:

	$ cpack -DCPACK_GENERATOR=<generator>

