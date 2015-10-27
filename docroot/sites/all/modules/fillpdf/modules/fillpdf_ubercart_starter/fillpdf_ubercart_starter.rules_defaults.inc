<?php
/**
 * @file
 * fillpdf_ubercart_starter.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function fillpdf_ubercart_starter_default_rules_configuration() {
  $items = array();
  $items['rules_product_has_linked_fillpdf_template'] = entity_import('rules_config', '{ "rules_product_has_linked_fillpdf_template" : {
      "LABEL" : "Product has linked FillPDF template",
      "PLUGIN" : "and",
      "OWNER" : "rules",
      "TAGS" : [ "fillpdf" ],
      "REQUIRES" : [ "rules" ],
      "USES VARIABLES" : { "ordered_product" : { "label" : "Ordered product", "type" : "uc_order_product" } },
      "AND" : [
        { "entity_has_field" : { "entity" : [ "ordered-product:node" ], "field" : "field_fillpdf_fid" } },
        { "NOT data_is_empty" : { "data" : [ "ordered-product:node:field-fillpdf-fid" ] } }
      ]
    }
  }');
  $items['rules_send_fillpdf_link_to_download_pdf_with_order_product_data'] = entity_import('rules_config', '{ "rules_send_fillpdf_link_to_download_pdf_with_order_product_data" : {
      "LABEL" : "If applicable, send FillPDF Link to download PDF with order product data",
      "PLUGIN" : "rule",
      "OWNER" : "rules",
      "TAGS" : [ "fillpdf" ],
      "REQUIRES" : [ "rules" ],
      "USES VARIABLES" : { "ordered_product" : { "label" : "Ordered product", "type" : "uc_order_product" } },
      "IF" : [
        { "entity_has_field" : { "entity" : [ "ordered-product:node" ], "field" : "field_fillpdf_fid" } },
        { "component_rules_product_has_linked_fillpdf_template" : { "ordered_product" : [ "ordered_product" ] } }
      ],
      "DO" : [
        { "variable_add" : {
            "USING" : {
              "type" : "text",
              "value" : [ "ordered-product:node:field-fillpdf-fid" ]
            },
            "PROVIDE" : { "variable_added" : { "fillpdf_fid" : "FillPDF form ID" } }
          }
        },
        { "entity_fetch" : {
            "USING" : { "type" : "uc_order", "id" : [ "ordered-product:order-id" ] },
            "PROVIDE" : { "entity_fetched" : { "order" : "Ordered product\\u0027s order" } }
          }
        },
        { "mail" : {
            "to" : [ "order:customer:mail" ],
            "subject" : "Link to the PDF you ordered ([ordered-product:title])",
            "message" : "Thank you for your order! You can download your personalized PDF here: [site:url]fillpdf?fid=[fillpdf-fid:value]\\u0026uc_order_product_id=[ordered-product:order-product-id]",
            "language" : [ "" ]
          }
        }
      ]
    }
  }');
  $items['rules_send_link_to_pdf_when_order_status_changes_to_completed'] = entity_import('rules_config', '{ "rules_send_link_to_pdf_when_order_status_changes_to_completed" : {
      "LABEL" : "Send link to PDF when order status changes to Completed",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "fillpdf" ],
      "REQUIRES" : [ "rules", "uc_order" ],
      "ON" : { "uc_order_status_update" : [] },
      "IF" : [
        { "NOT data_is" : { "data" : [ "order:order-status" ], "value" : "completed" } },
        { "data_is" : { "data" : [ "updated-order:order-status" ], "value" : "completed" } }
      ],
      "DO" : [
        { "LOOP" : {
            "USING" : { "list" : [ "updated-order:products" ] },
            "ITEM" : { "ordered_product" : "Current ordered product" },
            "DO" : [
              { "component_rules_send_fillpdf_link_to_download_pdf_with_order_product_data" : { "ordered_product" : [ "ordered_product" ] } }
            ]
          }
        }
      ]
    }
  }');
  return $items;
}
