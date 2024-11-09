/*
 * Tabulka s rozptýlenými položkami
 *
 * S využitím datových typů ze souboru hashtable.h a připravených koster
 * funkcí implementujte tabulku s rozptýlenými položkami s explicitně
 * zretězenými synonymy.
 *
 * Při implementaci uvažujte velikost tabulky HT_SIZE.
 */

#include "hashtable.h"
#include <stdlib.h>
#include <string.h>

int HT_SIZE = MAX_HT_SIZE;

/*
 * Rozptylovací funkce která přidělí zadanému klíči index z intervalu
 * <0,HT_SIZE-1>. Ideální rozptylovací funkce by měla rozprostírat klíče
 * rovnoměrně po všech indexech. Zamyslete sa nad kvalitou zvolené funkce.
 */
int get_hash(char *key) {
  int result = 1;
  int length = strlen(key);
  for (int i = 0; i < length; i++) {
    result += key[i];
  }
  return (result % HT_SIZE);
}

/*
 * Inicializace tabulky — zavolá sa před prvním použitím tabulky.
 */
void ht_init(ht_table_t *table) {
  for(int i = 0; i < HT_SIZE; i++){
    (*table)[i]= NULL;
  }
}

/*
 * Vyhledání prvku v tabulce.
 *
 * V případě úspěchu vrací ukazatel na nalezený prvek; v opačném případě vrací
 * hodnotu NULL.
 */
ht_item_t *ht_search(ht_table_t *table, char *key) {
  // Získáme index pomocí hashovací funkce
  int index = get_hash(key);
  ht_item_t *item = (*table)[index];

  // Procházíme prvky na daném indexu (spojený seznam v případě kolizí)
  while (item != NULL) {
    if (strcmp(item->key, key) == 0) {
      // Pokud najdeme odpovídající klíč, vrátíme ukazatel na tento prvek
      return item;
    }
    item = item->next;  // Pokračujeme na další prvek seznamu
  }
  return NULL;
}

/*
 * Vložení nového prvku do tabulky.
 *
 * Pokud prvek s daným klíčem už v tabulce existuje, nahraďte jeho hodnotu.
 *
 * Při implementaci využijte funkci ht_search. Pri vkládání prvku do seznamu
 * synonym zvolte nejefektivnější možnost a vložte prvek na začátek seznamu.
 */
void ht_insert(ht_table_t *table, char *key, float value) {
  // Nejprve zjistíme, zda prvek s tímto klíčem již existuje
  ht_item_t *item = ht_search(table, key);

  if (item != NULL) {
    // Pokud prvek s daným klíčem existuje, aktualizujeme jeho hodnotu
    item->value = value;
  } else {
    // Pokud prvek neexistuje, vytvoříme nový a vložíme jej do tabulky

    // Získáme index podle hashovací funkce
    int index = get_hash(key);

    // Vytvoříme nový prvek
    ht_item_t *new_item = (ht_item_t *)malloc(sizeof(ht_item_t));
    if (new_item == NULL) {
      return;  // Ošetření chyby při alokaci paměti
    }

    // Alokujeme paměť pro klíč a zkopírujeme řetězec ručně
    new_item->key = (char *)malloc(strlen(key) + 1);  // +1 pro nulový terminátor
    if (new_item->key == NULL) {
      free(new_item);  // Ošetření chyby při alokaci
      return;
    }
    strcpy(new_item->key, key);  // Zkopírujeme klíč do nově alokované paměti

    // Nastavíme hodnotu
    new_item->value = value;
    
    // Vložíme nový prvek na začátek seznamu synonym
    new_item->next = (*table)[index];  // Nastavíme nový prvek jako první v seznamu
    (*table)[index] = new_item;        // Tabulka nyní ukazuje na nový prvek
  }
}

/*
 * Získání hodnoty z tabulky.
 *
 * V případě úspěchu vrací funkce ukazatel na hodnotu prvku, v opačném
 * případě hodnotu NULL.
 *
 * Při implementaci využijte funkci ht_search.
 */
float *ht_get(ht_table_t *table, char *key) {
  // Vyhledáme prvek pomocí funkce ht_search
  ht_item_t *item = ht_search(table, key);

  // Pokud je prvek nalezen, vrátíme ukazatel na jeho hodnotu
  if (item != NULL) {
    return &(item->value);  // Vrátime ukazatel na hodnotu prvku
  }
  return NULL;
}

/*
 * Smazání prvku z tabulky.
 *
 * Funkce korektně uvolní všechny alokované zdroje přiřazené k danému prvku.
 * Pokud prvek neexistuje, funkce nedělá nic.
 *
 * Při implementaci NEPOUŽÍVEJTE funkci ht_search.
 */
void ht_delete(ht_table_t *table, char *key) {
  // Získáme index pomocí hashovací funkce
  int index = get_hash(key);
  ht_item_t *item = (*table)[index];
  ht_item_t *prev = NULL;

  // Procházíme seznamem na daném indexu
  while (item != NULL) {
    // Pokud najdeme prvek s odpovídajícím klíčem
    if (strcmp(item->key, key) == 0) {
      // Pokud je prvek první v seznamu (prev == NULL)
      if (prev == NULL) {
        // Nastavíme začátek seznamu na další prvek
        (*table)[index] = item->next;
      } 
      else {
        // Propojíme předchozí prvek s dalším, čímž přeskočíme prvek ke smazání
        prev->next = item->next;
      }

      // Uvolníme alokovanou paměť pro klíč a samotný prvek
      free(item->key);
      free(item);
      return;  // Po smazání ukončíme funkci
  }
  // Posuneme se na další prvek v seznamu
  prev = item;
  item = item->next;
  }
}

/*
 * Smazání všech prvků z tabulky.
 *
 * Funkce korektně uvolní všechny alokované zdroje a uvede tabulku do stavu po 
 * inicializaci.
 */
void ht_delete_all(ht_table_t *table) {
  // Projdeme všechny indexy v tabulce
  for (int i = 0; i < HT_SIZE; i++) {
    ht_item_t *item = (*table)[i];

    // Procházíme spojený seznam na každém indexu a uvolňujeme paměť
    while (item != NULL) {
      ht_item_t *next_item = item->next;  // Uložíme si ukazatel na další prvek
      free(item->key);  // Uvolníme paměť pro klíč
      free(item);       // Uvolníme samotný prvek
      item = next_item; // Posuneme se na další prvek v seznamu
    }

    // Po smazání všech prvků na daném indexu nastavíme tento index na NULL
    (*table)[i] = NULL;
  }
}
