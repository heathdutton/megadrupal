<?php

/**
 * @file
 * signup_share_track.rules_defaults.inc
 */

/**
 * Implementation of hook_default_rules_configuration().
 */
function signup_share_track_default_rules_configuration() {
    $items = array();
    $items['rules_signup_share_track_rule_for_no_of_users_1'] = entity_import('rules_config', '{ "rules_signup_share_track_rule_for_no_of_users_1" : {
    "LABEL" : "signup_share_track_rule_for_no_of_users_1",
    "PLUGIN" : "reaction rule",
    "REQUIRES" : [ "signup_share_track" ],
    "ON" : [ "signup_share_track_user_created_event" ],
    "IF" : [ { "signup_share_track_total_users_1" : { "user" : [ "user" ] } } ],
    "DO" : [ { "signup_share_track_action_for_sending_mail_1" : [] } ]
  }
}');
    $items['rules_signup_share_track_rule_for_no_of_users_3'] = entity_import('rules_config', '{ "rules_signup_share_track_rule_for_no_of_users_3" : {
    "LABEL" : "signup_share_track_rule_for_no_of_users_3",
    "PLUGIN" : "reaction rule",
    "REQUIRES" : [ "signup_share_track" ],
    "ON" : [ "signup_share_track_user_created_event" ],
    "IF" : [ { "signup_share_track_total_users_3" : [] } ],
    "DO" : [
      { "signup_share_track_action_for_sending_mail_3" : { "user" : [ "user" ] } },
      { "signup_share_track_action_for_changing_role" : { "user" : [ "user" ] } }
    ]
  }
}');
    return $items;
}