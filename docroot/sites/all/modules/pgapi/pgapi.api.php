<?php

/**
 * @file
 * Provides hook documentation for PGAPI module.
 */

/**
 * Returns gateway info.
 *
 * @return Array keyed by module name.
 */
function hook_pgapi_gw_info() {
  return array(
    'pg_manual_payment' => t('Manual payment gateway'),
  );
}

function hook_pgapi_callback($transaction) {

}

function hook_pgapi_format_price($type, $price, $symbol) {

}

function hook_pgapi_transaction($status, $transaction) {

}

function hook_pgapi_transaction_all($status, $transaction) {

}

/**
 * Executes pgapi hook
 *
 * @param $op
 *   The operation to be performed. Possible values:
 *   - "payment gateway info"
 *   - "display name"
 *   - "payment page"
 *   - "get form"
 *   - "process form"
 *   - "edit"
 *   - "edit"
 * @param unknown_type $a3
 * @param unknown_type $a4
 */
function hook_pgapi_gw($op, $a3 = NULL, $a4 = NULL) {

}

function hook_pgapi_transaction_status(&$status) {

}

function hook_pgapi_transaction_workflow(&$workflow) {

}
