<?php
/**
 * @file
 * austrofeedr_base_processing_autotag.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function austrofeedr_base_processing_autotag_default_rules_configuration() {
  $items = array();
  $items['processing_autotag'] = entity_import('rules_config', '{ "processing_autotag" : {
      "LABEL" : "processing_autotag_updates",
      "PLUGIN" : "reaction rule",
      "ACTIVE" : false,
      "REQUIRES" : [ "rules" ],
      "ON" : [ "node_presave" ],
      "IF" : [
        { "entity_has_field" : { "entity" : [ "node" ], "field" : "field_update_tags" } },
        { "NOT data_is_empty" : { "data" : [ "node:body" ] } }
      ],
      "DO" : [
        { "entity_query" : {
            "USING" : {
              "type" : "taxonomy_vocabulary",
              "property" : "machine_name",
              "value" : "update_tags",
              "limit" : "1"
            },
            "PROVIDE" : { "entity_fetched" : { "voc" : "Vocabulary" } }
          }
        },
        { "component_rules_autotag_tag" : {
            "text" : [ "node:body:value" ],
            "vocabulary" : [ "voc:0" ],
            "target_terms" : [ "node:field-update-tags" ]
          }
        }
      ]
    }
  }');
  return $items;
}
