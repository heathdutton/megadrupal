<?php

/**
 * @file
 * Admin page callbacks for access_code module.
 */

/**
 * Form constructor for access code settings.
 *
 * @see system_settings_form()
 * @ingroup forms
 */
function access_code_admin_settings($form) {

  $form['auto_code'] = array(
    '#type' => 'fieldset',
    '#title' => 'Access code generation',
  );

  $form['auto_code']['access_code_auto_code_length'] = array(
      '#type' => 'select',
      '#options' => array(
        4 => 4,
        5 => 5,
        6 => 6,
        7 => 7,
        8 => 8,
        9 => 9,
        10 => 10,
        11 => 11,
        12 => 12
      ),
      '#title' => t('Length of auto generated code'),
      '#default_value' => variable_get('access_code_auto_code_length', 8),
      '#description' => t('Total length of access code EXCLUDING the prefix.'),
  );

  $form['auto_code']['access_code_auto_code_prefix'] = array(
      '#type' => 'textfield',
      '#size' => 20,
      '#title' => t('Code prefix'),
      '#default_value' => variable_get('access_code_auto_code_prefix'),
      '#description' => t('Optional string of characters to append to the beginning of access codes.'),
  );

  $form['auto_code']['access_code_auto_code_format'] = array(
      '#type' => 'radios',
      '#title' => t('Format of code'),
      '#options' => array(
        'alpha' => t('Alphanumeric'),
        'numbers' => t('Numbers'),
        'letters' => t('Letters'),
      ),
      '#default_value' => variable_get('access_code_auto_code_format', 'alpha'),
  );

  $form['access_code_expiration_default'] = array(
      '#type' => 'select',
      '#title' => t('Default expiration period'),
      '#options' => array(
        'none' => t('None'),
        '1 day' => t('One Day'),
        '1 week' => t('One Week'),
        '1 month' => t('One Month'),
        '3 months' => t('Three Months'),
        '6 months' => t('Six Months'),
        '1 year' => t('One Year'),
        '2 years' => t('Two Years'),
        '3 years' => t('Three Years'),
        '5 years' => t('Five Years'),
        '10 years' => t('Ten Years'),
      ),
      '#default_value' => variable_get('access_code_expiration_default', '1 month'),
      '#description' => t('Automatically sets the expiration date a set period into the future when creating a new access code.'),
  );

  return system_settings_form($form);
}