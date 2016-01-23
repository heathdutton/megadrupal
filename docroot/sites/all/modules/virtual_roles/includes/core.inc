<?php
/**
 * @file
 * Contains the hook_virtual_roles() functions for core modules.
 */

/**
 * Implements hook_virtual_roles().
 */
function node_virtual_roles() {
  return array(
    'node_user_is_author' => array(
      'title' => t("User is node's author"),
      'callback' => 'node_user_is_author',
      'file' => 'node_user_is_author.inc',
      'path' => drupal_get_path('module', 'virtual_roles') . '/modules/node',
    ),
  );
}

