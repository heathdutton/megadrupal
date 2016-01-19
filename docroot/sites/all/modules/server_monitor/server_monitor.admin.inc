<?php

/**
 * @file
 * Controls the server monitor administration screens.
 */

/**
 * System settings form for the Server Monitor module.
 */
function server_monitor_admin_form($form, &$form_state) {
  $form['general_options'] = array(
    '#type' => 'fieldset',
    '#title' => t('General Configuration'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $last_run = variable_get('server_monitor_last_run', 0) ?
    date(SERVER_MONITOR_DATE_FORMAT, variable_get('server_monitor_last_run', 0)) :
    'Never';
  $form['general_options']['server_monitor_cron'] = array(
    '#type' => 'checkbox',
    '#title' => t('Check servers status when cron runs?'),
    '#description' => t('Last run: @date', array('@date' => $last_run)),
    '#default_value' => variable_get('server_monitor_cron', 1),
  );
  $form['general_options']['server_monitor_run'] = array(
    '#type' => 'submit',
    '#value' => t('Run monitor'),
  );
  $form['general_options']['server_monitor_use_json_block'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use JSON generated block?'),
    '#description' => t('In order to prevent caching of stale monitor results you can use the more dynamic JSON generated result block.<br /><em>Warning:</em> This method results in an additional file (!file) being loaded.',
      array(
        '!file' => l(t('status.json'), 'server_monitor/status.json', array(
          'attributes' => array('target' => '_blank'),
        )),
      )),
    '#default_value' => variable_get('server_monitor_use_json_block', 0),
  );
  $form['servers'] = array(
    '#type' => 'fieldset',
    '#title' => t('Current server list'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['servers']['list'] = array(
    '#tree' => TRUE,
    '#theme' => 'server_monitor_admin_form_table',
  );
  
  $result = db_select('server_monitor')
    ->fields('server_monitor')
    ->orderBy('weight', 'ASC')
    ->execute();
  $row_knt = 0;
  $servers = array();
  $status = array(-1 => t('N/A'), 0 => t('Down'), 1 => 'Up');
  while ($row = $result->fetchAssoc()) {
    $action = l(t('edit'), 'admin/config/services/server_monitor/edit/' . $row['sid']);
    if (user_access('delete server monitor')) {
      $action .= '&nbsp;|&nbsp;';
      $action .= l(t('delete'), 'admin/config/services/server_monitor/delete/' . $row['sid']);
    }
    $last_checked = $row['timestamp'] ?
      t('Last checked on @timestamp', array(
        '@timestamp' => date(SERVER_MONITOR_DATE_FORMAT, $row['timestamp']),
      )) : 'Never checked';
    $form['servers']['list'][$row['sid']] = array(
      'name' => array('#markup' => $row['name']),
      'address' => array(
        '#markup' => ($row['method'] == 'http' || $row['method'] == 'curl') ?
        l($row['address'], $row['address'], array(
          'attributes' => array('target' => '_blank', 'title' => $last_checked),
        )) :
        $row['address'],
      ),
      'method' => array('#markup' => $row['method']),
      'status' => array('#markup' => $status[$row['status']]),
      'action' => array('#markup' => $action),
      'weight' => array(
        '#type' => 'textfield',
        '#size' => 3,
        '#default_value' => $row['weight'],
        '#attributes' => array('class' => array('rank-weight')),
      ),
    );
    $row_knt++;
  }
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  $form['reset'] = array(
    '#type' => 'button',
    '#value' => t('Cancel'),
  );
  return $form;
}

/**
 * System settings form submit function.
 */
function server_monitor_admin_form_submit($form, &$form_state) {
  switch ($form_state['clicked_button']['#value']) {
    case t('Run monitor'):
      server_monitor_check_all();
      drupal_goto('admin/config/services/server_monitor');
      break;

    case t('Save'):
      variable_set('server_monitor_cron', $form_state['values']['server_monitor_cron']);
      variable_set('server_monitor_use_json_block', $form_state['values']['server_monitor_use_json_block']);
      if (isset($form_state['values']['list'])) {
        foreach ($form_state['values']['list'] as $sid => $value) {
        $sid = db_update('server_monitor')
          ->fields(array(
          'weight' => $value['weight'],
          ))
          ->condition('sid', $sid, '=')
          ->execute();
        }
      }
      drupal_set_message('Server Monitor configuration saved.');
      break;

  }
}

/**
 * Implements the Server Monitor server edit page.
 */
function server_monitor_form($form, &$form_state, $op = NULL, $sid = NULL) {
  $edit = array(
    'name' => '',
    'address' => '',
    'method' => 'http',
    'weight' => 0,
    'server_description' => '',
    'configuration' => array('timeout' => SERVER_MONITOR_DEFAULT_TIMEOUT),
  );
  if ($op == "edit" && $sid) {
    $edit = db_select('server_monitor')
      ->fields('server_monitor')
      ->condition('sid', $sid, '=')
      ->execute()
      ->fetchAssoc();
    $edit['configuration'] = unserialize($edit['configuration']);
    $form['sid'] = array(
      '#type' => 'hidden',
      '#value' => $sid,
    );
  }
  $form['monitor-settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Server monitor settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['monitor-settings']['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Server Name'),
    '#default_value' => $edit['name'],
    '#description' => t('Choose a name for this server. This name will be used in reports and blocks.'),
    '#size' => 30,
    '#maxlength' => 32,
    '#required' => TRUE,
  );
  $form['monitor-settings']['address'] = array(
    '#type' => 'textfield',
    '#title' => t('Server Address'),
    '#default_value' => $edit['address'],
    '#description' => t('Enter the full URI or IP address, including the port number (when applicable).'),
    '#size' => 80,
    '#maxlength' => 255,
    '#required' => TRUE,
  );

  $methods = array(
    'http' => t('HTTP Request'),
    'curl' => t('cURL request'),
    'ping' => t('Ping'),
  );

  if (!function_exists('curl_init')) {
    unset($methods['curl']);
  }
  $form['monitor-settings']['method'] = array(
    '#type' => 'select',
    '#title' => t('Monitoring method'),
    '#default_value' => $edit['method'],
    '#options' => $methods,
    '#description' => t('Please select how you would like to check the server status.'),
    '#required' => TRUE,
  );
  $form['monitor-settings']['timeout'] = array(
    '#type' => 'textfield',
    '#title' => t('Timeout'),
    '#default_value' => $edit['configuration']['timeout'],
    '#size' => 3,
    '#maxlength' => 10,
    '#description' => t('Please enter the desired timeout (in seconds).'),
    '#required' => TRUE,
  );
  $form['monitor-settings']['server_description'] = array(
    '#type' => 'textarea',
    '#title' => t('Server Description'),
    '#default_value' => $edit['server_description'],
    '#description' => t('Please enter a brief description of this server.'),
    '#required' => FALSE,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );
  return $form;
}

/**
 * Implements hook_validate().
 */
function server_monitor_form_validate($form, &$form_state) {
  $values = $form_state['values'];
  // Check if server name is already taken.
  if (!isset($values['sid'])) {
    $existing_name = db_select('server_monitor')
      ->fields('server_monitor', array('name'))
      ->condition('name', $values['name'], '=')
      ->execute()
      ->fetchField();
    if ($existing_name) {
      form_set_error('name', t('A Server montior with the name %name already exists.', array('%name' => $existing_name)));
    }
  }
  if (!isset($values['sid'])) {
    $existing_name = db_select('server_monitor')
      ->fields('server_monitor', array('address', 'method'))
      ->condition('address', $values['address'], '=')
      ->condition('method', $values['method'], '=')
      ->execute()
      ->fetchField();
    if ($existing_name) {
      form_set_error('address', t('A Server montior with the same address and method already exists.'));
    }
  }
  // Check if timeout is valid.
  if (!is_numeric($values['timeout'])) {
    form_set_error('timeout', t('The timeout value must be a number (seconds).'));
  }
}

/**
 * Submit hook for the submit monitor server form.
 */
function server_monitor_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  $configuration = array('timeout' => $values['timeout']);
  if (!isset($values['sid'])) {
    $sid = db_insert('server_monitor')
      ->fields(array(
      'name' => $values['name'],
      'address' => $values['address'],
      'method' => $values['method'],
      'server_description' => $values['server_description'],
      'configuration' => serialize($configuration),
      ))->execute();
    if ($sid) {
      drupal_set_message(t('Server monitor %name has been added.', array('%name' => $values['name'])));
      watchdog('server_monitor', 'Server monitor %name has been added.', array('%name' => $values['name']));
    }
    else {
      drupal_set_message(t('Failed to save new server monitor %name.', array('%name' => $values['name'])), 'error');
      watchdog('server_monitor', 'Failed to save new server monitor %name.', array('%name' => $values['name']), WATCHDOG_ERROR);
    }
  }
  else {
    $sid = db_update('server_monitor')
      ->fields(array(
        'name' => $values['name'],
        'address' => $values['address'],
        'method' => $values['method'],
        'server_description' => $values['server_description'],
        'configuration' => serialize($configuration),
        ))
      ->condition('sid', $values['sid'], '=')
      ->execute();
    drupal_set_message(t('Server monitor %name has been successfully modified.', array('%name' => $values['name'])));
    watchdog('server_monitor', 'Server monitor %name has been successfully modified.', array('%name' => $values['name']));
  }
  drupal_goto('admin/config/services/server_monitor');
}


/**
 * Implements the Server Monitor server delete page.
 */
function server_monitor_delete_form($form, &$form_state, $sid = NULL) {
  $name = db_select('server_monitor')
    ->fields('server_monitor', array('name'))
    ->condition('sid', $sid, '=')
    ->execute()
    ->fetchField();
  if (is_numeric($sid) && $name) {
    $form = array(
      'sid' => array('#type' => 'hidden', '#value' => $sid),
      'name' => array('#type' => 'hidden', '#value' => $name),
    );
    return confirm_form($form, t('Are you sure you want to delete the server %name?', array('%name' => $name)), 'admin/config/services/server_monitor', NULL, t('Delete'), t('Cancel'));
  }
  else {
    drupal_not_found();
    exit;
  }

  return $form;
}


/**
 * Submit hook for the server monitor delete page.
 */
function server_monitor_delete_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  if ($values['confirm'] && $values['sid']) {
    $deleted = db_delete('server_monitor')
      ->condition('sid', $values['sid'], '=')
      ->execute();
    drupal_set_message(t('Server monitor %name has been deleted.', array('%name' => $values['name'])));
    watchdog('server_monitor', 'Server monitor %name has been deleted.', array('%name' => $values['name']));
  }
  drupal_goto('admin/config/services/server_monitor');
}
