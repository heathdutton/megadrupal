<?php

/**
 * @file
 * Theme functions for User Disk Quota module.
 */

function theme_user_disk_quota($variables) {
  $account = $variables['account'];
  $output = t('You used %used disk space of your %quota quota.',
    array(
      '%used' => format_size(_user_disk_quota_get_user_disk_quota($account)),
      '%quota' => format_size(_user_disk_quota_get_user_max_quota($account)),
    )
  );
  return $output;  
}