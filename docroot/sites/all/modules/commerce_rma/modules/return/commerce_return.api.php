<?php
/**
 * @file
 * Hooks provided by the commerce_return module.
 */

/**
 * Defines statuses for line items attached to a return.
 *
 * The line item statuses array structure is as follows:
 * - name: machine-name identifying the status using lowercase
 *   alphanumeric characters, -, and _.
 * - title: the translatable title of the status, used in administrative
 *   interfaces.
 * - description: a translatable description of the status itself, used
 *   in user interfaces.
 * - weight: integer weight used for sorting lists of statuses; defaults to 0.
 *
 * @return array
 *   An array of return status keyed by name.
 */
function hook_commerce_return_line_item_status_info() {

  $statuses = array();

  $statuses['received_waiting_for_refund'] = array(
    'name' => 'received_waiting_for_refund',
    'title' => t('Received and waiting for refund'),
    'description' => t('The product was received back by the seller'),
    'weight' => -10,
  );

  $statuses['received_refunds_made'] = array(
    'name' => 'received_refunds_made',
    'title' => t('Received and refunds made'),
    'description' => t('The seller has found the products as refundable and paid back the client'),
    'weight' => -9,
  );

  return $statuses;
}

/**
 * Allows you to alter the return line item statuses.
 *
 * @param array $line_item_statuses
 *   Array of statuses keyed by name exposed by
 *   hook_commerce_return_line_item_status_info() implementations.
 *
 * @see hook_commerce_return_line_item_status_info()
 */
function hook_commerce_return_line_item_status_info_alter(&$line_item_statuses) {
  // Alter the weight of a particular return status.
  if (!empty($line_item_statuses['received_refunds_made'])) {
    $line_item_statuses['received_refunds_made']['weight'] = 10;
  }
}

/**
 * Defines return states for use in grouping return statuses together.
 *
 * An return state is a particular phase in the life-cycle of an return that is
 * comprised of one or more return statuses. In that regard, an return state is
 * little more than a container for return statuses with a default status per
 * state. This is useful for categorizing returns and advancing returns from one
 * state to the next without needing to know the particular status an return will
 * end up in.
 *
 * The Order module defines several return states in its own implementation of
 * this hook, commerce_return_commerce_return_state_info():
 * - Canceled: for returns that have been canceled through some user/admin
 *   action.
 * - Pending: for returns that have been created and are awaiting further
 *   action.
 * - Completed: for returns that have been completed as far as the customer
 *   should be concerned.
 *
 * The return state array structure is as follows:
 * - name: machine-name identifying the return state using lowercase alphanumeric
 *   characters, -, and _
 * - title: the translatable title of the return state, used in administrative
 *   interfaces
 * - description: a translatable description of the types of returns that would
 *   be in this state
 * - weight: integer weight of the state used for sorting lists of return states;
 *   defaults to 0
 * - default_status: name of the default return status for this state
 *
 * @return array
 *   An array of return state arrays keyed by name.
 */
function hook_commerce_return_state_info() {
  $order_states = array();

  $return_states['canceled'] = array(
    'name' => 'canceled',
    'title' => t('Canceled'),
    'description' => t('returns in this state have been canceled through some user/admin action.'),
    'weight' => -10,
    'default_status' => 'canceled',
  );

  return $order_states;
}

/**
 * Allows modules to alter the return state definitions of other modules.
 *
 * @param $return_states
 *   An array of return states defined by enabled modules.
 *
 * @see hook_commerce_return_state_info()
 */
function hook_commerce_return_state_info_alter(&$return_states) {
  $return_states['completed']['weight'] = 9;
}

/**
 * Defines return statuses for use in managing returns.
 *
 * An return status is a single step in the life-cycle of an return that
 * administrators can use to know at a glance what has occurred to the return
 * already and/or what the next step in processing the return will be.
 *
 * The Order module defines several return statuses in its own implementation of
 * this hook, commerce_return_commerce_return_status_info():
 * - Canceled: default status of the Canceled state; used for returns that are
 *   marked as canceled via the administrative user interface.
 * - Pending: default status of the Pending state; used to indicate the return
 *   has completed the return workflow and is awaiting further action before
 *   being considered complete (e.g.: customer has to return their products).
 * - Processing: additional status for the Pending state; used to indicate
 *   returns that have begun to be processed but are not yet completed
 * - Completed: default status of the Completed state; used for returns that
 *   don’t require any further attention or customer interaction. (e.g.:
 *   Customer has been fully reimbursed.).
 *
 * The return status array structure is as follows:
 * - name: machine-name identifying the return status using lowercase
 *   alphanumeric characters, -, and _
 * - title: the translatable title of the return status, used in administrative
 *   interfaces
 * - state: the name of the return state the return status belongs to
 * - cart: TRUE or FALSE indicating whether or not returns with this status
 *   should be considered shopping cart returns
 * - weight: integer weight of the status used for sorting lists of return
 *   statuses; defaults to 0
 * - status: TRUE or FALSE indicating the enabled status of this return status,
 *   with disabled statuses not being available for use; defaults to TRUE
 *
 * @return array
 *   An array of return status arrays keyed by name.
 */
function hook_commerce_return_status_info() {
  $return_statuses = array();

  $return_statuses['completed'] = array(
    'name' => 'completed',
    'title' => t('Completed'),
    'state' => 'completed',
  );

  return $return_statuses;
}

/**
 * Allows modules to alter the return status definitions of other modules.
 *
 * @param $return_statuses
 *   An array of return statuses defined by enabled modules.
 *
 * @see hook_commerce_return_status_info()
 */
function hook_commerce_return_status_info_alter(&$return_statuses) {
  $return_statuses['completed']['title'] = t('Finished');
}