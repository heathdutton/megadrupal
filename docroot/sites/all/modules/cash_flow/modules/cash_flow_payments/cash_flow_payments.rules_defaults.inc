<?php
/**
 * @file
 * cash_flow_payments.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function cash_flow_payments_default_rules_configuration() {
  $items = array();
  $items['rules_payment_saved'] = entity_import('rules_config', '{ "rules_payment_saved" : {
      "LABEL" : "Payment saved",
      "PLUGIN" : "reaction rule",
      "REQUIRES" : [ "rules" ],
      "ON" : [ "node_insert", "node_update", "node_delete" ],
      "IF" : [
        { "node_is_of_type" : { "node" : [ "node" ], "type" : { "value" : { "payment" : "payment" } } } }
      ],
      "DO" : [
        { "component_rules_re_save_income_and_expense" : {
            "line_item_nid" : [ "node:field-line-item:nid" ],
            "target_line_item_nid" : [ "node:field-target-line-item:nid" ]
          }
        }
      ]
    }
  }');
  $items['rules_re_save_income_and_expense'] = entity_import('rules_config', '{ "rules_re_save_income_and_expense" : {
      "LABEL" : "Re-save income and expense",
      "PLUGIN" : "action set",
      "REQUIRES" : [ "rules" ],
      "USES VARIABLES" : {
        "line_item_nid" : { "label" : "Source line item node ID", "type" : "integer" },
        "target_line_item_nid" : { "label" : "Target line item node ID", "type" : "integer" }
      },
      "ACTION SET" : [
        { "entity_fetch" : {
            "USING" : { "type" : "node", "id" : [ "line-item-nid" ] },
            "PROVIDE" : { "entity_fetched" : { "line_item_entity_fetched" : "Source line item" } }
          }
        },
        { "entity_fetch" : {
            "USING" : { "type" : "node", "id" : [ "target-line-item-nid" ] },
            "PROVIDE" : { "entity_fetched" : { "target_line_item_entity_fetched" : "Target line item" } }
          }
        },
        { "entity_save" : { "data" : [ "line-item-entity-fetched" ] } },
        { "entity_save" : { "data" : [ "target-line-item-entity-fetched" ] } }
      ]
    }
  }');
  return $items;
}
