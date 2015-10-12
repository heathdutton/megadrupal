<?php

/**
 * @file
 * Admin functions (settings) for the hide_submit module.
 */

/**
 * Form builder. Configure hide submit settings.
 *
 * @ingroup forms
 * @see system_settings_form()
 */
function hide_submit_settings() {
  $form = array();

  $form['hide_submit_method'] = array(
    '#type' => 'select',
    '#options' => array('disable' => t('Disable the submit buttons.'), 'hide' => t('Hide the submit buttons.')),
    '#default_value' => variable_get('hide_submit_method', 'disable'),
    '#title' => t('Blocking method'),
    '#description' => t('Choose the blocking method.'),
  );

  $form['hide_submit_reset_time'] = array(
    '#type' => 'textfield',
    '#title' => t('Reset buttons after some time (ms).'),
    '#description' => t('Enter a value in milliseconds after which all buttons will be enabled. To disable this enter 0.'),
    '#default_value' => variable_get('hide_submit_reset_time', 5000),
    '#element_validate' => array('_hide_submit_is_numeric'),
    '#required' => TRUE,
  );

  $form['hide_submit_disable'] = array(
    '#type' => 'fieldset',
    '#title' => t('Disabling settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['hide_submit_disable']['hide_submit_abtext'] = array(
    '#type' => 'textfield',
    '#title' => t('Append to buttons'),
    '#description' => t('This text will be appended to each of the submit buttons.'),
    '#default_value' => variable_get('hide_submit_abtext', ''),
  );

  $form['hide_submit_disable']['hide_submit_atext'] = array(
    '#type' => 'textarea',
    '#title' => t('Add next to buttons'),
    '#description' => t('This text will be added next to the submit buttons.'),
    '#default_value' => variable_get('hide_submit_atext', ''),
  );

  $form['hide_submit_hide'] = array(
    '#type' => 'fieldset',
    '#title' => t('Hiding settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['hide_submit_hide']['hide_submit_hide_fx'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use fade effects?'),
    '#default_value' => variable_get('hide_submit_hide_fx', FALSE),
    '#description' => t('Enabling a fade in / out effect.'),
  );

  $form['hide_submit_hide']['hide_submit_hide_text'] = array(
    '#type' => 'textfield',
    '#title' => t('Processing text'),
    '#default_value' => variable_get('hide_submit_hide_text', 'Processing...'),
    '#description' => t('This text will be shown to the user instead of the submit buttons.'),
  );

  return system_settings_form($form);
}

function _hide_submit_is_numeric($element, &$form_state, $form) {
  if (!is_numeric($element['#value']) || !ctype_digit($element['#value'])) {
    form_error($element, t('This field only accepts integers.'));
  }
}
