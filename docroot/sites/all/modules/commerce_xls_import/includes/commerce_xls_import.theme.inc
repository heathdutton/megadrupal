<?php

/**
 * @file
 * Theme and preprocess functions for the Commerce XLS Import module.
 */

/**
 * Implements template_preprocess_commerce_xls_import_instructions().
 */
function commerce_xls_import_preprocess_commerce_xls_import_instructions(&$variables) {
  $variables['product_types'] = array();

  foreach (commerce_product_types() as $product_type) {
    $fields = commerce_xls_import_get_possible_fields($product_type['type']);
    foreach ($fields as $field) {
      $info = field_info_instance('commerce_product', $field, $product_type['type']);

      if (isset($info['description']) && $info['description'] != NULL) {
        // Only display description if it exists.
        $variables['product_types'][$product_type['type']][$field] = array(
          '#markup' => '<div class="product-field"><h6>' . $field . '</h6><span class="product-field-description">' . t("@description", array('@description' => $info['description'])) . '</span></div><div style="clear:both;"></div>',
        );
      }
    }
  }
}
