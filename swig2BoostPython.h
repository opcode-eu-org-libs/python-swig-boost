#pragma   once

/*
Copyright (c) 2018-2019 Robert Ryszard Paciorek <rrp@opcode.eu.org>

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
*/

#include <boost/python.hpp>

template<typename T> struct convertRefToSWIG {
	// user friendly name of converting type (class)
	static const char* TypeName;
	
	// converter for function returning reference existing object (by pointer)
	static PyObject* convertRef(T* inPrt);
};

struct reference_existing_object_via_SWIG {
	template <class T> struct apply { struct type {
		inline PyObject* operator()(T inRef) const {
			return convertRefToSWIG<T>::convertRef(&inRef);
		}
		const PyTypeObject* get_pytype() const {
			return nullptr;
		}
	}; };
	template <class T> struct apply<T&> { struct type {
		inline PyObject* operator()(T& inRef) const {
			return convertRefToSWIG<T>::convertRef(&inRef);
		}
		const PyTypeObject* get_pytype() const {
			return nullptr;
		}
	}; };
	template <class T> struct apply<T*> { struct type {
		inline PyObject* operator()(T* inPrt) const {
			return convertRefToSWIG<T>::convertRef(inPrt);
		}
		const PyTypeObject* get_pytype() const {
			return nullptr;
		}
	}; };
};
