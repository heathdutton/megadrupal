<?php

/**
 * @file
 * Adding url redirect page..
 */

/**
 * Implements hook_form().
 */
function url_redirect_settings_form($form, &$form_state) {
  $form = array();
  $form['goto_list'] = array(
    '#markup' => l(t('Url Redirect List'), 'admin/config/url_redirect/list'),
  );
  $form['url'] = array(
    '#type' => 'fieldset',
    '#title' => t('Url Redirect'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['url']['path'] = array(
    '#type' => 'textfield',
    '#title' => 'Path',
    '#attributes' => array(
      'placeholder' => 'Enter Path',
    ),
    '#required' => TRUE,
    '#description' => t('This can be an internal Drupal path such as node/add Enter <front> to link to the front page.'),
  );
  $form['url']['redirect_path'] = array(
    '#type' => 'textfield',
    '#title' => 'Redirect Path',
    '#attributes' => array(
      'placeholder' => 'Enter Redirect Path',
    ),
    '#required' => TRUE,
     '#description' => t('This redirect path can be internal Drupal path such as node/add Enter <front> to link to the front page.'),
  );
  $form['url']['checked_for'] = array(
    '#type' => 'radios',
    '#title' => t('Select Redirect path for'),
    '#options' => drupal_map_assoc(array(t('Role'), t('User'))),
    '#required' => TRUE,
  );

  $form['url']['url_roles'] = array(
    '#type' => 'container',
    '#states' => array(
      'visible' => array(
        ':input[name="checked_for"]' => array('value' => 'Role'),
      ),
    ),
  );
  $option = user_roles($membersonly = TRUE, $permission = NULL);
  $form['url']['url_roles']['roles'] = array(
    '#type' => 'select',
    '#title' => t('Select Roles'),
    '#options' => $option,
    '#multiple' => TRUE,
  );

  $form['url']['url_user'] = array(
    '#type' => 'container',
    '#states' => array(
      'visible' => array(
        ':input[name="checked_for"]' => array('value' => 'User'),
      ),
    ),
  );
  $users = url_redirect_user_fetch();
  $form['url']['url_user']['user'] = array(
    '#type' => 'select',
    '#title' => t('Select Users.'),
    '#options' => $users,
    '#multiple' => TRUE,
  );
  $form['url']['message'] = array(
    '#type' => 'radios',
    '#title' => t('Display Message for Redirect'),
    '#required' => TRUE,
    '#description' => t('Show a message for redirect path.'),
    '#options' => drupal_map_assoc(array(t('Yes'), t('No'))),
  );
  $form['url']['status'] = array(
    '#type' => 'radios',
    '#title' => t('Status'),
    '#options' => array(
      0 => t('Disabled'),
      1 => t('Enabled'),
    ),
    '#required' => TRUE,
  );
  $form['url']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );
  $form['url']['reset'] = array(
    '#type' => 'submit',
    '#value' => t('Reset'),
  );
  return $form;
}

/**
 * Implements hook_form_validate().
 */
function url_redirect_settings_form_validate($form, &$form_state) {

  // For settings page.
  if ($form_state['values']['op'] == 'Reset') {
    drupal_goto(current_path());
  }
  if ($form_state['values']['op'] == 'Submit') {
    $path = $form_state['values']['path'];
    $path_check = url_redirect_path_check($path);
    if ($path_check > 0) {
      form_set_error('path', t("The path '@link_path' already used for redirect.", array('@link_path' => $path)));
    }
    $redirect_path = $form_state['values']['redirect_path'];
    if (!drupal_valid_path($path)) {
      form_set_error('path', t("The path '@link_path' is either invalid or you do not have access to it.", array('@link_path' => $path)));
    }
    if (!drupal_valid_path($redirect_path)) {
      form_set_error('redirect_path', t("The redirect path '@link_path' is either invalid or you do not have access to it.", array('@link_path' => $redirect_path)));
    }
    $checked_for = $form_state['values']['checked_for'];
    // Get Checked for User / Role.
    if ($checked_for == 'User') {
      $user_values = $form_state['values']['user'];
      if (!$user_values) {
        form_set_error('user', t("Select atleast one user for which you want to apply this redirect."));
      }
    }
    if ($checked_for == 'Role') {
      $roles_values = $form_state['values']['roles'];
      if (!$roles_values) {
        form_set_error('roles', t("Select atleast one role for which you want to apply this redirect."));
      }
    }
    $form_state['#rebuild'] = TRUE;
  }
}

/**
 * Implements hook_form_submit().
 */
function url_redirect_settings_form_submit($form, &$form_state) {
  $path = $form_state['values']['path'];
  $redirect_path = $form_state['values']['redirect_path'];
  $checked_for = $form_state['values']['checked_for'];
  // Get Checked for User / Role.
  if ($checked_for == 'User') {
    $user_values = $form_state['values']['user'];
    if ($user_values) {
      $users_values = json_encode($user_values);
      $role_values = '';
    }
  }
  if ($checked_for == 'Role') {
    $roles_values = $form_state['values']['roles'];
    if ($roles_values) {
      $role_values = json_encode($roles_values);
      $users_values = '';
    }
  }
  $status = $form_state['values']['status'];
  $message = $form_state['values']['message'];
  // Inserting the data in the url_redirect table.
  db_insert('url_redirect')
      ->fields(array(
        'path' => $path,
        'roles' => $role_values,
        'users' => $users_values,
        'redirect_path' => $redirect_path,
        'status' => $status,
        'message' => $message,
        'check_for' => $checked_for,
      ))
      ->execute();
  // Redirect to listing page.
  drupal_goto('admin/config/url_redirect/list');
}
