<?php
/**
 * @file
 * Features file for the Bazaarvoice productfeed module attribute mappings.
 *
 */

/**
 * Implements COMPONENT_features_export_options().
 */
function bazaarvoice_productfeed_mapping_features_export_options() {
  // DB query to fetch attributes.
  $results = db_select('bazaarvoice_productfeed_entity_mappings', 'bpfem')
    ->fields('bpfem')
    ->execute();

  $mappings = array();
  // Loop through the mappings and build keys.
  foreach ($results as $mapping) {
    $key = $mapping->entity_type . ':' . $mapping->entity_bundle . ':' . $mapping->attribute;
    $mappings[$key] = $key;
  }

  return $mappings;
}

/**
 * Implements COMPONENT_features_export().
 */
function bazaarvoice_productfeed_mapping_features_export($data, &$export, $module_name) {
  $export['dependencies']['bazaarvoice_productfeed'] = 'bazaarvoice_productfeed';
  $export['dependencies']['bazaarvoice'] = 'bazaarvoice';

  $pipe = array();
  module_load_include('inc', 'bazaarvoice_productfeed', 'includes/attributes');
  foreach ($data as $component) {
    $export['features']['bazaarvoice_productfeed_mapping'][$component] = $component;
    list($type, $bundle, $attribute) = explode(':', $component);
    // Add dependency on entity type.
    $pipe[$type][] = $bundle;
    // Add dependency on product.
    $pipe['bazaarvoice_product'][$type . ':' . $bundle] = $type . ':' . $bundle;
    // Custom attribute?
    if (bazaarvoice_productfeed_get_custom_attributes($attribute)) {
      // Add dependency on that attribute.
      $pipe['bazaarvoice_productfeed_attribute'][$attribute] = $attribute;
    }
  }

  return $pipe;
}

/**
 * Implements COMPONENT_features_export_render().
 */
function bazaarvoice_productfeed_mapping_features_export_render($module_name, $data) {
  module_load_include('inc', 'bazaarvoice_productfeed', 'includes/attributes');

  $items = array();
  foreach ($data as $item) {
    list($type, $bundle, $attribute) = explode(':', $item);
    // DB query to fetch attributes.
    $result = db_select('bazaarvoice_productfeed_entity_mappings', 'bpfem')
      ->fields('bpfem', array('attribute', 'field_name', 'sub_field_name'))
      ->condition('attribute', $attribute)
      ->condition('entity_type', $type)
      ->condition('entity_bundle', $bundle)
      ->range(0, 1)
      ->execute()
      ->fetchAssoc();
    if ($result && isset($result['attribute'])) {
      $items[$item] = array(
        'field' => $result['field_name'],
      );
      if ($result['sub_field_name']) {
        $items[$item]['sub_field'] = $result['sub_field_name'];
      }
    }
  }

  $code = "  \$items = " . features_var_export($items, '  ') . ";\n";
  $code .= '  return $items;';

  return array('bazaarvoice_productfeed_mapping' => $code);
}

/**
 * Implements COMPONENT_features_revert().
 */
function bazaarvoice_productfeed_mapping_features_revert($module) {
  bazaarvoice_productfeed_mapping_features_rebuild($module);
}

/**
 * Implements COMPONENT_features_enable_feature().
 */
function bazaarvoice_productfeed_mapping_features_enable_feature($module) {
  bazaarvoice_productfeed_mapping_features_rebuild($module);
}

/**
 * Implements COMPONENT_features_rebuild().
 */
function bazaarvoice_productfeed_mapping_features_rebuild($module) {
  module_load_include('inc', 'bazaarvoice_productfeed', 'includes/attributes');
  if ($defaults = features_get_default('bazaarvoice_productfeed_mapping', $module)) {
    $data = array();
    foreach ($defaults as $key => $mapping) {
      list($type, $bundle, $attribute) = explode(':', $key);
      $data[$type][$bundle][$attribute] = $mapping;
    }

    foreach ($data as $type => $bundles) {
      foreach ($bundles as $bundle => $mappings) {
        bazaarvoice_productfeed_save_entity_attributes($type, $bundle, $mappings);
      }
    }
  }
}
