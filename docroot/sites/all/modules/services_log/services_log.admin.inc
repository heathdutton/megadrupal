<?php

/**
 * @file
 *  Define administrative functions to support Services logging.
 */


/**
 * Display a single Services log entry.
 *
 * @param $entry_id
 *   entry ID from the services_log table
 * @return
 *   HTML of the entry
 */
function services_log_display_entry($entry_id) {

  if (!is_numeric($entry_id)) {
    drupal_not_found();
    return;
  }

  $entry = db_query('SELECT * FROM {services_log} WHERE entry_id = :entry_id', array(":entry_id" => $entry_id))->fetchObject();

  // Bail out if there's no such entry.
  if (empty($entry)) {
    drupal_not_found();
    return;
  }

  $output = '';
  $service_user = user_load($entry->uid);

  $timing = unserialize($entry->timing);
  $timing_total = $timing['total'];
  $timing_bootstrap = $timing['bootstrap'];
  $timing_request = $timing['total'] - $timing['bootstrap'];
  $timing_info = "$timing_total ms (total), $timing_bootstrap ms (bootstrap), $timing_request ms (request)";

  $rows = array();
  $req_link = $entry->request_uri;
  $rows[] = array( array('data' => t('URI'), 'header' => TRUE), l($req_link, $req_link));
  $rows[] = array( array('data' => t('Timestamp'), 'header' => TRUE), format_date($entry->timestamp, 'large'));
  $rows[] = array( array('data' => t('Duration'), 'header' => TRUE), $timing_info);
  $rows[] = array( array('data' => t('User'), 'header' => TRUE), theme('username', array('account' => $service_user)));
  $rows[] = array( array('data' => t('HTTP Status'), 'header' => TRUE), $entry->http_status);
  $rows[] = array( array('data' => t('HTTP Method'), 'header' => TRUE), $entry->http_method);
  $rows[] = array( array('data' => t('HTTP Headers'), 'header' => TRUE), _services_log_dump_wrapper($entry->http_headers));
  $rows[] = array( array('data' => t('HTTP Cookie Data'), 'header' => TRUE), _services_log_dump_wrapper($entry->http_cookies));
  $rows[] = array( array('data' => t('HTTP Request Data'), 'header' => TRUE), check_plain($entry->http_payload));
  $rows[] = array( array('data' => t('Services Endpoint'), 'header' => TRUE), $entry->endpoint_name);
  $rows[] = array( array('data' => t('Resource Callback'), 'header' => TRUE), $entry->resource_callback);
  $rows[] = array( array('data' => t('Resource Arguments'), 'header' => TRUE), _services_log_dump_wrapper($entry->resource_args));
  $rows[] = array( array('data' => t('Resource Result'), 'header' => TRUE), _services_log_dump_wrapper($entry->resource_result));
  $rows[] = array( array('data' => t('Resource Errors'), 'header' => TRUE), _services_log_dump_wrapper($entry->resource_errors));
  $rows[] = array( array('data' => t('Content Type'), 'header' => TRUE), check_plain($entry->request_content_type));
  $rows[] = array( array('data' => t('Hostname'), 'header' => TRUE), check_plain($entry->hostname));

  $build['services_log_table'] = array(
    '#theme' => 'table', 
    '#rows' => $rows
  );
  return $build;
}


/**
 * Wrap/beautify any data that will be displayed on the log entry page.
 *
 * @param $data
 *   raw data
 * @return
 *   human readable HTML of the data
 */
function _services_log_dump_wrapper($data) {
  $data = unserialize($data);
  if (function_exists('krumo') && FALSE) {
    ob_start();
    krumo($data);
    $output = ob_get_contents();
    ob_end_clean();
  }
  else {
    $output = array_filter(explode("\n", htmlentities(iconv('utf-8', 'utf-8//IGNORE', print_r($data, TRUE)), ENT_QUOTES, 'UTF-8')));
    foreach ($output as $key => $value) {
      if (str_replace('    ', '', $value) == "(") {
        $output[$key-1] .= ' (';
        unset($output[$key]);
      }
    }
    $output = str_replace('    ', '&nbsp;&nbsp;&nbsp;&nbsp;', nl2br(implode("\n", $output)));
//     $output = str_replace('    ', '&nbsp;&nbsp;&nbsp;&nbsp;', nl2br(htmlentities(iconv('utf-8', 'utf-8//IGNORE', print_r($data, TRUE)), ENT_QUOTES, 'UTF-8')));
  }
  return $output;
}

/**
 * menu callback; display a listing of log messages
 *
 * @return
 *   human-readable HTML table summary of log messages
 */
function services_log_overview() {
  $filter = services_log_build_filter_query();
  $rows = array();

  $build['services_log_filter_form'] = drupal_get_form('services_log_filter_form');

  $header = array(
    array('data' => t('Entry ID'), 'field' => 'sl.entry_id', 'sort' => 'desc'),
    array('data' => t('URI')),
    array('data' => t('Date'),     'field' => 'sl.timestamp'),
    array('data' => t('Status'),   'field' => 'sl.http_status'),
    array('data' => t('Method'),   'field' => 'sl.http_method'),
    array('data' => t('Endpoint'), 'field' => 'sl.endpoint_name'),
    array('data' => t('User'),     'field' => 'sl.uid'),
    array('data' => t('Host'),     'field' => 'sl.hostname'),
  );

  $slog_query = db_select('services_log', 'sl')
    ->extend('PagerDefault')
    ->extend('TableSort');
  $slog_query->leftJoin('users', 'u', 'sl.uid = u.uid');
  $slog_query->fields('sl', explode(',', 'request_uri,timestamp,http_status,http_reason_phrase,http_method,endpoint_name,uid,hostname,entry_id,http_headers'))
    ->addField('u', 'name');

  if (!empty($filter['where'])) {
    $slog_query->where($filter['where'], $filter['args']);
  }
  $page_size = variable_get('services_log_page_size', 50);
  try {
    $slog_result = $slog_query
      ->limit($page_size)
      ->orderByHeader($header)
      ->execute();
  } catch (PDOException $e) {
    form_set_error('where_clause', "Invalid WHERE clause: ".$e->getMessage());
    $slog_result = array();
  } 

  foreach ($slog_result as $entry) {
    $rows[] = array('data' =>
      array(
        # entry ID
        $entry->entry_id,
        # URI
        theme('services_log_uri', array('entry' => $entry)),
        # date
        format_date($entry->timestamp, 'small'),
        # status
        theme('http_status', array('entry' => $entry)),
        # method
        $entry->http_method,
        # endpoint
        $entry->endpoint_name,
        # user
        theme('username', array('account' => $entry)),
        # host
        check_plain($entry->hostname),
      ),
      // Attributes for tr
      #'class' => "dblog-". preg_replace('/[^a-z]/i', '-', $dblog->type) .' '. $dblog->severity
    );
  }

  if (!$rows) {
    #$rows[] = array(array('data' => t('No log messages available.'), 'colspan' => 8));
  }

  $build['services_log_table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#attributes' => array(),
    '#empty' => t('No log messages available.'),
  );
  $build['services_log_pager'] = array('#theme' => 'pager',);

  return $build;
}

/**
 * Shorten a link for display on the services log report page.
 *
 * @param $uri
 *   a full URI
 * @return
 *   HTML link with the shortened uri as its text
 */
function _services_log_shorten_link($uri) {
  $parsed_uri = parse_url($uri);
  if ($parsed_uri === FALSE) {
    $link_text = preg_replace('!.*://[^/]*!', '', $uri);
  }
  else {
    $link_text = $parsed_uri['path'];
    if (isset($parsed_uri['query'])) {
      $link_text .= '?' . $parsed_uri['query'];
    }
    if (isset($parsed_uri['fragment'])) {
      $link_text .= '#' . $parsed_uri['fragment'];
    }
  }

  return $link_text;
}

/**
 * Build query for services_log report filters based on session data.
 *
 * @return
 *   array containing SQL WHERE clause and arguments
 *     array('where' => 'foo = %d, 'args' => array(23));
 */
function services_log_build_filter_query() {
  if (empty($_SESSION['services_log_overview_filter'])) {
    return;
  }

  $filters = services_log_filters();

  // Build query
  $where = $args = array();
  foreach ($_SESSION['services_log_overview_filter'] as $key => $filter) {
    $filter_where = array();

    if ($key == 'where_clause') {
      $filter_where[] = $filter;
    }
    else {
      # If $filter isn't an array, it's a string to search for.
      if (!is_array($filter) && !empty($filter)) {
        $filter = array("%$filter%");
      }
      foreach ($filter as $value) {
        if (isset($filters[$key]['where'])) {
          $filter_where[] = $filters[$key]['where'];
          $args[] = $value;
        }
      }
    }
    if (!empty($filter_where)) {
      $where[] = '('. implode(' OR ', $filter_where) .')';
    }
  }
  $where = !empty($where) ? implode(' AND ', $where) : '';

  return array(
      'where' => $where,
      'args' => $args,
  );
}

/**
 * List services log filters.
 *
 * @return
 *   array containing services log filters
 */
function services_log_filters() {
  $filters = array();

  $filters['uri'] = array(
      'type' => 'string',
      'title' => t('Match URI:'),
      'where' => 'sl.request_uri LIKE ?',
  );

  $filters['payload'] = array(
      'type' => 'string',
      'title' => t('Match Payload:'),
      'where' => 'sl.http_payload LIKE ?',
  );

  $filters['response'] = array(
      'type' => 'string',
      'title' => t('Match Response:'),
      'where' => 'sl.resource_result LIKE ?',
  );

  # HTTP status
  $http_status_query = db_query('SELECT DISTINCT(sl.http_status) FROM {services_log} sl');
  $http_statuses = array();
  foreach ($http_status_query as $http_status) {
    $status_code = $http_status->http_status;
    $http_statuses[$status_code] = $status_code;
  }

  # add a default so that the form renders correctly
  if (empty($http_statuses)) {
    $http_statuses[200] = 200;
  }
  asort($http_statuses);
  $filters['http_status'] = array(
      'type' => 'select',
      'title' => t('HTTP Status'),
      'where' =>  'sl.http_status = ?',
      'options' => $http_statuses,
  );

  # HTTP methods
  $http_method_query = db_query('SELECT DISTINCT(sl.http_method) FROM {services_log} sl');
  $http_methods = array();
  foreach ($http_method_query as $http_method) {
    $method_name = $http_method->http_method;
    $http_methods[$method_name] = $method_name;
  }

  # add a default so that the form renders correctly
  if (empty($http_methods)) {
    $http_methods['GET'] = 'GET';
  }
  asort($http_methods);
  $filters['http_method'] = array(
      'type' => 'select',
      'title' => t('HTTP Method'),
      'where' =>  'sl.http_method = ?',
      'options' => $http_methods,
  );

  # user
  $users_query = db_query('SELECT DISTINCT(u.uid), u.name FROM {services_log} sl JOIN {users} u ON sl.uid = u.uid');
  $users = array('0' => 'Anonymous');
  foreach ($users_query as $user_data) {
    if ($user_data->uid == 0) {
      $user_data->name = 'Anonymous';
    }
    $users[$user_data->uid] = $user_data->name;
  }

  # add a default so that the form renders correctly
  if (empty($users)) {
    $users['0'] = 'Anonymous';
  }
  asort($users);
  $filters['user'] = array(
      'type' => 'select',
      'title' => t('User'),
      'where' => 'sl.uid = ?',
      'options' => $users,
  );

  # hostname
  $hosts = array();
  $host_query = db_query('SELECT DISTINCT(sl.hostname) FROM {services_log} sl');
  foreach ($host_query as $host_data) {
    $hosts[$host_data->hostname] = $host_data->hostname;
  }

  # add a default so that the form renders correctly
  if (empty($hosts)) {
    $hosts['127.0.0.1'] = '127.0.0.1';
  }
  asort($hosts);
  $filters['host'] = array(
      'type' => 'select',
      'title' => t('Host'),
      'where' => 'sl.hostname = ?',
      'options' => $hosts,
  );


  $filters['where_clause'] = array(
      'type' => 'string',
      'title' => t('SQL WHERE clause:'),
      'where' => '?',
  );

  return $filters;
}


/**
 * Return form for services log report filters.
 *
 * @ingroup forms
 * @see services_log_filter_form_submit()
 * @see services_log_filter_form_validate()
 */
function services_log_filter_form() {
  $session = &$_SESSION['services_log_overview_filter'];
  $session = is_array($session) ? $session : array();
  $filters = services_log_filters();

  $form['filters'] = array(
    '#type' => 'fieldset',
    '#title' => t('Filter log messages'),
    '#collapsible' => TRUE,
    '#collapsed' => empty($session),
    '#attributes' => array('class' => array('services-log-fieldset') ),
  );
  $prev_type = '';
  foreach ($filters as $key => $filter) {
    $extra_style = 'none';
    if ($prev_type != $filter['type']) {
      $extra_style = 'left';
    }
    $form['filters']['status'][] = array(
      '#type' => 'markup',
      '#markup' => "<div style='clear: $extra_style'></div>",
    );
    $prev_type = $filter['type'];

    switch ($filter['type']) {
      case 'select':
        $form['filters']['status'][$key] = array(
          '#title' => $filter['title'],
          '#type' => 'select',
          '#multiple' => TRUE,
          '#size' => 8,
          '#options' => $filter['options'],
        );
        if (!empty($session[$key])) {
          $form['filters']['status'][$key]['#default_value'] = $session[$key];
        }
        break;
      case 'string':
        $form['filters']['status'][$key] = array(
          '#title' => $filter['title'],
          '#type' => 'textfield',
          '#size' => 32,
        );
        if (!empty($session[$key])) {
          $form['filters']['status'][$key]['#default_value'] = $session[$key];
        }
        break;
    }
  }
  $form['filters']['status']['where_clause']['#autocomplete_path'] = 'admin/reports/services_log/where_autocomplete';
  $form['filters']['status']['where_clause']['#description'] = 'Enter a SQL WHERE clause.  Type "sl." to autocomplete database field names.';
  $form['filters']['status'][] = array(
    '#type' => 'markup',
    '#markup' => "<div style='clear: $extra_style'></div>",
  );

  $form['filters']['actions'] = array(
    '#type' => 'actions',
    '#attributes' => array('class' => array('container-inline')),
  );
  $form['filters']['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Filter'),
  );
  if (!empty($session)) {
    $form['filters']['actions']['reset'] = array(
      '#type' => 'submit',
      '#value' => t('Reset')
    );
  }

  return $form;
}

/**
 * Provide autocomplete suggestions for Services Log WHERE clauses
 */
function services_log_where_autocomplete($input) {
  list($last, $rest) = explode(' ', strrev($input), 2);
  $rest = strrev($rest);
  $last = strrev($last);
  if (!empty($rest)) {
    $rest .= ' ';
  }
  $matches = array();
  foreach (services_log_column_names() as $col_name) {
    if (stripos($col_name, $last) === 0) {
      $matches["$rest$col_name"] = $col_name;
    }
  }

  drupal_json_output($matches);
}

/**
 * Return a list of columns in the services_log table.
 **/
function services_log_column_names() {
  $schema = drupal_get_schema_unprocessed('services_log');
  $columns = array_keys($schema['services_log']['fields']);
  foreach ($columns as &$column) {
    $column = "sl.$column";
  }
  return $columns;
}

/**
 * Validate result from services log report filter form.
 */
function services_log_filter_form_validate($form, &$form_state) {
  foreach (array_keys(services_log_filters()) as $filter_name) {
    if (!empty($form_state['values'][$filter_name])) {
      return;
    }
  }
  form_set_error('type', t('Please select something to filter by.'));
}

/**
 * Process result from services log report filter form.
 */
function services_log_filter_form_submit($form, &$form_state) {
  $op = $form_state['values']['op'];
  $filters = services_log_filters();
  $_SESSION['services_log_overview_filter'] = array();
  switch ($op) {
    case t('Filter'):
      foreach ($filters as $name => $filter) {
        if (isset($form_state['values'][$name]) && !empty($form_state['values'][$name])) {
          $_SESSION['services_log_overview_filter'][$name] = $form_state['values'][$name];
        }
      }
      break;
    case t('Reset'):
      break;
  }
  return 'admin/reports/services_log';
}

/**
 * Build the Services Log admin form.
 */
function services_log_admin_form() {
  $form = array();

  $form['services_log_enable'] = array(
      '#type'           => 'checkbox',
      '#title'          => t('Enable Services logging'),
      '#description'    => t('No Services requests will be logged unless this option is enabled.'),
      '#default_value'  => variable_get('services_log_enable', 1),
  );

  $form['services_log_page_size'] = array(
      '#type'           => 'textfield',
      '#title'          => t('Entries per overview page'),
      '#description'    => t('Display this number of entries on the Services Log overview page.'),
      '#default_value'  => variable_get('services_log_page_size', 50),
  );

  $form['services_log_sanitize'] = array(
      '#type'  => 'checkboxes',
      '#title' => t('Sanitize User Data (not yet implemented)'),
      '#description' => t('Attempt to sanitize passwords in POST data.'),
      '#options' => array(
        'before_db' => t('Sanitize data when writing entries to the database.'),
        'after_db' => t('Sanitize data when displaying log entries.'),
      ),
      '#default_value' => variable_get('services_log_sanitize', array()),
  );

  $form['services_log_max_entries'] = array(
      '#type'           => 'textfield',
      '#title'          => t('Maximum Log Size'),
      '#size'           => 10,
      '#maximum_length' => 20,
      '#description'    => t('Automatically delete the oldest log entries when the count exceeds this number.  If this number is negative, entries will not be deleted.'),
      '#default_value'  => variable_get('services_log_max_entries', -1),
  );

  return system_settings_form($form);
}

/**
 * Validate the Services Log admin form.
 */
function services_log_admin_form_validate($form, &$form_state) {
  if (!is_numeric($form_state['values']['services_log_max_entries'])) {
    form_set_error('services_log_max_entries', "'Maximum Log Size' must be an integer.");
  }
}

/**
 * return html for an http status
 */
function theme_http_status($variables) {

  $reason_phrase = $variables['entry']->http_reason_phrase;
  $reason_phrase = check_plain($reason_phrase);
  $http_status   = $variables['entry']->http_status;

  $output  = '<div title="';
  $output .= $reason_phrase;
  $output .= '">';
  $output .= $http_status;
  $output .= '</div>';

  return $output;
}

/**
 * return html for a uri on the overview page
 */
function theme_services_log_uri($variables) {

  $entry     = $variables['entry'];
  $link_text = _services_log_shorten_link($entry->request_uri);
  $link_text = check_plain($link_text);

  $output = l($link_text, 'admin/reports/services_log/' . $entry->entry_id, array('html' => TRUE));

  return $output;
        
}


# vim: expandtab shiftwidth=2 tabstop=2 ft=php:
