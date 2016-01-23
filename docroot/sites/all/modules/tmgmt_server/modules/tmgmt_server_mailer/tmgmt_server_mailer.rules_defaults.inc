<?php

/*
 * @file
 * Contains default rules.
 */

/**
 * Implements hook_default_rules_configuration().
 */
function tmgmt_server_mailer_default_rules_configuration() {
  $data = '{ "rules_rules_tmgmt_server_mailer_new_job_notification" : {
      "LABEL" : "Send new translation job notification",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "TMGMT", "TMGMT Server" ],
      "REQUIRES" : [ "tmgmt_server_mailer", "tmgmt_server" ],
      "ON" : { "tmgmt_server_new_job_request" : [] },
      "DO" : [
        { "tmgmt_server_mailer_rules_new_job_notify" : {
            "tmgmt_job" : [ "tmgmt_job" ],
            "tmgmt_server_client" : [ "tmgmt_server_client" ]
          }
        }
      ]
    }
  }';
  $rule = rules_import($data);
  $configs[$rule->name] = $rule;

  return $configs;
}
