<?php

/**
 * @file
 * Provides default rules configuration for adding the donation value entered to
 * the line item price.
 */

/**
 * Implements hook_default_rules_configuration().
 */
function commerce_donate_default_rules_configuration() {
  $items = array();
  $items['rules_add_donation_value_to_price'] = entity_import('rules_config', '{ "rules_add_donation_value_to_price" : {
      "LABEL" : "Add donation value to price",
      "PLUGIN" : "reaction rule",
      "REQUIRES" : [ "rules", "commerce_line_item", "commerce_product_reference", "entity" ],
      "ON" : [ "commerce_product_calculate_sell_price", "commerce_line_item_presave" ],
      "IF" : [
        { "entity_has_field" : {
            "entity" : [ "commerce-line-item" ],
            "field" : "commerce_donate_amount"
          }
        }
      ],
      "DO" : [
        { "data_calc" : {
            "USING" : {
              "input_1" : [ "commerce-line-item:commerce-donate-amount" ],
              "op" : "*",
              "input_2" : "100"
            },
            "PROVIDE" : { "result" : { "donation" : "Donation amount" } }
          }
        },
        { "commerce_line_item_unit_price_amount" : {
            "commerce_line_item" : [ "commerce_line_item" ],
            "amount" : [ "donation" ],
            "component_name" : "base_price",
            "round_mode" : "0"
          }
        }
      ]
    }
  }');
  return $items;
}
