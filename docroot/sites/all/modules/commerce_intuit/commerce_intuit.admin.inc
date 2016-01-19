<?php

/**
 * @file
 * Administration forms for the Commerce Intuit.
 */

/**
 * Menu callback; Displays the administration settings for Intuit Payment.
 */
function commerce_intuit_admin_settings() {
  $form = array();

  $form['commerce_intuit'] = array(
    '#type' => 'fieldset',
    '#title' => t('Intuit Settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['commerce_intuit']['commerce_intuit_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#default_value' => variable_get('commerce_intuit_title', 'Intuit'),
    '#description' => t('Override the default title of the commerce intuit payment method.'),
  );

  $form['commerce_intuit']['commerce_intuit_production'] = array(
    '#type' => 'radios',
    '#title' => t('Intuit Server'),
    '#options' => array(
      0 => t('Intuit Sandbox'),
      1 => t('Intuit Production'),
    ),
    '#default_value' => variable_get('commerce_intuit_production', 0),
  );

  $form['commerce_intuit']['commerce_intuit_display_response'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display response messages'),
    '#description' => t('Display transaction response messages from Intuit when a payment fails. These messages are useful for debugging but you may want to hide them in production.'),
    '#default_value' => variable_get('commerce_intuit_display_response', 1),
  );

  $form['commerce_intuit']['production'] = array(
    '#type' => 'fieldset',
    '#title' => t('Intuit Production Settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['commerce_intuit']['production']['commerce_intuit_applogin'] = array(
    '#type' => 'textfield',
    '#title' => t('APP Login'),
    '#default_value' => variable_get('commerce_intuit_applogin', ''),
    '#states' => array(
      'required' => array(
        ':input[name="commerce_intuit_production"]' => array('value' => 1),
      ),
    ),
  );

  $form['commerce_intuit']['production']['commerce_intuit_connticket'] = array(
    '#type' => 'textfield',
    '#title' => t('Connection Ticket'),
    '#default_value' => variable_get('commerce_intuit_connticket', ''),
    '#states' => array(
      'required' => array(
        ':input[name="commerce_intuit_production"]' => array('value' => 1),
      ),
    ),
  );

  $form['commerce_intuit']['production']['commerce_intuit_appid'] = array(
    '#type' => 'textfield',
    '#title' => t('Application ID'),
    '#default_value' => variable_get('commerce_intuit_appid', ''),
    '#states' => array(
      'required' => array(
        ':input[name="commerce_intuit_production"]' => array('value' => 1),
      ),
    ),
  );

  $form['commerce_intuit']['production']['commerce_intuit_pem'] = array(
    '#type' => 'textfield',
    '#title' => t('.Pem PATH'),
    '#default_value' => variable_get('commerce_intuit_pem', ''),
    '#states' => array(
      'required' => array(
        ':input[name="commerce_intuit_production"]' => array('value' => 1),
      ),
    ),
  );

  $form['commerce_intuit']['sandbox'] = array(
    '#type' => 'fieldset',
    '#title' => t('Intuit Sandbox Settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['commerce_intuit']['sandbox']['commerce_intuit_sand_applogin'] = array(
    '#type' => 'textfield',
    '#title' => t('APP Login'),
    '#default_value' => variable_get('commerce_intuit_sand_applogin', ''),
    '#states' => array(
      'required' => array(
        ':input[name="commerce_intuit_production"]' => array('value' => 0),
      ),
    ),
  );

  $form['commerce_intuit']['sandbox']['commerce_intuit_sand_connticket'] = array(
    '#type' => 'textfield',
    '#title' => t('Connection Ticket'),
    '#default_value' => variable_get('commerce_intuit_sand_connticket', ''),
    '#states' => array(
      'required' => array(
        ':input[name="commerce_intuit_production"]' => array('value' => 0),
      ),
    ),
  );

  $form['commerce_intuit']['sandbox']['commerce_intuit_sand_appid'] = array(
    '#type' => 'textfield',
    '#title' => t('Application ID'),
    '#default_value' => variable_get('commerce_intuit_sand_appid', ''),
    '#states' => array(
      'required' => array(
        ':input[name="commerce_intuit_production"]' => array('value' => 0),
      ),
    ),
  );

  $form['commerce_intuit']['sandbox']['commerce_intuit_sand_pem'] = array(
    '#type' => 'textfield',
    '#title' => t('.Pem PATH'),
    '#default_value' => variable_get('commerce_intuit_sand_pem', ''),
    '#states' => array(
      'required' => array(
        ':input[name="commerce_intuit_production"]' => array('value' => 0),
      ),
    ),
  );

  return system_settings_form($form);
}

/**
 * Validate function for settings form.
 */
function commerce_intuit_admin_settings_validate($form, $form_state) {
  $key = $form_state['values']['commerce_intuit_production'] ? '' : 'sand_';
  if (trim($form_state['values']['commerce_intuit_' . $key . 'applogin']) == '') {
    form_set_error('commerce_intuit_' . $key . 'applogin', t('Please Enter a App Login'));
  }

  if (trim($form_state['values']['commerce_intuit_' . $key . 'connticket']) == '') {
    form_set_error('commerce_intuit_' . $key . 'connticket', t('Please Enter Connection Ticket'));
  }

  if (trim($form_state['values']['commerce_intuit_' . $key . 'appid']) == '') {
    form_set_error('commerce_intuit_' . $key . 'appid', t('Please Enter a Application Id'));
  }

  if (trim($form_state['values']['commerce_intuit_' . $key . 'pem']) == '') {
    form_set_error('commerce_intuit_' . $key . 'pem', t('Please Enter a Pem Path'));
  }
}
