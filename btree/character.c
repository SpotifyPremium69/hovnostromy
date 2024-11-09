#include "character.h"
#include <stdio.h>

const char *character_class_to_string(character_class_t class)
{
    switch (class)
    {
#define X(name) \
    case name:  \
        return #name;
        CHARACTER_CLASSES
#undef X
    default:
        return "Unknown class";
    }
}

void print_character(character_t* character)
{
    printf("%s, %s, %u",
           character->name,
           character_class_to_string(character->character_class),
           character->level);
}