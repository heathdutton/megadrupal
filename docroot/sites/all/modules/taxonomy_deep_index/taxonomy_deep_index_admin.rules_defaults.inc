<?php
/**
 * @file
 * Default rules implementation for taxonomy deep index.
 */

/**
 * Implements hook_default_rules_configuration().
 */
function taxonomy_deep_index_admin_default_rules_configuration() {
  $action_set = rules_action_set(array(
    'node' => array(
      'type' => 'node',
      'label' => 'Content',
    ),
  ));
  $action_set->label = 'Rebuild taxonomy deep index';
  $action_set->action('taxonomy_deep_index_rebuild', array('node:select' => 'node'));
  $configs['taxonomy_deep_index_rebuild'] = $action_set;

  return $configs;
}
