<?php

/**
 * @file
 * Customized inline entity form controller, as suggested in the
 * IEF issue http://drupal.org/node/1521274.
 */

class PriceTableCommerceProductInlineEntityFormController extends CommerceProductInlineEntityFormController {

  public function defaultFields($bundles) {
    $fields = parent::defaultFields($bundles);

    $weight = 0;
    if (isset($fields['commerce_price'])) {
      $weight = isset($fields['commerce_price']['weight']) ? $fields['commerce_price']['weight'] + 1 : 1;
      foreach ($bundles as $bundle) {
        foreach (commerce_price_table_get_field_instance_settings('commerce_product', $bundle) as $setting) {
          if (isset($setting['settings']['commerce_price_table']['hide_default_price']) && $setting['settings']['commerce_price_table']['hide_default_price']) {
            $fields['commerce_price']['visible'] = FALSE;
          }
        }
      }
    }

    // Add a column for the price table
    $fields['field_price_table'] = array (
      'type' => 'field',
      'label' => 'Price Table',
      'formatter' => 'commerce_multiprice_list_ief',
      'settings' => array(),
      'visible' => TRUE,
      'weight' => $weight,
    );

    return $fields;
  }
}
