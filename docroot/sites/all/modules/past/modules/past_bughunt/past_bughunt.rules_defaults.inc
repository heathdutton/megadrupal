<?php
/**
 * @file
 * Default rules configuration of the past_bughunt module.
 */

/**
 * Implements hook_default_rules_configuration().
 */
function past_bughunt_default_rules_configuration() {
  // Actionset component, marks events as done.
  $data = '{ "rules_mass_flag_as_done" : {
      "LABEL" : "Mark as done",
      "PLUGIN" : "action set",
      "REQUIRES" : [ "flag" ],
      "USES VARIABLES" : { "past_event" : { "label" : "Past Event", "type" : "past_event" } },
      "ACCESS_EXPOSED" : "1",
      "ACTION SET" : [
        { "flag_flagpast_event" : {
            "flag" : "event_done",
            "past_event" : [ "past_event" ],
            "flagging_user" : [ "site:current-user" ],
            "permission_check" : 0
          }
        }
      ]
    }
  }';
  $rule = rules_import($data);
  $configs[$rule->name] = $rule;
  // Actionset component, marks events as todo.
  $data = '{ "rules_mark_as_todo" : {
      "LABEL" : "Mark as TODO",
      "PLUGIN" : "action set",
      "REQUIRES" : [ "flag" ],
      "USES VARIABLES" : { "past_event" : { "label" : "Past Event", "type" : "past_event" } },
      "ACCESS_EXPOSED" : "1",
      "ACTION SET" : [
        { "flag_unflagpast_event" : {
            "flag" : "event_done",
            "past_event" : [ "past_event" ],
            "flagging_user" : [ "site:current-user" ],
            "permission_check" : 0
          }
        }
      ]
    }
  }';
  $rule = rules_import($data);
  $configs[$rule->name] = $rule;
  // Reaction rule that marks events as todo if severity isn't debug or info.
  $data = '{ "rules_mark_event_on_save_as_todo" : {
      "LABEL" : "Mark event on save as todo",
      "PLUGIN" : "reaction rule",
      "REQUIRES" : [ "rules", "flag", "past_db" ],
      "ON" : [ "past_event_insert" ],
      "IF" : [
        { "data_is" : {
            "data" : [ "past-event:severity" ],
            "op" : "IN",
            "value" : { "value" : { "5" : "5", "4" : "4", "3" : "3", "0" : "0", "1" : "1", "2" : "2" } }
          }
        }
      ],
      "DO" : [
        { "flag_flagpast_event" : {
            "flag" : "event_todo",
            "past_event" : [ "past_event" ],
            "flagging_user" : [ "site:current-user" ],
            "permission_check" : 1
          }
        }
      ]
    }
  }';
  $rule = rules_import($data);
  $configs[$rule->name] = $rule;
  return $configs;
}
