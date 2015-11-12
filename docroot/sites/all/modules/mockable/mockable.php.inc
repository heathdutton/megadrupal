<?php

/**
 * @file
 * Hook definitions. These functions are never called, they are here for
 * documentation purposes only. Replace "hook" with your module name, clear
 * your cache, and your function will be called at the appropriate time.
 */


/**
 * Help with building the mockable administration form.
 *
 * Add information about mock objects in your module.
 * See this module in action by enabling Mockable Time Example and visiting
 * admin/config/development/mockable.
 */
function hook_mockable_info() {
  return array(
    'mockable_time_example_time' => array(
      'title' => t('Simulate the current time'),
      'settings' => array(
        'mockable_time_example_time' => array(
          '#title' => t('Set the time to simulate'),
          '#type' => 'textfield',
          '#default_value' => format_date(variable_get('mockable_time_example_time', strtotime("Wed Nov 09 2011 1:30:00")), 'custom', 'H:i:s')
        ),
      ),
    ),
  );
}

/**
 * Help with validating the mockable administration form.
 *
 * Add validation for mock objects in your module.
 * See this module in action by enabling Mockable Time Example and visiting
 * admin/config/development/mockable.
 *
 * @param $form
 *   The current form being worked on. This is passed directly from the main
 *   validation handler for Mockable's administration form.
 * @param &$form_state
 *   The state of the current form being worked on. This is passed directly from
 *   the main validation handler for Mockable's administration form.
 */
function hook_mockable_info_validate($form, &$form_state) {
  $time = $form_state['complete form']['mockable_time_example_time_fieldset']['mockable_time_example_time']['#value'];
  if (!preg_match('/^([0-1][0-9]|2[0-3]):[0-5][0-9]:[0-5][0-9]$/', $time)) {
    form_set_error('mockable_time_example_time', t('Please enter a time in the format HH:MM:SS, for example 01:23:45, between 00:00:00 and 23:59:59.'));
  }
}

/**
 * Help with submitting the mockable administration form.
 *
 * Add submit handlers for mock objects in your module.
 * See this module in action by enabling Mockable Time Example and visiting
 * admin/config/development/mockable.
 *
 * @param $form
 *   The current form being worked on. This is passed directly from the main
 *   validation handler for Mockable's administration form.
 * @param &$form_state
 *   The state of the current form being worked on. This is passed directly from
 *   the main validation handler for Mockable's administration form.
 */
function hook_mockable_info_submit($form, &$form_state) {
  $time = $form_state['complete form']['mockable_time_example_time_fieldset']['mockable_time_example_time']['#value'];
  variable_set('mockable_time_example_time', strtotime('Wed Nov 09 2011 ' . $time));
  drupal_set_message(t('Now simulating the time @t.', array('@t' => $time)));
}
