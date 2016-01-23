<?php

/**
 * @file
 * The statistics administration page for the Campaign Monitor module.
 */

/**
 * Generates the statistics overview.
 *
 * @return string
 * The rendered HTML.
 */
function campaignmonitor_admin_settings_stats() {
  $account = variable_get('campaignmonitor_account', FALSE);
  if (!$account) {
    drupal_set_message(t('You have not entered your account information yet, hence statistics from Campaign Monitor can not be downloaded.'), 'error');
    return '';
  }

  // Load list information.
  $cm = CampaignMonitor::getConnector();
  $lists = $cm->getLists();
  $error = $cm->getLatestError();
  if ($error['code'] != 1) {
    drupal_set_message($error['message'], 'error');
  }

  // Loop over the lists and display stats for each list.
  $output = '';
  foreach ($lists as $id => $list) {
    if (campaignmonitor_is_list_enabled($id)) {
      // List was enabled so get the stats.
      $output .= theme('campaignmonitor_list_stats', array('list' => $list, 'stats' => $cm->getListStats($id)));
    }
  }

  drupal_add_css(drupal_get_path('module', 'campaignmonitor') . '/css/campaignmonitor.admin.css');
  return $output;
}
