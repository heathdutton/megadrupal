<?php

/**
 * @file
 * Hooks provided by the ec_store module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Allow modules to define custom transaction types.
 *
 * @return
 *   An associative array when key is the machine readable name and value
 *   is human readable one.
 */
function hook_ec_transaction_types() {
  return array(
    'ec_donate' => t('Donation'),
  );
}

/**
 * Act on transaction objects when loading them from the database.
 *
 * This hook can be used to load additional data and can return an array
 * containing pairs of fields => values to be merged into the node object. It
 * can also modify $txn object directly, since this object is passed by
 * reference.
 *
 * @param &$txn
 *   The transaction object that is being loaded.
 *
 * @return
 *   This hook can return an array containing pairs of fields => values to be
 *   merged into the node object. Or it can return nothing when $txn object is
 *   being modified directly.
 */
function hook_ec_transaction_load(&$txn) {
  $txn->mymodule_data = mymodule_load_transaction_data($txn->txnid);
}

/**
 * Act when inserting transactions into the database.
 *
 * This hook can be used to insert additional data or to trigger an action when
 * the transaction is being created (inserted into the database).
 *
 * @param &$txn
 *   The transaction object that is being inserted into the database.
 */
function hook_ec_transaction_insert(&$txn) {
  mymodule_insert_transaction_data($txn);
}

/**
 * Act when updating transactions into the database.
 *
 * This hook can be used to update module data or to trigger an action when
 * the transaction is being updated.
 *
 * @param &$txn
 *   The transaction object that is being updated into the database.
 */
function hook_ec_transaction_update(&$txn) {
  mymodule_update_transaction_data($txn);
}

/**
 * Act when deleting transactions from the database.
 *
 * This hook can be used to delete module data or to trigger an action when
 * the transaction is being deleted.
 *
 * @param &$txn
 *   The transaction object that is being deleted from the database.
 */
function hook_ec_transaction_delete(&$txn) {
  mymodule_delete_transaction_data($txn);
}

/**
 * Act before saving transaction into database.
 *
 * This hook is called before the transaction is saved to database and can be
 * used to change transaction object before it's inserted/updated into database.
 *
 * @param &$txn
 *   The transaction object that is about to be saved into the database.
 */
function hook_ec_transaction_pre_save(&$txn) {
  $txn->mymodule_data = mymodule_transaction_data();
}

/**
 * Act after saving transaction into database.
 *
 * This hook is called after the transaction is saved to database and can be
 * used to update module data or to trigger an action before transaction is
 * inserted/updated into database.
 *
 * @param &$txn
 *   The transaction object that was just saved into the database.
 */
function hook_ec_transaction_post_save(&$txn) {
  $txn->mymodule_data = mymodule_transaction_data();
}

/**
 * @} End of "addtogroup hooks".
 */
