<?php
/**
 * @file
 * A reference type plugin to describe field collections.
 */
$plugin = array(
  'handler' => array(
    'class' => 'reference_table_formatter_taxonomy_term_reference',
  )
);

/**
 * Class reference_table_formatter_field_collection
 */
class reference_table_formatter_taxonomy_term_reference extends reference_table_formatter_base_type {
  /**
   * {@inheritdoc}
   */
  public function get_bundles() {
    $bundles = array();
    foreach ($this->field['settings']['allowed_values'] as $allowed_values) {
      $bundles[] = $allowed_values['vocabulary'];
    }
    return $bundles;
  }

  /**
   * {@inheritdoc}
   */
  public function entity_name() {
    return 'taxonomy_term';
  }

  /**
   * {@inheritdoc}
   */
  function renderable_properties() {
    return array('name');
  }

  /**
   * {@inheritdoc}
   */
  function load_entity_from_item($item) {
    return taxonomy_term_load($item['tid']);
  }
}
