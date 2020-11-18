#include <reflex/input.h>
#include "beancount/parser/pyistream.h"


int main(int argc, char** argv)
{
    PyObject* f = NULL;    
    char buf[1024];
    size_t len;
    
    Py_Initialize();
    PyObject* io = PyImport_ImportModule("io");

    const char* string_ascii = "foo";
    
    {
        Py_XDECREF(f);
        f = PyObject_CallMethod(io, "BytesIO", "y", string_ascii);

        pyistream s(f);
        reflex::Input input(s);  

        len = input.get(buf, sizeof(buf));
        assert(len == strlen(string_ascii));
        assert(strncmp(buf, string_ascii, len) == 0);
    }

    const char* string_utf8 = "fo\xc3\xbc";
    
    // UTF-8 encoding is passed through as is
    {
        Py_XDECREF(f);
        f = PyObject_CallMethod(io, "BytesIO", "y", string_utf8);

        pyistream s(f);
        reflex::Input input(s);  

        len = input.get(buf, sizeof(buf));
        assert(len == 4);
        assert(strncmp(buf, string_utf8, len) == 0);
    }

    const char* string_latin1 = "fo\xfc";
    
    // Latin1 encoding converted to UTF-8
    {
        Py_XDECREF(f);
        f = PyObject_CallMethod(io, "BytesIO", "y", string_latin1);

        pyistream s(f);
        reflex::Input input(s);
        input.file_encoding(reflex::Input::file_encoding::latin);

        len = input.get(buf, sizeof(buf));

        for (size_t i = 0; i < len; i++) {
            if (isprint(buf[i]))
                printf("%c", buf[i]);
            else
                printf("\\x%hhx", buf[i]);
        }
        printf("\n");
        
        assert(len == 4);
        assert(strncmp(buf, string_utf8, len) == 0);
    }
    
}
