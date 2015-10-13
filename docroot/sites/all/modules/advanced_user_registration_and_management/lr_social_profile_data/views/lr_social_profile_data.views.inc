<?php

/**
 * @file
 * Provide views data and handlers for rpx_ui.module
 */

/**
 * Implements hook_views_data().
 */
function lr_social_profile_data_views_data() {
  $data = array();
  $data['users']['view_link'] = array(
    'group' => t('LoginRadius'),
    'title' => t('User Profile View Link'),
    'help' => t('This Link shows Extended Profile Data'),
    'real field' => 'uid',
    'field' => array(
      'handler' => 'lr_social_profile_data_handler_field_view_link',
    ),
  );
  return $data;
}
