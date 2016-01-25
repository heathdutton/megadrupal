<?php

/**
 * @file
 *
 */

/**
 */
function youtrack_admin_settings($form, &$form_state) {
  $form['youtrack_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enabled'),
    '#default_value' => variable_get('youtrack_enabled', TRUE),
  );
  $form['youtrack_base_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Base URL'),
    '#required' => TRUE,
    '#default_value' => variable_get('youtrack_base_url', ''),
  );
  $form['youtrack_username'] = array(
    '#type' => 'textfield',
    '#title' => t('Username'),
    '#required' => TRUE,
    '#default_value' => variable_get('youtrack_username', ''),
  );
  $form['youtrack_password'] = array(
    '#type' => 'password',
    '#title' => t('Password'),
    '#default_value' => variable_get('youtrack_password', ''),
  );
  $form['youtrack_email'] = array(
    '#type' => 'textfield',
    '#title' => t('Email address for fallbacks'),
    '#default_value' => variable_get('youtrack_email', ''),
  );

  $form['settings'] = array(
    '#type' => 'container',
    '#access' => _youtrack_check_server(),
  );
  $sets = array(
    'watchdog' => t('Watchdog'),
    'info' => t('Info'),
    'alert' => t('Alert'),
  );
  if (module_exists('contact')) {
    $sets['contact'] = t('Contact');
  }
  $projects = youtrack_rest_get_projects(TRUE);
  foreach ($sets as $set => $setname) {
    $pid = variable_get('youtrack_' . $set . '_project', '');
    $form['settings'][$set] = array(
      '#type' => 'fieldset',
      '#title' => $setname,
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $form['settings'][$set]['youtrack_' . $set] = array(
      '#type' => 'checkbox',
      '#title' => t('Create issues for @set entries', array('@set' => $set)),
      '#default_value' => variable_get('youtrack_' . $set, FALSE),
    );
    $form['settings'][$set]['settings'] = array(
      '#type' => 'container',
      '#states' => array(
        'invisible' => array(
          'input[name="youtrack_' . $set . '"]' => array('checked' => FALSE),
        ),
      ),
    );
    $form['settings'][$set]['settings']['youtrack_' . $set . '_severity'] = array(
      '#type' => 'select',
      '#title' => t('Severities'),
      '#options' => watchdog_severity_levels(),
      '#default_value' => variable_get('youtrack_' . $set . '_severity', WATCHDOG_ERROR),
      '#description' => t('Select the lowest severity from which you want to submit @set entries.', array('@set' => $set)),
    );
    $form['settings'][$set]['settings']['youtrack_' . $set . '_project'] = array(
      '#type' => 'select',
      '#title' => t('Project'),
      '#options' => $projects,
      '#default_value' => $pid,
    );
    $form['settings'][$set]['settings']['project_settings'] = array(
      '#type' => 'container',
    );
    $form['settings'][$set]['settings']['project_settings'] += youtrack_admin_settings_project($set, $pid);
  }

  $form['#validate'][] = 'youtrack_admin_settings_validate';
  $form['#submit'][] = 'youtrack_admin_settings_submit';

  $form = system_settings_form($form);

  $form['actions']['youtrack_refresh'] = array(
    '#type' => 'submit',
    '#value' => t('Refresh from YouTrack'),
  );
  return $form;
}

/**
 *
 */
function youtrack_admin_settings_validate($form, &$form_state) {
  $password = empty($form_state['values']['youtrack_password']) ?
    variable_get('youtrack_password', '') :
    $form_state['values']['youtrack_password'];
  if (!_youtrack_login($form_state['values']['youtrack_base_url'], $form_state['values']['youtrack_username'], $password)) {
    // Can not connect to the server.
    form_error($form['youtrack_base_url'], t('Can not connect to the YouTrack server with given parameters.'));
    return;
  }
  if ($form_state['values']['op'] == $form['actions']['youtrack_refresh']['#value']) {
    youtrack_rest_get_projects(FALSE, TRUE);
    form_error($form, t('Values have been refreshed'));
  }
}

/**
 *
 */
function youtrack_admin_settings_submit($form, &$form_state) {
  if (empty($form_state['values']['youtrack_password'])) {
    $form_state['values']['youtrack_password'] = variable_get('youtrack_password', '');
  }
}

function youtrack_admin_settings_project($set, $pid) {
  $form = array();

  $projects = variable_get('youtrack_projects', array());
  if (!empty($pid) && !empty($projects[$pid])) {
    $fields = array();
    foreach ($projects[$pid]['fields'] as $name => $values) {
      if (!empty($values)) {
        $fields[$name] = $name;
        $options = array_merge(array('-' => '- ' . t('default') . ' -'), $values);
        $form['youtrack_' . $set . '_project_field_' . $name] = array(
          '#type' => 'select',
          '#title' => $name,
          '#options' => $options,
          '#default_value' => variable_get('youtrack_' . $set . '_project_field_' . $name, ''),
        );
      }
    }
    $form['youtrack_' . $set . '_project_field_for_severity'] = array(
      '#type' => 'select',
      '#title' => 'Field for severity',
      '#options' => $fields,
      '#default_value' => variable_get('youtrack_' . $set . '_project_field_for_severity', ''),
      '#weight' => -10,
      '#description' => t('Select the field which is representing the severity.'),
    );
    $form['youtrack_' . $set . '_commands'] = array(
      '#type' => 'textarea',
      '#title' => 'Extra commands',
      '#default_value' => variable_get('youtrack_' . $set . '_commands', ''),
      '#description' => t('Provide more YouTrack commands, one per line.'),
    );
  }

  return $form;
}
