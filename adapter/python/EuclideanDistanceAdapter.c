#include "Python.h"
#include "../c/C_Distance.h"

static const char message[4][30] =
	{
		{"ARGUMENTS MUST BE ITERABLE"}, // 0
		{"SEQUENCE IS EMPTY"},          // 1
		{"SEQUENCE DIFFERS IN SIZE"},   // 2
		{"ALL ITEMS MUST BE NUMBERS"}   // 3
	};

static PyObject *
calculate_sequence_wrapper(PyObject *self, PyObject *args)
{
	/* The following element of an x array */
	PyObject *x_sequence;

	/* The following element of an y array */
	PyObject *y_sequence;

	/* the Python Array supplier */
	double *x;
	double *y;

	/* the result of the function */
	double result;

	/* the sequence length */
	Py_ssize_t x_sequence_length;
	Py_ssize_t y_sequence_length;

	/* Performs the association between PyObject and the arguments passed to the function */
	if(0 == PyArg_ParseTuple(args, "OO", &x_sequence, &y_sequence))
	{
		PyErr_SetString(PyExc_TypeError, "euclidean() takes exactly 2 arguments");
		return NULL;
	}

	/* store the argument as a PySequence_Fast into a PyObject variable */
	x_sequence = PySequence_Fast(x_sequence, "First argument must be sequence or iterable");
	if (0 == x_sequence)
		return NULL;

	y_sequence = PySequence_Fast(y_sequence, "Second argument must be sequence or iterable");
	if (!y_sequence)
		return NULL;

	/*
	 * Get the size of a given sequence
	 */
	x_sequence_length = PySequence_Fast_GET_SIZE(x_sequence);
	y_sequence_length = PySequence_Fast_GET_SIZE(y_sequence);
	if (x_sequence_length  == 0 || y_sequence_length  == 0)
	{
		PyErr_SetString(PyExc_TypeError, "Illegal size (0) of sequence");
		return NULL;
	}

	if(x_sequence_length != y_sequence_length)
	{
		PyErr_SetString(PyExc_TypeError, "Sequences differs in size");
		return NULL;
	}

	x = (double *) calloc(x_sequence_length, sizeof(double));
	y = (double *) calloc(y_sequence_length, sizeof(double));
	if (!x || !y)
	{
		Py_DECREF(x_sequence);
		Py_DECREF(y_sequence);
		return PyErr_NoMemory();
	}

	/*
	 * Starts filling up the array
	 * This can be optimized
	 */
	unsigned int i;
	for (i = 0; i < x_sequence_length; i++)
	{
		/* float value */
		PyObject *x_f_item;
		PyObject *y_f_item;

		/* item of the python array */
		PyObject *x_item = PySequence_Fast_GET_ITEM(x_sequence, i);
		PyObject *y_item = PySequence_Fast_GET_ITEM(y_sequence, i);
		if (!x_item || !y_item)
		{
			Py_DECREF(x_sequence);
			Py_DECREF(y_sequence);
			free(x);
			free(y);
			return NULL;
		}

		x_f_item = PyNumber_Float(x_item);
		y_f_item = PyNumber_Float(y_item);
		if (!x_f_item || !y_f_item )
		{
			Py_DECREF(x_sequence);
			Py_DECREF(y_sequence);
			free(x);
			free(y);
            PyErr_SetString(PyExc_TypeError, message[3]);
            return NULL;
        }

		x[i] = PyFloat_AS_DOUBLE(x_f_item);
		y[i] = PyFloat_AS_DOUBLE(y_f_item);
		Py_DECREF(x_f_item);
		Py_DECREF(y_f_item);
	}

	/* clean up */
	Py_DECREF(x_sequence);
	Py_DECREF(y_sequence);

	/* compute */;
	result = calculate(x, y, x_sequence_length);

	free(x);
	free(y);

	/*
	 * and return the result as a PyLong
	 */
	return PyFloat_FromDouble(result);
}


static PyMethodDef module_methods_list[] =
	{
		{"euclidean", (PyCFunction) calculate_sequence_wrapper, METH_VARARGS, NULL},
		{NULL, NULL, 0, NULL} /* sentinel */
	};

/*
 * @see https://docs.python.org/3/c-api/module.html#c.PyModuleDef
 */
static struct PyModuleDef moduleDef =
	{
		PyModuleDef_HEAD_INIT,
		"DistancePy",           /* name of module */
		"Some description",     /* module documentation, may be NULL */
		-1,                     /* size of per-interpreter state of the module, or -1 if the module keeps state in global variables. */
		module_methods_list,
		NULL,
		NULL,
		NULL,
		NULL
	};

PyMODINIT_FUNC
PyInit_DistancePy()
{
	return PyModule_Create(&moduleDef);
}
