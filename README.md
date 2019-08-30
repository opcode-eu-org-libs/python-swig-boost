About this repository
=====================

Helpers for using SWIG Python library interfaces in Boost::Python based projects.

## Usage example

```
#include "swig2BoostPython.inl"

AAA* getObject();
void setValues(const AAA&, const BBB&);
BBB  getValue();

PREPARE_EXPOSE_PTR_VIA_SWIG(AAA, "AAA *");
PREPARE_EXPOSE_VIA_SWIG(BBB);

BOOST_PYTHON_MODULE(MyPyAPI) {
	EXPOSE_PTR_VIA_SWIG(AAA);
	EXPOSE_VIA_SWIG(BBB);
	
	boost::python::def(
		"getObject",   &getObject,
		boost::python::return_value_policy<reference_existing_object_via_SWIG>()
	);
	boost::python::def("setValues", &setValues);
	boost::python::def("getValue",  &getValue);
}
```

Where `AAA` and `BBB` are types from library with SWIG Python interface.

* `AAA` can be abstract base class.
* `BBB` can't be abstract class but can be returned by copy.

Usage in Python:

```
import LibraryWithAAA   # import SWIG interface
import MyPyAPI          # import boost::python interface

a = MyPyAPI.getObject() # get SWIG object from boost::python system
```

### Build

1. generate swigpyrun.h file: `swig -python -external-runtime`
2. compile file with boost::python interface of your program


## License

```
Copyright © 2018-2019, Robert Ryszard Paciorek <rrp@opcode.eu.org>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```
