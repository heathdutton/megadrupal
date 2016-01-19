<?php

/**
 * @file
 *   Commerce Line Item migration.
 *
 *   @todo Do we need to fill out the serialized data column as well?
 *         It holds the path to the product node and an entity object,
 *         not sure how important it is.
 */
class CommerceMigrateMagentoLineItemMigration extends Migration {
  public function __construct(array$arguments) {
    parent::__construct($arguments);
    $store_id = $arguments['store_id'];
    $store_code = $arguments['store_code'];
    $this->orderSourceMigration = 'CommerceMagentoOrder' . ucfirst($store_code);
    // Create a map object for tracking the relationships between source rows
    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'item_id' => array(
          'type' => 'int',
          'unsigned' => TRUE,
          'not null' => TRUE,
          'description' => 'Magento Line Item ID',
        ),
      ),
      MigrateDestinationEntityAPI::getKeySchema('commerce_line_item', 'product')
    );

    // Create a MigrateSource object, which manages retrieving the input data.
    $connection = commerce_migrate_magento_get_source_connection();
    $query = $connection->select('sales_flat_order_item', 'sfoi')->fields('sfoi')->distinct();
    $query->condition('sfoi.store_id', $store_id, '=');
    $query->condition('sfoi.product_type', 'simple', '=');
    $this->source = new MigrateSourceSQL($query, array(), NULL, array('map_joinable' => FALSE));
    $this->destination = new MigrateDestinationEntityAPI('commerce_line_item', 'product');
    // Properties
    $this->addFieldMapping('order_id', 'order_id')->sourceMigration('CommerceMagentoOrder' . ucfirst($store_code));
    $this->addFieldMapping('line_item_label', 'name');
    $this->addFieldMapping('quantity', 'qty_ordered');
    $this->addFieldMapping('created', 'created_at');
    // Fields
    $this->addFieldMapping('commerce_unit_price', 'price');
    // @see prepare()
    $this->addFieldMapping('commerce_unit_price:tax_rate', 'tax_amount');
    $this->addFieldMapping('commerce_total', 'price');
    // $this->addFieldMapping('commerce_total:tax_rate', 'tax_amount');
    $this->addFieldMapping('commerce_product', 'product_id')->defaultValue(0)->sourceMigration($arguments['products']);
  }

  function prepare($line_item, stdClass$row) {

    // The destination line item type is product
    $line_item->type = 'product';

    // Add a base price component.
    $line_item_wrapper = entity_metadata_wrapper('commerce_line_item', $line_item);
    $price             = $line_item_wrapper->commerce_unit_price;
    $base_price        = array(
      'amount' => $price->amount->value(),
      'currency_code' => $price->currency_code->value(),
      'data' => array(),
    );
    $line_item_wrapper->commerce_unit_price->data = commerce_price_component_add($base_price, 'base_price', $base_price, TRUE);

    // Find Any Taxes associated with this product line item and add as
    // a price component
    //
    // Get the tax mapping if it exists
    // $tax_mappings = commerce_migrate_magento_get_mapping_array('commerce_migrate_magento_tax_mapping');

    // Create our query for tax components
    // if (array_key_exists($record->title, $tax_mappings)) {
    //   if ($tax_rate = commerce_tax_rate_load($tax_mappings[$record->title])) {
    //     commerce_tax_rate_apply($tax_rate, $line_item);
    //   }
  }
  
  function prepareRow($row) {
    $row->order_id = $order['destid1'];
    $row->created_at = strtotime($row->created_at);
  }

  /**
   * A line item has been saved. Reference it from the order.
   */
  function complete($line_item, stdClass$row) {
    // We need to do a full order save here so that commerce recalculates
    // the total with our price components
    $order = commerce_order_load($line_item->order_id);
    $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
    $order_wrapper->commerce_line_items[] = $line_item;
    $order_wrapper->save();
  }

  /**
   * The line item has been deleted, delete its references.
   */
  function completeRollback($line_item_id) {
    db_delete('field_data_commerce_line_items')->condition('commerce_line_items_line_item_id', $line_item_id)->execute();
  }
}

