<?php
/**
 * @file
 * commerce_saleprice.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function commerce_saleprice_default_rules_configuration() {
  $items = array();
  $items['rules_commerce_saleprice_component'] = entity_import('rules_config', '{ "rules_commerce_saleprice_component" : {
      "LABEL" : "Sale price (Rules component)",
      "PLUGIN" : "rule",
      "REQUIRES" : [ "rules", "commerce_line_item" ],
      "USES VARIABLES" : { "line_item" : { "label" : "Line item", "type" : "commerce_line_item" } },
      "IF" : [
        { "entity_has_field" : { "entity" : [ "line-item" ], "field" : "commerce_product" } },
        { "entity_has_field" : {
            "entity" : [ "line-item:commerce-product" ],
            "field" : "field_commerce_saleprice_on_sale"
          }
        },
        { "entity_has_field" : {
            "entity" : [ "line-item:commerce-product" ],
            "field" : "field_commerce_saleprice"
          }
        },
        { "NOT data_is_empty" : { "data" : [ "line-item:commerce-product:field-commerce-saleprice:amount" ] } },
        { "data_is" : {
            "data" : [ "line-item:commerce-product:field-commerce-saleprice-on-sale" ],
            "value" : 1
          }
        }
      ],
      "DO" : [
        { "commerce_line_item_unit_price_amount" : {
            "commerce_line_item" : [ "line-item" ],
            "amount" : [ "line-item:commerce-product:field-commerce-saleprice:amount" ],
            "component_name" : "discount",
            "round_mode" : "1"
          }
        }
      ]
    }
  }');
  $items['rules_commerce_saleprice_rule'] = entity_import('rules_config', '{ "rules_commerce_saleprice_rule" : {
      "LABEL" : "Sale price",
      "PLUGIN" : "reaction rule",
      "REQUIRES" : [ "rules", "commerce_product_reference" ],
      "ON" : [ "commerce_product_calculate_sell_price" ],
      "DO" : [
        { "component_rules_commerce_saleprice_component" : { "line_item" : [ "commerce-line-item" ] } }
      ]
    }
  }');
  return $items;
}
