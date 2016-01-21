<?php

/**
 * @file
 * Comstack PM UI extra views stuff.
 */

/**
 * Implements hook_views_data_alter().
 */
function comstack_pm_ui_views_data_alter(&$data) {
  $data['users']['comstack_pm_ui_link_message'] = array(
    'real field' => 'uid',
    'field' => array(
      'title' => t('Comstack Private Messaging send message link'),
      'help' => t('Generate a link to the private messaging app new message form, for the current user row.'),
      'handler' => 'comstack_pm_ui_handler_field_users_link_message',
    ),
    'group' => t('User relationships'),
  );
}
