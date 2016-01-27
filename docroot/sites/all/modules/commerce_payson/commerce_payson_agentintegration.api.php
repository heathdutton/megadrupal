<?php

/**
 * @file
 * Documents hooks provided by the Payson Agent Integration Module.
 */


/**
 * Allows modules to alter the name-value pair array for a request.
 *
 * This hook can be used to alter or add to the name-value pair array
 * for a Payson Agent Integration request before it is submitted. 
 *
 * @param array $data
 *   The name-value pair array for the Payson Agent Integration request.
 * @param object $order
 *   If available, the full order object the payment request is being submitted
 *   for, otherwise NULL.
 */
function hook_commerce_payson_agentintegration_data(&$data, $order) {
  // No example.
}
