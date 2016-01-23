<?php

/**
 * @file
 * Hook documentation.
 */

/**
 * Let modules hook in to the status request.
 *
 * For example to update the status of an external payment service.
 *
 * @param int $ideal_id
 *   The ideal ID.
 * @param object $transaction
 *   The ideal transaction entity.
 * @param bool $changed
 *   When the transaction is updated, changed will be true.
 */
function hook_ideal_advanced_status_request($ideal_id, $transaction, $changed) {

}

/**
 * The ideal advanced status request is called by cron.
 *
 * This hook can be used to alter the transactions returned by the db query.
 * It is also possible to inject an other ideal connector class, for example
 * when it is extended.
 *
 * @param int $ideal_id
 *   The ideal transaction ID.
 * @param object $transaction
 *   Ideal transaction object.
 * @param IdealAdvancedConnectorWrapper $ideal_connector
 *   The ideal connector wrapper class.
 */
function hook_ideal_advanced_status_request_alter(&$ideal_id, &$transaction, IdealAdvancedConnectorWrapper &$ideal_connector) {

}
