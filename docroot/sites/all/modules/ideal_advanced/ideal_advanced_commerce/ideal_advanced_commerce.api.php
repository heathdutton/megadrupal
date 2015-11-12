<?php

/**
 * @file
 * Hook documentation.
 */

/**
 * Alter the transaction purchase_id or description before sending.
 *
 * This hook allows changing transaction details before making the request to
 * the iDEAL server.
 *
 * @param int $purchase_id
 *   The issuers transaction id.
 * @param string $description
 *   The issuers transaction description.
 * @param object $order
 *   The commerce order object.
 */
function hook_ideal_advanced_commerce_transaction_alter(&$purchase_id, &$description, $order) {

}
