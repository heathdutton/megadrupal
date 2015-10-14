<?php
/**
 * @file
 * temp.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function commerce_pricelist_default_rules_configuration() {
  $items = array();
  $items['rules_commerce_price_list_fetch_price'] = entity_import('rules_config', '{ "rules_commerce_price_list_fetch_price" : {
    "LABEL" : "Fetch the price list price for a line item",
    "PLUGIN" : "rule",
    "OWNER" : "rules",
    "REQUIRES" : [ "commerce_pricelist", "rules" ],
    "USES VARIABLES" : {
      "commerce_line_item" : { "label" : "Line item", "type" : "commerce_line_item" },
      "commerce_pricelist_price" : {
        "label" : "Price list price",
        "type" : "list\u003Ccommerce_price\u003E",
        "parameter" : false
      }
    },
    "DO" : [
      { "commerce_pricelist_rate_apply" : {
          "USING" : {
            "commerce_line_item" : [ "commerce_line_item" ],
            "user" : [ "commerce-line-item:order:owner" ]
          },
          "PROVIDE" : { "commerce_pricelist_price_array" : { "commerce_pricelist_price_array" : "Commerce Pricelist price" } }
        }
      },
      { "data_set" : {
          "data" : [ "commerce-pricelist-price" ],
          "value" : [ "commerce-pricelist-price-array" ]
        }
      }
    ],
    "PROVIDES VARIABLES" : [ "commerce_pricelist_price" ]
  }
}');
  $items['rules_commerce_price_list_set_price'] = entity_import('rules_config', '{ "rules_commerce_price_list_set_price" : {
    "LABEL" : "Set the price for a product from price list",
    "PLUGIN" : "reaction rule",
    "WEIGHT" : "-10",
    "OWNER" : "rules",
    "REQUIRES" : [
      "commerce_pricelist",
      "commerce_line_item",
      "commerce_product_reference"
    ],
    "ON" : { "commerce_product_calculate_sell_price" : [] },
    "DO" : [
      { "commerce_pricelist_rate_apply" : {
          "USING" : {
            "commerce_line_item" : [ "commerce_line_item" ],
            "user" : [ "commerce_line_item:order:owner" ]
          },
          "PROVIDE" : { "commerce_pricelist_price_array" : { "commerce_pricelist_price_array" : "Commerce Pricelist price" } }
        }
      },
      { "LOOP" : {
          "USING" : { "list" : [ "commerce-pricelist-price-array" ] },
          "ITEM" : { "list_item" : "Current list item" },
          "DO" : [
            { "commerce_line_item_unit_price_currency_code" : {
                "commerce_line_item" : [ "commerce-line-item" ],
                "currency_code" : [ "list-item:currency-code" ]
              }
            },
            { "commerce_line_item_unit_price_amount" : {
                "commerce_line_item" : [ "commerce_line_item" ],
                "amount" : [ "list-item:amount" ],
                "component_name" : "base_price",
                "round_mode" : "0"
              }
            }
          ]
        }
      }
    ]
  }
}');
  $items['rules_disable_products_with_no_price'] = entity_import('rules_config', '{ "rules_disable_products_with_no_price" : {
      "LABEL" : "Disable products with no price",
      "PLUGIN" : "reaction rule",
      "WEIGHT" : "10",
      "OWNER" : "rules",
      "TAGS" : [ "Commerce Price list" ],
      "REQUIRES" : [ "rules", "commerce_product_reference" ],
      "ON" : { "commerce_product_calculate_sell_price" : [] },
      "IF" : [
        { "data_is" : {
            "data" : [ "commerce-line-item:commerce-unit-price:amount-decimal" ],
            "value" : "0"
          }
        },
        { "entity_has_field" : { "entity" : [ "commerce-line-item" ], "field" : "commerce_product" } }
      ],
      "DO" : [
        { "data_set" : { "data" : [ "commerce-line-item:quantity" ], "value" : "0" } },
        { "data_set" : { "data" : [ "commerce-line-item:commerce-unit-price:amount" ] } }
      ]
    }
  }');
  return $items;
}
