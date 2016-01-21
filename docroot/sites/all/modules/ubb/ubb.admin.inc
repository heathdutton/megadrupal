<?php

/**
 * @file
 * The administrative UI for the ubb module.
 */

/**
 * The module configuration form.
 */
function ubb_settings_form($form, $form_state) {
  // See if the UBB database is available.
  $version = NULL;
  $info = Database::getConnectionInfo('ubb');
  if (!empty($info)) {
    db_set_active('ubb');
    $version = db_query('SELECT DB_VERSION FROM {VERSION}')->fetchField();
    db_set_active();
  }
  $available = !empty($version);

  // Notify the user of UBB's availability.
  if ($available) {
    drupal_set_message(t('UBB database found: version @version.', array('@version' => $version)));
  }
  else {
    drupal_set_message(t('No UBB database found. To connect to it, you must add a "ubb" connection to the <code>$databases</code> array in your settings.php file. See <a href="@url">README.txt</a> in the ubb module for more information.', array('@url' => url(drupal_get_path('module', 'ubb') . '/README.txt'))));
  }

  // Activate the integrations.
  $form['ubb_enable'] = array(
    '#title' => t('Enable UBB integration'),
    '#type' => 'checkbox',
    '#default_value' => $available && variable_get('ubb_enable', FALSE),
    '#disabled' => !$available,
  );

  // The UBB group for new users.
  $form['ubb_group_name'] = array(
    '#title' => t('New Drupal users should be added to the UBB group with the following name (leave blank to disable)'),
    '#type' => 'textfield',
    '#default_value' => variable_get('ubb_group_name', 'Users'),
    '#size' => 60,
    '#maxlength' => 128,
  );

  // Cookie configuration.
  $form['ubb_cookie_prefix'] = array(
    '#title' => t('Prefix for UBB cookies'),
    '#description' => t("This field must match the COOKIE_PREFIX value in UBB's includes/config.inc.php file."),
    '#type' => 'textfield',
    '#default_value' => variable_get('ubb_cookie_prefix', ''),
    '#size' => 60,
    '#maxlength' => 128,
  );
  $form['ubb_cookie_path'] = array(
    '#title' => t('Path for UBB cookies'),
    '#description' => t("This field must match the COOKIE_PATH value in UBB's includes/config.inc.php file."),
    '#type' => 'textfield',
    '#default_value' => variable_get('ubb_cookie_path', '/'),
    '#size' => 60,
    '#maxlength' => 128,
    '#required' => TRUE,
  );

  return system_settings_form($form);
}

/**
 * The user import form.
 */
function ubb_importer_form($form, $form_state) {
  if (variable_get('ubb_enable', FALSE)) {
    // Count the number of UBB user accounts.
    db_set_active('ubb');
    $query = db_select('USERS', 'u')
      ->fields('u')
      ->condition('u.USER_ID', 1, '>')
      ->condition('u.USER_IS_APPROVED', 'yes');
    $count = $query->countQuery()->execute()->fetchField();
    db_set_active();
    $form['count'] = array('#type' => 'value', '#value' => $count);
    drupal_set_message(t('@count active users found in the UBB database.', array('@count' => $count)));

    // Display the status of the previous import.
    $last = variable_get('ubb_importer_last', -1);
    if ($last > 0) {
      $form['last'] = array(
        '#prefix' => '<p>',
        '#markup' => t('Last row imported: @last', array('@last' => $last)),
        '#suffix' => '</p>',
      );
    }

    // Add common fields for the import/export functions.
    ubb_importer_exporter_form($form, $count, $last, t('Import'));
  }
  else {
    $form['message'] = array('#markup' => t('UBB integration must be enabled before you can run the importer.'));
  }
  return $form;
}

/**
 * The user export form.
 */
function ubb_exporter_form($form, $form_state) {
  if (variable_get('ubb_enable', FALSE)) {
    // Count the number of Drupal user accounts.
    $query = db_select('users', 'u')
      ->fields('u')
      ->condition('u.uid', 1, '>');
    $count = $query->countQuery()->execute()->fetchField();
    $form['count'] = array('#type' => 'value', '#value' => $count);
    drupal_set_message(t('@count users found in the Drupal database (excluding user 1).', array('@count' => $count)));

    // Display the status of the previous export.
    $last = variable_get('ubb_exporter_last', -1);
    if ($last > 0) {
      $form['last'] = array(
        '#prefix' => '<p>',
        '#markup' => t('Last row exported: @last', array('@last' => $last)),
        '#suffix' => '</p>',
      );
    }

    // Add common fields for the import/export functions.
    ubb_importer_exporter_form($form, $count, $last, t('Export'));
  }
  else {
    $form['message'] = array('#markup' => t('UBB integration must be enabled before you can run the exporter.'));
  }
  return $form;
}

/**
 * Adds common fields to the user import/export forms.
 *
 * @param &$form
 *   The import/export form to alter.
 * @param $count
 *   The number of records available to import/export.
 * @param $last
 *   The last row imported/exported in the previous run.
 * @param $submit
 *   The translated label for the submit button.
 */
function ubb_importer_exporter_form(&$form, $count, $last, $submit) {
  // Starting point.
  $form['start'] = array(
    '#title' => t('Start at offset'),
    '#type' => 'textfield',
    '#default_value' => $last + 1,
    '#size' => 10,
    '#maxlength' => 10,
    '#required' => TRUE,
  );

  // Maximum number of rows to import/export.
  $form['max'] = array(
    '#title' => t('Maximum number of rows to include in this run'),
    '#type' => 'textfield',
    '#default_value' => $count - ($last + 1),
    '#size' => 10,
    '#maxlength' => 10,
    '#required' => TRUE,
  );

  // The submit button.
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => $submit,
  );

  // Add the common validation function.
  $form['#validate'][] = 'ubb_importer_exporter_form_validate';
}

/**
 * Validate the user import/export forms.
 */
function ubb_importer_exporter_form_validate($form, &$form_state) {
  // The starting row.
  $message = _ubb_importer_exporter_form_validate($form_state['values']['start'], $form_state['values']['count'] - 1, 'Offset');
  if (!empty($message)) {
    form_set_error('start', $message);
  }

  // The maximum number of rows.
  $message = _ubb_importer_exporter_form_validate($form_state['values']['max'], $form_state['values']['count'], 'Maximum number of rows');
  if (!empty($message)) {
    form_set_error('max', $message);
  }
}

/**
 * Helper function for ubb_importer_exporter_form_validate().
 */
function _ubb_importer_exporter_form_validate($value, $limit, $field = '') {
  if (!is_numeric($value)) {
    $message = t('@field must be numeric.', array('@field' => $field));
  }
  elseif ($value < 0 || $value > $limit) {
    $message = t('@field must be between 0 and @limit.', array('@field' => $field, '@limit' => $limit));
  }
  else {
    $message = '';
  }
  return $message;
}

/**
 * Submit the user import form.
 */
function ubb_importer_form_submit($form, &$form_state) {
  $offset = $form_state['values']['start'];
  $max = ($form_state['values']['max'] == 0) ? 9999999999 : $form_state['values']['max'];
  $succeeded = 0;
  $failed = 0;
  $skipped = 0;

  // Get the set of UBB users to import.
  db_set_active('ubb');
  $query = db_select('USERS', 'u');
  $query->addField('u', 'USER_LOGIN_NAME', 'name');
  $query->addField('u', 'USER_PASSWORD', 'pass');
  $query->addField('u', 'USER_REGISTRATION_EMAIL', 'mail');
  $query->addField('u', 'USER_IS_BANNED', 'status');
  $query->condition('u.USER_ID', 1, '>');
  $query->condition('u.USER_IS_APPROVED', 'yes');
  $query->range($offset, $max);
  $ubb_users = $query->execute();
  db_set_active();

  // Import each user in the set.
  foreach ($ubb_users as $ubb_user) {
    $offset++;

    // Invert the status field; if UBB's USER_IS_BANNED field is true, then Drupal's status field should be false (disabled).
    $ubb_user->status = empty($ubb_user->status);

    // Make sure the UBB username does not already exist in Drupal.
    $exists = user_load_by_name($ubb_user->name);
    if (empty($exists)) {
      // Add the new user.
      $account = user_save($ubb_user);
      if (!$account) {
        $failed++;
        watchdog('ubb', 'Failed to import user %name from UBB.', array('%name' => $ubb_user->name), WATCHDOG_ERROR);
      }
      else {
        $succeeded++;
        // @todo Since the password in UBB's USERS table is already hashed, how does this affect Drupal's new password hashing method on user_save()?
        watchdog('ubb', 'User %name (%email) imported from UBB.', array('%name' => $account->name, '%email' => $account->mail), WATCHDOG_NOTICE, l(t('edit'), 'user/'. $account->uid .'/edit'));
      }
    }
    else {
      $skipped++;
    }
  }

  // Update the offset count for next time.
  variable_set('ubb_importer_last', $offset - 1);

  drupal_set_message(t('Succeeded: @succeeded. Skipped: @skipped. Failed: @failed.', array('@succeeded' => $succeeded, '@skipped' => $skipped, '@failed' => $failed)));
}

/**
 * Submit the user export form.
 */
function ubb_exporter_form_submit($form, &$form_state) {
  $offset = $form_state['values']['start'];
  $max = ($form_state['values']['max'] == 0) ? 9999999999 : $form_state['values']['max'];
  $created = 0;
  $updated = 0;
  $failed = 0;

  // Get the set of Drupal users to export.
  $query = db_select('users', 'u');
  $query->fields('u', array('name', 'pass', 'created', 'mail', 'status'));
  $query->condition('u.uid', 1, '>');
  $query->range($offset, $max);
  $drupal_users = $query->execute();

  // Export each user in the set.
  foreach ($drupal_users as $drupal_user) {
    $offset++;
    $status = ubb_save_user_to_ubb($drupal_user);
    switch ($status) {
      case SAVED_NEW:
        $created++;
        break;

      case SAVED_UPDATED:
        $updated++;
        break;

      default:
        $failed++;
    }
  }

  // Update the offset count for next time.
  variable_set('ubb_exporter_last', $offset - 1);

  drupal_set_message(t('Created: @created. Updated: @updated. Failed: @failed.', array('@created' => $created, '@updated' => $updated, '@failed' => $failed)));
}
