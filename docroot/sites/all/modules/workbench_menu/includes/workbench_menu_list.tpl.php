<?php
/**
 * @file
 * Default theme implementation to display a single workbench menu list.
 *
 * Available Variables:
 * - $workbench_menu: an array with the following keys:
 *   - id: numeric id of the menu.
 *   - even: a boolean representing that this menu is even.
 *   - count: the menu number in regards to the total menus on the screen.
 *   - settings: an array of settings associated with the menu.
 * 'child': a boolean representing whether or not this is a child of an
 *  another list.
 *
 * Content Variables:
 * - 'list': an array with the following keys:
 *   - 'items': An array of items to be displayed in the list. If an item is a
 *      string, then it is used as is. If an item is an array, then the "data"
 *      element of the array is used as the contents of the list item. If an
 *      item is an array with a "children" element, those children are
 *      displayed in a nested list. All other elements are treated as
 *      attributes of the list item element.
 *   - 'title': The title of the list.
 *   - 'attributes': The attributes applied to the list element.
 */
?>
<ul <?php print(drupal_attributes($list['attributes'])); ?>>
  <?php foreach ((array) $list['items'] as $item) { print($item . "\n"); } ?>
</ul>
