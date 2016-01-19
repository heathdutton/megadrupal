<?php
/**
 * @file
 * A reference type plugin to describe paragraphs.
 */
$plugin = array(
  'handler' => array(
    'class' => 'reference_table_formatter_paragraphs',
  )
);

/**
 * Class reference_table_formatter_paragraphs
 */
class reference_table_formatter_paragraphs extends reference_table_formatter_base_type {
  /**
   * {@inheritdoc}
   */
  public function get_bundles() {
    return count($this->instance['settings']['allowed_bundles']) > 0 ? array_keys($this->instance['settings']['allowed_bundles']) : array_keys(paragraphs_bundle_load());
  }

  /**
   * {@inheritdoc}
   */
  public function entity_name() {
    return 'paragraphs_item';
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
    return paragraphs_item_load($item['value']);
  }
}
