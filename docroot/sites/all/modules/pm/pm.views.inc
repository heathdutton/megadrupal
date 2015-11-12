<?php

/**
 * @file
 * Views definitions for pm module.
 */

/**
 * Implements hook_views_data_alter().
 */
function pm_views_data_alter(&$data) {
  $data['node']['pm_handler_field_operation'] = array(
    'title' => t('PM Operations'),
    'help' => t('Exposes operations that could be performed on the given node.'),
    'field' => array(
      'handler' => 'pm_handler_field_operation',
      'group' => t('Content'),
    ),
  );
}
