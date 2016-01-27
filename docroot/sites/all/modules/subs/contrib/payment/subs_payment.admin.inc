<?php
/**
 * @file
 * Subs Payment administration pages.
 */

/**
 * Form callback.
 * Returns Subs Payment settings form.
 *
 * @see subs_payment_menu()
 */
function subs_payment_settings_form() {
  $form = array();

  $form['gateways'] = array(
    '#type' => 'fieldset',
    '#title' => t('Payment Gateways'),
    '#description' => t('Enable or disable available Payment Gateways'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $gateways = subs_payment_get_gateways(FALSE);

  foreach ($gateways as $name => $value) {
    $form['gateways'][$name] = array(
      '#title' => $value['label'],
      '#type' => 'checkbox',
      '#name' => "subs_payment_gateways_{$name}",
      '#default_value' => $value['status'],
    );
  }

  return system_settings_form($form);
}