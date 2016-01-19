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
  // Provide the percent rule only if percent coupons are in use.
  // @see commerce_couponprodref_modules_enabled()
  // @see commerce_couponprodref_modules_disabled()
  if (module_exists('commerce_coupon_pct')) {
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
  }
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
  $items['rules_commerce_couponprodref_remove_invalid_coupons'] = entity_import('rules_config', '{ "rules_commerce_couponprodref_remove_invalid_coupons" : {
      "LABEL" : "Remove unreferenced coupons",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "REQUIRES" : [ "commerce_couponprodref", "rules", "commerce_cart" ],
      "ON" : { "commerce_cart_product_remove" : [] },
      "DO" : [
        { "commerce_couponprodref_action_get_referencing_coupons_by_product" : {
            "USING" : {
              "commerce_order" : [ "commerce_order" ],
              "commerce_product" : [ "commerce_product" ],
              "field" : "field_commerce_couponprodref"
            },
            "PROVIDE" : { "order_referencing_coupons" : { "order_referencing_coupons" : "Coupons attached to the order referencing the product" } }
          }
        },
        { "LOOP" : {
            "USING" : { "list" : [ "order-referencing-coupons" ] },
            "ITEM" : { "coupon" : "Current list item" },
            "DO" : [
              { "component_rules_commerce_couponprodref_remove_unreferenced" : {
                  "commerce_order" : [ "commerce_order" ],
                  "commerce_coupon" : [ "coupon" ]
                }
              }
            ]
          }
        }
      ]
    }
  }');
  $items['rules_commerce_couponprodref_remove_message'] = entity_import('rules_config', '{ "rules_commerce_couponprodref_remove_message" : {
      "LABEL" : "Display removed referencing coupon message",
      "PLUGIN" : "rule set",
      "OWNER" : "rules",
      "REQUIRES" : [ "rules" ],
      "USES VARIABLES" : { "commerce_coupon" : { "label" : "Coupon", "type" : "commerce_coupon" } },
      "RULES" : [
        { "RULE" : {
            "IF" : [
              { "entity_has_field" : {
                  "entity" : [ "commerce-coupon" ],
                  "field" : "field_commerce_couponprodref"
                }
              },
              { "list_count_is" : {
                  "list" : [ "commerce-coupon:field-commerce-couponprodref" ],
                  "value" : "1"
                }
              }
            ],
            "DO" : [
              { "drupal_message" : {
                  "message" : "The coupon [commerce-coupon:commerce-coupon-code] has been removed from your order. If you change your mind and reorder [commerce-coupon:field-commerce-couponprodref] don\'t forget to use the coupon again.",
                  "repeat" : "0"
                }
              }
            ],
            "LABEL" : "Show single product message"
          }
        },
        { "RULE" : {
            "IF" : [
              { "entity_has_field" : {
                  "entity" : [ "commerce-coupon" ],
                  "field" : "field_commerce_couponprodref"
                }
              },
              { "list_count_is" : {
                  "list" : [ "commerce-coupon:field-commerce-couponprodref" ],
                  "op" : "\\u003E",
                  "value" : "1"
                }
              }
            ],
            "DO" : [
              { "drupal_message" : { "message" : "The coupon [commerce-coupon:commerce-coupon-code] has been removed from your order. You can use it again by buying any of the following: [commerce-coupon:field-commerce-couponprodref]." } }
            ],
            "LABEL" : "Show multiple product message"
          }
        }
      ]
    }
  }');
  $items['rules_commerce_couponprodref_remove_unreferenced'] = entity_import('rules_config', '{ "rules_commerce_couponprodref_remove_unreferenced" : {
      "LABEL" : "Remove unreferenced coupon",
      "PLUGIN" : "rule",
      "OWNER" : "rules",
      "REQUIRES" : [ "rules", "commerce_couponprodref", "commerce_coupon" ],
      "USES VARIABLES" : {
        "commerce_order" : { "label" : "Order", "type" : "commerce_order" },
        "commerce_coupon" : { "label" : "Coupon", "type" : "commerce_coupon" }
      },
      "IF" : [
        { "AND" : [
            { "entity_has_field" : {
                "entity" : [ "commerce-coupon" ],
                "field" : "field_commerce_couponprodref"
              }
            },
            { "NOT data_is_empty" : { "data" : [ "commerce-coupon:field-commerce-couponprodref" ] } },
            { "NOT commerce_couponprodref_order_has_referenced_product" : {
                "commerce_order" : [ "commerce_order" ],
                "commerce_coupon" : [ "commerce_coupon" ],
                "field" : "field_commerce_couponprodref"
              }
            }
          ]
        }
      ],
      "DO" : [
        { "commerce_coupon_action_remove_coupon_from_order" : {
            "commerce_order" : [ "commerce_order" ],
            "commerce_coupon" : [ "commerce_coupon" ]
          }
        },
        { "component_rules_commerce_couponprodref_remove_message" : { "commerce_coupon" : [ "commerce_coupon" ] } }
      ]
    }
  }');
  return $items;
}

/**
 * Implements hook_default_rules_configuration_alter().
 */
function commerce_couponprodref_default_rules_configuration_alter(&$configs) {

  // Change the inner loop action in the reaction rule "Apply percentage coupons
  // to product line item" to our component. There's no analogue for this with
  // fixed amount coupons because they apply to the whole order, not individual
  // line items.
  if (!empty($configs['commerce_coupon_pct_apply_pct_coupons_to_line_item'])) {
    $rule = $configs['commerce_coupon_pct_apply_pct_coupons_to_line_item'];
    $action_name = 'commerce_coupon_pct_apply_to_product_line_item';

    // Get default loop from commerce_coupon_pct.
    foreach ($rule->actions() as $loop) {
      if ($loop instanceof RulesLoop) {

        $deleted = FALSE;
        foreach ($loop as $action) {
          if (($action instanceof RulesAbstractPlugin) && ($action->getElementName() == $action_name)) {
            // Delete old loop action.
            $action->delete();
            $deleted = TRUE;
          }
        }

        if ($deleted) {
          // Add our component.
          $loop->action('component_rules_commerce_couponprodref_apply_coupon_to_line_item_component', array(
            'line_item:select' => 'commerce-line-item',
            'coupon:select' => 'list-coupon',
            'component_name:select' => 'list-coupon:price-component-name',
            'round_mode' => COMMERCE_ROUND_HALF_UP,
          ));
        }
      }
    }

    if (empty($deleted)) {
      watchdog('commerce_couponprodref', "Couldn't find the action 'Apply a percentage coupon to a product line item' to replace.", array(), WATCHDOG_WARNING);
    }
  }
}
