<?php

/**
 * @file
 * Hooks provided by the PAYULATAM module.
 */

/**
 * Allows modules to react to the confirmation of transaction.
 *
 * @param $order
 *   The order object.
 *
 * @param transaction
 *   The transaction object.
 */
function hook_commerce_payulatam_confirmation($order, $transaction) {
  // No example.
}

/**
 * Allows modules to react to the response of transaction.
 *
 * @param $content
 *   The content page respose.
 *
 * @param params
 *   The params response.
 */
function hook_commerce_payulatam_response_alter(&$content, $params) {
  $content['table']['#rows'][] = array(
    'colspan' => 2,
    'data' => t('Additional Information'),
  );
}