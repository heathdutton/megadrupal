<?php

/**
 * @file
 * Contains pages, forms and callbacks for Account Sentinel.
 */

/**
 * Generates the settings page.
 *
 * @return array
 *   The settings form.
 */
function account_sentinel_page_settings() {
  $roles = array();
  foreach (user_roles(TRUE) as $rid => $role) {
    $roles[$rid] = check_plain($role);
  }

  // Monitored roles.
  $form['account_sentinel_monitored_roles'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Monitored roles'),
    '#description' => t("Mark which roles' accounts should be monitored."),
    '#options' => $roles,
    '#default_value' => account_sentinel_get_monitored_roles(),
  );

  // E-mail notification recipients.
  $form['account_sentinel_email_to'] = array(
    '#type' => 'textfield',
    '#title' => t('Recipient(s) of notification e-mails'),
    '#description' => t('Comma separated list of e-mail addresses to notify of changes to monitored accounts.'),
    '#default_value' => variable_get('account_sentinel_email_to', ''),
  );

  // Cron settings.
  $cron_key = account_sentinel_get_cron_key();
  $cron_url = url(
    'system/account-sentinel-cron',
    array(
      'absolute' => TRUE,
      'query' => array('key' => $cron_key),
    )
  );
  $cron_reset_url = url(
    'system/account-sentinel-reset-cron-key',
    array('query' => array('token' => drupal_get_token($cron_key)))
  );
  if (($cron_last = variable_get('account_sentinel_audit_last', 0)) != 0) {
    $cron_status = t('Last run: %audit-last ago.', array('%audit-last' => format_interval(REQUEST_TIME - $cron_last)));
  }
  else {
    $cron_status = t('The audit has not been run yet!');
  }

  $form['account_sentinel_cron_method'] = array(
    '#type' => 'radios',
    '#title' => t('Automatic DB check method'),
    '#description' => t(
      'The custom cron URL is <a href="!cron_url" id="cron-link">!cron_url</a>.<br />You can request a new URL by clicking <a href="!cron_reset_url" id="cron-reset-link">here</a>. This will invalidate the current URL!',
      array(
        '!cron_url' => $cron_url,
        '!cron_reset_url' => $cron_reset_url,
      )
    ) . '<br /><strong>' . $cron_status . '</strong>',
    '#options' => array(
      'drupal' => t("Run with Drupal's inbuilt cron"),
      'custom' => t("Use Account Sentinel's own cron handler"),
    ),
    '#default_value' => variable_get('account_sentinel_cron_method', 'drupal'),
  );

  // Attach JavaScript.
  $form['#attached']['js'] = array(
    drupal_get_path('module', 'account_sentinel') . '/js/settings.js',
  );

  // Add submit handler.
  $form['#submit'][] = 'account_sentinel_page_settings_submit';

  return system_settings_form($form);
}

/**
 * Handles the settings form's validation.
 *
 * @param array $form
 *   The form's structure.
 * @param array $form_state
 *   The array containing the form's state.
 */
function account_sentinel_page_settings_validate(array $form, array &$form_state) {
  // E-mail notification recipient validation.
  $to = $form_state['values']['account_sentinel_email_to'];
  $to_valid = TRUE;
  $addresses = explode(',', $to);
  foreach ($addresses as $address) {
    $address = trim($address);
    if (strpos($address, '<') !== FALSE) {
      $start = strpos($address, '<') + 1;
      $end = strpos($address, '>');
      if ($end === FALSE) {
        $to_valid = FALSE;
        break;
      }
      $address = substr($address, $start, $end - $start);
    }
    if ($address != '' && !valid_email_address($address)) {
      $to_valid = FALSE;
      break;
    }
  }
  if (!$to_valid) {
    form_set_error('account_sentinel_email_to', t('Invalid e-mail address!'));
  }
}

/**
 * Handles the settings form's submission.
 *
 * @param array $form
 *   The form's structure.
 * @param array $form_state
 *   The array containing the form's state.
 */
function account_sentinel_page_settings_submit(array $form, array &$form_state) {
  // Only store IDs of monitored roles.
  $roles_new = &$form_state['values']['account_sentinel_monitored_roles'];
  $roles_new = array_keys(array_filter($roles_new));
  $roles_old = account_sentinel_get_monitored_roles();
  $roles_added = array_diff($roles_new, $roles_old);
  $roles_removed = array_diff($roles_old, $roles_new);

  // Check whether roles have changed.
  if (!empty($roles_added) || !empty($roles_removed)) {
    watchdog('account_sentinel', 'Monitored roles modified.');

    // Invoke snapshot integrity check.
    module_load_include('inc', 'account_sentinel', 'account_sentinel.audit');
    account_sentinel_audit_existence();
    account_sentinel_audit_integrity();

    // Delete unmonitored snapshots, create newly monitored users' snapshots.
    account_sentinel_rebuild_snapshots($roles_old, $roles_new);
  }
}

/**
 * Generates the report page.
 *
 * @return array
 *   The report page.
 */
function account_sentinel_page_report() {
  $filters = isset($_SESSION['account_sentinel_report_filter']) ? $_SESSION['account_sentinel_report_filter'] : array();

  $output = array();
  $output['account_sentinel_report_description'] = array(
    '#type' => 'markup',
    '#markup' => '<p>' . t(
      'Here you can review the changes that were made to accounts monitored by Account Sentinel. To configure Account Sentinel <a href="!configure">click here</a>.',
      array('!configure' => url('admin/config/system/account-sentinel'))
    ) . '</p>',
  );
  $output['account_sentinel_report_filter'] = drupal_get_form('account_sentinel_page_report_filter_form');

  // Query the events.
  $events_per_page = 15;
  $fields = array(
    'eid',
    'uid',
    'origin',
    'type',
    'data',
    'by_uid',
    'ip',
    'timestamp',
  );
  $select = db_select('account_sentinel_logs', 'l')->extend('PagerDefault');
  $select->fields('l', $fields)
    ->orderBy('timestamp', 'DESC')
    ->orderBy('eid', 'DESC')
    ->limit($events_per_page);

  foreach ($filters as $filter => $filter_value) {
    if (in_array($filter, $fields)) {
      $select->condition($filter, $filter_value);
    }
    elseif ($filter == 'before') {
      $select->condition('timestamp', $filter_value, '<=');
    }
    elseif ($filter == 'after') {
      $select->condition('timestamp', $filter_value, '>=');
    }
  }

  $events_queried = $select->execute();

  // Fill the rows.
  $rows = array();
  while (($row = $events_queried->fetchAssoc()) !== FALSE) {
    // Format date.
    $row['timestamp'] = format_date($row['timestamp']);

    // Format account names.
    if ($row['origin'] == ACCOUNT_SENTINEL_EVENT_ORIGIN_DB_CHECK) {
      $row['by'] = '<strong>' . t('DB alteration') . '</strong>';
    }
    else {
      $row['by'] = theme('account_sentinel_username', array('uid' => $row['by_uid']));
    }
    $row['account'] = theme('account_sentinel_username', array('uid' => $row['uid']));

    // Add event message.
    $row['data'] = unserialize($row['data']);
    $row['message'] = account_sentinel_get_event_message($row['type'], $row['data']);

    // Add classes.
    $origin_class = 'origin--' . str_replace('_', '-', $row['origin']);
    $type_class = 'type--' . str_replace('_', '-', $row['type']);

    // Insert row.
    $rows[] = array(
      'data' => array(
        $row['account'],
        $row['message'],
        $row['timestamp'],
        array(
          'data' => $row['by'],
          'class' => array('col--by'),
        ),
        $row['ip'],
      ),
      'class' => array($type_class, $origin_class),
    );
  }

  $output['account_sentinel_report_table'] = array(
    '#theme' => 'table',
    '#header' => array(
      t('Account'),
      t('Event details'),
      t('Date'),
      t('By'),
      t('IP'),
    ),
    '#rows' => $rows,
    '#attributes' => array(
      'class' => array('account-sentinel-table'),
    ),
  );
  $output['account_sentinel_report_pager'] = array('#theme' => 'pager');
  $output['#attached']['css'] = array(
    drupal_get_path('module', 'account_sentinel') . '/css/report.css',
  );
  return $output;
}

/**
 * Generates the filter form for the report page.
 *
 * @return array
 *   The filter form.
 */
function account_sentinel_page_report_filter_form() {
  $session = isset($_SESSION['account_sentinel_report_filter']) ? $_SESSION['account_sentinel_report_filter'] : array();

  $form['account_sentinel_filter'] = array(
    '#type' => 'fieldset',
    '#title' => t('Filter events'),
    '#collapsible' => TRUE,
    '#collapsed' => empty($session),
  );
  $filter = &$form['account_sentinel_filter'];

  $event_types = account_sentinel_event_type_strings();
  $origin_types = account_sentinel_event_origin_strings();
  $event_types = array_map('ucfirst', $event_types);
  $origin_types = array_map('ucfirst', $origin_types);
  asort($event_types);
  asort($origin_types);

  $weight = 0;
  $filter['fields'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('account-sentinel-filter-fields')),
  );
  $filter['fields']['subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject'),
    '#description' => t("The changed user's name, or alternatively their UID in \"#UID\" format."),
    '#size' => 20,
    '#maxlength' => 60,
    '#autocomplete_path' => 'user/autocomplete',
    '#default_value' => isset($session['subject']) ? $session['subject'] : '',
    '#weight' => $weight++,
  );

  $filter['fields']['ip'] = array(
    '#type' => 'textfield',
    '#title' => t('IP address'),
    '#description' => t('This field is irrelevant in cases when the database was modified.'),
    '#size' => 20,
    '#maxlength' => 45,
    '#default_value' => isset($session['ip']) ? $session['ip'] : '',
    '#weight' => $weight++,
  );

  $filter['fields']['origin'] = array(
    '#type' => 'select',
    '#multiple' => TRUE,
    '#title' => t('Event origin'),
    '#required' => FALSE,
    '#options' => $origin_types,
    '#size' => min(5, count($origin_types)),
    '#weight' => $weight++,
    '#default_value' => isset($session['origin']) ? $session['origin'] : array(),
  );

  $filter['fields']['type'] = array(
    '#type' => 'select',
    '#multiple' => TRUE,
    '#title' => t('Type of event'),
    '#required' => FALSE,
    '#options' => $event_types,
    '#size' => min(5, count($event_types)),
    '#weight' => $weight++,
    '#default_value' => isset($session['type']) ? $session['type'] : array(),
  );

  $filter['fields']['after'] = array(
    '#type' => 'date',
    '#title' => t('Occurred after'),
    '#required' => FALSE,
    '#weight' => $weight++,
    '#default_value' => isset($session['after']) ? account_sentinel_time_to_array($session['after']) : account_sentinel_time_to_array(0),
  );

  $filter['fields']['before'] = array(
    '#type' => 'date',
    '#title' => t('Occurred before'),
    '#required' => FALSE,
    '#weight' => $weight++,
    '#default_value' => isset($session['before']) ? account_sentinel_time_to_array($session['before']) : account_sentinel_time_to_array(time()),
  );

  $filter['actions'] = array(
    '#type' => 'actions',
    '#attributes' => array('class' => array('account-sentinel-filter-actions')),
  );
  $filter['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => (!empty($session) ? t('Refine') : t('Filter')),
  );

  if (!empty($session)) {
    $filter['actions']['reset'] = array(
      '#type' => 'submit',
      '#value' => t('Reset'),
    );
  }

  return $form;
}

/**
 * Converts a UNIX timestamp to a Form API date array.
 *
 * @param int $time
 *   UNIX timestamp.
 *
 * @return array
 *   Date array.
 */
function account_sentinel_time_to_array($time = 0) {
  return array(
    'year' => date('Y', $time),
    'month' => date('m', $time),
    'day' => date('d', $time),
  );
}

/**
 * Converts a Form API's date field's value to UNIX timestamp.
 *
 * @param array $date
 *   Date array.
 * @param bool $end_of_day
 *   If set to true, the last second of the day will be returned.
 *
 * @return int
 *   UNIX timestamp.
 */
function account_sentinel_array_to_time(array $date, $end_of_day = FALSE) {
  $time_of_day = $end_of_day ? '23:59:59' : '00:00:00';
  return strtotime($date['year'] . '-' . $date['month'] . '-' . $date['day'] . ' ' . $time_of_day);
}

/**
 * Handles the filter form's submission.
 *
 * @param array $form
 *   The form's structure.
 * @param array $form_state
 *   The array containing the form's state.
 */
function account_sentinel_page_report_filter_form_submit(array $form, array &$form_state) {
  $session = &$_SESSION['account_sentinel_report_filter'];
  $values = $form_state['values'];

  switch ($values['op']) {
    case t('Refine'):
    case t('Filter'):
      if (($subject = trim($values['subject'])) != '') {
        $session['subject'] = $subject;
        $matches = array();
        if (preg_match('/\\#([0-9]+)/', $subject, $matches)) {
          // Store UID.
          $session['uid'] = $matches[1];
        }
        else {
          // Load by name.
          $user = user_load_by_name($subject);
          if ($user !== FALSE) {
            $session['uid'] = $user->uid;
          }
        }
      }
      if (($ip = trim($values['ip'])) != '') {
        $session['ip'] = $ip;
      }
      if (!empty($values['origin'])) {
        $session['origin'] = array_keys($values['origin']);
      }
      if (!empty($values['type'])) {
        $session['type'] = array_keys($values['type']);
      }
      if (!empty($values['after'])) {
        $session['after'] = account_sentinel_array_to_time($values['after']);
      }
      if (!empty($values['before'])) {
        $session['before'] = account_sentinel_array_to_time($values['before'], TRUE);
      }
      break;

    case t('Reset'):
      $session = array();
      break;
  }

}

/**
 * Handler for the module's own cron URL.
 */
function account_sentinel_callback_cron() {
  // Only handle if the cron_method is set to custom.
  if (variable_get('account_sentinel_cron_method', 'drupal') == 'custom') {
    // Validate access.
    $query = drupal_get_query_parameters();
    $cron_key = account_sentinel_get_cron_key();
    if (isset($query['key']) && $query['key'] == $cron_key) {
      watchdog('account_sentinel', 'Invoked database audit from URL.');
      module_load_include('inc', 'account_sentinel', 'account_sentinel.audit');
      account_sentinel_audit();
    }
    else {
      watchdog('account_sentinel', 'Invalid access to audit URL.', array(), WATCHDOG_WARNING);
    }
  }
}

/**
 * Handler for the cron key resetting URL.
 */
function account_sentinel_callback_reset_cron_key() {
  $output = array();

  // Validate access.
  $query = drupal_get_query_parameters();
  $cron_key = account_sentinel_get_cron_key();
  if (isset($query['token']) && drupal_valid_token($query['token'], $cron_key)) {
    // Reset cron key.
    $cron_key = account_sentinel_reset_cron_key();

    // Return the new URL.
    $output['url_cron'] = url(
      'system/account-sentinel-cron',
      array(
        'absolute' => TRUE,
        'query' => array('key' => $cron_key),
      )
    );
    $output['url_reset'] = url(
      'system/account-sentinel-reset-cron-key',
      array(
        'query' => array('token' => drupal_get_token($cron_key)),
      )
    );
  }

  // Output JSON if JavaScript is enabled, otherwise refresh the page.
  if (isset($query['js'])) {
    drupal_json_output($output);
  }
  else {
    drupal_goto('admin/config/system/account-sentinel');
  }
}
