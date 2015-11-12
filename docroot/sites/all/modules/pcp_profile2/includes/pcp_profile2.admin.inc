<?php

/**
 * @file
 * Admin interface for the pcp module
 */

/**
 * Form builder; Configure PCP fields for core profile.
 *
 * @ingroup forms
 */
function pcp_profile2_admin_settings($form, &$form_state) {
  $form['pcp_profile2_offset'] = array(
    '#type' => 'textfield',
    '#title' => t('Offset to ignore for Calculation'),
    '#default_value' => variable_get('pcp_profile2_offset', '0'),
    '#description' => t("If offset is 3, The percentage will be 100% even when user 
    		completes 22 fields out of 25."),
    '#element_validate' => array('element_validate_integer_positive'), 
  );

  return system_settings_form($form);
}
