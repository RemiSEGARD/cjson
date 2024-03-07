#ifndef CJSON_H
#define CJSON_H

#include <stdbool.h>
#include <stddef.h>

typedef struct cjson_element cjson_element;

typedef struct 
{
    bool value;
} cjson_bool;

/**
 * @brief returns the boolean value of element
 */
bool cjson_as_bool(cjson_element *element);
/**
 * @brief returns true if element is of boolean type
 */
bool cjson_is_bool(cjson_element *element);
/**
 * @brief create a cjson_element containing the given boolean value
 */
cjson_element *cjson_create_bool(bool value);

typedef struct 
{
    int value;
} cjson_integer;

/**
 * @brief returns the integer value of element
 */
int cjson_as_integer(cjson_element *element);
/**
 * @brief returns true if element is of integer type
 */
bool cjson_is_integer(cjson_element *element);
/**
 * @brief create a cjson_element containing the given integer value
 */
cjson_element *cjson_create_integer(int value);

typedef struct 
{
    double value;
} cjson_float;

double cjson_as_float(cjson_element *element);
/**
 * @brief returns true if element is of floating type
 */
bool cjson_is_float(cjson_element *element);
/**
 * @brief create a cjson_element containing the given floating value
 */
cjson_element *cjson_create_float(double value);

typedef struct 
{
    char *value;
} cjson_string;

char *cjson_as_string(cjson_element *element);
/**
 * @brief returns true if element is of string type
 */
bool cjson_is_string(cjson_element *element);
/**
 * @brief create a cjson_element containing the given string value
 */
cjson_element *cjson_create_string(char *value);

typedef struct 
{
    cjson_element **elements;
    size_t size;
    size_t capacity;
} cjson_array;

/**
 * @brief returns an element as an array
 */
cjson_array *cjson_as_array(cjson_element *element);
/**
 * @brief returns true if element is of array type
 */
bool cjson_is_array(cjson_element *element);
/**
 * @brief create a cjson_element that is an empty array
 */
cjson_element *cjson_create_array();
/**
 * @brief appends an element to an array
 */
void cjson_array_append(cjson_array *array, cjson_element *element);
/**
 * @brief inserts an element at a given index in an array
 */
void cjson_array_insert(cjson_array *array, cjson_element *element, size_t index);

typedef struct cjson_map_item
{
    char *name;
    cjson_element *element;
    struct cjson_map_item *next;
} cjson_map_item;

typedef struct
{
    cjson_map_item **items;
    size_t capacity;
} cjson_map;

/**
 * @brief sets the item name with a value in a given object
 */
void cjson_map_insert(cjson_map *map, char *name, cjson_element *element);

typedef struct
{
    cjson_map members;
} cjson_object;

/**
 * @brief gets the value of a member of an object
 */
cjson_element *cjson_object_get(cjson_object *object, char *name);
/**
 * @brief sets the member name with a value in a given object
 */
void cjson_object_insert(cjson_object *object, char *name, cjson_element *value);
/**
 * @brief returns the element as an object
 */
cjson_object *cjson_as_object(cjson_element *element);
/**
 * @brief returns true if element is of object type
 */
bool cjson_is_object(cjson_element *element);
/**
 * @brief create a cjson_element with a map of size capacity (the bigger the
 *        size, the less chance of hash collosion)
 */
cjson_element *cjson_create_object(size_t capacity);

/**
 * @brief returns NULL value of element
 */
void *cjson_as_null(cjson_element *element);
/**
 * @brief returns true if element is of null type
 */
bool cjson_is_null(cjson_element *element);
/**
 * @brief create a cjson_element of type null
 */
cjson_element *cjson_create_null();

/**
 * @brief gets an element inside a json element using a path
 *
 * @example
 * cjson_element *i = cjson_get_element_from({"example": [ {"int": 42} ]}, ".example[0].int");
 * assert(cjson_as_integer(i) == 42);
 */
cjson_element *cjson_get_element_from(cjson_element *element, char *from);

typedef struct
{
    char *name;
    cjson_element *element;
    bool end;
#ifdef CJSON_IMPLEMENTATION
    cjson_map *map;
    size_t i;
    cjson_map_item *item;
#endif /* CJSON_IMPLEMENTATION */
} cjson_object_iterator;

/**
 * @brief creates an iterator to iterate over a json object
 */
cjson_object_iterator cjson_iterate_object(cjson_object *obj);
/**
 * @brief finds the next member of an object, .end is set to true if there is no
 *        more members.
 * 
 * @example
 * cjson_object_iterator it = cjson_iterate_object(&element->value.object);
 * while (!it.end)
 * {
 *     printf("%s", it.name);
 *     it = cjson_iterate_next(&it);
 * }
 */
cjson_object_iterator cjson_iterate_next(cjson_object_iterator *iterator);

typedef union
{
    cjson_bool boolean;
    cjson_integer integer;
    cjson_float fraction;
    cjson_string string;
    cjson_array array;
    cjson_object object;
} cjson_value;

struct cjson_element
{
    enum
    {
        CJSON_NULL,
        CJSON_BOOL,
        CJSON_INTEGER,
        CJSON_FLOAT,
        CJSON_STRING,
        CJSON_ARRAY,
        CJSON_OBJECT,
    } element_type;
    cjson_value value;
};

typedef struct
{
    char *str;
    size_t size;
    size_t capacity;
} cjson_str_builder;

/**
 * @brief appends a character to a string builder
 */
void cjson_str_builder_append_char(cjson_str_builder *sb, char c);
/**
 * @brief appends a string to a string builder
 */
void cjson_str_builder_append_cstr(cjson_str_builder *sb, char *cstr);

/**
 * @brief creates the string corresponding to the given element
 */
char *cjson_to_str(cjson_element *element, int pretty);
/**
 * @brief returns a deep copy of the element
 */
cjson_element *cjson_clone(cjson_element *element);
/**
 * @brief prints the element to stdout
 */
void cjson_dump(cjson_element *element, int pretty);

/**
 * @brief deletes a json element recursively
 */
void cjson_delete(cjson_element *element);

/**
 * @brief parses a string to create a json element. return NULL if it fails
 */
cjson_element *cjson_parse_str(char *str);

#ifdef CJSON_IMPLEMENTATION

#define _POSIX_C_SOURCE 200809L

#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TODO() assert(0 && "TODO")

void cjson_str_builder_append_char(cjson_str_builder *sb, char c)
{
    if (sb->capacity == 0)
    {
        sb->capacity = 8;
        sb->str = malloc(sb->capacity * sizeof(char));
    }
    if (sb->size == sb->capacity)
    {
        sb->capacity *= 2;
        sb->str = realloc(sb->str, sb->capacity * sizeof(char));
    }
    sb->str[sb->size++] = c;

}

void cjson_str_builder_append_cstr(cjson_str_builder *sb, char *cstr)
{
    for (size_t i = 0; cstr[i] != '\0'; i++)
        cjson_str_builder_append_char(sb, cstr[i]);
}

size_t cjson_hash(char *str)
{
    size_t res = 0;
    for (size_t i = 0; str[i] != '\0'; i++)
        res = (res << 3) + str[i];
    return res;
}

void cjson_map_insert(cjson_map *map, char *name, cjson_element *element)
{
    size_t h = cjson_hash(name) % map->capacity;
    cjson_map_item *item = map->items[h];
    while (item != NULL && strcmp(item->name, name) != 0)
        item = item->next;
    if (item == NULL)
    {
        cjson_map_item *item = malloc(sizeof(cjson_map_item));
        item->next = map->items[h];
        item->name = strdup(name);
        item->element = element;
        map->items[h] = item;
    }
    else
    {
        cjson_delete(item->element);
        item->element = element;
    }
}

enum
{
    CJSON_TOK_ERROR = -1,
    CJSON_TOK_NONE = 0,
    CJSON_TOK_EOF,
    CJSON_TOK_INTEGER,
    CJSON_TOK_FLOAT,
    CJSON_TOK_STRING,
    CJSON_TOK_TRUE,
    CJSON_TOK_FALSE,
    CJSON_TOK_NULL,
    CJSON_TOK_COLON,
    CJSON_TOK_COMMA,
    CJSON_TOK_LBRACE,
    CJSON_TOK_RBRACE,
    CJSON_TOK_LBRACK,
    CJSON_TOK_RBRACK,
};

typedef struct
{
    int type;
    char *content;
    size_t content_len;
    int integer_value;
    double float_value;
} cjson_token;

static char *cjson_extract_string(cjson_token *token)
{
    assert(token->type == CJSON_STRING);
    char *res = calloc(token->content_len - 1, sizeof(char));
    size_t j = 0;
    for (size_t i = 1; i < token->content_len - 1; i++)
    {
        if (token->content[i] == '\\')
        {
            i += 1;
            switch (token->content[i])
            {
            case '"':
                res[j] = '"';
                break;
            case '\\':
                res[j] = '\\';
                break;
            case '/':
                res[j] = '/';
                break;
            case 'b':
                res[j] = '\b';
                break;
            case 'f':
                res[j] = '\f';
                break;
            case 'n':
                res[j] = '\n';
                break;
            case 't':
                res[j] = '\t';
                break;
            case 'u':
                TODO();
            default:
                free(res);
                return NULL;
            }
        }
        else
            res[j] = token->content[i];
        j += 1;
    }
    return res;
}

typedef struct
{
    char *content;
    size_t location;
    cjson_token token;
} cjson_lexer;

void cjson_read_next_token(cjson_lexer *lexer)
{
    char *input = lexer->content + lexer->location;
    size_t token_len = 0;
    int sign = 1;
    switch (input[0])
    {
    case '-':
        sign = -1;
        /* FALLTHROUGH */
    case '0'...'9':
        lexer->token.type = CJSON_TOK_INTEGER;
        token_len = 1;
        if (input[0] != '0')
        {
            while (isdigit(input[token_len]))
                token_len += 1;
        }
        lexer->token.integer_value = strtol(input, NULL, 10) * sign;
        if (input[token_len] == '.' && isdigit(input[token_len + 1]))
        {
            lexer->token.type = CJSON_TOK_FLOAT;
            token_len += 1;
            while (isdigit(input[token_len + 1]))
                token_len += 1;
            lexer->token.float_value = strtod(input, NULL) * sign;
        }
        break;
    case 'f':
        if (strncmp(input, "false", 5) != 0)
            goto token_error;
        token_len = 5;
        lexer->token.type = CJSON_TOK_FALSE;
        break;
    case 'n':
        if (strncmp(input, "null", 4) != 0)
            goto token_error;
        token_len = 4;
        lexer->token.type = CJSON_TOK_NULL;
        break;
    case 't':
        if (strncmp(input, "true", 4) != 0)
            goto token_error;
        token_len = 4;
        lexer->token.type = CJSON_TOK_TRUE;
        break;
    case ',':
        lexer->token.type = CJSON_TOK_COMMA;
        token_len = 1;
        break;
    case ':':
        lexer->token.type = CJSON_TOK_COLON;
        token_len = 1;
        break;
    case '[':
        lexer->token.type = CJSON_TOK_LBRACK;
        token_len = 1;
        break;
    case ']':
        lexer->token.type = CJSON_TOK_RBRACK;
        token_len = 1;
        break;
    case '{':
        lexer->token.type = CJSON_TOK_LBRACE;
        token_len = 1;
        break;
    case '}':
        lexer->token.type = CJSON_TOK_RBRACE;
        token_len = 1;
        break;
    case '"':
        token_len = 1;
        lexer->token.type = CJSON_TOK_STRING;
        while (input[token_len] != '"')
        {
            if (input[token_len] == '\\')
            {
                switch (input[token_len + 1])
                {
                case '"': case '\\': case '/': case 'b': case 'f': case 'n':
                case 'r': case 't':
                    token_len += 2;
                    break;
                case 'u':
                    TODO();
                default:
                    goto token_error;
                }
            }
            else
                token_len += 1;
        }
        token_len += 1;
        break;
    case '\0':
        lexer->token.type = CJSON_TOK_EOF;
        break;
    default:
token_error:
        lexer->token.type = CJSON_TOK_ERROR;
    }

    lexer->token.content = input;
    lexer->token.content_len = token_len;
    lexer->location += token_len;
}

cjson_token cjson_lexer_peek(cjson_lexer *lexer)
{
    if (lexer->token.type == CJSON_TOK_NONE)
        cjson_read_next_token(lexer);
    return lexer->token;
}

cjson_token cjson_lexer_pop(cjson_lexer *lexer)
{
    if (lexer->token.type == CJSON_TOK_NONE)
        cjson_read_next_token(lexer);
    cjson_token res = lexer->token;
    lexer->token.type = CJSON_TOK_NONE;
    return res;
}

cjson_element *cjson_parse_element(cjson_lexer *lexer, int *error);

void cjson_parse_ws(cjson_lexer *lexer)
{
    while (isspace(lexer->content[lexer->location]))
        lexer->location += 1;
}

void cjson_parse_member(cjson_lexer *lexer, cjson_map *map, int *error)
{
    cjson_parse_ws(lexer);
    cjson_token str = cjson_lexer_pop(lexer); // parse string
    if (str.type != CJSON_TOK_STRING)
    {
        *error = 1;
        return;
    }
    char *name = strndup(str.content + 1, str.content_len - 2);
    cjson_parse_ws(lexer);
    if (cjson_lexer_peek(lexer).type != CJSON_TOK_COLON)
    {
        *error = 1;
        return;
    }
    cjson_lexer_pop(lexer);
    cjson_element *element = cjson_parse_element(lexer, error);
    cjson_map_insert(map, name, element);
    free(name);
}

void cjson_parse_members(cjson_lexer *lexer, cjson_map *map, int *error)
{
    cjson_parse_member(lexer, map, error);
    while (!*error && cjson_lexer_peek(lexer).type == CJSON_TOK_COMMA)
    {
        cjson_lexer_pop(lexer);
        cjson_parse_member(lexer, map, error);
    }
}

cjson_element *cjson_parse_object(cjson_lexer *lexer, int *error)
{
    // Clear '{'
    cjson_lexer_pop(lexer);
    cjson_element *res = calloc(1, sizeof(cjson_element));
    res->element_type = CJSON_OBJECT;
    res->value.object.members.capacity = 64;
    res->value.object.members.items = calloc(64, sizeof(cjson_map_item *));

    cjson_parse_ws(lexer);
    cjson_token token = cjson_lexer_peek(lexer);
    if (!*error && token.type != CJSON_TOK_RBRACE)
        cjson_parse_members(lexer, &res->value.object.members, error);
    token = cjson_lexer_pop(lexer);
    if (token.type != CJSON_TOK_RBRACE)
    {
        cjson_delete(res);
        return NULL;
    }
    return res;
}

cjson_element *cjson_parse_array(cjson_lexer *lexer, int *error)
{
    cjson_lexer_pop(lexer);
    cjson_element *res = calloc(1, sizeof(cjson_element));
    res->element_type = CJSON_ARRAY;
    cjson_parse_ws(lexer);
    if (!*error && cjson_lexer_peek(lexer).type != CJSON_TOK_RBRACK)
    {
        cjson_element *elt = cjson_parse_element(lexer, error);
        cjson_array_append(&res->value.array, elt);
        while (!*error && cjson_lexer_peek(lexer).type == CJSON_TOK_COMMA)
        {
            cjson_lexer_pop(lexer);
            elt = cjson_parse_element(lexer, error);
            cjson_array_append(&res->value.array, elt);
        }
        if (cjson_lexer_peek(lexer).type != CJSON_TOK_RBRACK)
        {
            cjson_delete(res);
            return NULL;
        }
    }
    cjson_lexer_pop(lexer);
    return res;
}

cjson_element *cjson_parse_value(cjson_lexer *lexer, int *error)
{
    cjson_token token = cjson_lexer_peek(lexer);
    cjson_element *res = NULL;
    switch (token.type)
    {
    case CJSON_TOK_LBRACE:
        res = cjson_parse_object(lexer, error);
        break;
    case CJSON_TOK_LBRACK:
        res = cjson_parse_array(lexer, error);
        break;
    case CJSON_TOK_STRING:
        res = calloc(1, sizeof(cjson_element));
        res->element_type = CJSON_STRING;
        res->value.string.value = cjson_extract_string(&token);
        cjson_lexer_pop(lexer);
        break;
    case CJSON_TOK_INTEGER:
        res = calloc(1, sizeof(cjson_element));
        res->element_type = CJSON_INTEGER;
        res->value.integer.value = token.integer_value;
        cjson_lexer_pop(lexer);
        break;
    case CJSON_TOK_FLOAT:
        res = calloc(1, sizeof(cjson_element));
        res->element_type = CJSON_INTEGER;
        res->value.fraction.value = token.float_value;
        cjson_lexer_pop(lexer);
        break;
    case CJSON_TOK_TRUE:
        res = calloc(1, sizeof(cjson_element));
        res->element_type = CJSON_BOOL;
        res->value.boolean.value = true;
        cjson_lexer_pop(lexer);
        break;
    case CJSON_TOK_FALSE:
        res = calloc(1, sizeof(cjson_element));
        res->element_type = CJSON_BOOL;
        res->value.boolean.value = false;
        cjson_lexer_pop(lexer);
        break;
    case CJSON_TOK_NULL:
        res = calloc(1, sizeof(cjson_element));
        res->element_type = CJSON_NULL;
        cjson_lexer_pop(lexer);
        break;
    }
    return res;
}

cjson_element *cjson_parse_element(cjson_lexer *lexer, int *error)
{
    cjson_parse_ws(lexer);
    cjson_element *value = cjson_parse_value(lexer, error);
    cjson_parse_ws(lexer);
    return value;
}

cjson_element *cjson_parse_str(char *str)
{
    int error = 0;

    cjson_lexer lexer = { .content = str };
    cjson_element *res = cjson_parse_element(&lexer, &error);
    if (error)
        return NULL;
    return res;
};


bool cjson_as_bool(cjson_element *element)
{
    assert(cjson_is_bool(element));
    return element->value.boolean.value;
}

bool cjson_is_bool(cjson_element *element)
{
    return element->element_type == CJSON_BOOL;
}

cjson_element *cjson_create_bool(bool value)
{
    cjson_element *res = calloc(1, sizeof(cjson_element));
    res->element_type = CJSON_BOOL;
    res->value.boolean.value = value;
    return res;
}

int cjson_as_integer(cjson_element *element)
{
    assert(cjson_is_integer(element));
    return element->value.integer.value;
}

bool cjson_is_integer(cjson_element *element)
{
    return element->element_type == CJSON_INTEGER;
}

cjson_element *cjson_create_integer(int value)
{
    cjson_element *res = calloc(1, sizeof(cjson_element));
    res->element_type = CJSON_INTEGER;
    res->value.integer.value = value;
    return res;
}

double cjson_as_float(cjson_element *element)
{
    assert(cjson_is_float(element));
    return element->value.fraction.value;
}

bool cjson_is_float(cjson_element *element)
{
    return element->element_type == CJSON_FLOAT;
}

cjson_element *cjson_create_float(double value)
{
    cjson_element *res = calloc(1, sizeof(cjson_element));
    res->element_type = CJSON_FLOAT;
    res->value.fraction.value = value;
    return res;
}

char *cjson_as_string(cjson_element *element)
{
    assert(cjson_is_string(element));
    return element->value.string.value;
}

bool cjson_is_string(cjson_element *element)
{
    return element->element_type == CJSON_STRING;
}

cjson_element *cjson_create_string(char *value)
{
    cjson_element *res = calloc(1, sizeof(cjson_element));
    res->element_type = CJSON_STRING;
    res->value.string.value = strdup(value);
    return res;
}

void cjson_array_append(cjson_array *array, cjson_element *element)
{
    if (array->capacity == 0)
    {
        array->capacity = 8;
        array->elements = malloc(array->capacity * sizeof(cjson_element *));
    }
    if (array->size == array->capacity)
    {
        array->capacity *= 2;
        array->elements = realloc(array->elements, array->capacity * sizeof(cjson_element *));
    }
    array->elements[array->size++] = element;
}

void cjson_array_insert(cjson_array *array, cjson_element *element, size_t index)
{
    cjson_array_append(array, element);
    size_t current = array->size - 1;
    while (current > index)
    {
        cjson_element *tmp = array->elements[current - 1];
        array->elements[current - 1] = array->elements[current];
        array->elements[current] = tmp;
        current -= 1;
    }
}

cjson_array *cjson_as_array(cjson_element *element)
{
    assert(cjson_is_array(element));
    return &element->value.array;
}

cjson_element *cjson_create_array()
{
    cjson_element *res = calloc(1, sizeof(cjson_element));
    res->element_type = CJSON_ARRAY;
    return res;
}

bool cjson_is_array(cjson_element *element)
{
    return element->element_type == CJSON_ARRAY;
}

cjson_element *cjson_object_get(cjson_object *object, char *name)
{
    size_t h = cjson_hash(name) % object->members.capacity;

    cjson_map_item *item = object->members.items[h];
    while (item != NULL && strcmp(item->name, name))
        item = item->next;

    if (item != NULL)
        return item->element;
    return NULL;
}

void cjson_object_insert(cjson_object *object, char *name, cjson_element *value)
{
    cjson_map_insert(&object->members, name, value);
}

cjson_object *cjson_as_object(cjson_element *element)
{
    assert(element->element_type == CJSON_OBJECT);
    return &element->value.object;
}

bool cjson_is_object(cjson_element *element)
{
    return element->element_type == CJSON_OBJECT;
}

cjson_element *cjson_create_object(size_t capacity)
{
    cjson_element *res = calloc(1, sizeof(cjson_element));
    res->element_type = CJSON_OBJECT;
    res->value.object.members.capacity = capacity;
    res->value.object.members.items = calloc(capacity, sizeof(cjson_map_item *));
    return res;
}

void *cjson_as_null(cjson_element *element)
{
    assert(cjson_is_null(element));
    return NULL;
}

bool cjson_is_null(cjson_element *element)
{
    return element->element_type == CJSON_NULL;
}

cjson_element *cjson_create_null()
{
    cjson_element *res = calloc(1, sizeof(cjson_element));
    res->element_type = CJSON_NULL;
    return res;
}

cjson_element *cjson_get_element_from_object(cjson_element *element, char *from)
{
    assert(cjson_is_object(element));
    assert((isalpha(from[1]) || from[1] == '_') && "invalid member name");
    cjson_object *object = cjson_as_object(element);
    size_t name_len = strspn(
            from + 1,
            "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_0123456789");
    char *name = strndup(from + 1, name_len);
    cjson_element *elt_at_name = cjson_object_get(object, name);
    assert(elt_at_name && "object does not contain member");
    free(name);
    if (from[name_len + 1] == '\0')
        return elt_at_name;
    return cjson_get_element_from(elt_at_name, from + name_len + 1);
}

cjson_element *cjson_get_element_from_array(cjson_element *element, char *from)
{
    assert(cjson_is_array(element));
    cjson_array *array = cjson_as_array(element);
    char *endptr = NULL;
    size_t i = strtoul(from + 1, &endptr, 10);
    assert(*endptr == ']' && "invalid syntax when indexing");
    cjson_element *elt_at_i = array->elements[i];
    if (endptr[1] == '\0')
        return elt_at_i;
    return cjson_get_element_from(elt_at_i, endptr + 1);
}

cjson_element *cjson_get_element_from(cjson_element *element, char *from)
{
    if (*from == '[')
        return cjson_get_element_from_array(element, from);
    else if (*from == '.')
        return cjson_get_element_from_object(element, from);
    assert(0 && "invalid syntax when getting from string path");
}

cjson_object_iterator cjson_iterate_object(cjson_object *obj)
{
    cjson_map_item *first = NULL;
    size_t i = 0;
    for (; i < obj->members.capacity; i++)
    {
        first = obj->members.items[i];
        if (first != NULL)
            break;
    }
    cjson_object_iterator res = {
        .map = &obj->members,
        .i = i,
        .end = true,
        .item = NULL,
    };
    if (first != NULL)
    {
        res.element = first->element;
        res.name = first->name;
        res.end = false;
        res.i = i;
        res.item = first;
    }
    return res;
}

cjson_object_iterator cjson_iterate_next(cjson_object_iterator *iterator)
{
    if (iterator->end)
        return *iterator;
    if (iterator->item->next != NULL)
    {
        iterator->item = iterator->item->next;
        iterator->name = iterator->item->name;
        iterator->element = iterator->item->element;
    }
    else 
    {
        for (iterator->i += 1; iterator->i < iterator->map->capacity; iterator->i++)
        {
            if (iterator->map->items[iterator->i])
            {
                iterator->item = iterator->map->items[iterator->i];
                iterator->name = iterator->item->name;
                iterator->element = iterator->item->element;
                break;
            }
        }
    }
    if (iterator->i == iterator->map->capacity)
        iterator->end = true;
    return *iterator;
}

void cjson_pretty_newline(int pretty, int indent)
{
    assert(indent >= 0);
    if (pretty)
        printf("\n%*s", indent, "");
}

void cjson_to_str_rec(cjson_element *element, int pretty, cjson_str_builder *sb)
{
    static size_t indent = 0;
    char buffer[11];
    switch (element->element_type)
    {
    case CJSON_NULL:
        cjson_str_builder_append_cstr(sb, "null");
        break;
    case CJSON_BOOL:
        if (element->value.boolean.value)
            cjson_str_builder_append_cstr(sb, "true");
        else
            cjson_str_builder_append_cstr(sb, "false");
        break;
    case CJSON_INTEGER:
        sprintf(buffer, "%d", element->value.integer.value);
        cjson_str_builder_append_cstr(sb, buffer);
        break;
    case CJSON_FLOAT:
        sprintf(buffer, "%lf", element->value.fraction.value);
        cjson_str_builder_append_cstr(sb, buffer);
        break;
    case CJSON_STRING:
        cjson_str_builder_append_char(sb, '"');
        cjson_str_builder_append_cstr(sb, element->value.string.value);
        cjson_str_builder_append_char(sb, '"');
        break;
    case CJSON_ARRAY:
        cjson_str_builder_append_char(sb, '[');
        indent += 2;
        if (pretty)
        {
            cjson_str_builder_append_char(sb, '\n');
            for (size_t i = 0; i < indent; i++)
                cjson_str_builder_append_char(sb, ' ');
        }
        if (element->value.array.size > 0)
        {
            cjson_to_str_rec(element->value.array.elements[0], pretty, sb);
        }
        for (size_t i = 1; i < element->value.array.size; i++)
        {
            cjson_str_builder_append_char(sb, ',');
            if (pretty)
            {
                cjson_str_builder_append_char(sb, '\n');
                for (size_t i = 0; i < indent; i++)
                    cjson_str_builder_append_char(sb, ' ');
            }
            cjson_to_str_rec(element->value.array.elements[i], pretty, sb);
        }
        indent -= 2;
        if (pretty)
        {
            cjson_str_builder_append_char(sb, '\n');
            for (size_t i = 0; i < indent; i++)
                cjson_str_builder_append_char(sb, ' ');
        }
        cjson_str_builder_append_char(sb, ']');
        break;
    case CJSON_OBJECT:
        cjson_str_builder_append_char(sb, '{');
        indent += 2;
        if (pretty)
        {
            cjson_str_builder_append_char(sb, '\n');
            for (size_t i = 0; i < indent; i++)
                cjson_str_builder_append_char(sb, ' ');
        }
        cjson_object_iterator it = cjson_iterate_object(&element->value.object);
        if (!it.end)
        {
            cjson_str_builder_append_char(sb, '"');
            cjson_str_builder_append_cstr(sb, it.name);
            cjson_str_builder_append_cstr(sb, "\":");
            cjson_to_str_rec(it.element, pretty, sb);
        }
        for (cjson_iterate_next(&it); !it.end; cjson_iterate_next(&it))
        {
            cjson_str_builder_append_char(sb, ',');
            if (pretty)
            {
                cjson_str_builder_append_char(sb, '\n');
                for (size_t i = 0; i < indent; i++)
                    cjson_str_builder_append_char(sb, ' ');
            }
            cjson_str_builder_append_char(sb, '"');
            cjson_str_builder_append_cstr(sb, it.name);
            cjson_str_builder_append_cstr(sb, "\":");
            cjson_to_str_rec(it.element, pretty, sb);
        }
        indent -= 2;
        if (pretty)
        {
            cjson_str_builder_append_char(sb, '\n');
            for (size_t i = 0; i < indent; i++)
                cjson_str_builder_append_char(sb, ' ');
        }
        cjson_str_builder_append_char(sb, '}');
        break;
    }
}

char *cjson_to_str(cjson_element *element, int pretty)
{
    cjson_str_builder sb = { 0 };
    cjson_to_str_rec(element, pretty, &sb);
    cjson_str_builder_append_char(&sb, '\0');
    return sb.str;
}

cjson_element *cjson_clone(cjson_element *element)
{
    if (element == NULL)
        return NULL;

    cjson_element *res = NULL;
    switch (element->element_type)
    {
    case CJSON_NULL:
    case CJSON_BOOL:
    case CJSON_INTEGER:
    case CJSON_FLOAT:
        res = calloc(1, sizeof(cjson_element));
        res->element_type = element->element_type;
        *res = *element;
        break;
    case CJSON_STRING:
        res = calloc(1, sizeof(cjson_element));
        res->element_type = CJSON_STRING;
        res->value.string.value = strdup(element->value.string.value);
        break;
    case CJSON_ARRAY: {
            res = cjson_create_array();
            cjson_array *src_arr = cjson_as_array(element);
            cjson_array *dst_arr = cjson_as_array(res);
            for (size_t i = 0; i < src_arr->size; i++)
                cjson_array_append(dst_arr, cjson_clone(src_arr->elements[i]));
        } break;
    case CJSON_OBJECT: {
            cjson_object *src_obj = cjson_as_object(element);
            res = cjson_create_object(src_obj->members.capacity);
            cjson_object *dst_obj = cjson_as_object(res);
            cjson_object_iterator it = cjson_iterate_object(src_obj);
            while (!it.end)
            {
                cjson_object_insert(dst_obj, it.name, cjson_clone(it.element));
                it = cjson_iterate_next(&it);
            }
        } break;
    }
    return res;
}

void cjson_dump(cjson_element *element, int pretty)
{
    static size_t indent = 0;
    switch (element->element_type)
    {
    case CJSON_NULL:
        printf("null");
        break;
    case CJSON_BOOL:
        if (element->value.boolean.value)
            printf("true");
        else
            printf("false");
        break;
    case CJSON_INTEGER:
        printf("%d", element->value.integer.value);
        break;
    case CJSON_FLOAT:
        printf("%d", element->value.integer.value);
        break;
    case CJSON_STRING:
        printf("\"%s\"", element->value.string.value);
        break;
    case CJSON_ARRAY:
        putchar('[');
        indent += 2;
        cjson_pretty_newline(pretty, indent);
        if (element->value.array.size > 0)
        {
            cjson_dump(element->value.array.elements[0], pretty);
        }
        for (size_t i = 1; i < element->value.array.size; i++)
        {
            putchar(',');
            cjson_pretty_newline(pretty, indent);
            cjson_dump(element->value.array.elements[i], pretty);
        }
        indent -= 2;
        cjson_pretty_newline(pretty, indent);
        putchar(']');
        break;
    case CJSON_OBJECT:
        putchar('{');
        indent += 2;
        cjson_pretty_newline(pretty, indent);
        cjson_object_iterator it = cjson_iterate_object(&element->value.object);
        if (!it.end)
        {
            printf("\"%s\":", it.name);
            cjson_dump(it.element, pretty);
        }
        for (cjson_iterate_next(&it); !it.end; cjson_iterate_next(&it))
        {
            putchar(',');
            cjson_pretty_newline(pretty, indent);
            printf("\"%s\":", it.name);
            cjson_dump(it.element, pretty);
        }
        indent -= 2;
        cjson_pretty_newline(pretty, indent);
        putchar('}');
        break;
    }
}

void cjson_delete(cjson_element *element)
{
    if (element->element_type == CJSON_STRING)
        free(element->value.string.value);
    else if (element->element_type == CJSON_ARRAY)
    {
        for (size_t i = 0; i < element->value.array.size; i++)
            cjson_delete(element->value.array.elements[i]);
        free(element->value.array.elements);
    }
    else if (element->element_type == CJSON_OBJECT)
    {
        cjson_object_iterator it = cjson_iterate_object(&element->value.object);
        while (!it.end)
        {
            free(it.name);
            cjson_delete(it.element);
            cjson_map_item *item = it.item;
            cjson_iterate_next(&it);
            free(item);
        }
        free(element->value.object.members.items);
    }
    free(element);
}

#endif /* CJSON_IMPLEMENTATION */

#endif /* ! CSJON_H */
