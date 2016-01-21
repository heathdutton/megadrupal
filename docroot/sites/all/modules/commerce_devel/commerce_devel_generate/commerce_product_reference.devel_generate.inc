<?php

/**
 * Support for processing commerce product reference fields in devel generate.
 */
function commerce_product_reference_devel_generate($object, $field, $instance, $bundle) {
  if (field_behaviors_widget('multiple values', $instance) == FIELD_BEHAVIOR_CUSTOM) {
    module_load_include('inc', 'commerce_devel_generate', 'commerce_product_reference.devel_generate');
    return devel_generate_multiple('_commerce_product_reference_devel_generate', $object, $field, $instance, $bundle);
  }
  else {
    return _commerce_product_reference_devel_generate($object, $field, $instance, $bundle);
  }
}

function _commerce_product_reference_devel_generate($object, $field, $instance, $bundle) {
  $object_field = array();
  // Get all the referenceable product types.
  $referenceable_types = array_keys($instance['settings']['referenceable_types']);
  foreach($instance['settings']['referenceable_types'] as $product_type => $value) {
    if ($value) {
      $referenceable_types[] = $product_type;
    }
  }

  // Get all the products id eligible for this product reference field.
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'commerce_product', '=');

  // Check if we should apply a bundle filter.
  if (!empty($referenceable_types)) {
    $query->entityCondition('bundle', $referenceable_types, 'IN');
  }
  $products = $query->execute();

  if (isset($products['commerce_product'])) {
    $product_ids = array_keys($products['commerce_product']);
    $random_key = array_rand($product_ids);
    $object_field['product_id'] = $product_ids[$random_key];
  }
  return $object_field;
}
