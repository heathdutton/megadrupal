<?php

/**
 * Implements hook_default_rules_configuration().
 */
function rules_autotag_default_rules_configuration() {

  $configs['rules_autotag_tag'] = rules_import('
    { "rules_autotag_tag" : {
      "LABEL" : "Autotag to terms list",
      "PLUGIN" : "action set",
      "REQUIRES" : [ "rules_autotag", "rules" ],
      "USES VARIABLES" : {
        "text" : { "type" : "text", "label" : "The text for extracting terms" },
        "vocabulary" : {
          "type" : "taxonomy_vocabulary",
          "label" : "The vocabulary used for extraction"
        },
        "target_terms" : { "type" : "list\u003ctaxonomy_term\u003e", "label" : "Target list for storing extracted terms", "save" : true }
      },
      "ACTION SET" : [
        { "rules_autotag_extract" : {
            "USING" : { "text" : [ "text" ], "vocabulary" : [ "vocabulary" ] },
            "PROVIDE" : { "extraced_terms" : { "extraced_terms" : "Extracted terms" } }
          }
        },
        { "data_set" : { "data" : [ "target_terms" ], "value" : [ "extraced-terms" ] } }
      ]
    }}'
  );
  return $configs;
}
