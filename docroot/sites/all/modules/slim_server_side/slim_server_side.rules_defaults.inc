<?php
/**
 * @file
 * slim_server_side.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function slim_server_side_default_rules_configuration() {
  $items = array();
  $items['rules_copy_registration_id_to_field'] = entity_import('rules_config', '{ "rules_copy_registration_id_to_field" : {
      "LABEL" : "Copy registration ID to field",
      "PLUGIN" : "reaction rule",
      "TAGS" : [ "Things" ],
      "REQUIRES" : [ "rules" ],
      "ON" : [ "node_presave" ],
      "IF" : [
        { "node_is_of_type" : { "node" : [ "node" ], "type" : { "value" : { "things" : "things" } } } }
      ],
      "DO" : [
        { "data_set" : { "data" : [ "node:field-registration-id" ], "value" : "[node:body]" } }
      ]
    }
  }');
  $items['rules_handle_gcm_error'] = entity_import('rules_config', '{ "rules_handle_gcm_error" : {
      "LABEL" : "Handle GCM error",
      "PLUGIN" : "reaction rule",
      "REQUIRES" : [ "rules", "views_rules", "gcm" ],
      "ON" : [ "gcm_event_registration_id_error" ],
      "DO" : [
        { "drupal_message" : { "message" : [ "registration-id" ] } },
        { "VIEW LOOP" : {
            "VIEW" : "things_by_registration_id",
            "DISPLAY" : "views_rules_1",
            "USING" : { "field_registration_id_value" : [ "registration-id" ] },
            "ROW VARIABLES" : { "registration_node" : { "registration_node" : "Content: Node" } },
            "DO" : [ { "entity_delete" : { "data" : [ "registration-node" ] } } ]
          }
        }
      ]
    }
  }');
  return $items;
}
