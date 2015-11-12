<?php

/**
 * Implements hook_default_rules_configuration().
 */
function commerce_pricing_attributes_default_rules_configuration() {
  $rules = array();

  $rule = '{ "commerce_pricing_attributes_calculation" : {
      "LABEL" : "Commerce pricing attributes calculation",
      "WEIGHT" : "-7",
      "PLUGIN" : "reaction rule",
      "REQUIRES" : [
        "rules",
        "commerce_pricing_attributes",
        "commerce_line_item",
        "commerce_product_reference"
      ],
      "ON" : [ "commerce_product_calculate_sell_price" ],
      "IF" : [
        { "entity_has_field" : { "entity" : [ "commerce-line-item" ], "field" : "commerce_product" } }
      ],
      "DO" : [
        { "commerce_pricing_attributes_calculate_adjusment" : {
            "USING" : { "commerce_line_item" : [ "commerce_line_item" ] },
            "PROVIDE" : { "commerce_attribute_adjustment" : { "commerce_attribute_adjustment" : "Attribute Adjustment" } }
          }
        },
        { "commerce_line_item_unit_price_add" : {
            "commerce_line_item" : [ "commerce-line-item" ],
            "amount" : [ "commerce-attribute-adjustment" ],
            "component_name" : "base_price",
            "round_mode" : "1"
          }
        }
      ]
    }
  }';

  $rules['commerce_pricing_attributes_calculation'] = rules_import($rule);

  return $rules;
}
