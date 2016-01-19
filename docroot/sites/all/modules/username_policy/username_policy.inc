<?php

/**
 * @file
 * Included file for the UserName Policy module.
 */

/**
 * Implements hook_form().
 */
function username_policy_admin_form($form, &$form_state) {
  $form['username_policy'] = array(
    '#type' => 'fieldset',
    '#title' => t('User Name Policy Configuration'),
  );
  $form['username_policy']['pattern'] = array(
    '#type' => 'textfield',
    '#default_value' => variable_get('username_policy_pattern'),
    '#title' => t('Pattern for username'),
  );
  $form['username_policy']['note'] = array(
    '#markup' => t('Note:
                    <ul>
                    <li>Use only the below pattern elements to set pattern.</li>
                    <li>Only mandatory fields from registration page are allowed as pattern elements.</li>
                    <li>If pattern is empty username will be your email id.</li>
                    </ul>
                    <br/>
                 '),
  );
  $form['username_policy']['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Submit',
  );
  $form['username_policy']['description'] = array(
    '#type' => 'item',
    '#title' => t('Only use the below pattern elements.'),
  );
  $form['username_policy']['token_tree'] = array(
    '#prefix' => '<div id="username_policy_token_tree">',
    '#suffix' => '</div>',
    '#tree' => TRUE,
    '#theme' => 'username_policy_pattern_elements',
  );
  foreach (generate_pattern_fields() as $field_name => $pattern_ele) {
    $form['username_policy']['token_tree'][] = array(
      '#markup' => $pattern_ele,
    );
  }
  $form['#validate'][] = 'username_policy_admin_form_validate';
  return $form;
}

/**
 * Generate pattern fields.
 */
function generate_pattern_fields() {
  $fieldinfo = field_info_instances('user', 'user');
  // Prepare pattern elements from user main profile.
  $_fields = array();
  foreach (array_keys($fieldinfo) as $entity) {
    if ($fieldinfo[$entity]['required'] && $fieldinfo[$entity]['settings']['user_register_form']) {
      $_fields[] = '[' . $entity . ']';
    }
  }
  // Prepare pattern elements from user other profile.
  $fieldinfo_profile2 = NULL;
  $_fields_profile2 = array();
  if (module_exists('profile2')) {
    $fieldinfo_profile2 = field_info_instances('profile2');
    $result = db_query("SELECT type, data FROM {profile_type}");
    $data_registration_status = array();
    foreach ($result as $record) {
      $data_reg_status = unserialize($record->data);
      $data_registration_status[] = $data_reg_status['registration'];
    }
    foreach (array_keys($fieldinfo_profile2) as $entity) {
      foreach (array_keys($fieldinfo_profile2[$entity]) as $a) {
        if ($fieldinfo_profile2[$entity][$a]['required']) {
          $_fields_profile2[] = '[' . $a . ']';
        }
      }
    }
    for ($i = 0; $i < count($data_registration_status); $i++) {
      if ($data_registration_status[$i] == 0) {
        $_fields_profile2[$i] = '';
      }
    }
  }

  if (module_exists('profile2')) {
    foreach ($_fields_profile2 as $field) {
      $_fields[] = $field;
    }
  }
  return $_fields;
}

/**
 * Implements hook_form_validate().
 */
function username_policy_admin_form_validate($form_id, $form_state) {
  $pattern = $form_state['input']['pattern'];
  $offset = 0;
  $not_found = array();
  $num_of_patterns = substr_count($pattern, '[');
  for ($i = 0; $i <= $num_of_patterns - 1; $i++) {
    $str1 = strpos($pattern, '[', $offset);
    $str2 = strpos($pattern, ']', $str1);
    $length = ($str2 - $str1) - 1;
    $offset = $str2;
    $for_query = substr($pattern, ($str1 + 1), $length);
    if (!field_info_field($for_query)) {
      $not_found[] = '[' . $for_query . ']';
    }
  }
  if (count($not_found)) {
    form_set_error('username_policy_pattern_error', 'These are non exist pattern sets!');
  }
}

/**
 * Implements hook_form_submit().
 */
function username_policy_admin_form_submit($form_id, $form_state) {
  cache_clear_username_policy();
  variable_set('username_policy_pattern', $form_state['values']['pattern']);
  drupal_set_message(t("UserName Policy pattern saved, now every user has username of this pattern."));
}

/**
 * Function to clear cache of user name pattern.
 */
function cache_clear_username_policy() {
  cache_clear_all('cache:variable_get:username_policy_pattern', 'cache');
}
