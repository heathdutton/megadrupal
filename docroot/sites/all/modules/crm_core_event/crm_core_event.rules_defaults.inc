<?php
/**
 * @file
 * crm_core_event.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function crm_core_event_default_rules_configuration() {
  $items = array();
  $items['rules_cmcev_cancel_registration'] = entity_import('rules_config', '{ "rules_cmcev_cancel_registration" : {
      "LABEL" : "CRM Core Event: Cancel registration",
      "PLUGIN" : "rule",
      "OWNER" : "rules",
      "TAGS" : [ "Bulk Operations", "CRM Core", "CRM Core Event" ],
      "REQUIRES" : [ "rules" ],
      "USES VARIABLES" : { "registration" : { "label" : "Event registration", "type" : "crm_core_activity" } },
      "IF" : [
        { "entity_is_of_bundle" : {
            "entity" : [ "registration" ],
            "type" : "crm_core_activity",
            "bundle" : { "value" : { "cmcev_event_registration" : "cmcev_event_registration" } }
          }
        },
        { "entity_has_field" : { "entity" : [ "registration" ], "field" : "field_cmcev_status" } }
      ],
      "DO" : [
        { "data_set" : { "data" : [ "registration:field-cmcev-status" ], "value" : "Cancelled" } }
      ]
    }
  }');
  $items['rules_cmcev_send_email'] = entity_import('rules_config', '{ "rules_cmcev_send_email" : {
      "LABEL" : "CRM Core Event: Send email",
      "PLUGIN" : "rule",
      "OWNER" : "rules",
      "TAGS" : [ "Bulk Operations", "CRM Core", "CRM Core Event" ],
      "REQUIRES" : [ "rules" ],
      "USES VARIABLES" : {
        "registration" : { "label" : "Registration", "type" : "crm_core_activity" },
        "subject" : { "label" : "Subject", "type" : "text" },
        "message" : { "label" : "Message", "type" : "text" }
      },
      "IF" : [
        { "entity_is_of_bundle" : {
            "entity" : [ "registration" ],
            "type" : "crm_core_activity",
            "bundle" : { "value" : { "cmcev_event_registration" : "cmcev_event_registration" } }
          }
        },
        { "entity_has_field" : { "entity" : [ "registration" ], "field" : "field_activity_participants" } },
        { "NOT data_is_empty" : { "data" : [ "registration:field-activity-participants:0:primary-email" ] } }
      ],
      "DO" : [
        { "mail" : {
            "to" : [ "registration:field-activity-participants:0:primary-email" ],
            "subject" : [ "subject" ],
            "message" : [ "message" ],
            "from" : [ "site:mail" ],
            "language" : [ "" ]
          }
        }
      ]
    }
  }');
  $items['rules_cmcev_thank_you_message'] = entity_import('rules_config', '{ "rules_cmcev_thank_you_message" : {
      "LABEL" : "CRM Core Event Registration Thank You Message",
      "PLUGIN" : "rule set",
      "OWNER" : "rules",
      "TAGS" : [ "CRM Core Event Registration Confirmation Message" ],
      "REQUIRES" : [ "rules", "mimemail" ],
      "USES VARIABLES" : {
        "email_to" : { "label" : "Email to", "type" : "text" },
        "registrant" : { "label" : "Registrant", "type" : "crm_core_contact" },
        "event" : { "label" : "Event", "type" : "node" },
        "amount" : { "label" : "Amount", "type" : "text" }
      },
      "RULES" : [
        { "RULE" : {
            "DO" : [
              { "mimemail" : {
                  "key" : "rules_cmcev_thank_you_message",
                  "to" : "[email-to:value]",
                  "subject" : "Registration on \\u0022[event:title]\\u0022 confirmation",
                  "body" : "\\u003Cstrong\\u003EDear [registrant:contact-name:given],\\u003C\\/strong\\u003E\\r\\n\\r\\nThank you for registering for the event \\u0022[event:title]\\u0022. We are looking for forward to having you join us!\\r\\n\\r\\n\\u003Cstrong\\u003EDate:\\u003C\\/strong\\u003E [event:field-cmcev-date:value] - [event:field-cmcev-date:value2]\\r\\n\\r\\n\\u003Cstrong\\u003ELocation:\\u003C\\/strong\\u003E [event:field-cmcev-location:country] [event:field-cmcev-location:postal_code] [event:field-cmcev-location:administrative_area] [event:field-cmcev-location:sub_administrative_area] [event:field-cmcev-location:locality] [event:field-cmcev-location:dependent_locality] [event:field-cmcev-location:thoroughfare] [event:field-cmcev-location:premise]\\r\\n\\r\\nWhy don\\u0027t you ask your friends to join you? There\\u0027s a sample message included below that you can forward to your personal network:\\r\\n\\r\\n------- FORWARD THIS TO YOUR FRIENDS -------\\r\\nHi,\\r\\n\\r\\nI just registered to attend the event \\u0022[event:title].\\u0022\\r\\n\\r\\nPlease consider joining me! It only takes a few moments to register online. \\r\\n\\r\\nHere\\u0027s the link:\\r\\n\\r\\n[event:url]\\r\\n\\r\\nThanks!\\r\\n[registrant:contact-name:given]\\r\\n\\r\\n------------------------------\\r\\n\\r\\nThank you again, \\r\\nYour friends at \\u003Ca href=\\u0022[site:url]\\u0022\\u003E[site:name]\\u003C\\/a\\u003E",
                  "plaintext" : "Dear [registrant:contact-name:given],\\r\\n\\r\\nThank you for registering for the event \\u0022[event:title]\\u0022. We are looking for forward to having you join us!\\r\\n\\r\\nDate: [event:field-cmcev-date:value] - [event:field-cmcev-date:value2]\\r\\n\\r\\nLocation: [event:field-cmcev-location:country] [event:field-cmcev-location:postal_code] [event:field-cmcev-location:administrative_area] [event:field-cmcev-location:sub_administrative_area] [event:field-cmcev-location:locality] [event:field-cmcev-location:dependent_locality] [event:field-cmcev-location:thoroughfare] [event:field-cmcev-location:premise]\\r\\n\\r\\nWhy don\\u0027t you ask your friends to join you? There\\u0027s a sample message included below that you can forward to your personal network:\\r\\n\\r\\n------- FORWARD THIS TO YOUR FRIENDS -------\\r\\nHi,\\r\\n\\r\\nI just registered to attend the event \\u0022[event:title].\\u0022\\r\\n\\r\\nPlease consider joining me! It only takes a few moments to register online. \\r\\n\\r\\nHere\\u0027s the link:\\r\\n\\r\\n[event:url]\\r\\n\\r\\nThanks!\\r\\n[registrant:contact-name:given]\\r\\n\\r\\n------------------------------\\r\\n\\r\\nThank you again, \\r\\nYour friends at [site:name] [site:url]",
                  "language" : [ "" ]
                }
              }
            ],
            "LABEL" : "Send Email"
          }
        }
      ]
    }
  }');
  return $items;
}
