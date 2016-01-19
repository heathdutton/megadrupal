<?php

/**
 * @file
 * erpal_invoice.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function erpal_invoice_default_rules_configuration() {
  $items = array();
  $items['commerce_billy_set_status_to_invoiced'] = entity_import('rules_config', '{ "commerce_billy_set_status_to_invoiced" : {
      "LABEL" : "Set status to invoiced",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Erpal Invoice" ],
      "REQUIRES" : [ "rules", "commerce_order", "entity" ],
      "ON" : { "commerce_order_insert" : [] },
      "IF" : [
        { "data_is" : { "data" : [ "commerce-order:type" ], "value" : "invoice" } },
        { "NOT data_is" : { "data" : [ "commerce-order:status" ], "value" : "invoiced" } }
      ],
      "DO" : [
        { "commerce_order_update_status" : { "commerce_order" : [ "commerce_order" ], "order_status" : "invoiced" } }
      ]
    }
  }');
  $items['rules_generate_due_date_on_invoice_creation'] = entity_import('rules_config', '{ "rules_generate_due_date_on_invoice_creation" : {
      "LABEL" : "Generate due date on invoice creation",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Erpal Invoice" ],
      "REQUIRES" : [ "commerce", "erpal_invoice", "entity" ],
      "ON" : { "commerce_order_insert" : [], "commerce_order_update" : [] },
      "IF" : [
        { "data_is" : { "data" : [ "commerce-order:type" ], "value" : "invoice" } }
      ],
      "DO" : [ { "erpal_invoice_set_due_date" : { "order" : [ "commerce-order" ] } } ]
    }
  }');
  return $items;
}
