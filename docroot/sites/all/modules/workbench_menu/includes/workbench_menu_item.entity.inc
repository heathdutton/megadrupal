<?php

/**
 * @file
 * Workbench menu item entity classes.
 */

/**
 * Controller class for workbench menu items.
 */
class WorkbenchMenuItemController extends DrupalDefaultEntityController {

  function attachLoad(&$queried_entities, $revision_id = FALSE) {
    $item_ids = array_keys($queried_entities);
    $items = (array) workbench_menu_get_menu_items(array('id' => $item_ids, 'fast' => NULL), 'id');

    foreach ($items as &$item) {
      $item->properties = unserialize($item->properties);
    }

    $queried_entities = &$items;

    parent::attachLoad($queried_entities, $revision_id);
  }
}
