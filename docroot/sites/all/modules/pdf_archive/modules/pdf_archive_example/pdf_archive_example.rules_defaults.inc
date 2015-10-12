<?php
/**
 * @file
 * pdf_archive_example.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function pdf_archive_example_default_rules_configuration() {
  $items = array();
  $items['rules_pdf_archive_example'] = entity_import('rules_config', '{ "rules_pdf_archive_example" : {
      "LABEL" : "PDF Archive example",
      "PLUGIN" : "reaction rule",
      "TAGS" : [ "PDF Archive" ],
      "REQUIRES" : [ "rules", "pdf_archive" ],
      "ON" : [ "node_insert" ],
      "IF" : [
        { "node_is_of_type" : {
            "node" : [ "node" ],
            "type" : { "value" : { "pdf_example" : "pdf_example" } }
          }
        }
      ],
      "DO" : [
        { "pdf_archive_create" : {
            "USING" : { "entity_wrapper" : [ "node" ], "view_mode" : "full", "rid" : "1" },
            "PROVIDE" : { "pdf_archive" : { "pdf_archive" : "PDF archive" } }
          }
        },
        { "pdf_archive_field_attach" : {
            "entity_wrapper" : [ "node" ],
            "pdf_archive" : [ "pdf_archive" ],
            "field_name" : "field_pdf_example_pdf_archive",
            "filename" : "public:\\/\\/pdf_archive\\/[node:nid].pdf"
          }
        }
      ]
    }
  }');
  return $items;
}
