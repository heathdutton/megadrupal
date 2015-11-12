<?php

/**
 * @file
 *   Commerce Order migration.
 */
class CommerceMigrateMagentoOrderMigration extends Migration {
  public function __construct(array$arguments) {
    parent::__construct($arguments);
    $store_id = $arguments['store_id'];
    $user_sharing = commerce_migrate_magento_variable_get('customer/account_share/scope');
    // Create a map object for tracking the relationships between source rows
    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'entity_id' => array(
          'type' => 'int',
          'unsigned' => TRUE,
          'not null' => TRUE,
          'description' => 'Magento Order Entity ID',
        ),
      ),
      MigrateDestinationEntityAPI::getKeySchema('commerce_order', 'commerce_order')
    );
    // Create a MigrateSource object, which manages retrieving the input data.
    $connection = commerce_migrate_magento_get_source_connection();
    $query = $connection->select('sales_flat_order', 'sfo')->fields('sfo');
    $query->condition('sfo.store_id', $store_id, '=');
    $this->source = new MigrateSourceSQL($query, array(), NULL, array('map_joinable' => FALSE));
    $this->destination = new MigrateDestinationEntityAPI('commerce_order', 'commerce_order');
    // Properties
    $this->addFieldMapping('order_number', 'increment_id');
    $this->addFieldMapping('mail', 'customer_email');
    $this->addFieldMapping('hostname', 'remote_ip');
    $this->addFieldMapping('status', 'status');
    if ($user_sharing) {
      $this->addFieldMapping('uid', 'customer_id')->sourceMigration('CommerceMagentoUser' . ucfirst($arguments['store_code']))->defaultValue(0);
    }
    else {
      $this->addFieldMapping('uid', 'customer_id')->sourceMigration('CommerceMagentoUserGlobal')->defaultValue(0);
    }
    $this->addFieldMapping('created', 'created_at');
    $this->addFieldMapping('changed', 'updated_at');
    // Fields
    $this->addFieldMapping('commerce_customer_billing', 'billing_address_id')->sourceMigration($arguments['customer_profiles']);
    $this->addFieldMapping('commerce_customer_shipping', 'shipping_address_id')->sourceMigration($arguments['customer_profiles']);
    $this->addFieldMapping('commerce_order_total', 'grand_total');
  }
  

  function prepare($order, stdClass $row) {
    if ($order->status == 'processing') {
      $order->status = 'processing';
    }
    if ($order->status == 'canceled') {
      $order->status = 'canceled';
    }
    if ($order->status == 'pending') {
      $order->status = 'pending';
    }
    if ($order->status == 'complete') {
      $order->status = 'completed';
    }
    if ($order->status == 'holded') {
      $order->status = 'shipping_delayed';
    }
    if ($order->status == 'closed') {
      $order->status = 'canceled';
    }
  }

  /**
   * The line item controller kills the order_total we set.
   * Until that gets fixed, here's a workaround.
   */
  function complete($order, stdClass $row) {
    db_update('field_data_commerce_order_total')
      ->fields(array(
        'commerce_order_total_amount' => commerce_currency_decimal_to_amount($row->grand_total, commerce_default_currency()),
      ))
      ->condition('entity_id', $order->order_id)
      ->condition('entity_type', 'commerce_order')
      ->execute();
  }
  
  public function prepareRow($row) {
    $row->created_at = strtotime($row->created_at);
    $row->updated_at = strtotime($row->updated_at);
  }
}

