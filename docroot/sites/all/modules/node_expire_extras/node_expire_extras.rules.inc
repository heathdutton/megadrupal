<?php

/**
 * @file
 * Rules module integration.
 */

use NodeExpireExtras as Ns;

/**
 * Implements hook_rules_action_info().
 *
 * @ingroup rules
 */
function node_expire_extras_rules_action_info() {
  return array(
    'node_expire_extras_update_expire' => array(
      'arguments' => array(
        'node' => array(
          'type' => 'node',
          'label' => t('content expired'),
        ),
      ),
      'label' => t('Update expiry date'),
      'module' => 'node_expire_extras',
    ),
  );
}

/**
 * Updates expire on nodes.
 *
 * @param object $node
 *   The Node object with all node IDs
 *   that should update expire.
 */
function node_expire_extras_update_expire($node) {
  // $expire = date(NODE_EXPIRE_FORMAT, strtotime("+1 year", REQUEST_TIME));
  $wrapper = entity_metadata_wrapper('node', $node);
  $value = $wrapper->value();
  $value->expire
    = Ns\NodeExpireDateHelper::extendExpireDate($node->type, $value->expire);
  // Fixes some cases, where $value->is_new is TRUE. This is always an update.
  $value->is_new = FALSE;
  $wrapper->set($value);
  $wrapper->save();
}

module_load_include('inc', 'node_expire_extras', 'includes/node_expire_extras');
