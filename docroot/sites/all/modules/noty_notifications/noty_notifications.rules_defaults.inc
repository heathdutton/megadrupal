<?php

/**
 * @file
 * Defines default rules.
 */

/**
 * Implements hook_default_rules_configuration().
 */
function noty_notifications_default_rules_configuration() {
  $configs = array();
  foreach (_noty_notifications_default_rules_configuration() as $key => $default) {
    $configs[$key] = rules_import($default);
  }

  return $configs;
}

/**
 * Helper function to define exports from rules.
 * The defaults will be imported with rules_import command.
 */
function _noty_notifications_default_rules_configuration() {
  $configs = array();

  $configs['rules_noty_test'] = '{ "rules_noty_test" : {
    "LABEL" : "Noty notification if some user created new content",
    "PLUGIN" : "reaction rule",
    "TAGS" : [ "noty" ],
    "REQUIRES" : [ "rules", "noty_notifications" ],
    "ON" : [ "node_insert" ],
    "DO" : [
      { "entity_fetch" : {
          "USING" : { "type" : "user", "id" : [ "node:author:uid" ] },
          "PROVIDE" : { "entity_fetched" : { "creator" : "Creator" } }
        }
      },
      { "noty_notifications_add_notification_action" : {
          "account" : "1",
          "noty_message" : "User \u003Ca href=\u0022[creator:url]\u0022\u003E[creator:name]\u003C\/a\u003E create new article \u003Ca href=\u0022[node:url]\u0022\u003E[node:title]\u003C\/a\u003E.",
          "noty_type" : { "value" : { "information" : "information" } },
          "removable" : 1
        }
      }
    ]
  }
}';

  return $configs;
}
