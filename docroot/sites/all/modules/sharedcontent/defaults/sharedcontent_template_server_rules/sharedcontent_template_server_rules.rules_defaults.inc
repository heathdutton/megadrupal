<?php
/**
 * @file
 * sharedcontent_template_server_rules.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function sharedcontent_template_server_rules_default_rules_configuration() {
  $items = array();
  $items['rules_sharedcontent_index_reason'] = entity_import('rules_config', '{ "rules_sharedcontent_index_reason" : {
      "LABEL" : "Shared Content set index reason",
      "PLUGIN" : "rule",
      "TAGS" : [ "Shared Content" ],
      "REQUIRES" : [ "rules", "sharedcontent" ],
      "USES VARIABLES" : {
        "index" : { "label" : "Shared Content Index", "type" : "sharedcontent_index" },
        "reason" : { "label" : "Reason", "type" : "text" }
      },
      "IF" : [
        { "entity_has_field" : { "entity" : [ "index" ], "field" : "field_sharedcontent_reason" } }
      ],
      "DO" : [
        { "sharedcontent_rules_get_term" : {
            "USING" : { "vocab_name" : "sharedcontent_reason", "term_name" : [ "reason" ] },
            "PROVIDE" : { "term" : { "term" : "Term" } }
          }
        },
        { "list_add" : {
            "list" : [ "index:field-sharedcontent-reason" ],
            "item" : [ "term" ],
            "unique" : 1
          }
        }
      ]
    }
  }');
  $items['rules_sharedcontent_index_update_for_node'] = entity_import('rules_config', '{ "rules_sharedcontent_index_update_for_node" : {
      "LABEL" : "Shared Content enrich index for node",
      "PLUGIN" : "reaction rule",
      "TAGS" : [ "Shared Content" ],
      "REQUIRES" : [ "rules", "sharedcontent" ],
      "ON" : [
        "sharedcontent_index_is_being_created",
        "sharedcontent_index_is_being_updated"
      ],
      "IF" : [ { "entity_is_of_type" : { "entity" : [ "entity" ], "type" : "node" } } ],
      "DO" : [
        { "sharedcontent_rules_get_keywords" : {
            "USING" : { "local_content" : [ "entity" ], "local_content_type" : "node" },
            "PROVIDE" : { "keywords" : { "keywords" : "Content keywords" } }
          }
        },
        { "data_set" : { "data" : [ "index:keywords" ], "value" : [ "keywords" ] } },
        { "sharedcontent_rules_get_tags" : {
            "USING" : { "local_content" : [ "entity" ], "local_content_type" : "node" },
            "PROVIDE" : { "tags" : { "tags" : "Content tags" } }
          }
        },
        { "data_set" : { "data" : [ "index:tags" ], "value" : [ "tags" ] } },
        { "variable_add" : {
            "USING" : { "type" : "integer", "value" : "2" },
            "PROVIDE" : { "variable_added" : { "status_default" : "Status" } }
          }
        },
        { "data_calc" : {
            "USING" : {
              "input_1" : [ "status-default" ],
              "op" : "-",
              "input_2" : [ "entity:status" ]
            },
            "PROVIDE" : { "result" : { "status" : "Status" } }
          }
        },
        { "data_set" : { "data" : [ "index:status" ], "value" : [ "status" ] } },
        { "component_rules_sharedcontent_index_reason" : { "index" : [ "index" ], "reason" : "default" } }
      ]
    }
  }');
  return $items;
}
