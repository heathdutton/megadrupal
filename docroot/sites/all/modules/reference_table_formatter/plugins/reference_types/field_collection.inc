<?php
/**
 * @file
 * A reference type plugin to describe field collections.
 */
$plugin = array(
  'handler' => array(
    'class' => 'reference_table_formatter_field_collection',
  )
);

/**
 * Class reference_table_formatter_field_collection
 */
class reference_table_formatter_field_collection extends reference_table_formatter_base_type {
  /**
   * {@inheritdoc}
   */
  public function get_bundles() {
    // Field collections can only have a single bundle, matching the field name.
    return array($this->instance['field_name']);
  }

  /**
   * {@inheritdoc}
   */
  public function entity_name() {
    return 'field_collection_item';
  }

  /**
   * {@inheritdoc}
   */
  function renderable_properties() {
    return array();
  }

  /**
   * {@inheritdoc}
   */
  function load_entity_from_item($item) {
    return field_collection_item_load($item['value']);
  }
}
