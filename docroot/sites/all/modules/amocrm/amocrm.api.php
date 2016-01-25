<?php

/**
 * @file
 * This file contains no working PHP code; it exists to provide additional
 * documentation for doxygen as well as to document hooks in the standard
 * Drupal manner.
 */

/**
 * @param array $data
 *   All data received from amoCRM webhook about lead
 * @param string $action
 *   Made action to amoCRM entity,
 *   may be "add", "update", "delete", "status" and "restore"
 * @param array $account
 *   Internal amoCRM user, author of current action.
 */
function hook_amocrm_leads($data, $action, $account) {
  // Your code.
}

/**
 * @param array $data
 *   All data received from amoCRM webhook about contact
 * @param string $action
 *   Made action to amoCRM entity,
 *   may be "add", "update", "delete", "status" and "restore"
 * @param array $account
 *   Internal amoCRM user, author of current action.
 */
function hook_amocrm_contacts($data, $action, $account) {
  // Your code.
}

/**
 * @param array $data
 *   All data received from amoCRM  webhook about company
 * @param string $action
 *   Made action to amoCRM entity,
 *   may be "add", "update", "delete", "status" and "restore"
 * @param array $account
 *   Internal amoCRM user, author of current action.
 */
function hook_amocrm_companies($data, $action, $account) {
  // Your code.
}
