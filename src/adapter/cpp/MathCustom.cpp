#include <cmath>
#include <Python.h>

const double e = 2.7182818284590452353602874713527;

double
sin_impl(double x)
{
	return (1 - pow(e, (-2 * x))) / (2 * pow(e, -x));
}

double
cosh_impl(double x)
{
	return (1 + pow(e, (-2 * x))) / (2 * pow(e, -x));
}

double
tanh_impl(double x)
{
	return sin_impl(x) / cosh_impl(x);
}

PyObject *
tanh_impl(PyObject *, PyObject *o)
{
	double x = PyFloat_AsDouble(o);
	double tanh_x = tanh_impl(x);
	return PyFloat_FromDouble(tanh_x);
}

PyObject *
cos_impl(PyObject *, PyObject *o)
{
	double x = PyFloat_AsDouble(o);
	double tanh_x = cosh_impl(x);
	return PyFloat_FromDouble(tanh_x);
}

PyObject *
sin_impl(PyObject *, PyObject *o)
{
	double x = PyFloat_AsDouble(o);
	double tanh_x = sin_impl(x);
	return PyFloat_FromDouble(tanh_x);
}

static PyMethodDef MathCustom_methods[] = {
	// The first property is the name exposed to Python, fast_tanh, the second is the C++
	// function name that contains the implementation.
	{"tan", (PyCFunction) tanh_impl, METH_O, nullptr},
	{"sin", (PyCFunction) sin_impl, METH_O, nullptr},
	{"cos", (PyCFunction) cos_impl, METH_O, nullptr},

	// Terminate the array with an object containing nulls.
	{nullptr, nullptr, 0, nullptr}
};

static PyModuleDef MathCustom_module = {
	PyModuleDef_HEAD_INIT,
	"MathCustom",                        // Module name to use with Python import statements
	"Provides some functions, but faster",  // Module description
	0,
	MathCustom_methods                   // Structure that defines the methods of the module
};

PyMODINIT_FUNC
PyInit_MathCustom()
{
	return PyModule_Create(&MathCustom_module);
}
