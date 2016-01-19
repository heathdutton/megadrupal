<?php

/**
 * @file
 * Watchdog reporter add/edit form for presets.
 */

/**
 * Callback function for drupal_get_form().
 *
 * @param int $preset_id
 *   Preset id
 *
 * @return array
 *   Array with form elements.
 */
function watchdog_reporter_preset_form($form, &$form_state, $preset_id) {

  // Getting watchdog reporter preset list.
  $form_state['watchdog_reporter_preset_list'] = $watchdog_presets = variable_get('watchdog_reporter_preset_list', array());

  $form_state['watchdog_reporter_preset_is_edit'] = ($preset_id) ? TRUE : FALSE;
  // Checking if preset id is valid.
  if ($preset_id && !isset($watchdog_presets[$preset_id])) {
    drupal_set_message(t('Wrong preset id.'), 'error');
    return $form;
  }
  elseif (!$form_state['watchdog_reporter_preset_is_edit']) {
    if ($watchdog_presets) {
      $preset_id = max(array_keys($watchdog_presets)) + 1;
    }
    else {
      $preset_id = 1;
    }
  }
  $form_state['watchdog_reporter_preset_id'] = $preset_id;

  // Form element for preset name.
  $form['watchdog_reporter_preset_name'] = array(
    '#type' => "textfield",
    '#title' => t('Preset Name'),
    '#default_value' => ($form_state['watchdog_reporter_preset_is_edit']) ? $watchdog_presets[$preset_id]['#values']['watchdog_reporter_preset_name'] : '',
    '#weight' => 0,
    '#required' => TRUE,
  );

  // Form element for email addresses.
  $form['watchdog_reporter_email'] = array(
    '#type' => "textfield",
    '#title' => t('Email list'),
    '#default_value' => ($form_state['watchdog_reporter_preset_is_edit']) ? $watchdog_presets[$preset_id]['#values']['watchdog_reporter_email'] : variable_get('site_mail', 'user@example.com'),
    '#weight' => 1,
    '#required' => TRUE,
    '#description' => t('Target email addresses separated with a comma.'),
  );

  // Cron rule.
  $form['watchdog_reporter_cronrule'] = array(
    '#type' => "textfield",
    '#title' => t('Cron job rule'),
    '#weight' => 3,
    '#required' => TRUE,
    '#default_value' => ($form_state['watchdog_reporter_preset_is_edit']) ? $watchdog_presets[$preset_id]['#values']['watchdog_reporter_cronrule'] : '* * * * *',
    '#description' => t('Example rule: "*/5 * * * *" for very 5 minutes. It must be a less or equally frequent rule as your cron.php is scheduled. For more info see !link and !link2.', array('!link' => l('crontab.org', 'http://crontab.org/'), '!link2'=> l('www.drupal.org/cron', 'https://www.drupal.org/cron'))),
  );

  // Form element for preset name.
  $form['watchdog_reporter_severity'] = array(
    '#type' => "select",
    '#title' => t('Severity level'),
    '#default_value' => ($form_state['watchdog_reporter_preset_is_edit']) ? $watchdog_presets[$preset_id]['#values']['watchdog_reporter_severity'] : '',
    '#options' => watchdog_severity_levels(),
    '#weight' => 4,
    '#multiple' => TRUE,
    '#description' => 'Filter entries by severity level. If none selected, no filter will be applied.',
  );

  $dblog_types = array_flip(_dblog_get_message_types());
  $dblog_types_saved = array_flip(variable_get('watchdog_reporter_dblog_types', array()));
  $types = array_keys(array_merge($dblog_types, $dblog_types_saved));
  variable_set('watchdog_reporter_dblog_types', $types);

  // Form element for preset name.
  $form['watchdog_reporter_type'] = array(
    '#type' => "select",
    '#title' => t('Type'),
    '#default_value' => ($form_state['watchdog_reporter_preset_is_edit']) ? $watchdog_presets[$preset_id]['#values']['watchdog_reporter_type'] : '',
    '#options' => $types,
    '#weight' => 5,
    '#multiple' => TRUE,
    '#description' => 'Filter entries by type. If none selected, no filter will be applied.',
  );

  // Form element for preset name.
  $form['watchdog_reporter_roles'] = array(
    '#type' => "select",
    '#title' => t('Roles'),
    '#default_value' => ($form_state['watchdog_reporter_preset_is_edit']) ? $watchdog_presets[$preset_id]['#values']['watchdog_reporter_roles'] : '',
    '#options' => user_roles(),
    '#weight' => 6,
    '#multiple' => TRUE,
    '#description' => 'Filter entries by user role. If none selected, no filter will be applied.',
  );

  // Form element for user email addresses.
  $form['watchdog_reporter_users'] = array(
    '#type' => "textfield",
    '#title' => t('User list'),
    '#default_value' => ($form_state['watchdog_reporter_preset_is_edit']) ? $watchdog_presets[$preset_id]['#values']['watchdog_reporter_users'] : '',
    '#weight' => 7,
    '#description' => t('User Email addresses separated with commas.'),
    '#autocomplete_path' => 'watchdog_reporter/user/autocomplete',
    '#multiple' => TRUE,
    '#description' => 'Filter entries by user. If none selected, no filter will be applied.',
  );

  $group_options = array(
    WATCHDOG_REPORTER_GROUP_NONE => t('Don`t group'),
    WATCHDOG_REPORTER_GROUP_SQL => t('SQL'),
    WATCHDOG_REPORTER_GROUP_PHP => t('PHP')
  );

  // Form element for user email addresses.
  $form['watchdog_reporter_group'] = array(
    '#type' => "radios",
    '#options' => $group_options,
    '#title' => t('Group result by'),
    '#default_value' => ($form_state['watchdog_reporter_preset_is_edit']) ? $watchdog_presets[$preset_id]['#values']['watchdog_reporter_group'] : WATCHDOG_REPORTER_GROUP_NONE,
    '#weight' => 8,
    '#required' => TRUE,
    '#description' => t('Option to group same entries. Since watchdog messages are stored in the database with placeholders, we can group them before (SQL) or after (PHP) the placeholders are replaced.'),
  );

  // Form element Submit button.
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Create'),
  );

  // If editing preset change Submit button name and add Delete button.
  if ($form_state['watchdog_reporter_preset_is_edit']) {
    $form['actions']['submit']['#value'] = t('Update');
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#submit' => array('watchdog_reporter_delete_preset_submit'),
    );
  }

  // Form element Cancel button.
  $form['actions']['cancel'] = array(
    '#markup' => l(t('Cancel'), 'admin/config/development/watchdog_reporter'),
  );

  return $form;
}

/**
 * Implements hook_formID_validate().
 */
function watchdog_reporter_preset_form_validate(&$form, &$form_state) {

  if (!preg_match('/^\\s*([0-9*, \/-]+[]+[0-9*, \/-]+[]+[0-9*, \/-]+[]+[0-9*, \/-]+[]+[0-9*, \/-]+)\\s*$/', $form_state['values']['watchdog_reporter_cronrule'])) {
    form_error($form['watchdog_reporter_cronrule'], t('Invalid rule: ' . $form_state['values']['watchdog_reporter_cronrule']));
  }

  foreach ($form_state['values'] as $key => $values) {
    if (strpos($key, 'watchdog_reporter') !== FALSE) {
      if ($values) {
        $form_state['watchdog_reporter_preset_list'][$form_state['watchdog_reporter_preset_id']]['#values'][$key] = $values;
      }
      else {
        unset($form_state['watchdog_reporter_preset_list'][$form_state['watchdog_reporter_preset_id']]['#values'][$key]);
      }
    }
  }

  $form_state['watchdog_reporter_preset_list'][$form_state['watchdog_reporter_preset_id']]['#enabled'] = TRUE;

  if (!isset($form_state['watchdog_reporter_preset_list'][$form_state['watchdog_reporter_preset_id']]['#last_run'])) {
    $form_state['watchdog_reporter_preset_list'][$form_state['watchdog_reporter_preset_id']]['#last_run'] = 0;
  }
}

/**
 * Implements hook_formID_submit().
 */
function watchdog_reporter_preset_form_submit(&$form, &$form_state) {

  $preset_list = $form_state['watchdog_reporter_preset_list'];

  // Save preset list.
  variable_set('watchdog_reporter_preset_list', $preset_list);
  drupal_set_message(t('Preset successfully saved.'), 'status');
}

/**
 * Implements watchdog_reporter_delete_preset_submit().
 */
function watchdog_reporter_delete_preset_submit(&$form, &$form_state) {

  drupal_goto('admin/config/development/watchdog_reporter/preset/delete/' . $form_state['watchdog_reporter_preset_id']);
}

/**
 * Callback function after drupal_get_form().
 */
function watchdog_reporter_delete_preset_form($form, &$form_state, $preset_id) {

  $desc = t('Are you sure you want to delete the preset? This action cannot be undone.');
  $ques = t('Are you sure?');
  $form_state['watchdog_reporter_preset_id'] = $preset_id;
  return confirm_form($form, $ques, 'admin/config/development/watchdog_reporter', $desc);
}

/**
 * Implements hook_formID_submit().
 */
function watchdog_reporter_delete_preset_form_submit(&$form, &$form_state) {

  watchdog_reporter_preset_form_delete($form_state['watchdog_reporter_preset_id']);
}

/**
 * Function for deleting presets.
 *
 * @param array $id
 *   Preset ID.
 * @param array $watchdog_presets
 *   List of presets.
 */
function watchdog_reporter_preset_form_delete($id, $watchdog_presets = array()) {

  // If delete called from list.
  if (empty($watchdog_presets)) {
    $watchdog_presets = variable_get('watchdog_reporter_preset_list', array());
  }

  // Delete preset.
  unset($watchdog_presets[$id]);

  variable_set('watchdog_reporter_preset_list', $watchdog_presets);
  drupal_set_message(t('Preset successfully deleted.'), 'status');

  drupal_goto("admin/config/development/watchdog_reporter");
}

/**
 * 
 * Autocomplete callback.
 * 
 * @param string $string
 */
function _watchdog_reporter_user_autocomplete($string) {

  $matches = array();

  if ($string) {
    $items = array_map('trim', explode(',', $string));
    $last_item = array_pop($items);
    $prefix = implode(', ', $items);

    $result = db_select('users')->fields('users', array('name'))->condition('name', '%' . db_like($last_item) . '%', 'LIKE')->range(0, 10)->execute();
    foreach ($result as $user) {
      $value = !empty($prefix) ? $prefix . ', ' . $user->name : $user->name;
      $matches[$value] = check_plain($user->name);
    }
  }

  drupal_json_output($matches);
}