<?php

/**
 * @file
 * Rules integration for the UC CCK Access module.
 */

/**
 * Implementation of hook_rules_event_info().
 */
function uc_cck_access_rules_event_info() {
  return array(
  );
}


/**
 * Implements hook_rules_action_info().
 */
function uc_cck_access_rules_action_info() {
  return array(
    'uc_cck_access_delay_access' => array(
      'label' => t('Grant CCK access to customer (delay & expiration schedule defined by product feature)'),
      'group' => t('UC CCK access'),
      'base' => 'uc_cck_access_delay_access',
      'parameter' => array(
        'order' => array('type' => 'uc_order', 'label' => t('Order')),
      ),
    ),

    'uc_cck_access_grant_access' => array(
      'label' => t('Grant CCK access to customer immediately (overriding delay but keeping expiration schedule defined by product feature)'),
      'group' => t('UC CCK access'),
      'base' => 'uc_cck_access_grant_access',
      'parameter' => array(
        'order' => array('type' => 'uc_order', 'label' => t('Order')),
      ),
    ),
  );
}

