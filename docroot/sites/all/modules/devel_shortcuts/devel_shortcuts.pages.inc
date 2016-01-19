<?php

/**
 * @file
 * This module holds the actual functionality for functions
 * for Development Shortcuts
 */

/**
 * Menu callback; goes to the link to edit the current content type.
 */
function devel_shortcuts_edit_entity_type($type, $object, $name = NULL) {
  switch ($type) {
    case 'node':
      $node_type = $object->type;
      drupal_goto('admin/structure/types/manage/' . $node_type . '/fields');
      break;

    case 'user':
      drupal_goto('admin/config/people/accounts/fields');
      break;

    case 'taxonomy_term':
      $vocabulary_machine_name = $object->vocabulary_machine_name;
      drupal_goto('admin/structure/taxonomy/' . $vocabulary_machine_name . '/fields');
      break;

  }
}
