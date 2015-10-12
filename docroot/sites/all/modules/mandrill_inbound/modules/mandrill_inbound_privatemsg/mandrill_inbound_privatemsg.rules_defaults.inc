<?php
/**
 * @file
 * mandrill_inbound_privatemsg.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function mandrill_inbound_privatemsg_default_rules_configuration() {
  $items = array();
  $items['mandrill_inbound_privatemsg_problems_with_replying'] = entity_import('rules_config', '{ "mandrill_inbound_privatemsg_problems_with_replying" : {
      "LABEL" : "Mandrill Inbound Privatemsg: Problems with replying",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Inbound", "Mandrill" ],
      "REQUIRES" : [ "rules", "rb_misc", "mandrill_inbound_privatemsg", "mandrill_inbound" ],
      "ON" : { "mandrill_webhook_inbound" : [] },
      "IF" : [
        { "text_matches" : { "text" : [ "type" ], "match" : "pm_email_notify" } },
        { "OR" : [
            { "user_is_blocked" : { "account" : [ "from-account" ] } },
            { "NOT rb_misc_condition_user_has_permission" : {
                "account" : [ "from-account" ],
                "permissions" : { "value" : { "write privatemsg" : "write privatemsg" } }
              }
            },
            { "NOT mandrill_inbound_privatemsg_user_is_involved" : { "thread_id" : [ "id" ], "account" : [ "from-account" ] } }
          ]
        }
      ],
      "DO" : [
        { "mail" : {
            "to" : "[from-email:value]",
            "subject" : "There\\u0027s some problem with your request.",
            "message" : "Hello! We received your email but it we weren\\u0027t able to process it. Please check that you\\u0027re sending from the same address you use on [site:name].",
            "language" : [ "" ]
          }
        }
      ]
    }
  }');
  $items['mandrill_inbound_privatemsg_reply_to_privatemsg'] = entity_import('rules_config', '{ "mandrill_inbound_privatemsg_reply_to_privatemsg" : {
      "LABEL" : "Mandrill Inbound Privatemsg: Reply to privatemsg",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Inbound", "Mandrill" ],
      "REQUIRES" : [
        "rules",
        "rb_misc",
        "mandrill_inbound_privatemsg",
        "privatemsg_rules",
        "mandrill_inbound"
      ],
      "ON" : { "mandrill_webhook_inbound" : [] },
      "IF" : [
        { "text_matches" : { "text" : [ "type" ], "match" : "pm_email_notify" } },
        { "NOT user_is_blocked" : { "account" : [ "from-account" ] } },
        { "rb_misc_condition_user_has_permission" : {
            "account" : [ "from-account" ],
            "permissions" : { "value" : { "write privatemsg" : "write privatemsg" } }
          }
        },
        { "mandrill_inbound_privatemsg_user_is_involved" : { "thread_id" : [ "id" ], "account" : [ "from-account" ] } }
      ],
      "DO" : [
        { "privatemsg_rules_reply" : {
            "thread_id" : [ "id" ],
            "author" : [ "from-account" ],
            "body" : [ "text:value" ]
          }
        }
      ]
    }
  }');
  return $items;
}
