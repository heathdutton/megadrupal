<?php

/**
 * @file
 * rules integration for the taxonomy deep index module
 *
 * @addtogroup rules
 * @{
 */

/**
 * Implements hook_rules_action_info().
 */
function taxonomy_deep_index_rules_action_info() {
  $items['taxonomy_deep_index_rebuild'] = array(
    'label' => 'Rebuild taxonomy index',
    'parameter' => array(
      'node' => array(
        'type' => 'node',
        'label' => t('Content'),
      ),
    ),
    'access callback' => 'rules_node_admin_access',
    'base' => 'taxonomy_deep_index_rebuild_action',
    'group' => t('Node'),
  );

  return $items;
}

/**
 * Rebuild taxonomy term index (including embedded term references).
 */
function taxonomy_deep_index_rebuild_action($node) {
  taxonomy_delete_node_index($node);
  taxonomy_deep_index_build_node_index($node);
}

/**
 * @}
 */
