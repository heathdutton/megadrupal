<?php

/**
 * @file
 * Settings form.
 */

/**
 * Config form shown at admin/config/system/google_analytics_referrer
 */
function google_analytics_referrer_admin($form, &$form_state) {

  $defaultcroninterval = 30;
  $chunk = 1000; // Could be up to 10000 but keeping the default low so that it works even for people without external cron.
  $dayquota = 10000;

  $times = array();
  $intervals = array(1, 3, 5, 10, 20, 30, 60, 180, 360, 720, 1440);
  foreach ($intervals as $interval) {
    $times[] = $interval;
  }
  $form['google_analytics_referrer_cron_interval'] = array(
    '#type' => 'select',
    '#title' => t('Minimum time between Google Analytics data fetching'),
    '#default_value' => variable_get('google_analytics_referrer_cron_interval', $defaultcroninterval),
    '#description' => t('Google Analytics statistical data is fetched and processed via a cron job. If your cron runs too frequently, you may waste your GA daily quota too fast. Set here the minimum time that needs to elapse before the Google Analytics Referrer cron runs (even if your cron job runs more frequently). Specify the time in <em>minutes</em>. Default: %defaultcroninterval minutes.', array('%defaultcroninterval' => check_plain($defaultcroninterval))),
    '#options' => drupal_map_assoc($times),
    '#required' => TRUE,
    );

  $times = array();
  $curquota = variable_get('google_analytics_referrer_api_dayquota', $dayquota);
  for ($chunks = 1; $chunks <= $curquota/1000; $chunks++) {
    $times[] = $chunks * 1000;
  }
  $form['google_analytics_referrer_chunk_to_fetch'] = array(
    '#type' => 'select',
    '#title' => t('Number of items to fetch from Google Analytics in one request'),
    '#default_value' => variable_get('google_analytics_referrer_chunk_to_fetch', $chunk),
    '#description' => t('How many items will be fetched from Google Analytics in one request (during a cron run). The maximum allowed by Google is 10000. Default: %chunk items.', array('%chunk' => check_plain($chunk))),
    '#options' => drupal_map_assoc($times),
    '#required' => TRUE,
    );

  $form['google_analytics_referrer_api_dayquota'] = array(
    '#type' => 'textfield',
    '#title' => t('Maximum GA API requests per day'),
    '#default_value' => variable_get('google_analytics_referrer_api_dayquota', $dayquota),
    '#size' => 9,
    '#maxlength' => 9,
    '#description' => t('This is the <em>daily limit</em> of requests <em>per profile</em> to the Google Analytics API. You don\'t need to change this value until Google relaxes their quota policy. Current value: %dayquota.<br />It is reasonable to expect that Google will increase this low number sooner rather than later, so watch the <a href="https://developers.google.com/analytics/devguides/reporting/core/v3/limits-quotas#discovery" target="_blank">quota</a> page for changes.<br />To get the full quota, you must <a href="https://developers.google.com/analytics/devguides/reporting/core/v3/limits-quotas#full_quota" target="_blank">register your Analytics API</a>.', array('%dayquota' => check_plain($dayquota))),
    '#required' => TRUE,
    );

  // GA response cache options
  $times = array();
  for ($hours = 1; $hours <= 24; $hours++) {
    $times[] = $hours * GOOGLE_ANALYTICS_REFERRER_HOUR;
  }
  for ($days = 1; $days <= 6; $days++) {
    $times[] = $days * GOOGLE_ANALYTICS_REFERRER_DAY;
  }
  for ($weeks = 1; $weeks <= 4; $weeks++) {
    $times[] = $weeks * GOOGLE_ANALYTICS_REFERRER_WEEK;
  }
  $form['google_analytics_referrer_cache_length'] = array(
    '#type' => 'select',
    '#title' => t('Google Analytics query cache'),
    '#description' => t('Limit the minimum time to elapse between getting fresh data for the same query from Google Analytics. Defaults to 1 day.'),
    '#options' => drupal_map_assoc($times, 'format_interval'),
    '#default_value' => variable_get('google_analytics_referrer_cache_length', GOOGLE_ANALYTICS_REFERRER_DAY),
    '#required' => TRUE,
    );

  // How many referrers to list for a given page
  $times = array();
  $intervals = array(1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 233);
  foreach ($intervals as $interval) {
      $times[] = $interval;
  }
  $defaultrownnumber = 13;
  $form['google_analytics_referrer_number_of_links'] = array(
      '#type' => 'select',
      '#title' => t('How many referrer links to show'),
      '#default_value' => variable_get('google_analytics_referrer_number_of_links', $defaultrownnumber),
      '#description' => t('Number of referring links to show. The list of referrers is always sorted by how many page views they contributed (most popular first). Default: %defaultrownnumber minutes.', array('%defaultrownnumber' => check_plain($defaultrownnumber))),
      '#options' => drupal_map_assoc($times),
      '#required' => TRUE,
  );

  $google_analytics_referrer_remove_organic = variable_get('google_analytics_referrer_remove_organic', 'no');
  $form['google_analytics_referrer_remove_organic'] = array(
    '#type' => 'select',
    '#title' => t('Exclude vague referrers'),
    '#default_value' => $google_analytics_referrer_remove_organic,
    '#description' => t('Referrers registered by Google Analytics have basically three types:
    <ul>
    <li>vague sources — e.g. organic sources such as "google" or "yahoo" (typically search results), or the string "(direct)" if there is no detected referrer</li>
    <li>domain names — e.g. yahoo.com, vacilando.org, or en.wikipedia.org</li>
    <li>precise URLs — e.g. https://en.wikipedia.org/wiki/Drupal</li>
    </ul>
    Select "yes" here if you wish to remove the vague referrers from display. Default: %google_analytics_referrer_remove_organic.', array('%google_analytics_referrer_remove_organic' => check_plain($google_analytics_referrer_remove_organic))),
    '#options' => array(
      'yes' => t('yes'),
      'no' => t('no'),
    ),
    '#required' => TRUE,
  );

  $google_analytics_referrer_remove_paths = variable_get('google_analytics_referrer_remove_paths', 'no');
  $form['google_analytics_referrer_remove_paths'] = array(
    '#type' => 'select',
    '#title' => t('Exclude precise URLs'),
    '#default_value' => $google_analytics_referrer_remove_paths,
    '#description' => t('Referrers registered by Google Analytics have basically three types:
    <ul>
    <li>vague sources — e.g. organic sources such as "google" or "yahoo" (typically search results), or the string "(direct)" if there is no detected referrer</li>
    <li>domain names — e.g. yahoo.com, vacilando.org, or en.wikipedia.org</li>
    <li>precise URLs — e.g. https://en.wikipedia.org/wiki/Drupal</li>
    </ul>
    Select "yes" here if you wish to remove the precise referrers from display. Default: %google_analytics_referrer_remove_paths.', array('%google_analytics_referrer_remove_paths' => check_plain($google_analytics_referrer_remove_paths))),
    '#options' => array(
      'yes' => t('yes'),
      'no' => t('no'),
    ),
    '#required' => TRUE,
  );

  return system_settings_form($form);
}
