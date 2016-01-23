<?php

/**
 * @file
 * Documentation of the hooks that invokes the module khipu_field.
 */

/**
 * Implements hook_khipu_field_theme_form_info().
 *
 * This hook returns an array of theme functions that print the payment button.
 */
function hook_khipu_field_theme_form_info() {
  return array(
    'khipu_field_redirect_form_field' => t('Default'),
    'khipu_field_redirect_form_table' => t('Table'),
  );
}


/**
 * This hook is called when creating new order. Additionally, attached an array
 * containing data that generated the order and other information that has been
 * attached to the form of khipu.
 *
 * @param $order
 *   The order.
 * @param $data
 *   Informacion que se envio del formulario khipu.
 */
function hook_khipu_field_create_new_order($order, $data) {

}


/**
 * This hook is called when updating the status of an order.
 *
 * @param $order
 *   The order.
 * @param $old_status
 *   The old status.
 * @param $new_status
 *   The new status.
 */
function hook_khipu_field_order_update_status($order, $old_status, $new_status) {

}
