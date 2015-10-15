<?php
/**
 * @file
 * computing.rules_defaults.inc
 */

/**
 * Implementation of hook_default_rules_configuration().
 */
function computing_default_rules_configuration() {
  $items = array();
  $items['rules_computing_handle_agent_output'] = entity_import('rules_config', '{ "rules_computing_handle_agent_output" : {
    "LABEL" : "Handle agent output",
    "PLUGIN" : "reaction rule",
    "OWNER" : "rules",
    "TAGS" : [ "computing" ],
    "REQUIRES" : [ "computing" ],
    "ON" : { "computing_event_finished" : [] },
    "DO" : [
      { "computing_action_handle_output" : { "computing_record" : [ "computing_record" ] } }
    ]
  }
}');
  return $items;
}
