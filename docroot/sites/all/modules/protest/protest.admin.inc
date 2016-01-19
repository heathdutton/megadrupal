<?php

/**
 * @file
 * Administrative page callbacks for the protest module.
 */


/**
* The admin settings form
*/
function protest_admin_settings($form, &$form_state) {
  $form = array();

  // Overlay option
  $form['protest_overlay'] = array(
    '#description' => t('Apply an overlay layer to the viewport making the content page no accessible.'),
    '#title' => t('Apply overlay layer'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('protest_overlay', TRUE),
  );

  // Display only once per session
  $form['protest_once'] = array(
    '#description' => t('Only display the blackout overlay one page load.'),
    '#title' => t('Display protest only once per session'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('protest_once', TRUE),
  );

  // Dates
  $protest_dates = variable_get('protest_dates', variable_get('protest_dates_default', FALSE));

  $form['protest_date_from'] = array(
    '#type' => 'fieldset',
    '#title' => t('From Date'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#description' => t('Set when your message should start to get displayed.'),
  );

  $form['protest_date_from']['date_from'] = array(
    '#type' => 'date',
    '#title' => t('Date'),
    '#default_value' => $protest_dates['from']['date'],
  );

  $form['protest_date_from']['time_from'] = array(
    '#type' => 'textfield',
    '#title' => t('Time'),
    '#default_value' => $protest_dates['from']['time'],
    '#size' => 5,
    '#maxlength' => 5,
    '#description' => t('Format 24H: "00:00" to "23:59"'),
  );

  $form['protest_date_to'] = array(
    '#type' => 'fieldset',
    '#title' => t('To Date'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#description' => t('Set when your message should stop being displayed.'),
  );

  $form['protest_date_to']['date_to'] = array(
    '#type' => 'date',
    '#title' => t('Date'),
    '#default_value' => $protest_dates['to']['date'],
  );

  $form['protest_date_to']['time_to'] = array(
    '#type' => 'textfield',
    '#title' => t('Time'),
    '#default_value' => $protest_dates['to']['time'],
    '#size' => 5,
    '#maxlength' => 5,
    '#description' => t('Format 24H: "00:00" to "23:59"'),
  );

  // Explanation Message
  $form['protest_body'] = array(
    '#description' => t('Write the message for your protest here.'),
    '#title' => t('Explanation Message'),
    '#type' => 'textarea',
    '#default_value' => variable_get('protest_body', variable_get('protest_body_default', FALSE)),
  );

  // Let the system module add submit buttons and manage validation
  //$form = system_settings_form($form);

  //Don't forget the submit buttons
  $form['buttons']['submit'] = array('#type' => 'submit', '#value' => t('Save configuration') );
  $form['buttons']['reset'] = array('#type' => 'submit', '#value' => t('Reset to defaults') );

  return $form;
}

/**
 * Validation function
 */
function protest_admin_settings_validate($form, &$form_state) {
  $time_24h = "/^(([0-1]{1}[0-9]{1})|([2]{1}[0-3]{1})):(([0-5]{1}[0-9]{1}))$/"; /* 00:00 to 23:59 */

  if (!preg_match($time_24h, $form_state['values']['time_from'])) {
    form_set_error('time_from', t('You must enter a valid time.'));
  }
  if (!preg_match($time_24h, $form_state['values']['time_to'])) {
    form_set_error('time_to', t('You must enter a valid time.'));
  }
}

/**
 * Submit function
 */
function protest_admin_settings_submit($form, &$form_state) {
  $op = isset($form_state['values']['op']) ? $form_state['values']['op'] : '';

  if ($op == t('Reset to defaults')) {
    protest_restore();
    drupal_set_message(t('The configuration options have been reset to their default values.'));
  }
  else {
    variable_set('protest_overlay', $form_state['values']["protest_overlay"]);
    variable_set('protest_once', $form_state['values']["protest_once"]);
    $protest_dates = array(
      'from' => array(
        'date' => $form_state['values']['date_from'],
        'time' => $form_state['values']['time_from']
      ),
      'to' => array(
        'date' => $form_state['values']['date_to'],
        'time' => $form_state['values']['time_to']
      ),
    );
    variable_set('protest_dates', $protest_dates);
    
    $from = $protest_dates['from']['date']['year'] . '-' .
            $protest_dates['from']['date']['month'] . '-' .
            $protest_dates['from']['date']['day'] . ' ' .
            $protest_dates['from']['time'];
    $to = $protest_dates['to']['date']['year'] . '-' .
          $protest_dates['to']['date']['month'] . '-' .
          $protest_dates['to']['date']['day'] . ' ' .
          $protest_dates['to']['time'];
          
    // We need a unix timestamp in the timezone of the Drupal website, not the
    // system time.
    $website_timezone = new DateTimeZone(variable_get('date_default_timezone', FALSE));

    $from_datetime = new DateTime($from, $website_timezone);
    $from_datestamp = $from_datetime->format('U');
    variable_set('protest_from_time', $from_datestamp);

    $to_datetime = new DateTime($to, $website_timezone);
    $to_datestamp = $to_datetime->format('U');
    variable_set('protest_to_time', $to_datestamp);
    
    variable_set('protest_body', $form_state['values']["protest_body"]);

    drupal_set_message(t('The configuration options have been saved.'));
  }

  //optionally set the redirect value in form_submit ...
}

/**
 * Restore the original values.
 */
function protest_restore() {
  variable_set('protest_overlay', TRUE);
  variable_set('protest_once', TRUE);
  variable_set('protest_dates', variable_get('protest_dates_default'));
  variable_set('protest_body', variable_get('protest_body_default', FALSE));
  variable_set('protest_from_time', variable_get('protest_from_time_default', FALSE));
  variable_set('protest_to_time', variable_get('protest_to_time_default', FALSE));
}
