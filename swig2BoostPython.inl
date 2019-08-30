#include "swig2BoostPython.h"

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

#include <string>
#include <iostream>

struct PySwigObject {
	PyObject_HEAD
	void *ptr;
	const char *desc;
};

static void* swigConvert(PyObject* obj) {
	char thisStr[] = "this";
	
	if (!PyObject_HasAttrString(obj, thisStr))
		return NULL;
	
	PyObject* thisAttr = PyObject_GetAttrString(obj, thisStr);
	if (thisAttr == NULL)
		return NULL;
	
	void* pointer = reinterpret_cast<PySwigObject*>(thisAttr)->ptr;
	Py_DECREF(thisAttr);
	return pointer;
}


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpragmas"
#pragma GCC diagnostic ignored "-Wold-style-cast"
#pragma GCC diagnostic ignored "-Wshadow"
#pragma GCC diagnostic ignored "-Wreserved-id-macro"
#define SWIGPYTHON_BUILTIN
#include "swigpyrun.h"
PyObject* self = NULL;
#pragma GCC diagnostic pop


template<typename T> PyObject* convertRefToSWIG<T>::convertRef(T* inPrt) {
	static swig_type_info* typeInfo = SWIG_TypeQuery(TypeName);
	
	if (typeInfo) {
		std::cout << "convert ptr (existing_object) to python for: " << TypeName << " / " << typeInfo->name << " // " << typeInfo->str << "\n";
	} else {
		throw std::logic_error(
			std::string("convert ptr to python: can't find typeInfo for: ") + TypeName
		);
	}
	
	return SWIG_NewPointerObj(reinterpret_cast<void*>(inPrt), typeInfo, 0 );
}

#define PREPARE_EXPOSE_PTR_VIA_SWIG(TT, TN) \
	template<> const char* convertRefToSWIG< TT >::TypeName = TN;\
	template PyObject* convertRefToSWIG< TT >::convertRef(TT* inPrt);

#define EXPOSE_PTR_VIA_SWIG(TT) \
	boost::python::converter::registry::insert(&swigConvert, boost::python::type_id< TT >());


// convertRefToSWIG<T> class template extension for types that can be passed by copy
template<typename T> struct convertCopyToSWIG : convertRefToSWIG<T> {
	/// converter for function returning object (by copy)
	static PyObject* convert(const T& inObj)  {
		static swig_type_info* typeInfo = SWIG_TypeQuery(convertRefToSWIG<T>::TypeName);
		
		if (typeInfo) {
			std::cout << "convert obj to python for: " << convertRefToSWIG<T>::TypeName << " / " << typeInfo->name << " // " << typeInfo->str << "\n";
		} else {
			throw std::logic_error(
				std::string("convert obj to python: can't find typeInfo for: ") + convertRefToSWIG<T>::TypeName
			);
		}
		
		return SWIG_NewPointerObj((new T(inObj)), typeInfo, SWIG_POINTER_OWN |  0 );
	}
};

#define PREPARE_EXPOSE_VIA_SWIG(TT, TN) \
	PREPARE_EXPOSE_PTR_VIA_SWIG(TT, TN) \
	template PyObject* convertCopyToSWIG< TT >::convert(const TT& inObj);

#define EXPOSE_VIA_SWIG(TT) \
	EXPOSE_PTR_VIA_SWIG(TT) \
	boost::python::to_python_converter< TT, convertCopyToSWIG< TT > >();
