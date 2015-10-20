<?php
/**
 * @file
 * User Disk Quota Views integration file.
 */

/**
 * Implements hook_views_data().
 */
function user_disk_quota_views_data() {
  $data['user_disk_quota_user']['table']['group'] = t('User');
  $data['user_disk_quota_user']['table']['join'] = array(
     // Directly links to users table.
    'users' => array(
      'left_field' => 'uid',
      'field' => 'uid',
    ),
  );
  $data['user_disk_quota_user']['quota_limit'] = array(
    'title' => t('User disk quota limit'),
    'help' => t('User disk quota limit'),
    'field' => array(
      'handler' => 'views_handler_field_user_disk_quota',
      'click sortable' => TRUE,
    ),
  );
  return $data;
}