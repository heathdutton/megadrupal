<?php

/**
 * @file
 *   Commerce Transaction migration.
 */
class CommerceMigrateMagentoTransactionMigration extends Migration {
  public function __construct(array$arguments) {
    parent::__construct($arguments);
    $store_id = $arguments['store_id'];
    $store_code = $arguments['store_code'];
    $this->orderSourceMigration = 'CommerceMagentoOrder' . ucfirst($store_code);
    $user_sharing = commerce_migrate_magento_variable_get('customer/account_share/scope');
    // Create a map object for tracking the relationships between source rows.
    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'entity_id' => array(
          'type' => 'int',
          'unsigned' => TRUE,
          'not null' => TRUE,
          'description' => 'Magento Entity ID',
        ),
      ),
      MigrateDestinationEntityAPI::getKeySchema('commerce_payment_transaction')
    );
    // Create a MigrateSource object, which manages retrieving the input data.
    $connection = commerce_migrate_magento_get_source_connection();
    $query = $connection->select('sales_flat_order_payment', 'sfop');
    $query->fields('sfop', array('entity_id', 'parent_id', 'method', 'amount_ordered', 'additional_information'))->distinct();
    $query->leftJoin('sales_flat_order', 'sfo', 'sfop.parent_id = sfo.entity_id');
    $query->fields('sfo', array('customer_id', 'created_at', 'updated_at', 'grand_total'));
    $query->condition('sfo.store_id', $store_id, '=');
    $this->source = new MigrateSourceSQL($query, array(), NULL, array('map_joinable' => FALSE));
    $this->destination = new MigrateDestinationEntityAPI('commerce_payment_transaction', 'payment');
    // Default uid to 0 if we're not mapping it.
    if ($user_sharing) {
      $this->addFieldMapping('uid', 'customer_id')->sourceMigration('CommerceMagentoUser' . ucfirst($arguments['store_code']))->defaultValue(0);
    }
    else {
      $this->addFieldMapping('uid', 'customer_id')->sourceMigration('CommerceMagentoUserGlobal')->defaultValue(0);
    }
    // @see prepareRow() we handle the source migration there
    $this->addFieldMapping('order_id', 'parent_id');
    $this->addFieldMapping('payment_method', NULL)->defaultValue('authnet_aim');
    $this->addFieldMapping('created', 'created_at');
    $this->addFieldMapping('changed', 'updated_at');
  }

  function prepare($transaction, stdClass$row) {
    $transaction->amount = commerce_currency_decimal_to_amount($row->grand_total, commerce_default_currency());
    $transaction->status = COMMERCE_PAYMENT_STATUS_SUCCESS;
    $transaction->data = unserialize($row->additional_information);
    $transaction->message = $row->method;
  }

  function prepareRow($row) {
    // The sourceMigration returns order_id and revions_id. We only need the
    // order id, so we call the source migration manually.
    $order = $this->handleSourceMigration($this->orderSourceMigration, $row->parent_id);
    // Don't create a transaction if we can't load the order.
    if (empty($order['destid1'])) {
      return FALSE;
    }
    $row->parent_id  = $order['destid1'];
    $row->created_at = strtotime($row->created_at);
    $row->updated_at = strtotime($row->updated_at);
    switch ($row->method) {
      case 'authorizenet':
        $row->method = 'authnet_aim';
        break;

      case 'free':
        $row->method = 'authnet_aim';
        break;

      default:
        $row->method = 'authnet_aim';
        break;
    }
  }
}

