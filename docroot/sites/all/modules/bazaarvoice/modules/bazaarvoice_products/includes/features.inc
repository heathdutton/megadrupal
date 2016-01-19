<?php
/**
 * @file
 * Features file for the bazaarvoice products module.
 */

/**
 * Implements COMPONENT_features_export_options().
 */
function bazaarvoice_product_features_export_options() {
  $options = array();

  if ($types = bazaarvoice_products_get_product_entity_types()) {
    foreach ($types as $type => $bundles) {
      foreach ($bundles as $bundle => $bundle_data) {
        $key = $type . ':' . $bundle;
        $options[$key] = ucwords($bundle) . ' (' . $type . ')';
      }
    }
  }

  return $options;
}

/**
 * Implements COMPONENT_features_export().
 */
function bazaarvoice_product_features_export($data, &$export, $module_name) {
  $export['dependencies']['bazaarvoice'] = 'bazaarvoice';
  $export['dependencies']['bazaarvoice_products'] = 'bazaarvoice_products';
  $pipe = array();
  foreach ($data as $key) {
    list($type, $bundle) = explode(':', $key);
    $export['features']['bazaarvoice_product'][$key] = $key;
    // Add dependency on entity type.
    $pipe[$type][] = $bundle;
  }

  return $pipe;
}

/**
 * Implements COMPONENT_features_export_render().
 */
function bazaarvoice_product_features_export_render($module_name, $data) {
  $items = array();
  if ($types = bazaarvoice_products_get_product_entity_types()) {
    foreach ($data as $key) {
      list($type, $bundle) = explode(':', $key);
      if (isset($types[$type][$bundle])) {
        $items[$type][$bundle] = $types[$type][$bundle]['product_id_field'];
      }
    }
  }

  $code = "  \$items = " . features_var_export($items, '  ') . ";\n";
  $code .= '  return $items;';

  return array('bazaarvoice_product' => $code);
}

/**
 * Implements COMPONENT_features_revert().
 */
function bazaarvoice_product_features_revert($module) {
  bazaarvoice_product_features_rebuild($module);
}

/**
 * Implements COMPONENT_features_enable_feature().
 */
function bazaarvoice_product_features_enable_feature($module) {
  bazaarvoice_product_features_rebuild($module);
}

/**
 * Implements COMPONENT_features_rebuild().
 */
function bazaarvoice_product_features_rebuild($module) {
  if ($defaults = features_get_default('bazaarvoice_product', $module)) {
    foreach ($defaults as $type => $bundles) {
      foreach ($bundles as $bundle => $product_id_field) {
        bazaarvoice_products_set_entity_type_product_id($type, $bundle, $product_id_field);
      }
    }
  }
}
