<?php

function registration_stats_month_page() {
  $stats = db_query("SELECT FROM_UNIXTIME(created, '%Y-%m-%d') as date, COUNT(*) as count
    FROM {users}
    WHERE created > :date AND status = 1
    GROUP BY date
    ORDER BY created", array(':date' => strtotime('1 month ago')))->fetchAllKeyed();

  $options = array('times' => array_keys($stats));

  // Get stats from third parties.
  $options['types'] = $types = module_invoke_all('registration_stats', 'month');
  $options['types']['drupal'] = _registration_stats_deduct_stats($stats, $options['types']);

  return theme('registration_stats_chart', $options);
}

function registration_stats_year_page() {
  $stats = db_query("SELECT FROM_UNIXTIME(created, '%Y %M') as date, COUNT(*) as count
    FROM {users}
    WHERE created > :date AND status = 1
    GROUP BY date
    ORDER BY created", array(':date' => strtotime('1 year ago')))->fetchAllKeyed();

  $options = array('times' => array_keys($stats));

  // Get stats from third parties.
  $options['types'] = $types = module_invoke_all('registration_stats', 'year');
  $options['types']['drupal'] = _registration_stats_deduct_stats($stats, $options['types']);

  return theme('registration_stats_chart', $options);
}

function registration_stats_all_page() {
  $stats = db_query("SELECT FROM_UNIXTIME(created, '%Y') as date, COUNT(*) as count
    FROM {users}
    WHERE status = 1
    GROUP BY date
    ORDER BY created")->fetchAllKeyed();

  $options = array('times' => array_keys($stats));

  // Get stats from third parties.
  $options['types'] = $types = module_invoke_all('registration_stats', 'all');
  $options['types']['drupal'] = _registration_stats_deduct_stats($stats, $options['types']);

  return theme('registration_stats_chart', $options);
}

/**
 * Deduct the received stats from total.
 *  to see how many users have registered them salves.
 *
 * @param array $stats
 *  An array of total registrations.
 * @param array $types
 *  An array of arrays of registrations from social services.
 *
 * @return array
 *  An array drupal registrations.
 */
function _registration_stats_deduct_stats($stats, $types) {
  $drupal_stats = array();
  foreach ($stats as $time => $count) {
    $drupal_stats[$time] = $count;

    foreach ($types as $sub_stats) {
      $drupal_stats[$time] -= $sub_stats[$time];
    }
  }

  return $drupal_stats;
}
