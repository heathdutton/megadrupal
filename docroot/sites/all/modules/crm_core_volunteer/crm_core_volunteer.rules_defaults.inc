<?php
/**
 * @file
 * crm_core_volunteer.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function crm_core_volunteer_default_rules_configuration() {
  $items = array();
  $items['rules_cmcv_cancel_registration'] = entity_import('rules_config', '{ "rules_cmcv_cancel_registration" : {
      "LABEL" : "Cancel Registration",
      "PLUGIN" : "rule",
      "OWNER" : "rules",
      "TAGS" : [ "CRM Core", "CRM Core Volunteer" ],
      "REQUIRES" : [ "rules" ],
      "USES VARIABLES" : { "volunteer_commitment" : { "label" : "Volunteer Commitment", "type" : "crm_core_activity" } },
      "IF" : [
        { "entity_is_of_bundle" : {
            "entity" : [ "volunteer-commitment" ],
            "type" : "crm_core_activity",
            "bundle" : { "value" : { "cmcv_volunteer_commitment" : "cmcv_volunteer_commitment" } }
          }
        },
        { "entity_has_field" : {
            "entity" : [ "volunteer-commitment" ],
            "field" : "field_cmcv_signup_status"
          }
        }
      ],
      "DO" : [
        { "data_set" : {
            "data" : [ "volunteer-commitment:field-cmcv-signup-status" ],
            "value" : "canceled"
          }
        }
      ]
    }
  }');
  $items['rules_cmcv_opportunity_signup'] = entity_import('rules_config', '{ "rules_cmcv_opportunity_signup" : {
      "LABEL" : "Volunteer opportunity signup",
      "PLUGIN" : "rule set",
      "OWNER" : "rules",
      "TAGS" : [ "CRM Core", "CRM Core Volunteer", "CRM Core Volunteer Message" ],
      "REQUIRES" : [ "rules", "mimemail" ],
      "USES VARIABLES" : {
        "email_to" : { "label" : "Email to", "type" : "text" },
        "signatory" : { "label" : "Signatory", "type" : "crm_core_contact" },
        "signature" : { "label" : "Signature", "type" : "crm_core_activity" },
        "opportunity" : { "label" : "Opportunity", "type" : "node" }
      },
      "RULES" : [
        { "RULE" : {
            "IF" : [
              { "NOT data_is_empty" : { "data" : [ "email-to" ] } },
              { "NOT data_is_empty" : { "data" : [ "signatory" ] } }
            ],
            "DO" : [
              { "mimemail" : {
                  "key" : "rules_cmcv_opportunity_signup",
                  "to" : [ "email-to" ],
                  "subject" : "Thank you for signing up to volunteer",
                  "body" : "Dear [signatory:contact-name:given],\\r\\n\\r\\nThank you for signing up for \\u0022[opportunity:title]\\u0022! We value your commitment and appreciate you for volunteering to help out.\\r\\n\\r\\nHere\\u0027s some information about to get involved, for your records. Please remember, if you need to check in on the details, you can always find them online at [opportunity:url].\\r\\n\\r\\n\\u003Cstrong\\u003ELocation:\\u003C\\/strong\\u003E\\r\\n[opportunity:field-cmcv-location:thoroughfare]\\r\\n[opportunity:field-cmcv-location:premise]\\r\\n[opportunity:field-cmcv-location:locality], [opportunity:field-cmcv-location:administrative_area] [opportunity:field-cmcv-location:postal_code]\\r\\n\\r\\n\\u003Cstrong\\u003EDate and Time:\\u003C\\/strong\\u003E\\r\\n[opportunity:field-cmcv-date:value]\\r\\n[opportunity:field-cmcv-date:value2]\\r\\n\\r\\n\\u003Cstrong\\u003EOther Notes:\\u003C\\/strong\\u003E\\r\\n\\r\\n[opportunity:body]\\r\\n\\r\\nYou can view this volunteer opportunity online at [opportunity:url].\\r\\n\\r\\nThank you again, \\r\\nYour friends at \\u003Ca href=\\u0022[site:url]\\u0022\\u003E[site:name]\\u003C\\/a\\u003E\\r\\n",
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
  $items['rules_cmcv_wl_notify'] = entity_import('rules_config', '{ "rules_cmcv_wl_notify" : {
      "LABEL" : "Volunteer opportunity waiting list",
      "PLUGIN" : "rule set",
      "OWNER" : "rules",
      "TAGS" : [ "CRM Core", "CRM Core Volunteer", "CRM Core Volunteer Wait Email" ],
      "REQUIRES" : [ "rules", "mimemail" ],
      "USES VARIABLES" : {
        "email_to" : { "label" : "Email to", "type" : "text" },
        "signatory" : { "label" : "Signatory", "type" : "crm_core_contact" },
        "signature" : { "label" : "Signature", "type" : "crm_core_activity" },
        "opportunity" : { "label" : "Opportunity", "type" : "node" }
      },
      "RULES" : [
        { "RULE" : {
            "IF" : [
              { "NOT data_is_empty" : { "data" : [ "email-to" ] } },
              { "NOT data_is_empty" : { "data" : [ "signatory" ] } }
            ],
            "DO" : [
              { "mimemail" : {
                  "key" : "rules_cmcv_wl_notify",
                  "to" : [ "email-to" ],
                  "subject" : "Thanks for signing to volunteer opportunity!",
                  "body" : "Dear [signatory:contact-name:given] [signatory:contact-name:family],\\r\\n\\r\\nThank you for signing to \\u0022[opportunity:title]\\u0022!\\r\\nUnfortunately all the empty slots are ended. You will be notified when slot would be available for you.",
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
  $items['rules_rules_cmcv_send_email'] = entity_import('rules_config', '{ "rules_rules_cmcv_send_email" : {
      "LABEL" : "Send Email",
      "PLUGIN" : "rule",
      "OWNER" : "rules",
      "TAGS" : [ "CRM Core", "CRM Core Volunteer" ],
      "REQUIRES" : [ "rules" ],
      "USES VARIABLES" : {
        "volunteer_commitment" : { "label" : "Volunteer Commitment", "type" : "crm_core_activity" },
        "subject" : { "label" : "Subject", "type" : "text" },
        "message" : { "label" : "Message", "type" : "text" }
      },
      "IF" : [
        { "entity_is_of_bundle" : {
            "entity" : [ "volunteer-commitment" ],
            "type" : "crm_core_activity",
            "bundle" : { "value" : { "cmcv_volunteer_commitment" : "cmcv_volunteer_commitment" } }
          }
        },
        { "entity_has_field" : {
            "entity" : [ "volunteer-commitment" ],
            "field" : "field_activity_participants"
          }
        }
      ],
      "DO" : [
        { "mail" : {
            "to" : [ "volunteer-commitment:field-activity-participants:0:primary-email" ],
            "subject" : [ "subject" ],
            "message" : [ "message" ],
            "from" : [ "site:mail" ],
            "language" : [ "volunteer-commitment:type" ]
          }
        }
      ]
    }
  }');
  return $items;
}
