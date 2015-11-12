<?php
/**
 * @file
 * Defines the new content type related code.
 */

/**
 * Implements hook_node_info().
 */
function mapplic_node_info() {
  return array(
    'mapplic_landmark' => array(
      'name' => t('Mapplic landmark'),
      'base' => 'mapplic_landmark',
      'description' => t('You can define new Mapplic landmarks here'),
      'has_title' => TRUE,
      'title_label' => t('Landmark name'),
    ),
  );
}
