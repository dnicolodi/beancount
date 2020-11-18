#include <istream>
#include <Python.h>

class pyistream : private std::streambuf , public std::istream
{
public:
    pyistream(PyObject* in) : std::istream(this) {
        in_ = in;
        Py_INCREF(in_);
    }

    ~pyistream() {
        Py_XDECREF(in_);
    }

private:
    PyObject* in_;

    std::streamsize xsgetn(char* buf, std::streamsize n) {
        PyObject* dest = NULL;
        PyObject* read = NULL;
        size_t ret = 0;

        dest = PyMemoryView_FromMemory(buf, static_cast<size_t>(n), PyBUF_WRITE);
        if (!dest) {
            goto out;
        }

        read = PyObject_CallMethod(in_, "readinto", "O", dest);
        if (!read) {
            goto out;
        }

        ret = PyLong_AsSize_t(read);
        if (PyErr_Occurred()) {
            ret = 0;
        }

    out:
        Py_XDECREF(dest);
        Py_XDECREF(read);

        return ret;
    }
};
