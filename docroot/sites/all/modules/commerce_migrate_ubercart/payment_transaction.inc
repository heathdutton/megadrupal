<?php

/**
 * @file
 *   Commerce Transaction migration.
 */

class CommerceMigrateUbercartTransaction extends Migration {
  public function __construct(array $arguments) {
    parent::__construct($arguments);
    $this->orderSourceMigration = 'CommerceMigrateUbercartOrder';

    // Create a map object for tracking the relationships between source rows.
    $this->map = new MigrateSQLMap($this->machineName,
        array(
          'receipt_id' => array(
            'type' => 'int',
            'unsigned' => TRUE,
            'not null' => TRUE,
            'description' => 'Ubercart receipt ID',
          ),
        ),
        MigrateDestinationEntityAPI::getKeySchema('commerce_payment_transaction')
      );

    // Create a MigrateSource object, which manages retrieving the input data.
    $connection = commerce_migrate_ubercart_get_source_connection();

    $query = $connection->select('uc_payment_receipts', 'upr');
    $query->innerJoin('uc_orders', 'uo', 'upr.order_id = uo.order_id');
    $query->condition('uo.order_status', 'in_checkout', '<>');
    $query->fields('upr');
    $this->source = new MigrateSourceSQL($query, array(), NULL, array('map_joinable' => FALSE));
    $this->destination = new MigrateDestinationEntityAPI('commerce_payment_transaction', 'payment');

    // Default uid to 0 if we're not mapping it.
    if (variable_get('commerce_migrate_ubercart_user_map_ok', FALSE) && variable_get('commerce_migrate_ubercart_user_migration_class', "") != "") {
      $this->addFieldMapping('uid', 'uid')->sourceMigration(variable_get('commerce_migrate_ubercart_user_migration_class', ""))->defaultValue(0);
    }
    else {
      $this->addFieldMapping('uid', 'uid')->defaultValue(0);
    }

    $this->addFieldMapping('order_id', 'order_id');
    $this->addFieldMapping('payment_method', 'method');
    $this->addFieldMapping('created', 'received');
    $this->addFieldMapping('changed', 'received');
  }

  function prepareRow($row) {
    // The sourceMigration returns order_id and revions_id. We only need the
    // order id, so we call the source migration manually.
    $order = $this->handleSourceMigration($this->orderSourceMigration, $row->order_id);

    // Don't create a transaction if we can't load the order.
    if (empty($order['destid1'])) {
      return FALSE;
    }

    $row->order_id = $order['destid1'];
  }

  function prepare($transaction, stdClass $row) {
    $transaction->amount = commerce_currency_decimal_to_amount($row->amount, commerce_default_currency());
    $transaction->status = COMMERCE_PAYMENT_STATUS_SUCCESS;
    $transaction->data = unserialize($row->data);
    $transaction->message = $row->comment;
  }
}
