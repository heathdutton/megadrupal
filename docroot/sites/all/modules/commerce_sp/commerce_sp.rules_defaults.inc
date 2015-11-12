<?php
/**
 * @file
 * commerce_sp.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function commerce_sp_default_rules_configuration() {
  $items = array();
  $items['rules_commerce_sp_line_item_is_subscription_product'] = entity_import('rules_config', '{ "rules_commerce_sp_line_item_is_subscription_product" : {
      "LABEL" : "Line item is subscription product",
      "PLUGIN" : "rule",
      "TAGS" : [ "commerce_sp" ],
      "REQUIRES" : [ "rules" ],
      "USES VARIABLES" : { "line_item" : { "label" : "Line item", "type" : "commerce_line_item" } },
      "IF" : [
        { "data_is" : { "data" : [ "line-item:type" ], "value" : "product" } },
        { "data_is" : {
            "data" : [ "line-item:commerce-product:type" ],
            "value" : "commerce_sp_subscription"
          }
        },
        { "data_is" : {
            "data" : [ "line-item:commerce-product:field-commerce-sp-time" ],
            "op" : "\\u003e",
            "value" : "0"
          }
        }
      ],
      "DO" : [
        { "data_calc" : {
            "USING" : {
              "input_1" : [ "line-item:commerce-product:field-commerce-sp-time" ],
              "op" : "*",
              "input_2" : [ "line-item:quantity" ]
            },
            "PROVIDE" : { "result" : { "subscription_time" : "Subscription time" } }
          }
        },
        { "component_rules_commerce_sp_subscription_time_to_validity" : {
            "subscription_time" : [ "subscription_time" ],
            "account" : [ "line-item:order:owner" ]
          }
        }
      ]
    }
  }');
  $items['rules_commerce_sp_subscription_aborted_as_account_is_del'] = entity_import('rules_config', '{ "rules_commerce_sp_subscription_aborted_as_account_is_del" : {
      "LABEL" : "Subscription aborted as user account is deleted",
      "PLUGIN" : "reaction rule",
      "TAGS" : [ "commerce_sp" ],
      "REQUIRES" : [ "rules", "rules_scheduler" ],
      "ON" : [ "user_delete" ],
      "IF" : [
        { "NOT data_is_empty" : { "data" : [ "account:field-commerce-sp-validity" ] } }
      ],
      "DO" : [
        { "schedule_delete" : {
            "component" : "rules_commerce_sp_subscription_end",
            "task" : "expire-[account:uid]"
          }
        }
      ]
    }
  }');
  $items['rules_commerce_sp_subscription_bought'] = entity_import('rules_config', '{ "rules_commerce_sp_subscription_bought" : {
      "LABEL" : "Apply bought subscription time",
      "PLUGIN" : "reaction rule",
      "TAGS" : [ "commerce_sp" ],
      "REQUIRES" : [ "rules", "commerce_checkout" ],
      "ON" : [ "commerce_checkout_complete" ],
      "DO" : [
        { "LOOP" : {
            "USING" : { "list" : [ "commerce-order:commerce-line-items" ] },
            "ITEM" : { "list_item" : "Current list item" },
            "DO" : [
              { "component_rules_commerce_sp_line_item_is_subscription_product" : { "line_item" : [ "list-item" ] } }
            ]
          }
        }
      ]
    }
  }');
  $items['rules_commerce_sp_subscription_end'] = entity_import('rules_config', '{ "rules_commerce_sp_subscription_end" : {
      "LABEL" : "Subscription end",
      "PLUGIN" : "rule set",
      "TAGS" : [ "commerce_sp" ],
      "REQUIRES" : [ "rules" ],
      "USES VARIABLES" : { "user" : { "label" : "User", "type" : "user" } },
      "RULES" : [
        { "RULE" : {
            "DO" : [ { "data_set" : { "data" : [ "user:field-commerce-sp-validity" ] } } ],
            "LABEL" : "Clear account validity date"
          }
        }
      ]
    }
  }');
  $items['rules_commerce_sp_subscription_start'] = entity_import('rules_config', '{ "rules_commerce_sp_subscription_start" : {
      "LABEL" : "Subscription start",
      "PLUGIN" : "rule set",
      "TAGS" : [ "commerce_sp" ],
      "USES VARIABLES" : { "user" : { "label" : "User", "type" : "user" } },
      "RULES" : []
    }
  }');
  $items['rules_commerce_sp_subscription_started_insert'] = entity_import('rules_config', '{ "rules_commerce_sp_subscription_started_insert" : {
      "LABEL" : "Subscription started (insert)",
      "PLUGIN" : "reaction rule",
      "TAGS" : [ "commerce_sp" ],
      "REQUIRES" : [ "rules", "rules_scheduler" ],
      "ON" : [ "user_insert" ],
      "IF" : [
        { "NOT data_is_empty" : { "data" : [ "account:field-commerce-sp-validity" ] } }
      ],
      "DO" : [
        { "component_rules_commerce_sp_subscription_start" : { "user" : [ "account" ] } },
        { "schedule" : {
            "component" : "rules_commerce_sp_subscription_end",
            "date" : [ "account:field-commerce-sp-validity" ],
            "identifier" : "expire-[account:uid]",
            "param_user" : [ "account" ]
          }
        }
      ]
    }
  }');
  $items['rules_commerce_sp_subscription_started_update'] = entity_import('rules_config', '{ "rules_commerce_sp_subscription_started_update" : {
      "LABEL" : "Subscription started (update)",
      "PLUGIN" : "reaction rule",
      "TAGS" : [ "commerce_sp" ],
      "REQUIRES" : [ "rules", "rules_scheduler" ],
      "ON" : [ "user_update" ],
      "IF" : [
        { "NOT data_is_empty" : { "data" : [ "account:field-commerce-sp-validity" ] } },
        { "data_is_empty" : { "data" : [ "account-unchanged:field-commerce-sp-validity" ] } }
      ],
      "DO" : [
        { "component_rules_commerce_sp_subscription_start" : { "user" : [ "account" ] } },
        { "schedule" : {
            "component" : "rules_commerce_sp_subscription_end",
            "date" : [ "account:field-commerce-sp-validity" ],
            "identifier" : "expire-[account:uid]",
            "param_user" : [ "account" ]
          }
        }
      ]
    }
  }');
  $items['rules_commerce_sp_subscription_time_to_validity'] = entity_import('rules_config', '{ "rules_commerce_sp_subscription_time_to_validity" : {
      "LABEL" : "Apply bought subscription time to user subscription validity",
      "PLUGIN" : "rule set",
      "TAGS" : [ "commerce_sp" ],
      "REQUIRES" : [ "rules" ],
      "USES VARIABLES" : {
        "subscription_time" : { "label" : "Subscription time", "type" : "duration" },
        "account" : { "label" : "User account to apply the time", "type" : "user" }
      },
      "RULES" : [
        { "RULE" : {
            "IF" : [
              { "data_is_empty" : { "data" : [ "account:field-commerce-sp-validity" ] } }
            ],
            "DO" : [
              { "data_set" : { "data" : [ "account:field-commerce-sp-validity" ], "value" : "now" } }
            ],
            "LABEL" : "User has no subscription yet"
          }
        },
        { "RULE" : {
            "PROVIDE" : { "result" : { "result" : "Calculation result" } },
            "DO" : [
              { "data_calc" : {
                  "USING" : {
                    "input_1" : [ "account:field-commerce-sp-validity" ],
                    "op" : "+",
                    "input_2" : [ "subscription-time" ]
                  },
                  "PROVIDE" : { "result" : { "result" : "Calculation result" } }
                }
              },
              { "data_set" : {
                  "data" : [ "account:field-commerce-sp-validity" ],
                  "value" : [ "result" ]
                }
              }
            ],
            "LABEL" : "Add the time period to the validity date"
          }
        }
      ]
    }
  }');
  $items['rules_commerce_sp_subscription_validity_changed'] = entity_import('rules_config', '{ "rules_commerce_sp_subscription_validity_changed" : {
      "LABEL" : "Subscription validity changed",
      "PLUGIN" : "reaction rule",
      "TAGS" : [ "commerce_sp" ],
      "REQUIRES" : [ "rules", "rules_scheduler" ],
      "ON" : [ "user_update" ],
      "IF" : [
        { "NOT data_is" : {
            "data" : [ "account:field-commerce-sp-validity" ],
            "value" : [ "account-unchanged:field-commerce-sp-validity" ]
          }
        },
        { "NOT data_is_empty" : { "data" : [ "account:field-commerce-sp-validity" ] } },
        { "NOT data_is_empty" : { "data" : [ "account-unchanged:field-commerce-sp-validity" ] } }
      ],
      "DO" : [
        { "schedule" : {
            "component" : "rules_commerce_sp_subscription_end",
            "date" : [ "account:field-commerce-sp-validity" ],
            "identifier" : "expire-[account:uid]",
            "param_user" : [ "account" ]
          }
        }
      ]
    }
  }');
  $items['rules_commerce_sp_subscription_validity_is_unset'] = entity_import('rules_config', '{ "rules_commerce_sp_subscription_validity_is_unset" : {
      "LABEL" : "Subscription validity is unset",
      "PLUGIN" : "reaction rule",
      "TAGS" : [ "commerce_sp" ],
      "REQUIRES" : [ "rules", "rules_scheduler" ],
      "ON" : [ "user_update" ],
      "IF" : [
        { "NOT data_is" : {
            "data" : [ "account:field-commerce-sp-validity" ],
            "value" : [ "account-unchanged:field-commerce-sp-validity" ]
          }
        },
        { "data_is_empty" : { "data" : [ "account:field-commerce-sp-validity" ] } }
      ],
      "DO" : [
        { "schedule_delete" : {
            "component" : "rules_commerce_sp_subscription_end",
            "task" : "expire-[account:uid]"
          }
        },
        { "component_rules_commerce_sp_subscription_end" : { "user" : [ "account" ] } }
      ]
    }
  }');
  return $items;
}
