<?php

/**
 * @file
 *   Commerce Line Item migration.
 *
 *   @todo Do we need to fill out the serialized data column as well?
 *         It holds the path to the product node and an entity object,
 *         not sure how important it is.
 */

class CommerceMigrateUbercartLineItem extends Migration {
  public function __construct(array $arguments) {
    parent::__construct($arguments);

    // Create a map object for tracking the relationships between source rows
    $this->map = new MigrateSQLMap($this->machineName,
        array(
          'order_product_id' => array(
            'type' => 'int',
            'unsigned' => TRUE,
            'not null' => TRUE,
            'description' => 'Ubercart order product ID',
          ),
        ),
        MigrateDestinationEntityAPI::getKeySchema('commerce_line_item', 'product')
      );

    // Create a MigrateSource object, which manages retrieving the input data.
    $connection = commerce_migrate_ubercart_get_source_connection();

    $query = $connection->select('uc_order_products', 'uop');
    $query->innerJoin('uc_orders', 'uo', 'uop.order_id = uo.order_id');
    $query->condition('uo.order_status', 'in_checkout', '<>');
    $query->fields('uop', array('order_product_id', 'order_id', 'nid', 'model', 'qty', 'price'));
    $this->source = new MigrateSourceSQL($query, array(), NULL, array('map_joinable' => FALSE));
    $this->destination = new MigrateDestinationEntityAPI('commerce_line_item', 'product');

    // Properties
    $this->addFieldMapping('order_id', 'order_id')
         ->sourceMigration('CommerceMigrateUbercartOrder');
    $this->addFieldMapping('line_item_label', 'model');
    $this->addFieldMapping('quantity', 'qty');
    // Fields
    $this->addFieldMapping('commerce_unit_price', 'price');
    $this->addFieldMapping('commerce_total', 'price');
    $this->addFieldMapping('commerce_product', 'nid')
      ->defaultValue(0)
      ->sourceMigration($arguments['products']);
  }

  function prepare($line_item, stdClass $row) {

    // The destination line item type is product
    $line_item->type = 'product';

    // Add a base price component.
    $line_item_wrapper = entity_metadata_wrapper('commerce_line_item', $line_item);
    $price = $line_item_wrapper->commerce_unit_price;
    $base_price = array(
      'amount' => $price->amount->value(),
      'currency_code' => $price->currency_code->value(),
      'data' => array(),
    );
    $line_item_wrapper->commerce_unit_price->data = commerce_price_component_add($base_price, 'base_price', $base_price, TRUE);

    // Find Any Taxes associated with this product line item and add as
    // a price component
    //
    // Get the tax mapping if it exists
    $tax_mappings = commerce_migrate_ubercart_get_mapping_array('commerce_migrate_ubercart_tax_mapping');

    // Create our query for tax components
    $connection = commerce_migrate_ubercart_get_source_connection();
    $query = $connection->select('uc_order_line_items', 'uol');
    $query->innerJoin('uc_orders', 'uo', 'uol.order_id = uo.order_id');
    $query->condition('uo.order_status', 'in_checkout', '<>');
    $query->condition('uol.order_id', $row->order_id, '=');
    $query->condition('uol.type', 'tax', '=');
    $query->fields('uol', array('type', 'title', 'amount'));
    $result = $query->execute();

    // Loop through the results and add tax components for each one
    foreach ($result as $record) {
      // If there is a mapping we add a price component for the tax
      if (array_key_exists($record->title, $tax_mappings)) {
        if ($tax_rate = commerce_tax_rate_load($tax_mappings[$record->title])) {
          commerce_tax_rate_apply($tax_rate, $line_item);
        }
      }
    }
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
}
