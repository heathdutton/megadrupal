<?php

/**
 * @file
 * Administration form for timed messages
 */

/**
 * The admin page form to set up timed messages.
 * @return array
 *   the form structure
 * @see system_settings_form()
 */
function timed_messages_admin() {
  $variables = variable_get('timed_messages');

  $form = array();

  $form['timed_messages'] = array(
    '#tree' => TRUE,
    '#type' => 'container',
  );
  // -----general-----
  $form['timed_messages']['generalset'] = array(
    '#type' => 'fieldset',
    '#title' => t('General settings'),
  );
  $form['timed_messages']['generalset']['messages_class'] = array(
    '#type' => 'textfield',
    '#title' => t('Messages class'),
    '#default_value' => isset($variables['generalset']['messages_class']) ?
        $variables['generalset']['messages_class'] : 'messages',
    '#size' => 30,
    '#description' => t("The CSS class to select a message."),
  );
  $form['timed_messages']['generalset']['hide_with_krumo'] = array(
    '#type' => 'checkbox',
    '#title' => t('Also hide when krumo is found in message'),
    '#default_value' => isset($variables['generalset']['hide_with_krumo']) ?
        $variables['generalset']['hide_with_krumo'] : FALSE,
    '#description' => t("If not ticked, the message will not be hidden after the timer runs out if a krumo message is found."),
  );

  // -----status-----
  $form['timed_messages']['statusset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Status messages'),
  );
  $form['timed_messages']['statusset']['hide_status'] = array(
    '#type' => 'checkbox',
    '#title' => t('Hide status messages after timeout?'),
    '#default_value' => isset($variables['statusset']['hide_status']) ?
        $variables['statusset']['hide_status'] : TRUE,
  );
  $form['timed_messages']['statusset']['status_container'] = array(
    '#type' => 'container',
    '#states' => array(
      'invisible' => array(
        'input[name="timed_messages[statusset][hide_status]"]' => array('checked' => FALSE),
      ),
    ),
  );
  $form['timed_messages']['statusset']['status_container']['status_time'] = array(
    '#type' => 'textfield',
    '#title' => t('Status message time'),
    '#default_value' => isset($variables['statusset']['status_container']['status_time']) ?
        $variables['statusset']['status_container']['status_time'] : 5000,
    '#size' => 6,
    '#maxlength' => 6,
    '#description' => t("How long should the status message be shown (in ms)."),
  );
  $form['timed_messages']['statusset']['status_container']['status_class'] = array(
    '#type' => 'textfield',
    '#title' => t('Status message class'),
    '#default_value' => isset($variables['statusset']['status_container']['status_class']) ?
        $variables['statusset']['status_container']['status_class'] : 'status',
    '#size' => 30,
    '#description' => t("The CSS class to select a status."),
  );

  // -----warning-----
  $form['timed_messages']['warningset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Warning messages'),
  );
  $form['timed_messages']['warningset']['hide_warning'] = array(
    '#type' => 'checkbox',
    '#title' => t('Hide messages after timeout?'),
    '#default_value' => isset($variables['warningset']['hide_warning']) ?
        $variables['warningset']['hide_warning'] : TRUE,
  );
  $form['timed_messages']['warningset']['warning_container'] = array(
    '#type' => 'container',
    '#states' => array(
      'invisible' => array(
        'input[name="timed_messages[warningset][hide_warning]"]' => array('checked' => FALSE),
      ),
    ),
  );
  $form['timed_messages']['warningset']['warning_container']['warning_time'] = array(
    '#type' => 'textfield',
    '#title' => t('Warning message time'),
    '#default_value' => isset($variables['warningset']['warning_container']['warning_time']) ?
        $variables['warningset']['warning_container']['warning_time'] : 10000,
    '#size' => 6,
    '#maxlength' => 6,
    '#description' => t("How long should the warning message be shown (in ms)."),
  );
  $form['timed_messages']['warningset']['warning_container']['warning_class'] = array(
    '#type' => 'textfield',
    '#title' => t('Warning message class'),
    '#default_value' => isset($variables['warningset']['warning_container']['warning_class']) ?
        $variables['warningset']['warning_container']['warning_class'] : 'warning',
    '#size' => 30,
    '#description' => t("The CSS class to select a warning."),
  );

  // -----error-----
  $form['timed_messages']['errorset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Error messages'),
  );
  $form['timed_messages']['errorset']['hide_error'] = array(
    '#type' => 'checkbox',
    '#title' => t('Hide messages after timeout?'),
    '#default_value' => isset($variables['errorset']['hide_error']) ?
        $variables['errorset']['hide_error'] : TRUE,
  );
  $form['timed_messages']['errorset']['error_container'] = array(
    '#type' => 'container',
    '#states' => array(
      'invisible' => array(
        'input[name="timed_messages[errorset][hide_error]"]' => array('checked' => FALSE),
      ),
    ),
  );
  $form['timed_messages']['errorset']['error_container']['error_time'] = array(
    '#type' => 'textfield',
    '#title' => t('Error message time'),
    '#default_value' => isset($variables['errorset']['error_container']['error_time']) ?
        $variables['errorset']['error_container']['error_time'] : 15000,
    '#size' => 6,
    '#maxlength' => 6,
    '#description' => t("How long should the error message be shown (in ms)."),
  );
  $form['timed_messages']['errorset']['error_container']['error_class'] = array(
    '#type' => 'textfield',
    '#title' => t('Error message class'),
    '#default_value' => isset($variables['errorset']['error_container']['error_class']) ?
        $variables['errorset']['error_container']['error_class'] : 'error',
    '#size' => 30,
    '#description' => t("The CSS class to select an error."),
  );

  // -----testbuttons-----
  $form['timed_messages']['buttonset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Test buttons'),
    '#description' => t('You have to save your changes before testing.'),
  );
  $form['timed_messages']['buttonset']['test_status'] = array(
    '#type' => 'submit',
    '#value' => 'test status message',
    '#submit' => array('timed_messages_test_status_messages'),
  );
  $form['timed_messages']['buttonset']['test_warning'] = array(
    '#type' => 'submit',
    '#value' => 'test warning message',
    '#submit' => array('timed_messages_test_warning_messages'),
  );
  $form['timed_messages']['buttonset']['test_error'] = array(
    '#type' => 'submit',
    '#value' => 'test error message',
    '#submit' => array('timed_messages_test_error_messages'),
  );
  $form['timed_messages']['buttonset']['test_krumo'] = array(
    '#type' => 'submit',
    '#value' => 'test krumo message',
    '#submit' => array('timed_messages_test_krumo_messages'),
  );
  return system_settings_form($form);
}

/**
 * Sets a drupal message to test the status message.
 */
function timed_messages_test_status_messages() {
  drupal_set_message(t('This is a status message.'), 'status');
}

/**
 * Sets a drupal message to test the error message.
 */
function timed_messages_test_warning_messages() {
  drupal_set_message(t('This is a warning message.'), 'warning');
}

/**
 * Sets a drupal message to test the error message.
 */
function timed_messages_test_error_messages() {
  drupal_set_message(t('This is a error message.'), 'error');
}

/**
 * Sets a krumo message with dmp() to test the  message.
 */
function timed_messages_test_krumo_messages() {
  if (module_exists('devel')) {
    dpm(array('foo' => 'bar'));
  }
  else {
    drupal_set_message(t('Devel must be installed!'), 'error');
  }
}
