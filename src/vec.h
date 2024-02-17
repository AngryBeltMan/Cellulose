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
    _type* elements;\
}

// constructor for vec attribute type
#define VEC_ATTR(_type, _name) struct _name##vec { \
    size_t max_size, length;\
    _type* elements;\
} _name
#define VEC_SIZED(_type, _size) {\
    .max_size = _size,\
    .length = 0,\
    .elements = malloc(sizeof(_type) * _size)\
}
#define VEC_NEW(_type) VEC_SIZED(_type, DEFAULT_SIZE)
#define VEC_LEN(_vec) _vec.length
#define VEC_FREE(_vec) \
    do { \
        free(_vec.elements);\
        _vec.elements = NULL; \
    } while (0);
#define VEC_APPEND(_vec, _element) \
    if (!(_vec.length < _vec.max_size)) {\
        _vec.max_size *= GROW_MULT;\
        _vec.max_size += GROW_ADD;\
        _vec.elements = realloc(_vec.elements, _vec.max_size * sizeof(_element));\
        assert(_vec.elements && "ERROR: reallocation resulted in NULL");\
    }\
    _vec.elements[_vec.length] = _element;\
    ++_vec.length;
#define VEC_APPEND_CPY(_vec, _element)\
    if (!(_vec.length < _vec.max_size)) {\
        _vec.max_size *= GROW_MULT;\
        _vec.max_size += GROW_ADD;\
        _vec.elements = realloc(_vec.elements, _vec.max_size * sizeof(_element));\
        assert(_vec.elements && "ERROR: reallocation resulted in NULL");\
    }\
    memcpy(&_vec.elements[_vec.length], &_element, sizeof(_element)); \
    ++_vec.length;
// iterates over the vector
#define VEC_ITER(_vec, _type, _element) _type _element = _vec.elements[0]; for (size_t _element##_index = 1; _element##_index <= _vec.length; _element = _vec.elements[_element##_index], ++_element##_index )

