<?php
/**
*/

/**
 * @file
 * Administration functions and forms.
 */

function pollim_settings_form() {
  $form = array();

  $form['pollim_threshold_count'] = array(
    '#type' => 'select',
    '#title' => t('How many votes is allowed to from a single IP address in ten minutes?'),
    '#default_value' => variable_get('pollim_threshold_count', 10),
    '#options' => array(
        '0' => t('Unlimited'),
        '1' => '1',
        '5' => '5',
        '10' => '10',
        '20' => '20',
        '50' => '50',
        '500' => '500',
      )
  );
  return system_settings_form($form);
}
