#pragma once
#include <stdlib.h>
#include <assert.h>
#define DEFAULT_SIZE 5
// how much the vector size should multiply when reallocing
#define GROW_MULT 2
// how much the vector size should additionaly when reallocing
#define GROW_ADD 0

#define VEC(_type) struct { \
    size_t max_size, length;\
    size_t element_size;\
    _type* elements;\
}

// constructor for vec attribute type
#define VEC_ATTR(_type, _name) struct _name { \
    size_t max_size, length, element_size;\
    _type* elements;\
} _name
#define VEC_SIZED(_type, _size) {\
    .max_size = _size,\
    .length = 0,\
    .element_size = sizeof(_type),\
    .elements = malloc(sizeof(_type) * _size)\
}
#define VEC_NEW(_type) VEC_SIZED(_type, DEFAULT_SIZE)
#define VEC_LEN(_vec) _vec.length
#define VEC_FREE(_vec) free(_vec.elements)
#define VEC_APPEND(_vec, _element) \
    if (!(_vec.length < _vec.max_size)) {\
        _vec.max_size *= GROW_MULT;\
        _vec.max_size += GROW_ADD;\
        _vec.elements = realloc(_vec.elements, _vec.max_size * _vec.element_size);\
        assert(_vec.elements && "ERROR: reallocation resulted in NULL");\
    }\
    _vec.elements[_vec.length] = _element;\
    ++_vec.length;

// iterates over the vector
#define VEC_ITER(_vec, _type, _element) _type _element = _vec.elements[0]; for (size_t _i = 1; _i <= _vec.length; _element = _vec.elements[_i], ++_i )

