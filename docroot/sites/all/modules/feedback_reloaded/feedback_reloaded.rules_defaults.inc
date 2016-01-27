<?php
/**
  * Implements hook_default_rules_configuration().
  */
function feedback_reloaded_default_rules_configuration() {
  $rules['rules_send_feedback_notification'] = entity_import('rules_config', '{ "rules_send_feedback_notification" : {
    "LABEL" : "Send Feedback Notification",
    "PLUGIN" : "reaction rule",
    "REQUIRES" : [ "rules", "feedback_reloaded" ],
    "ON" : [ "feedback_reloaded_insert" ],
    "IF" : [ { "entity_is_new" : { "entity" : [ "feedback-reloaded" ] } } ],
    "DO" : [
      { "mail" : {
          "to" : "[site:mail]",
          "subject" : "New feedback - [feedback-reloaded:title]",
          "message" : "Hi,\r\n\r\nA new feedback is submitted by user[ID] - [feedback-reloaded:uid] on you website at [feedback-reloaded:url].\r\nTitle - [feedback-reloaded:title]\r\nMessage:\r\n[feedback-reloaded:message]\r\nScreenshot URL : [feedback-reloaded:screenshoturl]\r\nUseragent : [feedback-reloaded:useragent]",
          "from" : "[site:current-user]",
          "language" : [ "feedback-reloaded:type" ]
        }
      }
    ]
  }
}');

  return $rules;
}
