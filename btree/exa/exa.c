/*
 * Použití binárních vyhledávacích stromů.
 *
 * S využitím Vámi implementovaného binárního vyhledávacího stromu (soubory ../iter/btree.c a ../rec/btree.c)
 * implementujte triviální funkci letter_count. Všimněte si, že výstupní strom může být značně degradovaný 
 * (až na úroveň lineárního seznamu). Jako typ hodnoty v uzlu stromu využijte 'INTEGER'.
 * 
 */

#include "../btree.h"
#include <stdio.h>
#include <stdlib.h>

#include <ctype.h>  // Pro funkci tolower

/**
 * Vypočítání frekvence výskytů znaků ve vstupním řetězci.
 * 
 * Funkce inicilializuje strom a následně zjistí počet výskytů znaků a-z (case insensitive), znaku 
 * mezery ' ', a ostatních znaků (ve stromu reprezentováno znakem podtržítka '_'). Výstup je v 
 * uložen ve stromu.
 * 
 * Například pro vstupní řetězec: "abBccc_ 123 *" bude strom po běhu funkce obsahovat:
 * 
 * key | value
 * 'a'     1
 * 'b'     2
 * 'c'     3
 * ' '     2
 * '_'     5
 * 
 * Pro implementaci si můžete v tomto souboru nadefinovat vlastní pomocné funkce.
*/

void bst_insert_or_update(bst_node_t **tree, char key) {
    bst_node_content_t value;
    value.type = INTEGER;  // Typ hodnoty je INTEGER
    value.value = malloc(sizeof(int));  // Alokace paměti pro hodnotu
    *((int*)value.value) = 1;  // Inicializujeme hodnotu na 1

    // Hledáme uzel ve stromu
    bst_node_t *current = *tree;
    bst_node_t *parent = NULL;
    
    while (current != NULL) {
        parent = current;
        if (key < current->key) {
            current = current->left;
        } else if (key > current->key) {
            current = current->right;
        } else {
            // Nalezen uzel se stejným klíčem, inkrementujeme jeho hodnotu
            *((int*)current->content.value) += 1;
            free(value.value);  // Uvolníme alokovanou paměť pro pomocnou hodnotu
            return;
        }
    }

    // Pokud uzel nebyl nalezen, vložíme nový uzel
    bst_node_t *new_node = (bst_node_t*)malloc(sizeof(bst_node_t));
    new_node->key = key;
    new_node->content = value;
    new_node->left = new_node->right = NULL;

    // Připojíme nový uzel k rodiči
    if (parent == NULL) {
        *tree = new_node;  // Strom byl prázdný
    } else if (key < parent->key) {
        parent->left = new_node;
    } else {
        parent->right = new_node;
    }
}

void letter_count(bst_node_t **tree, char *input) {
    // Inicializace stromu
    *tree = NULL;

    // Procházení každého znaku ve vstupním řetězci
    for (int i = 0; input[i] != '\0'; i++) {
        char c = input[i];
        
        // Převedení na malé písmeno pro case insensitive počítání
        if (isalpha(c)) {
            c = tolower(c);  // Malé písmeno
        } else if (c == ' ') {
            // Mezery zůstávají beze změny
            c = ' ';
        } else {
            // Ostatní znaky (např. čísla nebo speciální znaky) reprezentujeme '_'
            c = '_';
        }

        // Vložíme nebo aktualizujeme uzel ve stromu
        bst_insert_or_update(tree, c);
    }
}