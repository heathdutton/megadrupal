<?php
/**
 * @file
 * Administration page for the module.
 *
 * Module provide single configuration page for tuning variables and managing
 * PostgreSQL logins.
 */

/**
 * Administration settings/operation form.
 */
function pgsign_admin_settings($form, &$form_state) {
  $form['pgsign_sql_add_new_user'] = array(
    '#title' => t('Command to create a new user'),
    '#type' => 'textfield',
    '#default_value' => variable_get('pgsign_sql_add_new_user', "CREATE USER %username WITH PASSWORD '%password'; GRANT drupal TO %username;"),
    '#description' => t('Please enter here an SQL command which will be fired on creation or new user. Please consider adding Superuser privileges. The %username variable should be used to represent Drupal user login name. Optionally %password can be used to specify a password.'),
  );

  $users_sql = _pgsign_get_user_list();

  $users_drupal = entity_load('user');

  // Anonymous do not need any special care.
  unset($users_drupal[0]);

  // Collect Drupal users with and without SQL login.
  $present = $missing = array();

  foreach ($users_drupal as $user_drupal) {
    $user_sql = _pgsign_get_db_username($user_drupal);
    if (isset($users_sql[$user_sql])) {
      $present[$user_drupal->uid] = $user_drupal->name;
    }
    else {
      $missing[$user_drupal->uid] = $user_drupal->name;
    }
  }

  if (!empty($present)) {
    $form['pgsign_drop_users'] = array(
      '#title' => t('Drop all PostgreSQL users'),
      '#type' => 'checkbox',
      '#default_value' => 0,
      '#description' => t('%count_sql of %count_total Drupal users have dedicated SQL login. To remove users from Postgres check this box. This will not affect Drupal users. User names: %names.',
        array(
          '%count_sql' => count($present),
          '%count_total' => count($users_drupal),
          '%names' => implode(', ', $present),
        )),
    );
  }

  if (!empty($missing)) {
    $form['pgsign_create_users'] = array(
      '#title' => t('Create missing PostgreSQL users'),
      '#type' => 'checkbox',
      '#default_value' => 0,
      '#description' => t('%count_missing of %count_total Drupal users do not have dedicated SQL login. To create personal Postgres login check this box. User names: %names.',
        array(
          '%count_missing' => count($missing),
          '%count_total' => count($users_drupal),
          '%names' => implode(', ', $missing),
        )),
    );
  }

  $form['pgsign_existing_users'] = array('#type' => 'hidden', '#value' => $present);
  $form['pgsign_missing_users'] = array('#type' => 'hidden', '#value' => $missing);

  $form['#submit'][] = 'pgsign_admin_settings_submit';

  return system_settings_form($form);
}

/**
 * Submit handler for settings form.
 */
function pgsign_admin_settings_submit($form, &$form_state) {
  global $user;

  $count_drop = 0;
  $count_add = 0;
  $values = $form_state['values'];

  // Drop should go first for case both checkboxes are selected.
  if (isset($values['pgsign_drop_users']) && $values['pgsign_drop_users']) {

    // Do not remove own account with active connection.
    // unset($values['pgsign_existing_users'][$user->uid]);

    foreach ($values['pgsign_existing_users'] as $uid => $username) {
      if (_pgsign_drop_db_user(user_load($uid))) {
        $count_drop++;
      }
    }

    // If recreation is scheduled just deleted users have to be processed as well.
    $values['pgsign_missing_users'] += $values['pgsign_existing_users'];
  }

  if (isset($values['pgsign_create_users']) && $values['pgsign_create_users']) {
    // In case command was also updated.
    variable_set('pgsign_sql_add_new_user', $values['pgsign_sql_add_new_user']);

    foreach ($values['pgsign_missing_users'] as $uid => $username) {
      _pgsign_add_db_user(user_load($uid));
      $count_add++;
    }
  }
  if ($count_drop + $count_add) {
    drupal_set_message(t('%count_drop users dropped, %count_add users added.',
      array('%count_drop' => $count_drop, '%count_add' => $count_add)));
  }
}
