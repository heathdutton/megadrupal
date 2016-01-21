<?php

/*
 * @file
 * Contains default rules declarations for the commerce_pado module.
 */

function commerce_pado_default_rules_configuration() {

  // Rule to add a product to the cart if selected as a product add on.

  /*
   * Commented out until bug is fixed regarding data selection on the sku parameter.
  $rule = rules_reaction_rule();
  $rule->label = 'Add the selected product add-on to the cart';
  $rule->active = TRUE;
  $rule->event('commerce_pado_add_to_cart')
        ->action('commerce_cart_product_add_by_sku', array('user:select' => 'site:current-user'),
          array('sku:select' => 'commerce-product:sku'));
  */

  $exported_rule = '{ "commerce_pado_add_selected_add_ons_to_cart" : {
    "LABEL" : "Add the selected product add-on to the cart",
    "PLUGIN" : "reaction rule",
    "REQUIRES" : [ "commerce_cart", "commerce_pado" ],
    "ON" : [ "commerce_pado_add_to_cart" ],
    "DO" : [
      { "commerce_cart_product_add_by_sku" : {
          "USING" : {
            "user" : [ "site:current-user" ],
            "sku" : [ "commerce-product:sku" ],
            "quantity" : [ "quantity" ],
            "combine" : 1
          },
          "PROVIDE" : { "product_add_line_item" : { "product_add_line_item" : "Added product line item" } }
        }
      }
    ]
  }
}';

  //$rules['commerce_pado_add_selected_add_ons_to_cart'] = $rule;
  $rules['commerce_pado_add_selected_add_ons_to_cart'] = rules_import($exported_rule);

  return $rules;
}
