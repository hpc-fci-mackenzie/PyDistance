#include "CaliperFactory.hpp"
#include <Python.h>

double
euclidean(const double *x, const double *y, unsigned long n)
{
    auto *factory = new CaliperFactory();
    auto *d_measure = factory->produce();
    return d_measure->euclidean(x, y, n);
}

const char *
class_name()
{
    auto *factory = new CaliperFactory();
    auto *d_measure = factory->produce();
    return d_measure->getClassSimpleName();
}

PyObject *
_class_name(PyObject *module)
{
    return PyUnicode_FromString(class_name());
}

PyObject *
euclidean(PyObject *self, PyObject *args)
{
    PyObject *x_sequence;
    PyObject *y_sequence;

    double *x;
    double *y;

    double result;

    Py_ssize_t x_sequence_length;
    Py_ssize_t y_sequence_length;
    PyArg_ParseTuple(args, "OO", &x_sequence, &y_sequence);

    x_sequence = PySequence_Fast(x_sequence, "First argument must be sequence or iterable");
    y_sequence = PySequence_Fast(y_sequence, "Second argument must be sequence or iterable");

    x_sequence_length = PySequence_Fast_GET_SIZE(x_sequence);
    y_sequence_length = PySequence_Fast_GET_SIZE(y_sequence);

    x = (double *) calloc(x_sequence_length, sizeof(double));
    y = (double *) calloc(y_sequence_length, sizeof(double));

    unsigned int i;
    for (i = 0; i < x_sequence_length; i++)
    {
        PyObject *x_f_item;
        PyObject *y_f_item;

        PyObject *x_item = PySequence_Fast_GET_ITEM(x_sequence, i);
        PyObject *y_item = PySequence_Fast_GET_ITEM(y_sequence, i);

        x_f_item = PyNumber_Float(x_item);
        y_f_item = PyNumber_Float(y_item);

        x[i] = PyFloat_AS_DOUBLE(x_f_item);
        y[i] = PyFloat_AS_DOUBLE(y_f_item);
        Py_DECREF(x_f_item);
        Py_DECREF(y_f_item);
    }

    Py_DECREF(x_sequence);
    Py_DECREF(y_sequence);

    result = euclidean(x, y, x_sequence_length);

    free(x);
    free(y);

    return PyFloat_FromDouble(result);
}

static PyMethodDef PyDistance_methods[] = {
    {"euclidean", (PyCFunction) euclidean, METH_VARARGS, nullptr},
    {"getClassName", (PyCFunction) _class_name, METH_NOARGS, nullptr},
    {nullptr, nullptr, 0, nullptr}
};

static PyModuleDef PyDistance_module = {
    PyModuleDef_HEAD_INIT,
    "PyDistance",
    "Provides some functions, but faster",
    0,
    PyDistance_methods
};

PyMODINIT_FUNC
PyInit_PyDistance()
{
    return PyModule_Create(&PyDistance_module);
}
