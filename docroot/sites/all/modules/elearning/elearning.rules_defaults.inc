<?php
/**
 * @file
 * Default rules Action set configurations.
 *
 * This file is automatically picked up by the Rules module but can't go in a
 * subfolder unfortunately.
 */

/**
 * Implements hook_default_rules_configuration().
 */
function elearning_default_rules_configuration() {
  $items = array();
  $items['elearning_grant_access_to_course'] = entity_import('rules_config', '{ "elearning_grant_access_to_course" : {
    "LABEL" : "Grant access to course",
    "PLUGIN" : "action set",
    "OWNER" : "rules",
    "TAGS" : [ "E-learning" ],
    "REQUIRES" : [ "elearning" ],
    "USES VARIABLES" : {
      "elearning_course_access" : { "label" : "Course Access", "type" : "elearning_course_access" }
    },
    "ACTION SET" : [
    { "elearning_action_course_grant_access" : { "elearning_course_access" : [ "elearning_course_access" ] } }
    ]
  }
}');
  $items['elearning_deny_access_to_course'] = entity_import('rules_config', '{ "elearning_deny_access_to_course" : {
    "LABEL" : "Deny access to course",
    "PLUGIN" : "action set",
    "OWNER" : "rules",
    "TAGS" : [ "E-learning" ],
    "REQUIRES" : [ "elearning" ],
    "USES VARIABLES" : {
        "elearning_course_access" : { "label" : "Course Access", "type" : "elearning_course_access" }
    },
    "ACTION SET" : [
    { "elearning_action_course_deny_access" : { "elearning_course_access" : [ "elearning_course_access" ] } }
    ]
  }
}');
  return $items;
}
