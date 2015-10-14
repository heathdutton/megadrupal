<?php

/**
 * @file
 * Contains default rule configurations for the Commerce Export module.
 *
 * @addtogroup rules
 * @{
 */

/**
 * Implements hook_default_rules_configuration().
 */
function commerce_export_default_rules_configuration() {
  $items = array();

  $items['commerce_export_export_order'] = entity_import('rules_config', '{ "commerce_export_export_order" : {
      "LABEL" : "Export an order",
      "PLUGIN" : "action set",
      "OWNER" : "rules",
      "TAGS" : [ "Commerce Export" ],
      "REQUIRES" : [ "rules" ],
      "USES VARIABLES" : {
        "commerce_order" : { "label" : "Commerce order", "type" : "commerce_order" },
        "commerce_orders_provided" : {
          "label" : "Commerce orders (provided)",
          "type" : "list\u003Ccommerce_order\u003E",
          "parameter" : false
        },
        "export_file_provided" : {
          "label" : "Export file (provided)",
          "type" : "file",
          "parameter" : false
        }
      },
      "ACTION SET" : [
        { "variable_add" : {
            "USING" : { "type" : "list\u003Ccommerce_order\u003E" },
            "PROVIDE" : { "variable_added" : { "commerce_orders" : "Commerce orders" } }
          }
        },
        { "list_add" : { "list" : [ "commerce-orders" ], "item" : [ "commerce-order" ] } },
        { "data_set" : {
            "data" : [ "commerce-orders-provided" ],
            "value" : [ "commerce-orders" ]
          }
        },
        { "commerce_export" : {
            "USING" : {
              "commerce_orders" : [ "commerce-orders" ],
              "export_name" : "export-[commerce-orders:0:order-id].txt"
            },
            "PROVIDE" : { "export_file" : { "export_file" : "Export file" } }
          }
        },
        { "data_set" : { "data" : [ "export-file-provided" ], "value" : [ "export-file" ] } }
      ],
      "PROVIDES VARIABLES" : [ "commerce_orders_provided", "export_file_provided" ]
    }
  }');
  $items['commerce_export_export_multiple_orders'] = entity_import('rules_config', '{ "commerce_export_export_multiple_orders" : {
      "LABEL" : "Export multiple orders",
      "PLUGIN" : "action set",
      "OWNER" : "rules",
      "TAGS" : [ "Commerce Export" ],
      "REQUIRES" : [ "rules" ],
      "USES VARIABLES" : {
        "commerce_orders" : { "label" : "Commerce orders", "type" : "list\u003Ccommerce_order\u003E" },
        "export_file_provided" : {
          "label" : "Export file (provided)",
          "type" : "file",
          "parameter" : false
        }
      },
      "ACTION SET" : [
        { "commerce_export" : {
            "USING" : {
              "commerce_orders" : [ "commerce-orders" ],
              "export_name" : "export-[date:raw].txt"
            },
            "PROVIDE" : { "export_file" : { "export_file" : "Export file" } }
          }
        },
        { "data_set" : { "data" : [ "export-file-provided" ], "value" : [ "export-file" ] } }
      ],
      "PROVIDES VARIABLES" : [ "export_file_provided" ]
    }
  }');
  $items['commerce_export_export_order_email_checkout_completion'] = entity_import('rules_config', '{ "commerce_export_export_order_email_checkout_completion" : {
      "LABEL" : "Export an order and email on checkout completion",
      "PLUGIN" : "reaction rule",
      "ACTIVE" : false,
      "OWNER" : "rules",
      "TAGS" : [ "Commerce Checkout", "Commerce Export" ],
      "REQUIRES" : [ "rules", "mimemail", "commerce_checkout" ],
      "ON" : { "commerce_checkout_complete" : [] },
      "DO" : [
        { "component_commerce_export_export_order" : {
            "USING" : { "commerce_order" : [ "commerce_order" ] },
            "PROVIDE" : {
              "commerce_orders_provided" : { "commerce_orders_provided" : "Commerce orders (provided)" },
              "export_file_provided" : { "export_file_provided" : "Export file (provided)" }
            }
          }
        },
        { "mimemail" : {
            "key" : "export_order_email_checkout_completion_[commerce-orders-provided:0:order-id]",
            "to" : "info@example.com",
            "subject" : "[site:name] order export: [commerce-orders-provided:0:order-id]",
            "attachments" : "[export-file-provided:path]",
            "language" : [ "" ]
          }
        },
        { "entity_delete" : { "data" : [ "export-file-provided" ] } }
      ]
    }
  }');
  $items['commerce_export_export_orders_email_cron'] = entity_import('rules_config', '{ "commerce_export_export_orders_email_cron" : {
      "LABEL" : "Export orders and email on cron",
      "PLUGIN" : "reaction rule",
      "ACTIVE" : false,
      "OWNER" : "rules",
      "TAGS" : [ "Commerce Export" ],
      "REQUIRES" : [ "rules", "mimemail" ],
      "ON" : { "cron" : [] },
      "DO" : [
        { "variable_add" : {
            "USING" : { "type" : "token", "value" : "pending" },
            "PROVIDE" : { "variable_added" : { "status" : "Status" } }
          }
        },
        { "entity_query" : {
            "USING" : {
              "type" : "commerce_order",
              "property" : "status",
              "value" : [ "status" ]
            },
            "PROVIDE" : { "entity_fetched" : { "commerce_orders" : "Commerce orders" } }
          }
        },
        { "component_commerce_export_export_multiple_orders" : {
            "USING" : { "commerce_orders" : [ "commerce-orders" ] },
            "PROVIDE" : { "export_file_provided" : { "export_file_provided" : "Export file (provided)" } }
          }
        },
        { "mimemail" : {
            "key" : "export_orders_email_cron_[date:raw]",
            "to" : "info@example.com",
            "subject" : "[site:name] order export: [date:raw]",
            "attachments" : "[export-file-provided:path]",
            "language" : [ "" ]
          }
        },
        { "entity_delete" : { "data" : [ "export-file-provided" ] } }
      ]
    }
  }');

  return $items;
}

/**
 * @}
 */
