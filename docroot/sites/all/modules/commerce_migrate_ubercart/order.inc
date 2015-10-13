<?php

/**
 * @file
 *   Commerce Order migration.
 */

class CommerceMigrateUbercartOrderMigration extends Migration {
  public function __construct(array $arguments) {
    parent::__construct($arguments);

    // Create a map object for tracking the relationships between source rows
    $this->map = new MigrateSQLMap($this->machineName,
        array(
          'order_id' => array(
            'type' => 'int',
            'unsigned' => TRUE,
            'not null' => TRUE,
            'description' => 'Ubercart order ID',
          ),
        ),
        MigrateDestinationEntityAPI::getKeySchema('commerce_order', 'commerce_order')
      );

    // Create a MigrateSource object, which manages retrieving the input data.
    $connection = commerce_migrate_ubercart_get_source_connection();

    $query = $connection->select('uc_orders', 'uo')
      ->fields('uo', array(
        'order_id', 'order_status', 'order_total',
        'primary_email', 'created', 'modified', 'host'));
    $query->condition('uo.order_status', 'in_checkout', '<>');
    if (variable_get('commerce_migrate_ubercart_user_map_ok', FALSE)) {
      $query->addField('uo', 'uid', 'uid');
    }

    $this->source = new MigrateSourceSQL($query, array(), NULL, array('map_joinable' => FALSE));
    $this->destination = new MigrateDestinationEntityAPI('commerce_order', 'commerce_order');

    // Properties
    $this->addFieldMapping('order_number', 'order_id');
    $this->addFieldMapping('mail', 'primary_email');
    $this->addFieldMapping('hostname', 'host');
    $this->addFieldMapping('status', 'order_status');
    // Default uid to 0 if we're not mapping it.
    if (variable_get('commerce_migrate_ubercart_user_map_ok', FALSE) && variable_get('commerce_migrate_ubercart_user_migration_class', "") != "") {
      $this->addFieldMapping('uid', 'uid')->sourceMigration(variable_get('commerce_migrate_ubercart_user_migration_class', ""))->defaultValue(0);
    }
    else {
      $this->addFieldMapping('uid', 'uid')->defaultValue(0);
    }

    $this->addFieldMapping('created', 'created');
    $this->addFieldMapping('changed', 'modified');
    // Fields
    $this->addFieldMapping('commerce_customer_billing', 'order_id')
         ->sourceMigration('CommerceMigrateUbercartCustomerBillingProfile');
    $this->addFieldMapping('commerce_order_total', 'order_total');
  }

  function prepare($order, stdClass $row) {
    // Most order statuses (pending, cancelled, completed) are the same
    // between the two systems. However, some are not.
    if ($order->status == 'payment_received') {
      $order->status = 'processing';
    }
    if ($order->status == 'in_checkout') {
      $order->status = 'checkout_checkout';
    }
    if ($order->status == 'paypal_pending') {
      $order->status = 'pending';
    }
  }

  /**
   * The line item controller kills the order_total we set.
   * Until that gets fixed, here's a workaround.
   */
  function complete($order, stdClass $row) {
    db_update('field_data_commerce_order_total')
      ->fields(array(
        'commerce_order_total_amount' => commerce_currency_decimal_to_amount($row->order_total, commerce_default_currency()),
      ))
      ->condition('entity_id', $order->order_id)
      ->condition('entity_type', 'commerce_order')
      ->execute();
  }
}
