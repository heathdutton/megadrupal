<?php

/**
 * @file
 * Administrative pages
 * 
 * Copyright (c) 2011-2012 DOCOMO Innovations, Inc.
 */

/**
 * Build the listing page for watchdog messages
 */
function couchbase_watchdog_list() {
  $icons = array(
    WATCHDOG_DEBUG    => '',
    WATCHDOG_INFO     => '',
    WATCHDOG_NOTICE   => '',
    WATCHDOG_WARNING  => theme('image', array('path' => 'misc/watchdog-warning.png', 'alt' => t('warning'), 'title' => t('warning'))),
    WATCHDOG_ERROR    => theme('image', array('path' => 'misc/watchdog-error.png', 'alt' => t('error'), 'title' => t('error'))),
    WATCHDOG_CRITICAL => theme('image', array('path' => 'misc/watchdog-error.png', 'alt' => t('critical'), 'title' => t('critical'))),
    WATCHDOG_ALERT    => theme('image', array('path' => 'misc/watchdog-error.png', 'alt' => t('alert'), 'title' => t('alert'))),
    WATCHDOG_EMERGENCY    => theme('image', array('path' => 'misc/watchdog-error.png', 'alt' => t('emergency'), 'title' => t('emergency'))),
  );
  $classes = array(
    WATCHDOG_DEBUG     => 'dblog-debug',
    WATCHDOG_INFO      => 'dblog-info',
    WATCHDOG_NOTICE    => 'dblog-notice',
    WATCHDOG_WARNING   => 'dblog-warning',
    WATCHDOG_ERROR     => 'dblog-error',
    WATCHDOG_CRITICAL  => 'dblog-critical',
    WATCHDOG_ALERT     => 'dblog-alert',
    WATCHDOG_EMERGENCY => 'dblog-emerg',
  );
  
  // initialize the pager stuff
  global $pager_page_array, $pager_total, $pager_total_items, $pager_limits;
  $page = isset($_GET['page']) ? $_GET['page'] : '';
  $pager_page_array = explode(',', $page);
  $on_page = $pager_page_array[0];
  $skip = $on_page * COUCHBASE_WATCHDOG_PER_PAGE;
  // get the documents from couch
  $docs = couchbase_watchdog_get_docs($skip);
  $rows = array();
  if (isset($docs['rows'])) {
    foreach ($docs['rows'] as $doc) {
      $message = truncate_utf8(strip_tags(t($doc['value']['message'], (array)$doc['value']['variables'])), 100, TRUE, TRUE);
      $rows[] = array('data' =>
        array(
          $icons[$doc['value']['severity']], // Icon
          $doc['value']['type'],
          empty($doc['value']['timestamp']) ? '' : format_date($doc['value']['timestamp'], 'short'),
          l($message, 'admin/reports/couchbase-watchdog/' . $doc['id']),
          theme('username', array('account' => $doc['value']['user'])),
        ),
        'class' => array(drupal_html_class('dblog-' . $doc['value']['type']), $classes[$doc['value']['severity']]),
      );    
    }
  }
  
  $build['couchbase_watchdog_filter_form'] = drupal_get_form('couchbase_watchdog_filter_form');
  $build['couchbase_watchdog_log_form'] = drupal_get_form('couchbase_watchdog_clear_log_form');
  $build['couchbase_watchdog_table'] = array(
    '#theme' => 'table',
    '#header' => couchbase_watchdog_header(),
    '#rows' => $rows,
    '#empty' => t('No log messages available.'),
  );
  
  // Add the pager.
  if ($on_page > 0 || count($rows) >= COUCHBASE_WATCHDOG_PER_PAGE) {
    $pager_total_items[0] = $docs['total_rows'];
    $pager_total[0] = ceil($pager_total_items[0] / COUCHBASE_WATCHDOG_PER_PAGE);
    $pager_page_array[0] = max(0, min((int) $pager_page_array[0], ((int)$pager_total[0]) - 1));
    $pager_limits[0] = COUCHBASE_WATCHDOG_PER_PAGE;
    $build['pager'] = array(
      '#theme' => 'pager',
    );
  }
  return $build;
}


/**
 * Watchdog entry detail page
 */
function couchbase_watchdog_detail($id) {
  $severity = watchdog_severity_levels();
  
  $cb = couchbase('default', COUCHBASE_WATCHDOG_DB);
  $watchdog_details = json_decode($cb->get($id), TRUE);
  if ($watchdog_details) {
    // theme out the table
    $rows = array(
      array(
        array('data' => t('Type'), 'header' => TRUE),
        t($watchdog_details['type']),
      ),
      array(
        array('data' => t('Date'), 'header' => TRUE),
        format_date($watchdog_details['timestamp'], 'long'),
      ),
      array(
        array('data' => t('User'), 'header' => TRUE),
        theme('username', array('account' => $watchdog_details['user'])),
      ),
      array(
        array('data' => t('Referrer'), 'header' => TRUE),
        l($watchdog_details['referer'], $watchdog_details['referer']),
      ),
      array(
        array('data' => t('Message'), 'header' => TRUE),
        t($watchdog_details['message'], (array)$watchdog_details['variables']),
      ),
      array(
        array('data' => t('Severity'), 'header' => TRUE),
        $severity[$watchdog_details['severity']],
      ),
      array(
        array('data' => t('Hostname'), 'header' => TRUE),
        check_plain($watchdog_details['ip']),
      ),
      array(
        array('data' => t('Operations'), 'header' => TRUE),
        $watchdog_details['link'],
      ),
    );
    $build['dblog_table'] = array(
      '#theme' => 'table',
      '#rows' => $rows,
      '#attributes' => array('class' => array('dblog-event')),
    );
    return $build;
  }
  else {
    return '';
  }
}

/**
 * Helper function determines the available filters 
 * the overview page can filter by
 */
function couchbase_filters() {
  $filters = array();
  $types = _couchbase_watchdog_get_message_types();
  if (count($types)) {
    $filters['type'] = array(
      'title' => t('Type'),
      'options' => $types,
    );
  }
  $filters['severity'] = array(
    'title' => t('Severity'),
    'options' => watchdog_severity_levels(),
  );
  return $filters;
}


/**
 * Watchdog filter form
 */
function couchbase_watchdog_filter_form($form) {
  $filters = couchbase_filters();
  
  $form['filters'] = array(
    '#type' => 'fieldset',
    '#title' => t('Filter log messages'),
    '#collapsible' => TRUE,
    '#collapsed' => empty($_SESSION['couchbase_watchdog_filter']),
  );
  foreach ($filters as $key => $filter) {
    $form['filters']['status'][$key] = array(
      '#title' => check_plain($filter['title']),
      '#type' => 'select',
      '#multiple' => TRUE,
      '#size' => 8,
      '#options' => $filter['options'],
    );
    if (!empty($_SESSION['couchbase_watchdog_filter'][$key])) {
      $form['filters']['status'][$key]['#default_value'] = $_SESSION['couchbase_watchdog_filter'][$key];
    }
  }  
  $form['filters']['buttons']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Filter'),
  );
  if (!empty($_SESSION['couchbase_watchdog_filter'])) {
    $form['filters']['buttons']['reset'] = array(
        '#type' => 'submit',
        '#value' => t('Reset')
    );
  }
  return $form;
}

/**
 * Filter validation
 */
function couchbase_watchdog_filter_form_validate($form, &$form_state) {
  if ($form_state['values']['op'] == t('Filter') && empty($form_state['values']['type']) && empty($form_state['values']['severity'])) {
    form_set_error('type', t('You must select something to filter by.'));
  }
}

/**
 * Filter submit
 */
function couchbase_watchdog_filter_form_submit($form, &$form_state) {
  $op = $form_state['values']['op'];
  $filters = couchbase_filters();
  switch ($op) {
    case t('Filter'):
      foreach ($filters as $name => $filter) {
        if (isset($form_state['values'][$name])) {
          $_SESSION['couchbase_watchdog_filter'][$name] = $form_state['values'][$name];
        }
      }
      break;
    case t('Reset'):
      $_SESSION['couchbase_watchdog_filter'] = array();
      break;
  }
  return 'admin/reports/couchbase-watchdog';
}

/**
 * Clear the watchdog documents from couch
 */
function couchbase_watchdog_clear_log_form($form) {
  $form['couchbase_watchdog_clear'] = array(
    '#type' => 'fieldset',
    '#title' => t('Clear log messages'),
    '#description' => t('This will permanently remove the log messages from the Couchbase.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['couchbase_watchdog_clear']['clear'] = array(
    '#type' => 'submit',
    '#value' => t('Clear log messages'),
    '#submit' => array('couchbase_watchdog_clear_log_form_submit'),
  );
  
  return $form;
}

/**
 * Clear the watchdog database 
 */
function couchbase_watchdog_clear_log_form_submit($form, &$form_values) {
  $_SESSION['couchbase_watchdog_filter'] = array();
  $cb = couchbase('default', COUCHBASE_WATCHDOG_DB);
  $cb->flush();
}

/**
 * Get the documents via a couchbase view.
 */
function couchbase_watchdog_get_docs($skip) {
  $cb = couchbase('default', COUCHBASE_WATCHDOG_DB);
  $view = _couchbase_watchdog_init_order();
  $descending = (_couchbase_watchdog_init_sort()) ? 'true' : 'false';
  $docs = array();
  
  if (!empty($_SESSION['couchbase_watchdog_filter'])) {
    $types = $_SESSION['couchbase_watchdog_filter']['type'] ? $_SESSION['couchbase_watchdog_filter']['type'] : NULL;
    $severities = $_SESSION['couchbase_watchdog_filter']['severity'] ? $_SESSION['couchbase_watchdog_filter']['severity'] : NULL;
    $keys = array();
    if ($types != NULL && $severities != NULL) {
      $view = 'list_by_type_severity';
      // build each possible key combination based
      // on the user selection
      foreach ($types as $type) {
        foreach ($severities as $severity) {
          $keys[] = array($type, (int) $severity);
        }
      }
    }
    elseif ($types != NULL) {
      $view = 'list_by_type';
      $keys = array_values($types);
    }
    elseif ($severities != NULL) {
      $view = 'list_by_severity';
      // iterate and cast the severity into an integer
      foreach ($severities as $severity) {
        $keys[] = (int) $severity;
      }
    }
    
    // @todo Remove use of cb_rest once couchbase->view supports posting keys
    try {
      $cb_rest = couchbase_rest();
      $docs = $cb_rest->getView(COUCHBASE_WATCHDOG_DB, 'watchdog_views', $view, $keys, $descending, '', '', COUCHBASE_WATCHDOG_PER_PAGE, array('skip=' . $skip));
    }
    catch (CouchbaseRestException $re) {
      drupal_set_message(t('Error retrieving logs from CouchBase: @error', array('@error' => $re)));
    }
  }
  else {
    // no complex filtering needed, so just execute the view and let couch handle everything
    $docs = $cb->view('_design/watchdog_views/_view', "$view?descending={$descending}&skip={$skip}&limit=" . COUCHBASE_WATCHDOG_PER_PAGE);
  }
  return $docs;
}

/**
 * Get the available message types stored in couch
 */
function _couchbase_watchdog_get_message_types() {
  $types = array();
  $cb = couchbase('default', COUCHBASE_WATCHDOG_DB);
  $results = $cb->view('_design/watchdog_views/_view', 'distinct_types?group=true');
  foreach ($results['rows'] as $result) {
    if (isset($result['key'])) {
      $types[$result['key']] = $result['key'];
    }
  }
  return $types;
}

/**
 * Helper function that determines which
 * couch view to query by
 */
function _couchbase_watchdog_init_order() {
  $header = couchbase_watchdog_header();
  $order = tablesort_get_order($header);
  // set the default list
  $view = "list_by_date";
  if (isset($order['sql'])) {
    $view = $order['sql'];
  }
  return $view;
}

/**
 * Helper function that determines which
 * sort order
 */
function _couchbase_watchdog_init_sort() {
  $header = couchbase_watchdog_header();
  $sort = tablesort_get_sort($header);
  $descending = TRUE;
  $sort = tablesort_get_sort($header);
  if (isset($sort)) {
    if ($sort == 'asc') {
      $descending = FALSE;
    }
  }
  return $descending;
}

/**
 * Function build the table header
 */
function couchbase_watchdog_header() {
  $header = &drupal_static(__FUNCTION__);
  if (count($header) == 0) {
    $header = array(
      '', 
      array('data' => t('Type'), 'field' => 'list_by_type'),
      array('data' => t('Date'), 'field' => 'list_by_date', 'sort' => 'desc'),
      t('Message'),
      array('data' => t('User'), 'field' => 'list_by_user'),
    );
  }
  return $header;
}
