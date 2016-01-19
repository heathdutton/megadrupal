<?php
/**
 * @file
 * Configuration settings for submission countdown.
 */

/**
 * Function to implement configuration form.
 */
function form_submission_timeout_countdown_configuration($form, &$form_state) {
  $form['#attached']['js'] = array(
    drupal_get_path('module', 'form_submission_timeout') . '/js/form_submission_timeout.js',
  );
  $form['sub_out_timeout_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Submission Timer'),
    '#theme' => 'form_submission_timeout_config_form',
    '#tree' => TRUE,
    '#collapsed' => FALSE,
    '#collapsible' => FALSE,
  );

  // Populate form ids into the settings form.
  $form_ids = variable_get('sub_out_form_ids', array());
  foreach ($form_ids as $form_id => $form_detail) {
    $form['sub_out_timeout_fieldset'][$form_id]['sub_out_form_id_display'] = array(
      '#markup' => $form_detail['sub_out_form_id'],
    );
    $form['sub_out_timeout_fieldset'][$form_id]['sub_out_form_id'] = array(
      '#type' => 'hidden',
      '#value' => $form_detail['sub_out_form_id'],
    );
    $form['sub_out_timeout_fieldset'][$form_id]['sub_out_show_timer'] = array(
      '#type' => 'checkbox',
      '#default_value' => $form_detail['sub_out_show_timer'],
      '#title' => t('Show'),
    );
    $form['sub_out_timeout_fieldset'][$form_id]['sub_out_timeout_period'] = array(
      '#type' => 'textfield',
      '#size' => 15,
      '#default_value' => $form_detail['sub_out_timeout_period'],
    );
    $form['sub_out_timeout_fieldset'][$form_id]['sub_out_timeout_message'] = array(
      '#type' => 'textfield',
      '#default_value' => $form_detail['sub_out_timeout_message'],
      '#attributes' => array(
        'placeholder' => t('Session timed out.')
      ),
    );
  }

  // Input new values.
  $form['sub_out_timeout_fieldset']['new_form_entry']['sub_out_new_show_timer'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show'),
  );
  $form['sub_out_timeout_fieldset']['new_form_entry']['sub_out_new_form_id'] = array(
    '#type' => 'textfield',
    '#size' => 35,
    '#attributes' => array(
      'placeholder' => t('$form_id'),
    ),
  );
  $form['sub_out_timeout_fieldset']['new_form_entry']['sub_out_new_timeout_period'] = array(
    '#type' => 'textfield',
    '#size' => 15,
    '#attributes' => array(
      'placeholder' => t('300'),
    ),
  );
  $form['sub_out_timeout_fieldset']['new_form_entry']['sub_out_new_timeout_message'] = array(
    '#type' => 'textfield',
    '#attributes' => array(
      'placeholder' => t('Session timed out.')
    ),
  );

  $form['sub_out_submit'] = array(
    '#type' => 'submit',
    '#value' => 'Save configuration',
  );
  return $form;
}

/**
 * Function to validate form submit values.
 */
function form_submission_timeout_countdown_configuration_validate($form, &$form_state) {
  $values = $form_state['values']['sub_out_timeout_fieldset'];
  $new_values = $values['new_form_entry'];
  $form_ids = variable_get('sub_out_form_ids', array());
  $new_form_id = $new_values['sub_out_new_form_id'];
  unset($values['new_form_entry']);

  // Validate existing form_id timer period.
  foreach ($values as $form_id => $info) {
    if (empty($info['sub_out_timeout_period'])) {
      form_set_error('sub_out_timeout_fieldset][' . $form_id . '][sub_out_timeout_period', t('Timeout period cannot be blank'));
    }
    if (empty($info['sub_out_timeout_message'])) {
      form_set_error('sub_out_timeout_fieldset][' . $form_id . '][sub_out_timeout_message', t('Timeout message cannot be blank'));
    }
    if (!preg_match('/^[0-9]*$/', $info['sub_out_timeout_period'])) {
      form_set_error('sub_out_timeout_fieldset][' . $form_id . '][sub_out_timeout_period', t('Timeout value can only be numeric'));
    }
  }

  // Check for duplicate values and blank spaces for new form IDs.
  if (!empty($new_values['sub_out_new_form_id'])) {
    if (array_key_exists($new_form_id, $form_ids)) {
      form_set_error('sub_out_timeout_fieldset][new_form_entry][sub_out_new_form_id', t('Only one rule per form is allowed.'));
    }
    else {
      if (strlen($new_form_id) != 0 && strpos($new_form_id, ' ')) {
        form_set_error('sub_out_timeout_fieldset][new_form_entry][sub_out_new_form_id', t('Form ID cannot contain blank spaces'));
      }
    }

    if (!preg_match('/^[0-9]*$/', $new_values['sub_out_new_timeout_period'])) {
      form_set_error('sub_out_timeout_fieldset][new_form_entry][sub_out_new_timeout_period', t('Timeout value can only be numeric'));
    }
  }
}

/**
 * Submit handler for countdown configuration form.
 */
function form_submission_timeout_countdown_configuration_submit($form, &$form_state) {
  $values = $form_state['values']['sub_out_timeout_fieldset'];
  $new_values = $values['new_form_entry'];

  // Update existing form_id values.
  if (count($values) > 1) {
    unset($values['new_form_entry']);
    _form_submission_timeout_update_form_ids('sub_out_form_ids', $values, 'update');
  }

  if (!empty($new_values['sub_out_new_form_id'])) {
    $new_form_id[$new_values['sub_out_new_form_id']] = array(
      'sub_out_form_id' => $new_values['sub_out_new_form_id'],
      'sub_out_show_timer' => $new_values['sub_out_new_show_timer'],
      'sub_out_timeout_period' => empty($new_values['sub_out_new_timeout_period']) ? '300' : $new_values['sub_out_new_timeout_period'],
      'sub_out_timeout_message' => empty($new_values['sub_out_new_timeout_message']) ? 'Session timed out.' : $new_values['sub_out_new_timeout_message'],
    );

    // Save and update the settings.
    _form_submission_timeout_update_form_ids('sub_out_form_ids', $new_form_id, 'add');
  }
  drupal_set_message(t('The settings have been saved'), 'status');
}
