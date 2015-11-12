<?php
/**
 * @file
 * A reference type plugin to describe commerce product references.
 */
$plugin = array(
  'handler' => array(
    'class' => 'reference_table_formatter_commerce_product_reference',
  )
);

class reference_table_formatter_commerce_product_reference extends reference_table_formatter_base_type {
  /**
   * {@inheritdoc}
   */
  public function get_bundles() {
    $bundles = array();
    // Check if the bundle has been nominated by the user.
    if (isset($this->instance['settings']['referenceable_types'])) {
      $settings = $this->instance['settings']['referenceable_types'];
      foreach ($settings as $key => $type) {
        if ($type != '0') {
          $bundles[] = $type;
        }
      }
      if (count($bundles) > 0) {
        return $bundles;
      }
    }
    // Otherwise all field references from all bundles.
    $info = entity_get_info($this->entity_name());
    return array_keys($info['bundles']);
  }


  /**
   * {@inheritdoc}
   */
  public function entity_name() {
    return 'commerce_product';
  }

  /**
   * {@inheritdoc}
   */
  public function renderable_properties() {
    return array(
      'title',
      'sku',
      'commerce_stock',
      'product_id',
    );
  }

  /**
   * {@inheritdoc}
   */
  function load_entity_from_item($item) {
    $loaded_entity = entity_load($this->entity_name(), array($item['product_id']));
    return array_shift($loaded_entity);
  }
}
