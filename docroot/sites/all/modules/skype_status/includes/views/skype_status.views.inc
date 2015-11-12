<?php

/**
 * @file
 * Views integration for Skype Status..
 */

/**
 * Implements hook_views_data_alter().
 */
function skype_status_views_data_alter(&$data) {
  $settings = skype_status_get_profile_settings();
  $field = $settings['field'];
  if (!empty($field)) {
    if (isset($data['field_data_' . $field])) {
      $data['field_data_' . $field][$field]['field']['handler']= 'skype_status_handler_field_field';
    }
    elseif (module_exists('profile') && isset($data['profile_value_' . $field])) {
      $data['profile_value_' . $field]['value']['field']['handler']= 'skype_status_handler_field_user';
    }
  }
}