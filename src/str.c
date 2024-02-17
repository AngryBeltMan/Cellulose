#include "str.h"
// constructs an empty str with STR_DEFAULT_SIZE
str_res strNew() {
    str str;
    if ((str.contents = calloc(STR_DEFAULT_SIZE, 1)) == NULL) {
        return (str_res){.result = str_mem_error};
    }
    str.len = 0;
    str.max_len = STR_DEFAULT_SIZE;
    return (str_res){
        .string = str,
            .result = str_no_error,
    };
}

// constructs a struct str with the contents containing the given char array
str_res fromCharArray(char *value) {
    str str;
    // malloc the amount of bytes value has
    str.len = strlen(value);
    str.max_len = strlen(value) + 5;
    if ((str.contents = calloc(str.max_len, 1)) == NULL)
        return (str_res){.result = -1};
    // no need for strncpy cause str.contents will always have enough memory to
    // hold value
    strcpy(str.contents, value);
    return (str_res){.string = str, .result = 0};
}
// creates a new str from a reference of another str
str_res strClone(str *string) {
    str new_string = {.len = string->len, .max_len = string->max_len};
    if ((new_string.contents = calloc(string->max_len, 1)) == NULL)
        return (str_res){.result = -1};
    strcpy(new_string.contents, string->contents);
    return (str_res){.string = new_string, .result = 0};
}

// return a str with its contents holding value as a char array
str_res fromDouble(long double value) {
    str str;
    int is_negative = value < 0;
    // gets the number places of value
    str.len = floor(value == 0 ? 0 : log10(value * (1 - (is_negative * 2)))) + 8 + is_negative;
    str.max_len = str.len + 1;
    if ((str.contents = calloc(str.max_len, 1)) == NULL)
        return (str_res){.result = -1};
    snprintf(str.contents, str.len + 8 + is_negative, "%Lf", value);
    return (str_res){.string = str, .result = 0};
}

// return a str with its contents holding value as a char array
str_res fromNum(unsigned int value) {
    str str;
    // gets the number places of value
    str.len = floor(value == 0 ? 0 : log10(value)) + 1;
    str.max_len = str.len;
    if ((str.contents = calloc(str.max_len, 1)) == NULL)
        return (str_res){.result = -1};
    snprintf(str.contents, str.len + 1, "%i", value);
    return (str_res){.string = str, .result = 0};
}

static inline str_res_id strGrow(str *string) {
    string->max_len *= 2;
    if ((string->contents = realloc(string->contents, string->max_len)) == NULL)
        return str_mem_error;
#ifdef CHECK_MEMSET
    char *original_address = string->contents;
#endif
    // sets all of the garbage bytes from realloc to zero
    memset(string->contents + string->len, 0x0, string->max_len - string->len);
#ifdef CHECK_MEMSET
    // checks to see if the memory address changed after memset
    // if it did that means an error had occured
    if (original_address != string->contents)
        return str_corrupted_pointer;
#endif
    return 0;
}
int pushChar(str *string, char character) {
    string->contents[string->len] = character;
    if (++string->len == string->max_len)
        strGrow(string);
    return 0;
}
inline str_res_id appendStr(str *string, str *value) {
    return appendCharArray(string, value->contents, value->len);
}

// appends a char array to the right of the str.
// @string the str that will be effected
// @value the value that will be append on the string
// @v_len the length of the argumen value
inline str_res_id appendCharArray(str *string, const char* value, size_t v_len) {
    if (value == NULL)
        return str_null_input;
    // increase the size until there is enough space
    while (string->max_len < v_len + string->len)
        if (strGrow(string) != str_no_error)
            return -1;
    strcpy(string->contents + string->len, value);
    string->len += v_len;
    return 0;
}

int popFront(str *string) {
    // if the length is zero that means we can't pop anything
    if (string->len == 0)
        return -2;
    // reallocate to a smaller size when the string length is much smaller than
    // the string max
    if (string->len * SHRINK_REQ < string->max_len) {
        string->max_len /= 1.25;
        if ((string->contents = realloc(string->contents, string->max_len)) == NULL)
            return -1;
    }
    string->contents[--string->len] = 0x0;
    return 0;
}

bool isNum(str *string) {
    bool decimal_appear = false;
    if (!((string->contents[0] >= '0') && (string->contents[0] <= '9')) && string->contents[0] != '-')
        return false;
    for (size_t char_index = 1; char_index < string->len; ++char_index) {
        if ((string->contents[char_index] >= 48) &&
                (string->contents[char_index] < 58)) {
            continue;
        }
        // this means we encountered another decimal in the number making it invalid
        if (string->contents[char_index] == '.' && !decimal_appear)
            decimal_appear = true;
        else
            return false;
    }
    return true;
}

int insertAt_char(str *str, size_t index, char in) {
    if (str->len + 1 >= str->max_len)
        if (strGrow(str) != str_no_error)
            return -1;
    // shift all of the bytes to the right one to make room for the new byte that
    // is being inserted
    memmove(str->contents + index, str->contents + index + 1,
            str->len - (index + 1));
    str->contents[index] = in;
    return 0;
}
int appendDouble(str *string, long double value) {
    str_res conv_val = fromDouble(value);
    if (conv_val.result != str_no_error)
        return conv_val.result;
    appendStr(string, &conv_val.string);
    free(conv_val.string.contents);
    return str_no_error;
}
int appendNum(str *string, unsigned int value) {
    str_res conv_val = fromNum(value);
    if (conv_val.result != str_no_error)
        return conv_val.result;
    appendStr(string, &conv_val.string);
    free(conv_val.string.contents);
    return str_no_error;
}
str_res_id insertAt_char_array(str *str, size_t index, char *in) {
    if (in == NULL)
        return str_null_input;
    size_t insert_str_len = strlen(in);
    for (size_t char_index = str->len; char_index >= index; --char_index) {
        str->contents[char_index + insert_str_len] = str->contents[char_index];
    }
    strncpy(str->contents + index, in, insert_str_len - 1);
    str->contents[insert_str_len + index - 1] = in[insert_str_len - 1];
    return str_no_error;
}
