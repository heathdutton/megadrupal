<?php
/**
 * @file
 * Administrative page callbacks for the JSerror module.
 */

/**
 * Page callback: Displays a listing of JavaScript error messages.
 *
 * @see jserror_clear_log_form()
 * @see jserror_filter_form()
 * @see jserror_menu()
 */
function jserror_errors_overview() {
  drupal_add_css(drupal_get_path('module', 'jserror') . '/jserror-admin.css');
  $where = jserror_filter_query();
  $filters = jserror_filters();

  if (!empty($filters)) {
    $build['jserror_filter_form'] = drupal_get_form('jserror_filter_form');
    $build['jserror_clear_log_form'] = drupal_get_form('jserror_clear_log_form');
  }

  $errors = array();
  $query = db_select('jserror', 'j')->extend('PagerDefault');
  $query->fields('j', array('eid',
    'mid',
    'message',
    'file',
    'page',
    'line',
    'col',
    'pageload',
    'user_agent',
    'browser',
    'browser_version',
    'platform',
    'platform_version',
    'stack',
    'browser',
    'lang',
    'cookie',
    'timestamp',
  ));
  if (!empty($where['where'])) {
    $query->where($where['where'], $where['args']);
  }
  $query->groupBy('mid');
  $query->addExpression('count(mid)', 'row_count');
  $result = $query->limit(10)->execute();
  foreach ($result as $error) {
    $file = preg_replace('/\?.*/', '', filter_xss($error->file));
    $errors[] = array(
      'row_count' => $error->row_count,
      'mid' => $error->mid,
      'message' => filter_xss($error->message),
      'page' => filter_xss($error->page),
      'file' => $file,
      'line' => check_plain($error->line),
      'col' => check_plain($error->col),
      'pageload' => check_plain($error->pageload),
      'browser' => $error->browser,
      'browser_version' => $error->browser_version,
      'platform' => $error->platform,
      'platform_version' => $error->platform_version,
      'stack' => check_plain($error->stack),
      'lang' => check_plain($error->lang),
      'cookie' => check_plain($error->cookie),
      'timestamp' => check_plain($error->timestamp),
    );
  }
  $build['jserror_display'] = array(
    '#theme' => 'jserror_list',
    '#errors' => $errors,
    '#empty' => t('No log messages available.'),
  );
  $build['jserror_pager'] = array('#theme' => 'pager');

  return $build;
}

/**
 * Page callback: Displays the details of a JavaScript error.
 *
 * @see jserror_clear_log_form()
 * @see jserror_filter_form()
 * @see jserror_menu()
 */
function jserror_error_details($mid) {
  drupal_add_css(drupal_get_path('module', 'jserror') . '/jserror-admin.css');
  $errors = array();
  $query = db_select('jserror', 'j')->extend('PagerDefault');
  $query->condition('j.mid', $mid, '=')->fields('j', array('eid',
    'mid',
    'message',
    'file',
    'page',
    'line',
    'col',
    'pageload',
    'user_agent',
    'browser',
    'browser_version',
    'platform',
    'platform_version',
    'stack',
    'lang',
    'cookie',
    'timestamp',
  ));
  $result = $query->limit(5)->execute();
  $browser = $platform = $where = $on_page = array();
  foreach ($result as $error) {
    $line = check_plain($error->line);
    $col = check_plain($error->col);
    $file = preg_replace('/\?.*/', '', filter_xss($error->file));
    $page = filter_xss($error->page);
    $errors['occurrence'][] = array(
      'mid' => $error->mid,
      'message' => filter_xss($error->message),
      'page' => $page,
      'file' => $file,
      'line' => $line,
      'col' => check_plain($error->col),
      'pageload' => check_plain($error->pageload),
      'user_agent' => check_plain($error->user_agent),
      'browser' => $error->browser,
      'browser_version' => $error->browser_version,
      'platform' => $error->platform,
      'platform_version' => $error->platform_version,
      'stack' => check_plain($error->stack),
      'lang' => check_plain($error->lang),
      'cookie' => check_plain($error->cookie),
      'timestamp' => check_plain($error->timestamp),
    );
    if (!in_array($page, $on_page)) {
      array_push($on_page, $page);
    }
    if (!in_array('Line ' . $line . ':' . $col . ' in ' . $file, $where)) {
      array_push($where, 'Line ' . $line . ':' . $col . ' in ' . $file);
    }
    if (!in_array($error->browser . ' ' . $error->browser_version, $browser)) {
      array_push($browser, $error->browser . ' ' . $error->browser_version);
    }
    if (!in_array($error->platform, $platform)) {
      array_push($platform, $error->platform);
    }
  }

  drupal_set_title($errors['occurrence'][0]['message']);
  $errors['page'] = $on_page;
  $errors['where'] = $where;
  $errors['browser'] = $browser;
  $errors['platform'] = $platform;
  $build['jslog_display_error'] = array(
    '#theme' => 'jserror',
    '#errors' => $errors,
    '#empty' => t('No error messages available.'),
  );
  $build['jserror_pager'] = array('#theme' => 'pager');

  return $build;
}

/**
 * Form builder. Configure JSerror.
 *
 * @ingroup forms
 * @see system_settings_form()
 * @see jserror_menu()
 */
function jserror_admin_settings() {
  $form = array();
  $form['jserror_debug_mode'] = array(
    '#type' => 'checkbox',
    '#title' => t('Debug Mode'),
    '#default_value' => variable_get('jserror_debug_mode', 0),
    '#description' => t('Send unminified JavaScript. Helpful in debugging the module. Disable in production environment.'),
  );
  $form['jserror_suppress_warning'] = array(
    '#type' => 'checkbox',
    '#title' => t('Subpress warnings in console'),
    '#default_value' => variable_get('jserror_suppress_warning', 0),
    '#description' => t('Subpresses console errors. Useful on production sites. Your users will not know when your JavaScript breaks!'),
  );
  $form['jserror_page_limit'] = array(
    '#type' => 'select',
    '#title' => t('Page request limit'),
    '#default_value' => variable_get('jserror_page_limit', 100),
    '#options' => array(
      -1 => t('Unlimited')) + drupal_map_assoc(array(
        5,
        10,
        50,
        100,
        500,
        1000,
      )),
    '#description' => t('Number of request a client can send from a page.'),
  );
  $form['jserror_client_limit'] = array(
    '#type' => 'select',
    '#title' => t('Client request limit'),
    '#default_value' => variable_get('jserror_client_limit', 10000),
    '#options' => array(
      -1 => t('Unlimited')) + drupal_map_assoc(array(
        10,
        100,
        1000,
        10000,
        100000,
      )),
    '#description' => t('Overall number of request a client can send in an hour.'),
  );
  $form['jserror_percentage'] = array(
    '#type' => 'textfield',
    '#title' => t('Percentage of users for logging errors'),
    '#default_value' => variable_get('jserror_percentage', 100),
    '#required' => TRUE,
    '#description' => t('The percentage of visits for which jserror will be enabled. Lower value is helpful on production site.'),
  );
  $form['jserror_row_limit'] = array(
    '#type' => 'select',
    '#title' => t('Javascript log messages to keep'),
    '#default_value' => variable_get('jserror_row_limit', 1000),
    '#options' => array(0 => t('All')) + drupal_map_assoc(
      array(
        100,
        1000,
        10000,
        100000,
        1000000,
      )),
    '#description' => t('The maximum number of messages to keep in the JavaScript error log. Requires a <a href="@cron">cron maintenance task</a>.', array('@cron' => url('admin/reports/status'))),
  );
  $form['#validate'][] = 'jserror_admin_settings_validate';
  return system_settings_form($form);
}

/**
 * Form validation handler for jserror_admin_settings().
 *
 * @see jserror_admin_settings()
 */
function jserror_admin_settings_validate($form, $form_state) {
  $percentage = $form_state['values']['jserror_percentage'];
  if (!is_numeric($percentage)) {
    form_set_error('jserror_percentage', t('Please enter a numaric value'));
  }
  elseif ($percentage > 100 || $percentage < 0) {
    form_set_error('jserror_percentage', t('The Value should be between 0 to 100'));
  }
}

/**
 * Builds a query for error log administration filters based on session.
 *
 * @return array
 *   An associative array with keys 'where' and 'args'.
 */
function jserror_filter_query() {
  if (empty($_SESSION['jserror_overview_filter'])) {
    return;
  }

  $filters = jserror_filters();

  // Build query.
  $where = $args = array();
  foreach ($_SESSION['jserror_overview_filter'] as $key => $filter) {
    $filter_where = array();
    foreach ($filter as $value) {
      $filter_where[] = $filters[$key]['where'];
      $args[] = $value;
    }
    if (!empty($filter_where)) {
      $where[] = '(' . implode(' OR ', $filter_where) . ')';
    }
  }
  $where = !empty($where) ? implode(' AND ', $where) : '';

  return array(
    'where' => $where,
    'args' => $args,
  );
}

/**
 * Form constructor for the JSerror logging filter form.
 *
 * @see jserror_filter_form_validate()
 * @see jserror_filter_form_submit()
 * @see jserror_admin_overview()
 *
 * @ingroup forms
 */
function jserror_filter_form($form) {
  $filters = jserror_filters();

  $form['filters'] = array(
    '#type' => 'fieldset',
    '#title' => t('Filter error messages'),
    '#collapsible' => TRUE,
    '#collapsed' => empty($_SESSION['jserror_overview_filter']),
  );
  foreach ($filters as $key => $filter) {
    $form['filters']['status'][$key] = array(
      '#title' => check_plain($filter['title']),
      '#type' => 'select',
      '#multiple' => TRUE,
      '#size' => 8,
      '#options' => $filter['options'],
    );
    if (!empty($_SESSION['jserror_overview_filter'][$key])) {
      $form['filters']['status'][$key]['#default_value'] = $_SESSION['jserror_overview_filter'][$key];
    }
  }

  $form['filters']['actions'] = array(
    '#type' => 'actions',
    '#attributes' => array('class' => array('container-inline')),
  );
  $form['filters']['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Filter'),
  );
  if (!empty($_SESSION['jserror_overview_filter'])) {
    $form['filters']['actions']['reset'] = array(
      '#type' => 'submit',
      '#value' => t('Reset'),
    );
  }
  return $form;
}

/**
 * Form validation handler for jserror_filter_form().
 *
 * @see jserror_filter_form_submit()
 */
function jserror_filter_form_validate($form, &$form_state) {
  if ($form_state['values']['op'] == t('Filter') && empty($form_state['values']['browser']) && empty($form_state['values']['platform'])) {
    form_set_error('browser', t('You must select something to filter by.'));
  }
}

/**
 * Form submission handler for jserror_filter_form().
 *
 * @see jserror_filter_form_validate()
 */
function jserror_filter_form_submit($form, &$form_state) {
  $op = $form_state['values']['op'];
  $filters = jserror_filters();
  switch ($op) {
    case t('Filter'):
      foreach ($filters as $name => $filter) {
        if (isset($form_state['values'][$name])) {
          $_SESSION['jserror_overview_filter'][$name] = $form_state['values'][$name];
        }
      }
      break;

    case t('Reset'):
      $_SESSION['jserror_overview_filter'] = array();
      break;

  }
  return 'admin/reports/jserror';
}

/**
 * Form constructor for the form that clears out the error logs.
 *
 * @see jserror_clear_log_form_submit()
 * @ingroup forms
 */
function jserror_clear_log_form($form) {
  $form['jserror_clear'] = array(
    '#type' => 'fieldset',
    '#title' => t('Clear error messages'),
    '#description' => t('This will permanently remove the error messages from the database.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['jserror_clear']['clear'] = array(
    '#type' => 'submit',
    '#value' => t('Clear error messages'),
  );

  return $form;
}

/**
 * Form submission handler for jserror_clear_log_form().
 */
function jserror_clear_log_form_submit() {
  $_SESSION['jserror_overview_filter'] = array();
  db_delete('jserror')->execute();
  drupal_set_message(t('JSerror log cleared.'));
}

/**
 * Creates a list of error administration filters that can be applied.
 *
 * @return array
 *   Associative array of filters.
 *   The top-level keys are used as the form element names for the filters,
 *   and the values are arrays with the following
 *   elements:
 *   - title: Title of the filter.
 *   - where: The filter condition.
 *   - options: Array of options for the select list for the filter.
 */
function jserror_filters() {
  static $filters;

  if (isset($filters)) {
    return $filters;
  }

  $filters = array();

  foreach (_jserror_get_browsers() as $browser) {
    $browsers[$browser] = $browser;
  }

  if (!empty($browsers)) {
    $filters['browser'] = array(
      'title' => t('Browser'),
      'where' => 'j.browser = ?',
      'options' => $browsers,
    );
  }
  foreach (_jserror_get_platforms() as $platform) {
    $platforms[$platform] = $platform;
  }
  if (!empty($browsers)) {
    $filters['platform'] = array(
      'title' => t('Platform'),
      'where' => 'j.platform = ?',
      'options' => $platforms,
    );
  }
  return $filters;
}

/**
 * Gathers a list of uniquely identified Browsers.
 *
 * @return array
 *   List of uniquely Browsers.
 */
function _jserror_get_browsers() {
  $browser = array();

  $result = db_query('SELECT DISTINCT(browser) FROM {jserror}');
  foreach ($result as $object) {
    $browser[] = $object->browser;
  }

  return $browser;
}

/**
 * Gathers a list of uniquely identified Platform.
 *
 * @return array
 *   List of unique Platforms.
 */
function _jserror_get_platforms() {
  $platform = array();

  $result = db_query('SELECT DISTINCT(platform) FROM {jserror}');
  foreach ($result as $object) {
    $platform[] = $object->platform;
  }

  return $platform;
}
