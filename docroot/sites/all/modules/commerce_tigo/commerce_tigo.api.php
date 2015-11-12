<?php

/**
 * @file
 * Hook documentation for the Tigo Money module.
 */

/**
 * Alter the data array for the Tigo Money redirect form.
 *
 * Allows modules to alter the data array used to create a Tigo Money redirect
 * form prior to its form elements being created.
 *
 * @param &$data
 *   The data array used to create redirect form elements.
 * @param $order
 *   The full order object the redirect form is being generated for.
 *
 * @see commerce_tigo_redirect_form()
 */
function hook_commerce_tigo_redirect_form_data_alter(&$data, $order) {
  // No example.
}

/**
 * Act on a tigo transaction that just finished
 *
 * Allows modules to execute an action after a tigo transaction finished
 *
 * @param array $params
 *   The params returned from tigo.
 * @param object $order
 *   The full order object for the transaction.
 *
 * @see commerce_tigo_redirect_form_validate()
 */
function hook_commerce_tigo_finish_transaction($params, $order) {
  if (isset($params['nroFactura'])) {
    $order->field_nro_factura['und'][0]['value'] = $params['nroFactura'];
    commerce_order_save($order);
  }
}
