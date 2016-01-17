<?php
/**
 * @file
 * TMGMT Server rules integration.
 */

/**
 * Implements hook_rules_event_info().
 */
function tmgmt_server_rules_event_info() {
  $items = array(
    'tmgmt_server_client_created_via_service' => array(
      'label' => t('Client created via services'),
      'group' => t('TMGMT Server'),
      'module' => 'tmgmt_server',
      'access callback' => 'tmgmt_server_rules_access',
      'variables' => array(
        'account' => array(
          'type' => 'user',
          'label' => t('Client owner account'),
        ),
        'client' => array(
          'type' => 'tmgmt_server_client',
          'label' => t('Remote client'),
        ),
      ),
    ),
    'tmgmt_server_new_job_request' => array(
      'label' => t('New job request'),
      'group' => 'TMGMT Server',
      'module' => 'tmgmt_server',
      'variables' => array(
        'tmgmt_job' => array(
          'type' => 'tmgmt_job',
          'label' => t('TMGMT Job'),
        ),
        'tmgmt_server_client' => array(
          'type' => 'tmgmt_server_client',
          'label' => t('TMGMT Client'),
        ),
      ),
    ),
  );

  return $items;
}

/**
 * @param $type
 * @param $name
 * @return bool
 */
function tmgmt_server_rules_access($type, $name) {
  return TRUE;
}
