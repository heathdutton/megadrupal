<?php

require_once 'gplus_sync.constants.inc';

/**
 * gplus_sync_user_page().
 */
function gplus_sync_user_page($account) {
  $return['gplus'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('profile', 'gplus-profile')),
  );
  $profile_settings = variable_get('field_bundle_settings_user__user');
  $gplus_fields = module_invoke('gplus_sync', 'gplus_fields');
  foreach ($gplus_fields as $field_id => $field_info) {
    if ($field_info['type'] != 'hidden') {
   // theme each field
      switch ($field_info['type']) {
        case 'url_to_image' : $c_markup = theme('image', array('path' => $account->{$field_id}, 'width' => 100)); break;
        case 'url'          : $c_markup = l($account->{$field_id}, $account->{$field_id}, array('attributes' => array('target' => '_blank'))); break;
        case 'date'         : $c_markup = $account->{$field_id} ? format_date($account->{$field_id}, 'medium', 'Y-m-d') : NULL; break;
        case 'text'         : $c_markup = $account->{$field_id}; break;
      }
   // get field weight and visibility
      $c_is_visible = TRUE;
      $c_weight = 0;
      if (isset($profile_settings['extra_fields']['display'][$field_id]['default'])) {
        $c_is_visible = $profile_settings['extra_fields']['display'][$field_id]['default']['visible'];
        $c_weight = $profile_settings['extra_fields']['display'][$field_id]['default']['weight'];
      }
   // collect all data
      $return['gplus'][$field_id] = array(
        '#theme'  => 'user_profile_item',
        '#title'  => $field_info['title'],
        '#markup' => $c_markup ? $c_markup : GPLUS_SYNC_HTML_NA,
        '#access' => $c_is_visible,
        '#weight' => $c_weight,
      );
    }
  }
  return $return;
}

