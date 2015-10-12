<?php

/**
 * @file
 * Admin functions for User Disk Quota module.
 */

/**
 * Module settings form.
 *
 * @return array $form
 *   The settings form.
 */
function user_disk_quota_role_form() {
  $roles = user_roles();
  $form = array();
  $description = array();
  $description[] = t('Specify the user quota limit that applies to each role.');
  $description[] = t("Enter a value like @es1, @es2 or @es3 in order to restrict a user role's disk space quota.",
      array(
        '@es1' => '"512" (bytes)',
        '@es2' => '"80 KB" (kilobytes)',
        '@es3' => '"50 MB" (megabytes)',
      )
    );
  $description[] = t('Leave the field empty to not set any limit.');  
  $form['roles'] = array(
    '#type' => 'fieldset',
    '#collapsed' => FALSE,
    '#collapsible' => FALSE,
    '#title' => t('Roles'),
    '#description' => implode(' ', $description),
  );
  foreach ($roles as $rid => $role) {
    $default = variable_get('user_disk_quota_role_' . $rid, '');
    if (!empty($default)) {
      $default = format_size($default);
    }
    $form['roles']['user_disk_quota_role_' . $rid] = array(
      '#type' => 'textfield',
      '#title' => t("@role's disk quota limit", array('@role' => check_plain($role))),
      '#default_value' => $default,
      '#element_validate' => array('_user_disk_quota_form_quota_validate'),
    );
  }
  $form['default'] = array(
    '#type' => 'fieldset',
    '#collapsed' => FALSE,
    '#collapsible' => TRUE,
    '#title' => t('Default values'),
    '#description' => '',
  );
  $default_percentage = variable_get('user_disk_quota_warning_percentage', 70);
  $form['default']['user_disk_quota_warning_percentage'] = array(
    '#type' => 'textfield',
    '#title' => t('Warning message'),
    '#default_value' => $default_percentage,
    '#description' => t('Set the default percentage of user disk quota to be filled up before a warning appear to the user.'),
    '#size' => 3,
    '#maxlength' => 3,
    '#required' => TRUE,
    '#field_suffix' => '%<div class="user-disk-percentage-slider-range-max"></div>'
  );
  $form['default']['user_disk_quota_warning_percentage']['#attributes']['class'][] = 'user-disk-percentage-slider';
  drupal_add_library('system', 'ui');
  drupal_add_library('system', 'ui.slider');
  $settings = array(
    'user_disk_quota_admin' => array(
      'default_percentage' => $default_percentage,
    ),
  );
  $form['#attached']['js'] = array(
    drupal_get_path('module', 'user_disk_quota') . '/js/user_disk_quota-admin.js' => array(
      'type' => 'file',
    ),
  );
  $form['#attached']['js'][] = array(
    'data' => $settings,
    'type' => 'setting',
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  return $form;
}

/**
 * Module settings form validate callback.
 *
 * @return array $form
 *   The settings form.
 * @param array &$form_state
 *   The form state containing form values.
 */
function user_disk_quota_role_form_validate($form, &$form_state) {
  $value = $form_state['values']['user_disk_quota_warning_percentage'];
  $title = $form['default']['user_disk_quota_warning_percentage']['#title'];
  if (!empty($value)) {
    $value = trim($value);
  }
  if (!is_numeric($value) || intval($value)==0) {
    form_set_error('user_disk_quota_warning_percentage',
      t('The %field value must be a numeric integer.', array('%field' => check_plain($title))));
  }
  if ($value<0 || $value>100) {
    form_set_error('user_disk_quota_warning_percentage',
      t('The allowed %field value must be in a range between 0 and 100.', array('%field' => check_plain($title))));
  }
}

/**
 * Module settings form submit callback.
 *
 * @return array $form
 *   The settings form.
 * @param array &$form_state
 *   The form state containing form values.
 */
function user_disk_quota_role_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  $strlenght = strlen('user_disk_quota_role_');
  foreach ($values as $key => $value) {
    if (substr($key, 0, $strlenght) == 'user_disk_quota_role_') {
      $value = parse_size(check_plain($value));
      if ($value == 0) {
        $value = '';
      }
      variable_set($key, $value);
    }
  }
  variable_set('user_disk_quota_warning_percentage', $values['user_disk_quota_warning_percentage']);
  drupal_set_message(t('Disk quota settings have been updated.'), 'status');
}