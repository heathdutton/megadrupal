<?php
/**
 * @file
 * Hooks provided by the Cleaner module.
 */

/**
 * Create Cleaner settings form.
 *
 * @return array
 *   Form of the cleaner settings page.
 */
function hook_cleaner_settings() {
  // Add CSS to the admin settings page.
  drupal_add_css(drupal_get_path('module', 'cleaner') . '/cleaner.css');
  $form = array();
  $yes_no = array(t('No'), t('Yes'));
  $inline = array('class' => array('container-inline'));

  $interval = array(0 => t('Every time')) + Cleaner::$intervals;
  $form['cleaner_cron'] = array(
    '#type'           => 'radios',
    '#title'          => t('Run interval'),
    '#options'        => $interval,
    '#default_value'  => variable_get('cleaner_cron', 3600),
    '#description'    => t('This is how often the options below will occur. The actions will occur on the next Cron run after this interval expires. "Every time" means on every Cron run.'),
    '#attributes'     => $inline,
  );

  $form['cleaner_clear_cache'] = array(
    '#type'           => 'radios',
    '#options'        => $yes_no,
    '#title'          => t('Clean up cache'),
    '#default_value'  => variable_get('cleaner_clear_cache', 0),
    '#description'    => Cleaner::cleanerGetCacheTablesTable(),
    '#attributes'     => $inline,
  );

  $form['cleaner_empty_watchdog'] = array(
    '#type'           => 'radios',
    '#options'        => $yes_no,
    '#title'          => t('Clean up Watchdog'),
    '#default_value'  => variable_get('cleaner_empty_watchdog', 0),
    '#description'    => t('There is a standard setting for controlling Watchdog contents. This is more useful for test sites.'),
    '#attributes'     => $inline,
  );

  $cookie = session_get_cookie_params();
  $select = db_select('sessions', 's')
    ->fields('s', array('timestamp'))
    ->condition('timestamp', REQUEST_TIME - $cookie['lifetime'], '<');
  $count = $select->execute()->rowCount();

  $form['cleaner_clean_sessions'] = array(
    '#type'           => 'radios',
    '#options'        => $yes_no,
    '#title'          => t('Clean up Sessions table'),
    '#default_value'  => variable_get('cleaner_clean_sessions', 0),
    '#description'    => t('The sessions table can quickly become full with old, abandoned sessions. This will delete all sessions older than @interval (as set by your site administrator). There are currently @count such sessions.',
      array('@interval' => format_interval($cookie['lifetime']), '@count' => $count)),
    '#attributes'     => $inline,
  );

  $form['cleaner_clean_cssdir'] = array(
    '#type'           => 'radios',
    '#options'        => $yes_no,
    '#title'          => t('Clean up CSS files'),
    '#default_value'  => variable_get('cleaner_clean_cssdir', 0),
    '#description'    => t('The CSS directory can become full with stale and outdated cache files.  This will delete all CSS cache files but the latest.'),
    '#attributes'     => $inline,
  );

  $form['cleaner_clean_jsdir'] = array(
    '#type'           => 'radios',
    '#options'        => $yes_no,
    '#title'          => t('Clean up JS files'),
    '#default_value'  => variable_get('cleaner_clean_jsdir', 0),
    '#description'    => t('The JS directory can become full with stale and outdated cache files.  This will delete all JS cache files but the latest.'),
    '#attributes'     => $inline,
  );

  // We can only offer OPTIMIZE to MySQL users.
  if (db_driver() == 'mysql') {
    $form['cleaner_optimize_db'] = array(
      '#type'           => 'radios',
      '#options'        => $yes_no + array('2' => 'Local only'),
      '#title'          => t('Optimize tables with "overhead" space'),
      '#default_value'  => variable_get('cleaner_optimize_db', 0),
      '#description'    => t('The module will compress (optimize) all database tables with unused space. <strong>NOTE</strong>: During an optimization, the table will locked against any other activity; on a high vloume site, this may be undesirable. "Local only" means do not replicate the optimization (if it is being done).'),
      '#attributes'     => $inline,
    );
  }
  else {
    // If not MySQL, delete(reset) the variable.
    variable_del('cleaner_optimize_db');
  }

  return array('cleaner' => $form);
}

/**
 * Cleaner execution hook.
 */
function hook_cleaner_run() {
  // Watchdog.
  if (variable_get('cleaner_empty_watchdog', FALSE)) {
    Cleaner::cleanerWatchdogClear();
  }

  // Cache.
  if (variable_get('cleaner_clear_cache', FALSE)) {
    Cleaner::cleanerCacheClear();
  }

  // Sessions.
  if (variable_get('cleaner_clean_sessions', 0)) {
    Cleaner::cleanerSessionsClear();
  }

  // CSS.
  if (variable_get('cleaner_clean_cssdir', FALSE)) {
    Cleaner::cleanerCssClear();
  }

  // JS.
  if (variable_get('cleaner_clean_jsdir', FALSE)) {
    Cleaner::cleanerJsClear();
  }

  // MySQL optimizing.
  if ($opt = variable_get('cleaner_optimize_db', FALSE)) {
    Cleaner::cleanerMysqlOptimizing($opt);
  }
}
