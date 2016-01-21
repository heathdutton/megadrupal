<?php
/**
 * @file
 * Integration with the Rules module.
 */

/**
 * Implements hook_rules_action_info().
 */
function features_overridden_notify_rules_event_info() {
  $items = array(
    'features_overridden_notify_features_overridden' => array(
      'label' => t('Features are overridden'),
      'group' => t('Features'),
    ),
  );
  return $items;
}
