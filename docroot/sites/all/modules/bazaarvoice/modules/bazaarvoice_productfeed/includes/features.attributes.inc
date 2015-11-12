<?php
/**
 * @file
 * Features file for the bazaarvoice Productfeed module.
 */

/**
 * Implements COMPONENT_features_export_options().
 */
function bazaarvoice_productfeed_attribute_features_export_options() {
  return db_select('bazaarvoice_productfeed_custom_attributes', 'bvpca')
    ->fields('bvpca', array('attribute', 'attribute'))
    ->execute()
    ->fetchAllKeyed();
}

/**
 * Implements COMPONENT_features_export().
 */
function bazaarvoice_productfeed_attribute_features_export($data, &$export, $module_name) {
  $export['dependencies']['bazaarvoice_productfeed'] = 'bazaarvoice_productfeed';
  $export['dependencies']['bazaarvoice'] = 'bazaarvoice';

  foreach ($data as $component) {
    $export['features']['bazaarvoice_productfeed_attribute'][$component] = $component;
  }
  // No pipe to return.
  return array();
}

/**
 * Implements COMPONENT_features_export_render().
 */
function bazaarvoice_productfeed_attribute_features_export_render($module_name, $data) {
  module_load_include('inc', 'bazaarvoice_productfeed', 'includes/attributes');
  $attributes = bazaarvoice_productfeed_get_custom_attributes($data);
  $items = array();
  foreach ($attributes as $attribute_id => $attribute) {
    unset($attribute['attribute']);
    $items[$attribute_id] = $attribute;
  }

  $code = " \$items = " . features_var_export($items, '  ') . ";\n";
  $code .= '  return $items;';

  return array('bazaarvoice_productfeed_attribute' => $code);
}

/**
 * Implements COMPONENT_features_revert().
 */
function bazaarvoice_productfeed_attribute_features_revert($module) {
  bazaarvoice_productfeed_attribute_features_rebuild($module);
}

/**
 * Implements COMPONENT_features_enable_feature().
 */
function bazaarvoice_productfeeed_attribute_features_enable_feature($module) {
  bazaarvoice_productfeed_attribute_features_rebuild($module);
}

/**
 * Implements COMPONENT_features_rebuild().
 */
function bazaarvoice_productfeed_attribute_features_rebuild($module) {
  module_load_include('inc', 'bazaarvoice_productfeed', 'includes/attributes');
  if ($defaults = features_get_default('bazaarvoice_productfeed_attribute', $module)) {
    foreach ($defaults as $attribute_id => $attribute_data) {
      bazaarvoice_productfeed_set_custom_attribute($attribute_id, $attribute_data);
    }
  }
}
