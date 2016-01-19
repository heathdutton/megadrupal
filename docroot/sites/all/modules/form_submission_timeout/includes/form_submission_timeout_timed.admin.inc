<?php
/**
 * @file
 * File for configuration settings.
 */

/**
 * Function to implement configuration form.
 */
function form_submission_timeout_timed_configuration($form, &$form_state) {
  /**
   * One Time - Form submission will be activated only once for given date and time.
   * Everyday - Form submission will activate everyday during a specific time period.
   * Weekdays - Form submission will activate on weekdays only.
   * Weekends - Form submission will activate on weekends only.
   */
  $form['sub_out_stop_submission_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Timed Submission'),
    '#theme' => 'form_submission_timeout_timed_config_form',
    '#tree' => TRUE,
    '#collapsed' => FALSE,
    '#collapsible' => FALSE,
  );

  // Populate form ids into the settings form.
  $form_ids = variable_get('sub_out_stop_form_ids', array());
  foreach ($form_ids as $form_id => $form_detail) {
    $form['sub_out_stop_submission_fieldset'][$form_id]['sub_out_stop_form_id_display'] = array(
      '#markup' => $form_detail['sub_out_stop_form_id'],
    );
    $form['sub_out_stop_submission_fieldset'][$form_id]['sub_out_stop_form_id'] = array(
      '#type' => 'hidden',
      '#value' => $form_detail['sub_out_stop_form_id'],
    );
    $form['sub_out_stop_submission_fieldset'][$form_id]['sub_out_timeout_frequency'] = array(
      '#type' => 'select',
      '#options' => array(
        'none' => t('None'),
        'once' => t('Once'),
        'everyday' => t('Everyday'),
        'weekdays' => t('Weekdays'),
        'weekends' => t('Weekends')
      ),
      '#default_value' => $form_detail['sub_out_timeout_frequency'],
    );
    $form['sub_out_stop_submission_fieldset'][$form_id]['sub_out_start_date'] = array(
      '#type' => 'date',
      '#default_value' => $form_detail['sub_out_start_date'],
    );
    $form['sub_out_stop_submission_fieldset'][$form_id]['sub_out_start_timeout_period'] = array(
      '#type' => 'textfield',
      '#default_value' => $form_detail['sub_out_start_timeout_period'],
      '#size' => 5
    );
    $form['sub_out_stop_submission_fieldset'][$form_id]['sub_out_stop_date'] = array(
      '#type' => 'date',
      '#default_value' => $form_detail['sub_out_stop_date'],
      '#states' => array(
        'invisible' => array(
          ':input[name="sub_out_stop_submission_fieldset[' . $form_id . '][sub_out_timeout_frequency]"]' => array('value' => 'once'),
        ),
      ),
    );
    $form['sub_out_stop_submission_fieldset'][$form_id]['sub_out_stop_timeout_period'] = array(
      '#type' => 'textfield',
      '#default_value' => $form_detail['sub_out_stop_timeout_period'],
      '#size' => 5
    );
    $form['sub_out_stop_submission_fieldset'][$form_id]['sub_out_stop_timeout_message'] = array(
      '#type' => 'textfield',
      '#size' => 15,
      '#default_value' => $form_detail['sub_out_stop_timeout_message'],
      '#attributes' => array(
        'placeholder' => t('Session timed out.')
      ),
    );
  }

  $form['sub_out_stop_submission_fieldset']['new_form_entry']['sub_out_new_stop_form_id'] = array(
    '#type' => 'textfield',
    '#size' => '25',
    '#attributes' => array(
      'placeholder' => t('$form_id'),
    ),
    '#description' => t('Form id'),
  );
  $form['sub_out_stop_submission_fieldset']['new_form_entry']['sub_out_new_timeout_frequency'] = array(
    '#type' => 'select',
    '#options' => array(
      'none' => t('None'),
      'once' => t('Once'),
      'everyday' => t('Everyday'),
      'weekdays' => t('Weekdays'),
      'weekends' => t('Weekends')
    ),
    '#description' => t('Frequency'),
  );

  $form['sub_out_stop_submission_fieldset']['new_form_entry']['sub_out_new_start_date'] = array(
    '#type' => 'date',
    '#description' => t('Session start date'),
  );

  $form['sub_out_stop_submission_fieldset']['new_form_entry']['sub_out_new_start_timeout_period'] = array(
    '#type' => 'textfield',
    '#size' => '5',
    '#attributes' => array(
      'placeholder' => '00:00',
      'id' => 'start-time',
    ),
    '#description' => t('24 hour format') . '<br>' . t('e.g, hh:mm'),
  );

  $form['sub_out_stop_submission_fieldset']['new_form_entry']['sub_out_new_stop_date'] = array(
    '#type' => 'date',
    '#description' => t('Session stop date'),
    '#states' => array(
      'invisible' => array(
        ':input[name="sub_out_stop_submission_fieldset[new_form_entry][sub_out_new_timeout_frequency]"]' => array('value' => 'once'),
      ),
    ),
  );

  $form['sub_out_stop_submission_fieldset']['new_form_entry']['sub_out_new_stop_timeout_period'] = array(
    '#type' => 'textfield',
    '#size' => '5',
    '#attributes' => array(
      'placeholder' => '00:00',
      'id' => 'end-time',
    ),
    '#description' => t('24 hour format') . '<br>' . t('e.g, hh:mm'),
  );

  $form['sub_out_stop_submission_fieldset']['new_form_entry']['sub_out_new_stop_timeout_message'] = array(
    '#type' => 'textfield',
    '#size' => 15,
    '#attributes' => array(
      'placeholder' => 'Session timeout.',
      'id' => 'session-msg',
    ),
    '#description' => t('Message to show on') . '<br>' . t('form submission timeout'),
  );

  $form['#attached']['css'] = array(
    drupal_get_path('module', 'form_submission_timeout') . '/css/form-submission-timeout.css',
  );
  $form['#attached']['js'] = array(
    drupal_get_path('module', 'form_submission_timeout') . '/js/form_submission_timeout.js',
  );

  $form['sub_out_submit'] = array(
    '#type' => 'submit',
    '#value' => 'Save',
  );
  return $form;
}

/**
 * Function to validate form submit values.
 */
function form_submission_timeout_timed_configuration_validate($form, &$form_state) {
  $values = $form_state['values']['sub_out_stop_submission_fieldset'];
  $new_values = $values['new_form_entry'];
  $form_ids = variable_get('sub_out_stop_form_ids', array());
  $new_form_id = $new_values['sub_out_new_stop_form_id'];
  unset($values['new_form_entry']);

  // Check for duplicate values and blank spaces for new form IDs.
  if (!empty($new_values['sub_out_new_stop_form_id'])) {
    if (array_key_exists($new_form_id, $form_ids)) {
      form_set_error('sub_out_stop_submission_fieldset][new_form_entry][sub_out_new_stop_form_id', t('Only one rule per form is allowed.'));
    }
    else {
      if (strlen($new_form_id) != 0 && strpos($new_form_id, ' ')) {
        form_set_error('sub_out_stop_submission_fieldset][new_form_entry][sub_out_new_stop_form_id', t('Form ID cannot contain blank spaces.'));
      }
    }

    if (!preg_match("/(2[0-3]|[00][0-9]|10):([0-5][0-9])/", $new_values['sub_out_new_start_timeout_period'])) {
      form_set_error('sub_out_stop_submission_fieldset][new_form_entry][sub_out_new_start_timeout_period', t('Timeout value can only be in 24 hour time format e.g, hh:mm.'));
    }
    if (!preg_match("/(2[0-3]|[00][0-9]|10):([0-5][0-9])/", $new_values['sub_out_new_stop_timeout_period'])) {
      form_set_error('sub_out_stop_submission_fieldset][new_form_entry][sub_out_new_stop_timeout_period', t('Timeout value can only be in 24 hour time format e.g, hh:mm.'));
    }

    if ($new_values['sub_out_new_start_timeout_period'] == $new_values['sub_out_new_stop_timeout_period']) {
      form_set_error('sub_out_stop_submission_fieldset][new_form_entry][sub_out_new_stop_timeout_period', t('Timeout value can not be same as Time start value.'));
    }
    if ($new_values['sub_out_new_start_date'] == $new_values['sub_out_new_stop_date'] && $new_values['sub_out_new_start_timeout_period'] == $new_values['sub_out_new_stop_timeout_period']) {
      form_set_error('sub_out_stop_submission_fieldset][new_form_entry][sub_out_new_stop_date', t('Start date can not be same as End date.'));
    }
  }

}

/**
 * Submit handler for config form.
 */
function form_submission_timeout_timed_configuration_submit($form, &$form_state) {
  $values = $form_state['values']['sub_out_stop_submission_fieldset'];
  $new_values = $values['new_form_entry'];

  // Update existing form_id values.
  if (count($values) > 1) {
    unset($values['new_form_entry']);
    _form_submission_timeout_update_form_ids('sub_out_stop_form_ids', $values, 'update');
  }

  if (!empty($new_values['sub_out_new_stop_form_id'])) {
    $new_form_id[$new_values['sub_out_new_stop_form_id']] = array(
      'sub_out_stop_form_id' => $new_values['sub_out_new_stop_form_id'],
      'sub_out_timeout_frequency' => $new_values['sub_out_new_timeout_frequency'],
      'sub_out_start_date' => $new_values['sub_out_new_start_date'],
      'sub_out_start_timeout_period' => $new_values['sub_out_new_start_timeout_period'],
      'sub_out_stop_date' => $new_values['sub_out_new_stop_date'],
      'sub_out_stop_timeout_period' => $new_values['sub_out_new_stop_timeout_period'],
      'sub_out_stop_timeout_message' => empty($new_values['sub_out_new_stop_timeout_message']) ? 'Session expired.' : $new_values['sub_out_new_stop_timeout_message'],
    );

    // Save and update the settings.
    _form_submission_timeout_update_form_ids('sub_out_stop_form_ids', $new_form_id, 'add');
  }
  drupal_set_message(t('The settings have been saved'), 'status');
}
