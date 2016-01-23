<?php

/**
 * @file
 * Implements functionality for to use Khipu.
 */

/**
 * This hook is called before sending the data to the site of khipu.
 *
 * On this hook is possible to create an order and enclose the ID within the
 * data.
 *
 * @param mixed $data
 *   Has all the parameter to be sent to Khipu and additionally the instance ID
 *   that uses the payment.
 */
function hook_khipu_data_alter(&$data) {
  if ($data['instance_id'] == 'my_instance') {
    $order_id = my_commerce_create_order(array(
      'amount' => $data['amount'],
      'khipu_id' => $khipu_account->kaid,
      'status' => 'checkout',
    ));
    // Le asignamos el valor a transaction_id
    $data['transaction_id'] = $order_id;
  }
}


/**
 * This hook is called after all validation is finished.
 *
 * After this hook the user will be redirected to Khipu.
 *
 * @param object $khipu_account
 *   The Khipu Account object.
 * @param array $data
 *   Has all the parameter to be sent to Khipu.
 * @param string $instance_id
 *   The instance ID that uses the payment.
 */
function hook_khipu_user_redirect($khipu_account, $data, $instance_id = NULL) {

}


/**
 * This hook is called when khipu sends a notification to the website.
 *
 * The notification is send to confirm the payment.
 *
 * @param string $instance_id
 *   The instance ID that uses the payment.
 */
function hook_khipu_verified($instance_id) {
  if ($instance_id == 'my_module') {
    $id_transaction = $data['transaction_id'];
    $order = my_module_get_order($id_transaction);
    if ($order) {
      my_module_complete_order($order);

      // Maybe send an email.
    }
  }
}
