<?php

/**
 * @file hosting_stats.summary.inc
 * Provide summary report via email
 */

/**
 * Summarize data in text-only format for emailing.
 */
function hosting_stats_summarize() {
  // Get php ini.
  $ini = ini_get_all();
  $initable = array();
  foreach ($ini as $key => $value) {
    array_push($initable, sprintf("%s: %s, %s", $key, $value['global_value'], $value['local_value']));
  }

  $string = t('Hosting Diagnostics (http://www.drupal.org/project/hosting_stats) report for @site (@url)', array('@site' => variable_get('site_name', ''), '@url' => $_SERVER['HTTP_HOST']));
  $string .= "\n=======================================\n";
  $string .= "Environment:\n";
  $string .= hosting_stats_formatmail('hosting_stats_env');
  $string .= "\n=======================================\n";
  $string .= "Performance:\n";
  $string .= hosting_stats_formatmail('hosting_stats_perf');
  $string .= "\n=======================================\n";
  $string .= "MySQL:\n";
  $string .= hosting_stats_formatmail('hosting_stats_db');
  $string .= "\n=======================================\n";
  $string .= "Filesystem:\n";
  $string .= hosting_stats_formatmail('hosting_stats_file');
  $string .= "\n=======================================\n";
  $string .= "PHP (global, local):\n";
  $string .= implode("\n", $initable);
  return $string;
}

/**
 * Send stats to specfied address.
 */
function hosting_stats_send() {
  $message = array(
    'to' => variable_get('hosting_stats_mailto', 'presales@acquia.com'),
    'subject' => t('Hosting Diagnostics report for @site (@url)', array('@site' => variable_get('site_name', ''), '@url' => $_SERVER['HTTP_HOST'])),
    'body' => array(hosting_stats_summarize()),
    'headers' => array('From' => variable_get('site_mail', '')),
  );
  $system = drupal_mail_system('hosting_stats', 'drush');
  $message = $system->format($message);
  $system->mail($message);

  if (function_exists('drush_log')) {
    drush_log(dt("Sent report to !email", array('!email' => variable_get('hosting_stats_mailto', 'presales@acquia.com'))), 'success');
  }
  else {
    return t("Sent report to @email", array('@email' => variable_get('hosting_stats_mailto', 'presales@acquia.com')));
  }
}

/**
 * Format variables into emailable text.
 */
function hosting_stats_formatmail($var) {
  $data = unserialize(variable_get($var, 'a:0:{}'));
  $out = '';
  foreach ($data as $value) {
    $out .= sprintf("%s: %s\n", $value[0], $value[1]);
  }
  return $out;
}
