<?php
/**
 * @file
 * mandrill_inbound_subscriptions.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function mandrill_inbound_subscriptions_default_rules_configuration() {
  $items = array();
  $items['mandrill_inbound_subscriptions_problems_with_replying'] = entity_import('rules_config', '{ "mandrill_inbound_subscriptions_problems_with_replying" : {
      "LABEL" : "Mandrill Inbound Subscriptions: Problems with replying",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Inbound", "Mandrill" ],
      "REQUIRES" : [
        "rules",
        "rb_misc",
        "mandrill_inbound_subscriptions",
        "mandrill_inbound"
      ],
      "ON" : { "mandrill_webhook_inbound" : [] },
      "IF" : [
        { "text_matches" : { "text" : [ "type" ], "match" : "subscriptions_mail" } },
        { "OR" : [
            { "user_is_blocked" : { "account" : [ "from-account" ] } },
            { "NOT rb_misc_condition_user_has_permission" : {
                "account" : [ "from-account" ],
                "permissions" : { "value" : { "post comments" : "post comments" } }
              }
            },
            { "NOT mandrill_inbound_subscriptions_able_to_comment" : { "nid" : [ "id" ] } }
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
  $items['mandrill_inbound_subscriptions_reply_to_subscriptions'] = entity_import('rules_config', '{ "mandrill_inbound_subscriptions_reply_to_subscriptions" : {
      "LABEL" : "Mandrill Inbound Subscriptions: Reply to Subscriptions",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Inbound", "Mandrill" ],
      "REQUIRES" : [
        "rules",
        "rb_misc",
        "mandrill_inbound_subscriptions",
        "mandrill_inbound"
      ],
      "ON" : { "mandrill_webhook_inbound" : [] },
      "IF" : [
        { "text_matches" : { "text" : [ "type" ], "match" : "subscriptions_mail" } },
        { "NOT user_is_blocked" : { "account" : [ "from-account" ] } },
        { "rb_misc_condition_user_has_permission" : {
            "account" : [ "from-account" ],
            "permissions" : { "value" : { "post comments" : "post comments" } }
          }
        },
        { "mandrill_inbound_subscriptions_able_to_comment" : { "nid" : [ "id" ] } }
      ],
      "DO" : [
        { "entity_fetch" : {
            "USING" : { "type" : "node", "id" : [ "id" ] },
            "PROVIDE" : { "entity_fetched" : { "entity_fetched" : "Fetched entity" } }
          }
        },
        { "entity_create" : {
            "USING" : {
              "type" : "comment",
              "param_subject" : [ "subject" ],
              "param_node" : [ "entity-fetched" ],
              "param_author" : [ "from-account" ],
              "param_comment_body" : [ "text" ]
            },
            "PROVIDE" : { "entity_created" : { "entity_created" : "Created entity" } }
          }
        }
      ]
    }
  }');
  return $items;
}
