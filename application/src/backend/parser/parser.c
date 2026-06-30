#include "../../../include/backend/parser/parser.h"

class_t Parser_Class = {0};

int parser_constructor(parser_t* instance, int argc, void** argv)
{
    if (instance && argc == 1)
    {
        const char* filepath = argv[0];

        return 1;
    }

    return 0;
}
void parser_destructor(parser_t* instance)
{
    core_object_destroy(instance);
}

void Parser_Class_Init()
{
    core_class_init(&Parser_Class, NULL, "ParserClass", 0, 0, NULL, NULL, parser_constructor, parser_destructor);
}
void Parser_Class_Destroy()
{
    core_class_destroy(&Parser_Class);
}