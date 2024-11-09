/*
 * Hlavičkový soubor pro "Character".
 * Tento soubor neupravujte.
 */

#ifndef IAL_CHARACTER_H
#define IAL_CHARACTER_H

// povolání
#define CHARACTER_CLASSES \
    X(Bard)              \
    X(Cleric)            \
    X(Monk)              \
    X(Wizard)            \
    X(Paladin)           \
    X(Fighter)

typedef enum character_class {
    #define X(name) name,
    CHARACTER_CLASSES
    #undef X
} character_class_t;

typedef struct character {
    char* name;                        // jméno
    character_class_t character_class; // povolání
    unsigned char level;               // úroveň
} character_t;

const char* character_class_to_string(character_class_t class);
void print_character(character_t* character);

#endif