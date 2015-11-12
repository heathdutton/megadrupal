<?php

/**
 * @file
 * Workbench menu entity classes.
 */

/**
 * Controller class for workbench menus.
 */
class WorkbenchMenuController extends DrupalDefaultEntityController {

  function attachLoad(&$queried_entities, $revision_id = FALSE) {
    $menu_ids = array_keys($queried_entities);
    $menus = (array) workbench_menu_get_menus(array('id' => $menu_ids, 'fast' => NULL), 'id');
    $queried_entities = &$menus;

    parent::attachLoad($queried_entities, $revision_id);
  }
}
