<?php

/**
 * @todo Write file documentation.
 */

/**
 * Mollie Payment settings.
 */
function mollie_payment_settings_form($form, &$form_state) {
  $form['mollie_payment_test_message'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show message on all pages if Mollie is in test mode.'),
    '#default_value' => variable_get('mollie_payment_test_message', TRUE),
  );

  return system_settings_form($form);
}
