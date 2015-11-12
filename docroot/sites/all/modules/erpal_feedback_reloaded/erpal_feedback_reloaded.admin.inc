<?php

/**
 * @file
 * Admin settings form.
 */

/**
 * Callback function ERPAL feedback settings form.
 */
function erpal_feedback_reloaded_settings($form) {

  $form['sync'] = array(
    '#type' => 'fieldset',
    '#title' => t('Sync ERPAL data'),
    '#description' => t('Synchronize field values of statuses, priorities and types with data from ERPAL.'),
    '#collapsible' => TRUE,
  );

  $form['sync']['erpal_feedback_rebuild'] = array(
    '#type' => 'submit',
    '#value' => t('Fetch data'),
    '#submit' => array('erpal_feedback_reloaded_rebuild_settings_submit'),
  );

  $field_info = field_info_field('field_erpal_feedback_status');
  if (!empty($field_info['settings']['allowed_values'])) {
    $form['sync']['#collapsed'] = TRUE;
  }

  $form['auth'] = array(
    '#type' => 'fieldset',
    '#title' => t('Authentication info'),
    '#collapsible' => TRUE,
  );

  $form['auth']['erpal_feedback_url'] = array(
    '#type' => 'textfield',
    '#title' => t('ERPAL URL'),
    '#default_value' => variable_get('erpal_feedback_url', ''),
    '#description' => t('Base URL of web site where ERPAL is installed.'),
    '#required' => TRUE,
  );

  $form['auth']['erpal_feedback_login'] = array(
    '#type' => 'textfield',
    '#title' => t('ERPAL Login'),
    '#default_value' => variable_get('erpal_feedback_login', ''),
    '#description' => t('User name from web site with ERPAL installation that has privileges to create tasks or tickets.'),
    '#required' => TRUE,
  );

  $password = variable_get('erpal_feedback_password', '');
  $form['auth']['erpal_feedback_password'] = array(
    '#type' => 'textfield',
    '#title' => t('ERPAL Password'),
    '#description' => t('Password of user above.'),
    '#required' => TRUE,
  );

  if (!empty($password)) {
    $form['auth']['erpal_feedback_password']['#required'] = FALSE;
    $form['auth']['erpal_feedback_password']['#description'] .= '<br/>' . t('Your password is stored in the system but not displayed because of security. If you want to update it, just type in new value and save form.');
  }

  $form['erpal'] = array(
    '#type' => 'fieldset',
    '#title' => t('ERPAL information'),
    '#collapsible' => TRUE,
  );

  $form['erpal']['erpal_feedback_project_nid'] = array(
    '#type' => 'textfield',
    '#title' => t('Project ID'),
    '#default_value' => variable_get('erpal_feedback_project_nid', ''),
    '#description' => t('Node ID of a project in ERPAL to which you want to attach a feedbacks.'),
    '#element_validate' => array('element_validate_integer_positive'),
    '#required' => TRUE,
  );

  $form['erpal']['erpal_feedback_task_nid'] = array(
    '#type' => 'textfield',
    '#title' => t('Task ID'),
    '#default_value' => variable_get('erpal_feedback_task_nid', ''),
    '#description' => t('Node ID of a task in ERPAL that will be parent for submited feedbacks.'),
    '#element_validate' => array('element_validate_integer_positive'),
  );

  $form['erpal']['erpal_feedback_ticket'] = array(
    '#type' => 'select',
    '#title' => t('Feedback node type'),
    '#options' => array(
      0 => t('Task'),
      1 => t('Ticket'),
    ),
    '#default_value' => variable_get('erpal_feedback_ticket', 0),
    '#description' => t('Select what type of node should be created by submited feedback in ERPAL.'),
  );

  $form['erpal']['erpal_feedback_task_body'] = array(
    '#type' => 'textarea',
    '#title' => t('Feedback body template'),
    '#default_value' => variable_get('erpal_feedback_task_body', ERPAL_FEEDBACK_RELOADED_TASK_DEFAULT_TEXT),
    '#description' => t('Description template of task or ticket that will be automatically created by feedback in ERPAL. You can use tokens here.'),
    '#required' => TRUE,
  );

  $form['erpal']['tokens'] = array(
    '#type' => 'fieldset',
    '#title' => t('Replacement patterns'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#description' => theme('token_tree', array('token_types' => array('feedback_reloaded'))),
    '#weight' => 10,
  );

  $form['#validate'][] = 'erpal_feedback_reloaded_settings_validate';
  $form['#submit'][] = 'erpal_feedback_reloaded_settings_submit';

  return system_settings_form($form);
}

/**
 * Validate callback for ERPAL admin settings form.
 */
function erpal_feedback_reloaded_settings_validate($form, &$form_state) {

  // Set a default value for a password to keep password security high.
  $stored_password = variable_get('erpal_feedback_password', '');
  $new_password = &$form_state['values']['erpal_feedback_password'];
  if (!empty($stored_password) && empty($new_password)) {
    $new_password = $stored_password;
  }
}

/**
 * ERPAL admin settings form submit handler.
 */
function erpal_feedback_reloaded_settings_submit($form, &$form_state) {
  $form_state['values']['erpal_feedback_url'] = rtrim($form_state['values']['erpal_feedback_url'], '/');
  unset($form_state['values']['erpal_feedback_rebuild']);
}

/**
 * Rebuild form submit handler.
 */
function erpal_feedback_reloaded_rebuild_settings_submit($form, &$form_state) {
  $vocabnames = array(
    'erpal_feedback_status' => 'task_status_terms',
    'erpal_feedback_priority' => 'priority_terms',
    'erpal_feedback_type' => 'task_type_terms',
  );

  $login = erpal_feedback_reloaded_request_login();

  if ($login) {
    foreach ($vocabnames as $key => $value) {
      $terms = erpal_feedback_reloaded_request(ERPAL_FEEDBACK_RELOADED_TAXONOMY_TERMS_URL . '?vocabname=' . $value, 'GET', NULL, $login);
      if (!$terms) {
        drupal_set_message(t('ERPAL get terms error.'), 'error');
        return;
      }

      $terms = drupal_json_decode($terms->data);

      $option_list = array();
      foreach ($terms as $tkey => $term) {
        $option_list[$term['tid']] = $term['name'];
      }

      // Get the field info.
      $info = field_info_field('field_' . $key);
      $info['settings']['allowed_values'] = $option_list;
      field_update_field($info);
    }
    drupal_set_message(t('Field values updated.'));
  }
  else {
    drupal_set_message(t('ERPAL login error.'), 'error');
  }
}
