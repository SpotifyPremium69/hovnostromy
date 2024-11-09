/*
 * Binární vyhledávací strom — iterativní varianta
 *
 * S využitím datových typů ze souboru btree.h, zásobníku ze souboru stack.h
 * a připravených koster funkcí implementujte binární vyhledávací
 * strom bez použití rekurze.
 */

#include "../btree.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Inicializace stromu.
 *
 * Uživatel musí zajistit, že inicializace se nebude opakovaně volat nad
 * inicializovaným stromem. V opačném případě může dojít k úniku paměti (memory
 * leak). Protože neinicializovaný ukazatel má nedefinovanou hodnotu, není
 * možné toto detekovat ve funkci.
 */
void bst_init(bst_node_t **tree)
{
  *tree = NULL; // Inicializuje kořen stromu na NULL
}

/*
 * Vyhledání uzlu v stromu.
 *
 * V případě úspěchu vrátí funkce hodnotu true a do proměnné value zapíše
 * ukazatel na obsah daného uzlu. V opačném případě funkce vrátí hodnotu false a proměnná
 * value zůstává nezměněná.
 *
 * Funkci implementujte iterativně bez použité vlastních pomocných funkcí.
 */
bool bst_search(bst_node_t *tree, char key, bst_node_content_t **value)
{
  bst_node_t *current = tree;  // Nastavíme ukazatel na kořen

  // Procházíme stromem, dokud nenalezneme uzel nebo nedojdeme na konec
  while (current != NULL) {
      if (key == current->key) {  // Pokud se klíče shodují
          *value = &current->content;  // Nastavíme ukazatel na obsah nalezeného uzlu
          return true;  // Vracíme true při úspěšném nalezení
      } 
      else if (key < current->key) {  // Pokud hledaný klíč je menší
          current = current->left;  // Jdeme do levého podstromu
      } 
      else {  // Pokud hledaný klíč je větší
          current = current->right;  // Jdeme do pravého podstromu
      }
  }

  // Pokud nebyl uzel nalezen, vrátíme false
  return false;
}

/*
 * Vložení uzlu do stromu.
 *
 * Pokud uzel se zadaným klíče už ve stromu existuje, nahraďte jeho hodnotu.
 * Jinak vložte nový listový uzel.
 *
 * Výsledný strom musí splňovat podmínku vyhledávacího stromu — levý podstrom
 * uzlu obsahuje jenom menší klíče, pravý větší.
 *
 * Funkci implementujte iterativně bez použití vlastních pomocných funkcí.
 */
void bst_insert(bst_node_t **tree, char key, bst_node_content_t value)
{
  bst_node_t *current = *tree;
  bst_node_t *parent = NULL;

  // Hledáme správné místo pro nový uzel
  while (current != NULL) {
      parent = current;
      if (key == current->key) {
          // Pokud uzel existuje, uvolníme původní obsah a přepíšeme hodnotu
          if (current->content.value != NULL) {
              free(current->content.value);  // Uvolnění původního obsahu
          }
          current->content = value;  // Přiřazení nové hodnoty
          return;
      } else if (key < current->key) {
          current = current->left;
      } else {
          current = current->right;
      }
  }

  // Vytvoření nového uzlu
  bst_node_t *new_node = (bst_node_t *)malloc(sizeof(bst_node_t));
  if (new_node == NULL) {
      // Pokud alokace selže, ukončíme funkci
      return;
  }

  new_node->key = key;
  new_node->content = value;
  new_node->left = NULL;
  new_node->right = NULL;

  if (parent == NULL) {
      *tree = new_node;  // Strom je prázdný, nový uzel se stává kořenem
  } else if (key < parent->key) {
      parent->left = new_node;
  } else {
      parent->right = new_node;
  }
}

/*
 * Pomocná funkce která nahradí uzel nejpravějším potomkem.
 *
 * Klíč a hodnota uzlu target budou nahrazené klíčem a hodnotou nejpravějšího
 * uzlu podstromu tree. Nejpravější potomek bude odstraněný. Funkce korektně
 * uvolní všechny alokované zdroje odstraněného uzlu.
 *
 * Funkce předpokládá, že hodnota tree není NULL.
 *
 * Tato pomocná funkce bude využita při implementaci funkce bst_delete.
 *
 * Funkci implementujte iterativně bez použití vlastních pomocných funkcí.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree)
{
  bst_node_t *parent = NULL;
  bst_node_t *current = *tree;

  // Hledání nejpravějšího uzlu
  while (current->right != NULL) {
      parent = current;
      current = current->right;
  }

  // Přenesení hodnot z nejpravějšího uzlu
  target->key = current->key;
  target->content = current->content;

  // Pokud má nejpravější uzel levého potomka, nahradíme ho
  if (parent != NULL) {
      parent->right = current->left;
  } else {
      *tree = current->left;
  }

  // Uvolnění paměti pro nejpravější uzel
  free(current);
}

/*
 * Odstranění uzlu ze stromu.
 *
 * Pokud uzel se zadaným klíčem neexistuje, funkce nic nedělá.
 * Pokud má odstraněný uzel jeden podstrom, zdědí ho rodič odstraněného uzlu.
 * Pokud má odstraněný uzel oba podstromy, je nahrazený nejpravějším uzlem
 * levého podstromu. Nejpravější uzel nemusí být listem.
 *
 * Funkce korektně uvolní všechny alokované zdroje odstraněného uzlu.
 *
 * Funkci implementujte iterativně pomocí bst_replace_by_rightmost a bez
 * použití vlastních pomocných funkcí.
 */
void bst_delete(bst_node_t **tree, char key)
{
  bst_node_t *current = *tree;
  bst_node_t *parent = NULL;

  // Find the node to delete
  while (current != NULL && current->key != key) {
      parent = current;
      if (key < current->key) {
          current = current->left;
      } else {
          current = current->right;
      }
  }

  if (current == NULL) {
      return;  // Node not found
  }

  // Free the content of the node before freeing the node itself
  if (current->content.value != NULL) {
      free(current->content.value);
  }

  // If the node has both children, replace it with the rightmost child
  if (current->left != NULL && current->right != NULL) {
      bst_replace_by_rightmost(current, &current->left);
  } else {
      bst_node_t *child = (current->left != NULL) ? current->left : current->right;

      if (parent == NULL) {
          *tree = child;  // If the node to delete is the root
      } else if (parent->left == current) {
          parent->left = child;
      } else {
          parent->right = child;
      }

      // Free the node itself
      free(current);
  }
}

/*
 * Zrušení celého stromu.
 *
 * Po zrušení se celý strom bude nacházet ve stejném stavu jako po
 * inicializaci. Funkce korektně uvolní všechny alokované zdroje rušených
 * uzlů.
 *
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití
 * vlastních pomocných funkcí.
 */
void bst_dispose(bst_node_t **tree)
{
  if (tree == NULL || *tree == NULL) {
        return;
    }

    stack_bst_t to_visit;
    stack_bst_init(&to_visit);

    // Začneme s kořenem
    stack_bst_push(&to_visit, *tree);

    while (!stack_bst_empty(&to_visit)) {
        bst_node_t *current = stack_bst_pop(&to_visit);

        // Přidejte levý a pravý podstrom do zásobníku
        if (current->left != NULL) {
            stack_bst_push(&to_visit, current->left);
        }
        if (current->right != NULL) {
            stack_bst_push(&to_visit, current->right);
        }

        // Uvolněte uzel
        if (current->content.value != NULL) {
            free(current->content.value);
        }
        free(current);
    }

    // Po zrušení stromu nastavíme kořen na NULL
    *tree = NULL;
}

/*
 * Pomocná funkce pro iterativní preorder.
 *
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu.
 * Nad zpracovanými uzly zavolá bst_add_node_to_items a uloží je do zásobníku uzlů.
 *
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití
 * vlastních pomocných funkcí.
 */
void bst_leftmost_preorder(bst_node_t *tree, stack_bst_t *to_visit, bst_items_t *items)
{
  // Procházíme všechny uzly po levé straně, dokud nenarazíme na NULL
  while (tree != NULL) {
      // Přidáme aktuální uzel do seznamu items pomocí bst_add_node_to_items
      bst_add_node_to_items(tree, items);

      // Uložíme aktuální uzel do zásobníku pro pozdější zpracování
      stack_bst_push(to_visit, tree);

      // Pokračujeme nalevo k dalšímu uzlu
      tree = tree->left;
  }
}

/*
 * Preorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_preorder a
 * zásobníku uzlů a bez použití vlastních pomocných funkcí.
 */
void bst_preorder(bst_node_t *tree, bst_items_t *items)
{
  // Inicializace zásobníku pro uchování uzlů během iterativního průchodu
  stack_bst_t to_visit;
  stack_bst_init(&to_visit);

  // Provede první průchod po levé větvi stromu a přidá uzly do seznamu a zásobníku
  bst_leftmost_preorder(tree, &to_visit, items);

  // Hlavní smyčka pro zpracování uzlů ze zásobníku
  while (!stack_bst_empty(&to_visit)) {
      // Vezmeme uzel z vrcholu zásobníku
      tree = stack_bst_pop(&to_visit);

      // Zpracujeme pravého potomka, pokud existuje, voláním bst_leftmost_preorder
      if (tree->right != NULL) {
          bst_leftmost_preorder(tree->right, &to_visit, items);
      }
  }
}

/*
 * Pomocná funkce pro iterativní inorder.
 *
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu a ukládá uzly do
 * zásobníku uzlů.
 *
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití
 * vlastních pomocných funkcí.
 */
void bst_leftmost_inorder(bst_node_t *tree, stack_bst_t *to_visit)
{
  // Pokračujeme po levé větvi, dokud neexistuje levý potomek
  while (tree != NULL) {
      // Přidáme aktuální uzel do zásobníku pro pozdější zpracování
      stack_bst_push(to_visit, tree);

      // Posuneme se na levého potomka
      tree = tree->left;
  }
}

/*
 * Inorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_inorder a
 * zásobníku uzlů a bez použití vlastních pomocných funkcí.
 */
void bst_inorder(bst_node_t *tree, bst_items_t *items)
{
  stack_bst_t to_visit;
  stack_bst_init(&to_visit);

  // Použijeme bst_leftmost_inorder k naplnění zásobníku uzly z levého podstromu
  bst_leftmost_inorder(tree, &to_visit);

  // Projdeme zásobník, dokud není prázdný
  while (!stack_bst_empty(&to_visit)) {
      // Vezmeme nejvyšší uzel ze zásobníku
      bst_node_t *current = stack_bst_pop(&to_visit);

      // Přidáme aktuální uzel do výsledného pole items
      bst_add_node_to_items(current, items);

      // Pokud má pravý podstrom, projdeme jeho levou větev
      if (current->right != NULL) {
          bst_leftmost_inorder(current->right, &to_visit);
      }
  }
}

/*
 * Pomocná funkce pro iterativní postorder.
 *
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu a ukládá uzly do
 * zásobníku uzlů. Do zásobníku bool hodnot ukládá informaci, že uzel
 * byl navštíven poprvé.
 *
 * Funkci implementujte iterativně pomocí zásobníku uzlů a bool hodnot a bez použití
 * vlastních pomocných funkcí.
 */
void bst_leftmost_postorder(bst_node_t *tree, stack_bst_t *to_visit,
                            stack_bool_t *first_visit)
{
  while (tree != NULL) {
      // Push the current node onto the stack
      stack_bst_push(to_visit, tree);
      stack_bool_push(first_visit, true);  // First visit, will visit the right subtree later
      tree = tree->left;  // Move to the left child
  }
}

/*
 * Postorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_postorder a
 * zásobníku uzlů a bool hodnot a bez použití vlastních pomocných funkcí.
 */
void bst_postorder(bst_node_t *tree, bst_items_t *items)
{
  // Initialize stacks for nodes and first visit status
  stack_bst_t to_visit;
  stack_bool_t first_visit;

  stack_bst_init(&to_visit);
  stack_bool_init(&first_visit);

  bst_leftmost_postorder(tree, &to_visit, &first_visit);

  while (!stack_bst_empty(&to_visit)) {
      bst_node_t *current = stack_bst_top(&to_visit);  // Get the top node from the stack

      if (stack_bool_top(&first_visit)) {
          // First time we visit the node, push its right child if it exists
          stack_bool_pop(&first_visit);
          stack_bool_push(&first_visit, false);  // Mark as second visit

          // Push right child onto the stack if exists
          if (current->right != NULL) {
              bst_leftmost_postorder(current->right, &to_visit, &first_visit);
          }
      } else {
          // Second visit, process the node and pop from the stack
          bst_add_node_to_items(current, items);
          stack_bst_pop(&to_visit);
          stack_bool_pop(&first_visit);
      }
  }
}
