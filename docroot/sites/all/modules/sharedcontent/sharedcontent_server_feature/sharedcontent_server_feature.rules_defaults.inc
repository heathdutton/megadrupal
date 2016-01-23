<?php
/**
 * @file
 * sharedcontent_server_feature.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function sharedcontent_server_feature_default_rules_configuration() {
  $items = array();
  $items['rules_sharedcontent_create_update_node_index'] = entity_import('rules_config', '{ "rules_sharedcontent_create_update_node_index" : {
      "LABEL" : "Shared Content create or update index record for Node",
      "PLUGIN" : "rule",
      "TAGS" : [ "Shared Content" ],
      "REQUIRES" : [ "sharedcontent" ],
      "USES VARIABLES" : { "node" : { "label" : "Entity", "type" : "node" } },
      "DO" : [
        { "sharedcontent_create_or_update_index_for_entity" : { "entity" : [ "node" ], "type" : "node" } }
      ]
    }
  }');
  $items['rules_sharedcontent_index_delete_node'] = entity_import('rules_config', '{ "rules_sharedcontent_index_delete_node" : {
      "LABEL" : "Shared Content mark index record for node as deleted",
      "PLUGIN" : "rule",
      "REQUIRES" : [ "sharedcontent" ],
      "USES VARIABLES" : { "node" : { "label" : "Node", "type" : "node" } },
      "DO" : [
        { "sharedcontent_index_mark_as_deleted_if_exists" : { "entity" : [ "node" ], "type" : "node" } }
      ]
    }
  }');
  return $items;
}
