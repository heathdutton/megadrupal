<?php
/**
 * @file
 * rules functions for the ls_answ module.
 */

/**
 * Implements hook_default_rules_configuration().
 *
 * Create a inactive Rule, permitting to limit 1 answer per connected user per
 * survey.
 */
function ls_answ_default_rules_configuration() {
  $configs = array();
  $rule1 = '{ "rules_limit_1_limesurvey_answer_per_connected_user" : {
    "LABEL" : "Limit 1 LimeSurvey answer per connected user",
    "PLUGIN" : "reaction rule",
    "ACTIVE" : false,
    "OWNER" : "rules",
    "REQUIRES" : [ "rules", "ls_answ" ],
    "ON" : { "ls_answ_ask_new_answer" : [] },
    "IF" : [
      { "NOT data_is_empty" : { "data" : [ "site:current-user:uid" ] } },
      { "ls_answ_condition_answer_exists" : { "node" : [ "node" ], "account" : [ "site:current-user" ] } }
    ],
    "DO" : [
      { "drupal_message" : { "message" : "You have already fill an answer" } },
      { "ls_answ_recent_answer_variable" : {
          "USING" : { "account" : [ "site:current-user" ], "node" : [ "node" ] },
          "PROVIDE" : { "most_recent_answer" : { "most_recent_answer" : "the most recent created answer for that author" } }
        }
      },
      { "redirect" : { "url" : "[most-recent-answer:url]" } }
    ]
  }
}';
  $configs['ls_answ_limit_1_answer_per_user'] = rules_import($rule1);

  $rule2 = '{ "rules_force_a_user_to_complete_its_answer" : {
    "LABEL" : "Force a user to complete its answer",
    "PLUGIN" : "reaction rule",
    "ACTIVE" : false,
    "OWNER" : "rules",
    "REQUIRES" : [ "ls_answ", "rules" ],
    "ON" : { "node_view" : [] },
    "IF" : [
      { "ls_answ_condition_node_is_answer" : { "node" : [ "node" ] } },
      { "NOT data_is_empty" : { "data" : [ "site:current-user:uid" ] } },
      { "data_is" : { "data" : [ "site:current-user:uid" ], "value" : [ "node:author:uid" ] } },
      { "NOT data_is" : { "data" : [ "node:ls-status" ], "value" : "1" } },
      { "data_is" : { "data" : [ "node:ls-sync" ], "op" : "\u003C", "value" : "4" } }
    ],
    "DO" : [
      { "ls_answ_create_invitation" : {
          "USING" : { "account" : [ "site:current-user" ], "node" : [ "node" ] },
          "PROVIDE" : { "answer_ls_url" : { "answer_ls_url" : "the url for filling the survey" } }
        }
      },
      { "redirect" : { "url" : "[answer-ls-url:value]" } }
    ]
  }
}';
  $configs['rules_force_a_user_to_complete_its_answer'] = rules_import($rule2);

  return $configs;
}
