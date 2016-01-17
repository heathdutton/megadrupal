<?php

/**
 * @file
 * Admin page callbacks file for the search_log module.
 */

/**
 * Display admin settings.
 *
 * @return
 *   array of form content.
 */
function search_log_admin_settings($form, &$form_state) {
  $form = array();

  $form['logging'] = array(
    '#type' => 'fieldset',
    '#title' => t('Search log settings'),
  );

  $terms_options = array(
    SEARCH_LOG_TERMS_LOWERCASE => t('lowercase (%1 stored as %2)', array('%1' => 'Apple iPod', '%2' => 'apple ipod')),
    SEARCH_LOG_TERMS_UPPERCASE_FIRST => t('uppercase first word (%1 stored as %2)', array('%1' => 'Apple iPod', '%2' => 'Apple ipod')),
    SEARCH_LOG_TERMS_UPPERCASE_WORDS => t('uppercase all words (%1 stored as %2)', array('%1' => 'Apple iPod', '%2' => 'Apple Ipod')),
  );
  $form['logging']['search_log_terms'] = array(
    '#type' => 'radios',
    '#title' => t('Search term normalization'),
    '#description' => t('Search terms are normalized before they are stored in Search log. Changing this value may result in duplicate terms for the current day.'),
    '#options' => $terms_options,
    '#default_value' => variable_get('search_log_terms', SEARCH_LOG_TERMS_LOWERCASE),
  );

  foreach (module_implements('search_info') as $module) {
    $module_options[$module] = $module;
  }
  $form['logging']['search_log_modules_enabled'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Modules'),
    '#description' => t('Select modules to record in Search log. If no modules are checked, all modules which implement !hook_search_info() will be recorded.', array('!hook_search_info()' => l('hook_search_info()', 'http://api.drupal.org/api/drupal/modules--search--search.api.php/function/hook_search_info/7'))),
    '#options' => $module_options,
    '#default_value' => variable_get('search_log_modules_enabled', array()),
  );

  $form['logging']['search_log_preprocess'] = array(
    '#type' => 'checkbox',
    '#title' => t('!warning Collect search result with preprocess_search_results()', array('!warning' => '<span class="error">[Experimental]</span>')),
    '#description' => t('Search does not have a hook to obtain the number of search results. This theme function will work in certain circumstances. If enabled, the function will add one extra DB write for failed search results.'),
    '#default_value' => variable_get('search_log_preprocess', TRUE),
  );

  $form['status'] = array(
    '#type' => 'fieldset',
    '#title' => t('Search log status'),
  );

  $count = db_query('SELECT COUNT(qid) FROM {search_log}')->fetchField();
  $form['status']['search_log_count']['#markup'] = '<p>' . t('There are %count entries in the Search log table. !clear', array('%count' => number_format($count), '!clear' => l(t('Clear log'), 'admin/config/search/search_log/clear'))) . '</p>';

  $form['status']['search_log_cron'] = array(
    '#type' => 'textfield',
    '#title' => t('Days to keep search log'),
    '#description' => t('Search log table can be automatically truncated by cron. Set to 0 to never truncate Search log table.'),
    '#size' => 4,
    '#default_value' => variable_get('search_log_cron', 0),
  );

  $form['#validate'][] = 'search_log_admin_settings_validate';

  return system_settings_form($form);
}

/**
 * Validate admin settings.
 */
function search_log_admin_settings_validate($form, &$form_state) {
  if (!preg_match('/^[0-9]+$/', $form_state['values']['search_log_cron'])) {
    form_set_error('search_log_cron', t('The number of days must be a number 0 or greater.'));
  }

  $any_enabled = FALSE;
  foreach ($form_state['values']['search_log_modules_enabled'] as $module => $enabled) {
    if ($enabled) {
      $any_enabled = TRUE;
      break;
    }
  }
  if (!$any_enabled) {
    form_set_value($form['logging']['search_log_modules_enabled'], NULL, $form_state);
  }
}

/**
 * Form builder; confirm form for truncate search_log table.
 *
 * @ingroup forms
 * @see search_log_confirm_truncate_submit()
 */
function search_log_confirm_truncate($form, &$form_state) {
  $form = array();
  return confirm_form($form,
    t('Are you sure you want to clear the Search log?'),
    'admin/config/search/search_log',
    t('This action cannot be undone.'),
    t('Clear'), t('Cancel'));
}

/**
 * Truncate search_log table.
 */
function search_log_confirm_truncate_submit($form, &$form_state) {
  db_query("TRUNCATE {search_log}");
  drupal_set_message(t('Search log cleared.'));

  if (!isset($_GET['destination'])) {
    $form_state['redirect'] = 'admin/config/search/search_log';
  }
}

/**
 * Display search log report.
 */
function search_log_report($period = NULL) {
  // Set defaults.
  $today = getdate(_search_log_get_time());
  $date['from'] = $date['to'] = date('Y-m-d', $today[0]);
  $filter['modules'] = isset($_SESSION['search_log']['modules']) ? $_SESSION['search_log']['modules'] : array();
  $filter['languages'] = isset($_SESSION['search_log']['languages']) ? $_SESSION['search_log']['languages'] : array();
  $filter['result'] = isset($_SESSION['search_log']['result']) ? $_SESSION['search_log']['result'] : SEARCH_LOG_RESULT_UNKNOWN;
  $rows = array();

  // Handle $period arg and set $_SESSION.
  switch ($period) {
    // Today begins at 00:00.
    case 'today':
      $_SESSION['search_log']['from'] = $_SESSION['search_log']['to'] = $date['from'];
      break;

    // Week begins on Sunday.
    case 'week':
      $date['from'] = $_SESSION['search_log']['from'] = date('Y-m-d', $today[0] - (($today['wday']) * SEARCH_LOG_DAY));
      $date['to'] = $_SESSION['search_log']['to'] = date('Y-m-d', $today[0] + (6 - $today['wday']) * SEARCH_LOG_DAY);
      break;

    case 'month':
      $date['from'] = $_SESSION['search_log']['from'] = date('Y-m-d', mktime(0, 0, 0, $today['mon'], 1, $today['year']));
      $date['to'] = $_SESSION['search_log']['to'] = date('Y-m-d', mktime(0, 0, 0, $today['mon'] + 1, 1, $today['year']) - SEARCH_LOG_DAY);
      break;

    case 'year':
      $date['from'] = $_SESSION['search_log']['from'] = date('Y-m-d', mktime(0, 0, 0, 1, 1, $today['year']));
      $date['to'] = $_SESSION['search_log']['to'] = date('Y-m-d', mktime(0, 0, 0, 12, 31, $today['year']));
      break;

    default:
      $date['from'] = isset($_SESSION['search_log']['from']) ? $_SESSION['search_log']['from'] : $date['from'];
      $date['to'] = isset($_SESSION['search_log']['to']) ? $_SESSION['search_log']['to'] : $date['from'];
  }

  // Convert all human-readable dates back to time.
  $time['from'] = strtotime($date['from']);
  $time['to'] = strtotime($date['to']);

  if ($time['from'] && $time['to']) {
    // Result filter is enabled if there are any failed searches.
    $enabled['result'] = db_query_range('SELECT qid FROM {search_log} WHERE result = :result', 0, 1, array(':result' => SEARCH_LOG_RESULT_FAILED))->fetchField();

    // Build header.
    $header = array(
      array(
        'data' => t('Search term'),
        'field' => 'q',
      ),
      array(
        'data' => t('Module'),
        'field' => 'module',
      ),
      array(
        'data' => t('Language'),
        'field' => 'language',
      ),
    );
    if ($enabled['result']) {
      array_push($header, array('data' => t('Result'), 'field' => 'result'));
    }
    array_push($header, array('data' => t('Total'), 'field' => 'total', 'sort' => 'desc'));

    // Build query.
    $query = db_select('search_log')
      ->fields('search_log', array(
          'q',
          'module',
          'language',
          'result',
        ))
      ->condition('day', $time['from'], '>=')
      ->condition('day', $time['to'], '<=')
      ->groupBy('q, module')
      ->extend('PagerDefault')
        ->limit(SEARCH_LOG_ADMIN_ROWS)
      ->extend('TableSort')
        ->orderByHeader($header);
    $query->addExpression('SUM(counter)', 'total');
    _search_log_get_query_filter($query, $time, $filter);

    // Build results.
    $result = $query->execute();
    while ($data = $result->fetchObject()) {
      $row = array();
      $row[] = l(truncate_utf8($data->q, SEARCH_LOG_ADMIN_TERM_LENGTH, TRUE, TRUE), "search/$data->module/$data->q");
      $row[] = $data->module;
      $row[] = $data->language;
      if ($enabled['result']) {
        $row[] = $data->result == SEARCH_LOG_RESULT_FAILED ? '<span class="error">' . t('Failed') . '</span>' : '';
      }
      $row[] = $data->total;
      $rows[] = $row;
    }

    if ($rows) {
      // Calculate total searches.
      $query = db_select('search_log')
        ->condition('day', $time['from'], '>=')
        ->condition('day', $time['to'], '<=');
      $query->addExpression('SUM(counter)', 'total');
      _search_log_get_query_filter($query, $time, $filter);
      $total = $query->execute()->fetchField();

      // Calculate unique terms.
      $query = db_select('search_log')
        ->condition('day', $time['from'], '>=')
        ->condition('day', $time['to'], '<=');
      $query->addExpression('COUNT(DISTINCT q, module)', 'total');
      _search_log_get_query_filter($query, $time, $filter);
      $unique = $query->execute()->fetchField();

      // Calculate failed searches.
      switch ($filter['result']) {
        case SEARCH_LOG_RESULT_UNKNOWN:
          $query = db_select('search_log')
            ->condition('day', $time['from'], '>=')
            ->condition('day', $time['to'], '<=')
            ->condition('result', SEARCH_LOG_RESULT_FAILED, '=');
          $query->addExpression('SUM(counter)', 'total');
          _search_log_get_query_filter($query, $time, $filter, FALSE);
          $failed = (int) $query->execute()->fetchField();
          break;

        case SEARCH_LOG_RESULT_SUCCESS:
          $failed = 0;
          break;

        case SEARCH_LOG_RESULT_FAILED:
          $failed = $total;
          break;
      }

      return theme('search_log_report', array(
        'summary' => module_exists('chart') ? _search_log_summary_chart($total, $unique, $failed, $time, $filter) : theme('search_log_summary', array('total' => $total, 'unique' => $unique, 'failed' => $failed)),
        'filters' => drupal_get_form('search_log_report_form', $date, $filter),
        'table' => theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => 'search-log'))),
        'pager' => theme('pager', array('tags' => array())),
      ));
    }
  }

  return theme('search_log_report', array(
    'summary' => t('No searches found for period.'),
    'filters' => drupal_get_form('search_log_report_form', $date, $filter),
    'table' => NULL,
    'pager' => NULL,
  ));
}

/**
 * Report filter form.
 */
function search_log_report_form($form, &$form_state, $date = array(), $filter = array()) {
  $form = array();

  // Search period
  $today = date('Y-m-d', _search_log_get_time());

  $form['period'] = array(
    '#type' => 'fieldset',
    '#title' => t('Period'),
  );

  $links[] = array(
    'title' => t('Today'),
    'href' => 'admin/reports/search/today',
  );
  $links[] = array(
    'title' => t('This week'),
    'href' => 'admin/reports/search/week',
  );
  $links[] = array(
    'title' => t('This month'),
    'href' => 'admin/reports/search/month',
  );
  $links[] = array(
    'title' => t('This year'),
    'href' => 'admin/reports/search/year',
  );
  $form['period']['links']['#markup'] = theme('links', array('links' => $links, 'attributes' => array('class' => 'search-log-links')));

  $form['period']['from_date'] = array(
    '#type' => 'textfield',
    '#title' => t('From'),
    '#default_value' => $date['from'] ? $date['from'] : $today,
  );

  $form['period']['to_date'] = array(
    '#type' => 'textfield',
    '#title' => t('To'),
    '#default_value' => $date['to'] ? $date['to'] : $today,
    '#description' => t('Enter custom period for search reporting.'),
  );

  // Search result.
  if (db_query_range('SELECT qid FROM {search_log} WHERE result = :result', 0, 1, array(':result' => SEARCH_LOG_RESULT_FAILED))->fetchField()) {
    $result_options = array(
      SEARCH_LOG_RESULT_UNKNOWN => t('All'),
      SEARCH_LOG_RESULT_SUCCESS => t('Success'),
      SEARCH_LOG_RESULT_FAILED => t('Failed'),
    );

    $form['result'] = array(
      '#type' => 'fieldset',
      '#title' => t('Result'),
    );

    $form['result']['result'] = array(
      '#type' => 'radios',
      '#description' => t('Select result to include in search reporting.'),
      '#options' => $result_options,
      '#default_value' => $filter['result'] ? $filter['result'] : SEARCH_LOG_RESULT_UNKNOWN,
      '#required' => TRUE,
    );
  }

  // Search modules.
  $module_options = array();
  $query = db_query('SELECT DISTINCT module FROM {search_log}');
  while ($row = $query->fetchObject()) {
    $module_options[$row->module] = $row->module;
  }
  if (count($module_options) > 1) {
    $module_default = !empty($filter['modules']) ? $filter['modules'] : array_keys($module_options);

    $form['modules'] = array(
      '#type' => 'fieldset',
      '#title' => t('Modules'),
    );

    $form['modules']['modules'] = array(
      '#type' => 'checkboxes',
      '#description' => t('Select modules to include in search reporting.'),
      '#options' => $module_options,
      '#default_value' => $module_default,
    );
  }

  // Search languages.
  $language_options = array();
  $query = db_query('SELECT DISTINCT language FROM {search_log}');
  while ($row = $query->fetchObject()) {
    $language_options[$row->language] = $row->language;
  }
  if (count($language_options) > 1) {
    $language_default = !empty($filter['languages']) ? $filter['languages'] : array_keys($language_options);

    $form['languages'] = array(
      '#type' => 'fieldset',
      '#title' => t('Languages'),
    );

    $form['languages']['languages'] = array(
      '#type' => 'checkboxes',
      '#description' => t('Select languages to include in search reporting.'),
      '#options' => $language_options,
      '#default_value' => $language_default,
    );
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update Report'),
  );
  $form['#action'] = url('admin/reports/search');

  return $form;
}

/**
 * Report filter form validation.
 */
function search_log_report_form_validate($form, &$form_state) {
  $today = _search_log_get_time();

  $from = strtotime($form_state['values']['from_date']);
  if (!$from) {
    $from = $today;
  }

  if ($from > $today) {
    form_set_error('from_date', t('From date cannot be after today.'));
  }

  $to = strtotime($form_state['values']['to_date']);
  if (!$to) {
    $to = $today;
  }

  if ($from > $to) {
    form_set_error('from_date', t('From date cannot be after To date.'));
  }

  if (isset($form_state['values']['modules'])) {
    $modules = array_flip($form_state['values']['modules']);
    unset($modules[0]);
    if (count($modules) < 1) {
      form_set_error('modules', t('At least one module must be selected.'));
    }
  }

  if (isset($form_state['values']['languages'])) {
    $languages = array_flip($form_state['values']['languages']);
    unset($languages[0]);
    if (count($languages) < 1) {
      form_set_error('languages', t('At least one language must be selected.'));
    }
  }
}

/**
 * Report filter submit.
 */
function search_log_report_form_submit($form, &$form_state) {
  $modules = isset($form_state['values']['modules']) ? array_flip($form_state['values']['modules']) : array();
  unset($modules[0]);

  $languages = isset($form_state['values']['languages']) ? array_flip($form_state['values']['languages']) : array();
  unset($languages[0]);

  $_SESSION['search_log'] = array(
    'from' => $form_state['values']['from_date'],
    'to' => $form_state['values']['to_date'],
    'modules' => array_keys($modules),
    'languages' => array_keys($languages),
    'result' => $form_state['values']['result'],
  );
}

/**
 * Theme report.
 */
function theme_search_log_report($variables) {
  drupal_add_css(drupal_get_path('module', 'search_log') . '/search_log.css');

  $output = '<div class="search-log-filter">' . drupal_render($variables['filters']) . '</div>';
  $output .= '<div class="search-log-results">';
  $output .= '<fieldset class="summary"><legend>' . t('Report results') . '</legend><div class="fieldset-wrapper">' . $variables['summary'] . '</div></fieldset>';
  $output .= $variables['table'];
  $output .= $variables['pager'];
  $output .= '</div>';

  return $output;
}

/**
 * Theme report summary information.
 */
function theme_search_log_summary($variables) {
  $output = '<div><strong>' . t('Total searches') . ':</strong> ' . $variables['total'] . '</div>';
  $output .= '<div><strong>' . t('Unique terms') . ':</strong> ' . $variables['unique'] . ' (' . sprintf("%01.2f", 100 * $variables['unique'] / $variables['total']) . '%)</div>';
  $output .= '<div><strong>' . t('Failed searches') . ':</strong> ' . $variables['failed'] . ' (' . sprintf("%01.2f", 100 * $variables['failed'] / $variables['total']) . '%)</div>';

  return $output;
}

/**
 * Internal function for summary chart.
 */
function _search_log_summary_chart($total = 0, $unique = 0, $failed = 0, $time = array(), $filter = array()) {
  $output = '';

  // Build daily searches.
  if ($time['from'] && $time['to']) {
    $chart = array(
      '#chart_id' => 'daily',
      '#title' => t('Total searches (@total)', array('@total' => number_format($total))),
      '#type' => CHART_TYPE_LINE,
      '#adjust_resolution' => TRUE,
      '#grid_lines' => chart_grid_lines(20, 20),
      '#size' => chart_size(280, 130),
    );

    // Line graph cannot be a single point.
    if ($time['from'] == $time['to']) {
      $time['to'] += SEARCH_LOG_DAY;
    }

    // Calculate maximum days per label.
    $days = ($time['to'] - $time['from']) / SEARCH_LOG_DAY;
    if ($days < 8) {
      $days_per_label = 1;
    }
    elseif ($days < 15) {
      $days_per_label = 2;
    }
    elseif ($days < 32) {
      $days_per_label = 7;
    }
    elseif ($days < 93) {
      $days_per_label = 30;
    }
    else {
      $days_per_label = 90;
    }

    // Add empty data set for days in range.
    $days = 1;
    for ($day = $time['from']; $day <= $time['to']; $day+=SEARCH_LOG_DAY) {
      $chart['#data'][$day] = 0;
      if (--$days == 0) {
        $chart['#labels'][$day] = date('j M', $day);
        $days = $days_per_label;
      }
    }

    // Build query.
    $query = db_select('search_log')
      ->fields('search_log', array(
          'day',
        ))
      ->condition('day', $time['from'], '>=')
      ->condition('day', $time['to'], '<=')
      ->groupBy('day');
    $query->addExpression('SUM(counter)', 'total');
    _search_log_get_query_filter($query, $time, $filter);

    // Build results.
    $label_max = 0;
    $result = $query->execute();
    while ($data = $result->fetchObject()) {
      $chart['#data'][$data->day] = $data->total;
      if ($data->total > $label_max) {
        $label_max = $data->total;
      }
    }
    ksort($chart['#data']);
    $chart['#mixed_axis_labels'][CHART_AXIS_Y_LEFT][0][] = chart_mixed_axis_range_label(0, $label_max);

    $output .= theme('chart', array('chart' => $chart));
  }

  // Build Unique terms.
  $chart = array(
    '#chart_id' => 'unique',
    '#type' => CHART_TYPE_PIE_3D,
    '#size' => chart_size(200, 130),
  );
  $chart['#title'] = t('Unique terms') . ' (' . number_format($unique) . ')';
  $chart['#data']['unique'] = $unique;
  $chart['#data']['remainder']  = $total - $unique;
  $output .= theme('chart', array('chart' => $chart));

  // Build Failed searches.
  $chart = array(
    '#chart_id' => 'failed',
    '#type' => CHART_TYPE_PIE_3D,
    '#size' => chart_size(200, 130),
  );
  $chart['#title'] = t('Failed searches') . ' (' . number_format($failed) . ')';
  $chart['#data']['failed'] = $failed;
  $chart['#data']['remainder']  = $total - $failed;
  $chart['#data_colors'][] = 'ff6666';
  $output .= theme('chart', array('chart' => $chart));

  return $output;
}

/**
 * Internal function to add filter conditions.
 */
function _search_log_get_query_filter(&$query, $time = array(), $filter = array(), $result_filter = TRUE) {
  if ($result_filter) {
    if ($filter['result'] == SEARCH_LOG_RESULT_SUCCESS) {
      $query->condition('result', SEARCH_LOG_RESULT_UNKNOWN, '>=');
    }
    elseif ($filter['result'] == SEARCH_LOG_RESULT_FAILED) {
      $query->condition('result', SEARCH_LOG_RESULT_FAILED, '=');
    }
  }
  if (!empty($filter['modules'])) {
    $query->condition('module', $filter['modules'], 'IN');
  }
  if (!empty($filter['languages'])) {
    $query->condition('language', $filter['languages'], 'IN');
  }
}
