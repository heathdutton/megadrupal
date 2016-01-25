<?php

/**
 * @file
 * Implementation of doublerecall hooks for core modules.
 */

/**
 * Implements hook_doublerecall_should_hide().
 */
function node_doublerecall_should_hide() {
  $object = menu_get_object();
  if (!empty($object)) {
    $types = variable_get('doublerecall_hide_node', array());
    if (!empty($types[$object->type])) {
      return TRUE;
    }
  }
}