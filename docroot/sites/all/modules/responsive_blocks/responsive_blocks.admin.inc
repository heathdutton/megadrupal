<?php

/**
 * @file
 * Administrative page callbacks for Responsive Block module.
 */

/**
 * Form constructor for the admin settings form.
 */
function responsive_blocks_admin_settings() {
  $form = array();
  $form['responsive_blocks_desktop'] = array(
    '#type' => 'textfield',
    '#title' => t('Desktop Resolution - Start point'),
    '#description' => t('Enter the Desktop Resolution Starting point in pixel'),
    '#default_value' => variable_get('responsive_blocks_desktop', 1600),
    '#size' => 40,
    '#required' => TRUE,
  );
  $form['responsive_blocks_ipad'] = array(
    '#type' => 'textfield',
    '#title' => t('IPAD Resolution - Start point'),
    '#description' => t('Enter the IPAD Resolution Starting point in pixel'),
    '#default_value' => variable_get('responsive_blocks_ipad', 780),
    '#size' => 40,
    '#required' => TRUE,
  );
  $form['responsive_blocks_mobile'] = array(
    '#type' => 'textfield',
    '#title' => t('Mobile Resolution - Start point'),
    '#description' => t('Enter the Mobile Resolution Starting point in pixel'),
    '#default_value' => variable_get('responsive_blocks_mobile', 480),
    '#size' => 40,
    '#required' => TRUE,
  );
  $form['#validate'][] = 'responsive_blocks_admin_validate';

  return system_settings_form($form);
}

/**
 * Handles Validation.
 */
function responsive_blocks_admin_validate($form, &$form_state) {
  $validate_device = array('responsive_blocks_desktop', 'responsive_blocks_ipad', 'responsive_blocks_mobile');
  foreach ($validate_device as $value) {
    if (!is_numeric($form_state['values'][$value])) {
      form_set_error($form[$value], t('Please enter a numeric value.'));
    }
  }
}
