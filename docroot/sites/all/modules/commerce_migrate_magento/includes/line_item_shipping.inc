<?php

/**
 * @file
 *   Commerce Line Item migration.
 *
 *   @todo Do we need to fill out the serialized data column as well?
 *         It holds the path to the product node and an entity object,
 *         not sure how important it is.
 */
class CommerceMigrateMagentoShippingLineItemMigration extends Migration {
  public function __construct(array$arguments) {
    parent::__construct($arguments);
    $store_id = $arguments['store_id'];
    $store_code = $arguments['store_code'];
    $this->orderSourceMigration = 'CommerceMagentoOrder' . ucfirst($store_code);
    // Create a map object for tracking the relationships between source rows
    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'entity_id' => array(
          'type' => 'int',
          'unsigned' => TRUE,
          'not null' => TRUE,
          'description' => 'Magento Entity ID',
        ),
      ),
      MigrateDestinationEntityAPI::getKeySchema('commerce_line_item', 'shipping')
    );

    // Create a MigrateSource object, which manages retrieving the input data.
    $connection = commerce_migrate_magento_get_source_connection();
    $query = $connection->select('sales_flat_order', 'sfo');
    // $query->leftJoin('sales_flat_shipment', 'sfs', 'sfs.order_id = sfo.entity_id');
    $query->fields('sfo')->distinct();
    $query->condition('sfo.store_id', $store_id, '=');
    $this->source = new MigrateSourceSQL($query, array(), NULL, array('map_joinable' => FALSE));
    $this->destination = new MigrateDestinationEntityAPI('commerce_line_item', 'shipping');
    // Properties
    $this->addFieldMapping('order_id', 'entity_id')->sourceMigration('CommerceMagentoOrder' . ucfirst($store_code));
    $this->addFieldMapping('type')->defaultValue('shipping');
    $this->addFieldMapping('line_item_label', 'shipping_description');
    $this->addFieldMapping('quantity')->defaultValue(1);
    $this->addFieldMapping('created', 'created_at');
    // Fields
    $this->addFieldMapping('commerce_unit_price', 'base_shipping_amount');
    $this->addFieldMapping('commerce_total', 'base_grand_total');
  }
  
  function prepare($line_item, stdClass $row) {

    // Add a price component.
    // Use an entity metadata wrapper to make things easier on ourselves
    $line_item_wrapper = entity_metadata_wrapper('commerce_line_item', $line_item);
    // Get the price entity
    $price = $line_item_wrapper->commerce_unit_price;
    // Build a price component price array
    $shipping_price = array(
      'amount' => $price->amount->value(),
      'currency_code' => $price->currency_code->value(),
      'data' => array(),
    );
    $shipping_mappings = commerce_migrate_magento_get_mapping_array('commerce_migrate_ubercart_shipping_mapping');
    if (array_key_exists($row->shipping_description, $shipping_mappings)) {
      if ($shipping_service = commerce_shipping_service_load($shipping_mappings[$row->shipping_description])) {
        $price_component_type = $shipping_service['price_component'];
        // Populate the line item with the appropriate shipping information
        commerce_shipping_line_item_populate($line_item, $shipping_service['name'], $price);
      }
    }
    else {
      // Just give this a generic type name since our shipping types don't map
      // exactly. Really we just need this migrated for historical purposes
      // so this should suffice
      $price_component_type = 'shipping';
      $line_item->data['shipping_service']['display_title'] = $line_item_wrapper->line_item_label;
    }

    // Add the price component
    $line_item_wrapper->commerce_unit_price->data = commerce_price_component_add(
      $line_item_wrapper->commerce_unit_price->value(),
      $price_component_type,
      $shipping_price,
      TRUE, FALSE
    );
    // The destination mapping schema has two primary keys when revisioning is
    // enabled (order_id and revision_id). When the field mapping looks up the
    // source migration to match the order_id, it gets both the destid1 and
    // destid2 from the mapping table. We need to pick one here or every
    // order_id ends up as 1
    if (is_array($line_item->order_id) && isset($line_item->order_id['destid1'])) {
      $line_item->order_id = $line_item->order_id['destid1'];
    }

  }
  
  /**
   * A line item has been saved. Reference it from the order.
   */
  function complete($line_item, stdClass $row) {
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
    db_delete('field_data_commerce_line_items')
      ->condition('commerce_line_items_line_item_id', $line_item_id)
      ->execute();
  }
  
  public function prepareRow($row) {
    $row->created_at = strtotime($row->created_at);
  }
}

