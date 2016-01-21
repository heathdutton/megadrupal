<?php
/**
 * @file at_cm.rules.inc
 *
 * Rules integration.
 */

/**
 * Implements hook_rules_action_info()
 */
function at_cm_rules_action_info() {
  return array(
    'at_cm_subscribers_add' => array(
      'group' => t('CampaignMonitor'),
      'label' => t('Add a contact to a list'),
      'parameter' => array(
        'list_ids' => array(
          'type' => 'list<text>',
          'label' => t('List ID'),
          'options list' => 'at_cm_lists_options',
          'optional' => FALSE,
        ),
        'email' => array(
          'type' => 'text',
          'label' => t('Email'),
          'description' => t('Email address of contact'),
          'optional' => FALSE,
        ),
        'name' => array(
          'type' => 'text',
          'label' => t('Name'),
          'description' => t('Full name of contact'),
          'optional' => TRUE,
        ),
      ),
      'base' => 'at_cm_action_subscribers_add',
      'access callback' => 'rules_system_integration_access',
    ),
    // unsubscribe
  );
}

/**
 * Callback for action: at_cm_subscribers_add
 * @param  array $list_ids [description]
 * @param  string $mail     [description]
 * @param  string $name     [description]
 */
function at_cm_action_subscribers_add($list_ids, $mail, $name) {
    foreach ($list_ids as $list_id) {
        at_cm_list_add_contact($list_id, $mail, $name);
    }
}
