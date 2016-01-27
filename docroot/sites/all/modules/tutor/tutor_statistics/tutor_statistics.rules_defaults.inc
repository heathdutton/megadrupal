<?php
/**
 * @file
 * tutor_statistics.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function tutor_statistics_default_rules_configuration() {
  $items = array();
  $items['rules_tutor_statistics_streak_plus'] = entity_import('rules_config', '{ "rules_tutor_statistics_streak_plus" : {
      "LABEL" : "Increase streak on correct answer",
      "PLUGIN" : "reaction rule",
      "TAGS" : [ "Tutor" ],
      "REQUIRES" : [ "rules", "tutor_statistics", "tutor" ],
      "ON" : [ "tutor_correct" ],
      "IF" : [ { "entity_is_of_type" : { "entity" : [ "entity" ], "type" : "node" } } ],
      "DO" : [
        { "tutor_statistics_plus_one" : {
            "USING" : {
              "account" : [ "account" ],
              "question_node" : [ "entity" ],
              "question_name" : [ "question" ]
            },
            "PROVIDE" : { "stats" : { "stats" : "Statistics entity" } }
          }
        }
      ]
    }
  }');
  $items['rules_tutor_statistics_streak_reset'] = entity_import('rules_config', '{ "rules_tutor_statistics_streak_reset" : {
      "LABEL" : "Reset streak on incorrect answer",
      "PLUGIN" : "reaction rule",
      "TAGS" : [ "Tutor" ],
      "REQUIRES" : [ "rules", "tutor_statistics", "tutor" ],
      "ON" : [ "tutor_incorrect" ],
      "IF" : [ { "entity_is_of_type" : { "entity" : [ "entity" ], "type" : "node" } } ],
      "DO" : [
        { "tutor_statistics_reset" : {
            "USING" : {
              "account" : [ "account" ],
              "question_node" : [ "entity" ],
              "question_name" : [ "question" ]
            },
            "PROVIDE" : { "stats" : { "stats" : "Statistics entity" } }
          }
        }
      ]
    }
  }');
  return $items;
}
