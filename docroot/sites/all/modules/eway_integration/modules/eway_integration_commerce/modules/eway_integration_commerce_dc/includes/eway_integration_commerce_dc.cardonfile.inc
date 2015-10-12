<?php

/**
 * @file
 * Commerce cardonfile integration.
 */

/**
 * Alter cardonfile fields.
 * @param $form
 * @param $form_state
 * @param $form_id
 */
function eway_integration_commerce_dc_form_alter(&$form, &$form_state, $form_id) {
  if ($form_id == 'commerce_cardonfile_card_form') {
    $form['credit_card']['type']['#options'] = eway_integration_credit_card_types();
    // Load Payment information settings.
    $payment_method = commerce_payment_method_instance_load(EWAY_INTEGRATION_PAYMENT_METHOD_DC_NAME.'|commerce_payment_'.EWAY_INTEGRATION_PAYMENT_METHOD_DC_NAME);
    $card_types = $payment_method['settings']['card_types'];
    $form['credit_card']['type']['#options'] = array_intersect_key(eway_integration_credit_card_types(), drupal_map_assoc($card_types));
  }
}
