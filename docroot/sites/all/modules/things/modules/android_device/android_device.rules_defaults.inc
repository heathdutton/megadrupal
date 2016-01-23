<?php
/**
 * @file
 * android_device.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function android_device_default_rules_configuration() {
  $items = array();
  $items['rules_send_privatemsg_to_mobile_user'] = entity_import('rules_config', '{ "rules_send_privatemsg_to_mobile_user" : {
      "LABEL" : "Send privatemsg to mobile user",
      "PLUGIN" : "reaction rule",
      "TAGS" : [ "Mobile4Social Privatemsg" ],
      "REQUIRES" : [ "rules", "views_rules", "devel", "gcm", "privatemsg_rules" ],
      "ON" : [ "privatemsg_insert" ],
      "IF" : [
        { "user_has_role" : { "account" : [ "recipient" ], "roles" : { "value" : { "3" : "3" } } } }
      ],
      "DO" : [
        { "VIEW LOOP" : {
            "VIEW" : "gcm_identifier_by_user",
            "DISPLAY" : "views_rules_1",
            "USING" : { "field_owner_target_id" : [ "recipient:uid" ] },
            "ROW VARIABLES" : { "token" : { "token" : "(GCM id for thing) Things Identifiers: Token" } },
            "DO" : [
              { "devel_debug" : { "value" : [ "token" ] } },
              { "gcm_action_send_message" : {
                  "tokens" : [ "token" ],
                  "keyValue" : "type=privatemsg,action=create,nid=[privatemsg-message:mid],url=[privatemsg-message:url],title=[privatemsg-message:subject],desc=From [privatemsg-message:author]",
                  "delay_while_idle" : 0
                }
              }
            ]
          }
        },
        { "devel_debug" : { "value" : [ "privatemsg-message" ] } }
      ]
    }
  }');
  $items['rules_when_thing_with_gcm_identifier_is_saved'] = entity_import('rules_config', '{ "rules_when_thing_with_gcm_identifier_is_saved" : {
      "LABEL" : "When thing with GCM identifier is saved",
      "PLUGIN" : "reaction rule",
      "TAGS" : [ "Mobile4Social Privatemsg" ],
      "REQUIRES" : [ "rules", "privatemsg_rules", "gcm", "things" ],
      "ON" : [ "things_update" ],
      "IF" : [
        { "entity_has_field" : { "entity" : [ "things" ], "field" : "field_gcm_id" } },
        { "entity_has_field" : { "entity" : [ "things" ], "field" : "field_owner" } },
        { "NOT data_is_empty" : { "data" : [ "things:field-gcm-id" ] } },
        { "NOT data_is_empty" : { "data" : [ "things-unchanged:things-id" ] } },
        { "data_is_empty" : { "data" : [ "things-unchanged:field-gcm-id:things-ids-id" ] } }
      ],
      "DO" : [
        { "entity_fetch" : {
            "USING" : { "type" : "user", "id" : [ "things:field-owner:uid" ] },
            "PROVIDE" : { "entity_fetched" : { "thing_owner" : "Thing owner" } }
          }
        },
        { "entity_fetch" : {
            "USING" : { "type" : "user", "id" : "1" },
            "PROVIDE" : { "entity_fetched" : { "admin" : "Site admin" } }
          }
        },
        { "privatemsg_rules_new" : {
            "recipient" : [ "thing-owner" ],
            "author" : [ "admin" ],
            "subject" : "Welcome to [site:name]",
            "body" : "Welcome to [site:name], and thanks for using Mobile4Social Privatemsg Android application.\\u000D\\u000APlease feel free to use it to interact with mobile4Social users and any other user of the site, and to ask anything related to the site and its concept.\\u000D\\u000ARegards,\\u000D\\u000AMobile4Social.com"
          }
        },
        { "entity_fetch" : {
            "USING" : { "type" : "things_ids", "id" : [ "things:field-gcm-id:things-ids-id" ] },
            "PROVIDE" : { "entity_fetched" : { "gcm_id" : "GCM identifier" } }
          }
        },
        { "gcm_action_send_message" : {
            "tokens" : "[gcm-id:token]",
            "keyValue" : "type=user,action=index",
            "delay_while_idle" : 0,
            "time_to_live" : "3600",
            "collapse_key" : "user_index"
          }
        }
      ]
    }
  }');
  return $items;
}
