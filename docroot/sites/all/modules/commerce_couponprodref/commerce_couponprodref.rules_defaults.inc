<?php
/**
 * @file
 * commerce_couponprodref.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function commerce_couponprodref_default_rules_configuration() {
  $items = array();
  $items['rules_commerce_couponprodref_apply_coupon_to_line_item_component'] = entity_import('rules_config', '{ "rules_commerce_couponprodref_apply_coupon_to_line_item_component" : {
      "LABEL" : "Apply coupon to line item (checking product reference)",
      "PLUGIN" : "rule",
      "REQUIRES" : [ "rules", "commerce_coupon_pct" ],
      "USES VARIABLES" : {
        "line_item" : { "label" : "Line item", "type" : "commerce_line_item" },
        "coupon" : { "label" : "Coupon", "type" : "commerce_coupon" }
      },
      "IF" : [
        { "OR" : [
            { "AND" : [
                { "entity_has_field" : { "entity" : [ "coupon" ], "field" : "field_commerce_couponprodref" } },
                { "entity_has_field" : { "entity" : [ "line-item" ], "field" : "commerce_product" } },
                { "list_contains" : {
                    "list" : [ "coupon:field-commerce-couponprodref" ],
                    "item" : [ "line-item:commerce-product" ]
                  }
                }
              ]
            },
            { "AND" : [
                { "entity_has_field" : { "entity" : [ "coupon" ], "field" : "field_commerce_couponprodref" } },
                { "data_is_empty" : { "data" : [ "coupon:field-commerce-couponprodref" ] } }
              ]
            },
            { "NOT entity_has_field" : { "entity" : [ "coupon" ], "field" : "field_commerce_couponprodref" } }
          ]
        }
      ],
      "DO" : [
        { "commerce_coupon_pct_apply_to_product_line_item" : {
            "line_item" : [ "line_item" ],
            "coupon" : [ "coupon" ],
            "component_name" : [ "coupon:price-component-name" ],
            "round_mode" : "1"
          }
        }
      ]
    }
  }');
  $items['rules_commerce_couponprodref_validate_refererenced_products'] = entity_import('rules_config', '{ "rules_commerce_couponprodref_validate_refererenced_products" : {
      "LABEL" : "Coupon Validation: Check the referenced products",
      "PLUGIN" : "reaction rule",
      "REQUIRES" : [ "rules", "commerce_couponprodref", "commerce_coupon" ],
      "ON" : [ "commerce_coupon_validate" ],
      "IF" : [
        { "entity_has_field" : { "entity" : [ "coupon" ], "field" : "field_commerce_couponprodref" } },
        { "NOT data_is_empty" : { "data" : [ "coupon:field-commerce-couponprodref" ] } },
        { "NOT commerce_couponprodref_order_has_referenced_product" : {
            "commerce_order" : [ "commerce_order" ],
            "commerce_coupon" : [ "coupon" ],
            "field" : "field_commerce_couponprodref"
          }
        }
      ],
      "DO" : [ { "commerce_coupon_action_is_invalid_coupon" : [] } ]
    }
  }');
  return $items;
}
