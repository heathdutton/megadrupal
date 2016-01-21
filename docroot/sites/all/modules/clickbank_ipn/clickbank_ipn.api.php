<?php
/**
 * @file
 * Hooks provided by this module.
 */
/**
 * @addtogroup hooks
 * @{
 */
/**
 * Acts on Clickbank Orders being loaded from the database.
 *
 * This hook is invoked during Clickbank Order loading, which is handled by
 * entity_load(), via the EntityCRUDController.
 *
 * @param array $clickbank_transactions
 *   An array of Clickbank Order entities being loaded, keyed by id.
 *
 * @see hook_entity_load()
 */
function hook_clickbank_transaction_load(array $clickbank_transactions) {
  $result = db_query('SELECT pid, foo FROM {mytable} WHERE pid IN(:ids)', array(':ids' => array_keys($clickbank_transactions)));
  foreach ($result as $record) {
    $clickbank_transactions[$record->pid]->foo = $record->foo;
  }
}
/**
 * Responds when a Clickbank Order is inserted.
 *
 * This hook is invoked after the Clickbank Order is inserted into the database.
 *
 * @param $clickbank_transaction
 *   The Clickbank Order that is being inserted.
 *
 * @see hook_entity_insert()
 */
function hook_clickbank_transaction_insert($clickbank_transaction) {
  db_insert('mytable')
    ->fields(array(
      'id' => entity_id('clickbank_transaction', $clickbank_transaction),
      'extra' => print_r($clickbank_transaction, TRUE),
    ))
    ->execute();
}
/**
 * Acts on a Clickbank Order being inserted or updated.
 *
 * This hook is invoked before the Clickbank Order is saved to the database.
 *
 * @param $clickbank_transaction
 *   The Clickbank Order that is being inserted or updated.
 *
 * @see hook_entity_presave()
 */
function hook_clickbank_transaction_presave($clickbank_transaction) {
  $clickbank_transaction->name = 'foo';
}
/**
 * Responds to a Clickbank Order being updated.
 *
 * This hook is invoked after the Clickbank Order has been updated in the database.
 *
 * @param $clickbank_transaction
 *   The Clickbank Order that is being updated.
 *
 * @see hook_entity_update()
 */
function hook_clickbank_transaction_update($clickbank_transaction) {
  db_update('mytable')
    ->fields(array('extra' => print_r($clickbank_transaction, TRUE)))
    ->condition('id', entity_id('clickbank_transaction', $clickbank_transaction))
    ->execute();
}
/**
 * Responds to Clickbank Order deletion.
 *
 * This hook is invoked after the Clickbank Order has been removed from the database.
 *
 * @param $clickbank_transaction
 *   The Clickbank Order that is being deleted.
 *
 * @see hook_entity_delete()
 */
function hook_clickbank_transaction_delete($clickbank_transaction) {
  db_delete('mytable')
    ->condition('pid', entity_id('clickbank_transaction', $clickbank_transaction))
    ->execute();
}
/**
 * @}
 */