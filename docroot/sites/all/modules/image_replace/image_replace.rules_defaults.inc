<?php

/**
 * @file
 * Default rules for Image Replace module.
 */

/**
 * Implements hook_default_rules_configuration().
 */
function image_replace_default_rules_configuration() {
  $items = array();
  $items['image_replace_rebuild_mapping'] = entity_import('rules_config', '{ "image_replace_rebuild_mapping" : {
      "LABEL" : "Rebuild Image Replacement Mapping",
      "PLUGIN" : "rule",
      "OWNER" : "rules",
      "REQUIRES" : [ "image_replace" ],
      "USES VARIABLES" : { "entity" : { "label" : "Entity", "type" : "entity" } },
      "DO" : [
        { "image_replace_rebuild_mapping_action" : { "entity" : [ "entity" ] } }
      ]
    }
  }');

  return $items;
}
