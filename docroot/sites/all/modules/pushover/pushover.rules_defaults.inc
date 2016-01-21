<?php
/**
 * @file
 * Provides some default rules for example and convenience.
 */

/**
 * Implements hook_default_rules_configuration().
 */
function pushover_default_rules_configuration() {
  $rules_pushover_warning_notification = '{ "rules_pushover_warning_notification" : {
    "LABEL" : "Pushover Warning Notification",
    "PLUGIN" : "reaction rule",
    "TAGS" : [ "notification", "push", "watchdog" ],
    "REQUIRES" : [ "rules", "pushover" ],
    "ON" : [ "watchdog" ],
    "IF" : [ { "data_is" : { "data" : [ "log-entry:severity" ], "value" : "2" } } ],
    "DO" : [
      { "pushover_to_user" : {
          "user" : "",
          "subject" : "[site:name]: Critical error",
          "message" : [ "log-entry:message" ],
          "priority" : "1"
        }
      }
    ]
  }
}';
  $configs['rules_pushover_warning_notification'] = rules_import($rules_pushover_warning_notification);

  return $configs;
}
