#include <stdio.h>

#define CJSON_IMPLEMENTATION
#include "../cjson.h"

int main()
{
    char *input = "{\"test\": 1, \"test2\": 3}";
    cjson_element *element = cjson_parse_str(input);
    cjson_object *obj = cjson_as_object(element);
    cjson_element *elt2 = cjson_object_get(obj, "test");
    assert(elt2);
    printf("Value of `test': %d\n", cjson_as_integer(elt2));

    char *input2 = "{\"test\": { \"test2\": [1,2, {\"test3\": 4, \"test4\": 5}]}}";
    cjson_element *element2 = cjson_parse_str(input2);
    cjson_element *t3 = cjson_get_element_from(element2, ".test.test2[2].test3");
    printf("Value of `test.test2[2].test3': %d\n", cjson_as_integer(t3));

    char *input3 = "{"
                   "\"glossary\": {"
                   "\"title\": \"example glossary\","
                   "\"GlossDiv\": {"
                   "\"title\": \"S\","
                   "\"GlossList\": {"
                   "\"GlossEntry\": {"
                   "\"ID\": \"SGML\","
                   "\"SortAs\": \"SGML\","
                   "\"GlossTerm\": \"Standard Generalized Markup Language\","
                   "\"Acronym\": \"SGML\","
                   "\"Abbrev\": \"ISO 8879:1986\","
                   "\"GlossDef\": {"
                   "\"para\": \"A meta-markup language, used to create markup "
                   "languages such as DocBook.\","
                   "\"GlossSeeAlso\": [\"GML\", \"XML\"]"
                   "},"
                   "\"GlossSee\": \"markup\""
                   "}"
                   "}"
                   "}"
                   "}}";

    cjson_element *element3 = cjson_parse_str(input3);
    cjson_dump(element3, 0);
    putchar('\n');

    char *dump = cjson_to_str(element3, 0);
    puts(dump);

    cjson_delete(element);
    cjson_delete(element2);
    cjson_delete(element3);
    free(dump);

    cjson_element *elt = cjson_create_object(10);
    cjson_object *creating = cjson_as_object(elt);
    cjson_object_insert(creating, "f1", cjson_create_integer(42));
    cjson_object_insert(creating, "f2", cjson_create_string("42"));
    cjson_object_insert(creating, "f3", cjson_create_array());
    cjson_array *arr = cjson_as_array(cjson_object_get(creating, "f3"));
    cjson_array_append(arr, cjson_create_integer(1));
    cjson_array_append(arr, cjson_create_string("2"));
    cjson_array_append(arr, cjson_create_string("3"));
    cjson_array_insert(arr, cjson_create_integer(-1), 0);
    cjson_array_insert(arr, cjson_create_integer(0), 1);
    cjson_array_insert(arr, cjson_create_integer(5), 5);
    cjson_array_insert(arr, cjson_create_integer(4), 5);

    cjson_dump(elt, 1);
    putchar('\n');
    cjson_delete(elt);

    return 0;
}
