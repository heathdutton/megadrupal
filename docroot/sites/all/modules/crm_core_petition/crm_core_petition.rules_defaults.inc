<?php
/**
 * @file
 * crm_core_petition.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function crm_core_petition_default_rules_configuration() {
  $items = array();
  $items['rules_cmcp_target_email'] = entity_import('rules_config', '{ "rules_cmcp_target_email" : {
      "LABEL" : "CRM Core Petition: Target Email",
      "PLUGIN" : "rule set",
      "OWNER" : "rules",
      "TAGS" : [ "CRM Core Petition Sign Email" ],
      "REQUIRES" : [ "rules", "mimemail" ],
      "USES VARIABLES" : {
        "email_to" : { "label" : "Email to", "type" : "text" },
        "target" : { "label" : "Target", "type" : "crm_core_contact" },
        "signatory" : { "label" : "Signatory", "type" : "crm_core_contact" },
        "petition" : { "label" : "Petition", "type" : "node" },
        "signature" : { "label" : "Signature", "type" : "crm_core_activity" },
        "sign" : { "label" : "Sign", "type" : "text" },
        "message" : { "label" : "Message", "type" : "text" }
      },
      "RULES" : [
        { "RULE" : {
            "DO" : [
              { "mimemail" : {
                  "key" : "rules_cmcp_target_email",
                  "to" : "[email-to:value]",
                  "from_name" : "[signatory:name-contact-name]",
                  "from_mail" : "[signatory:primary-email]",
                  "subject" : "[petition:field_cmcp_target_email_subject]",
                  "body" : "\\u003Cstrong\\u003EDear [target:contact-name:given] [target:contact-name:family],\\u003Cstrong\\u003E\\r\\n[petition:field_cmcp_cta]\\r\\n[message:value]\\r\\nSincerely,\\r\\n[sign:value]",
                  "plaintext" : "Dear [target:contact-name:given] [target:contact-name:family],\\r\\n[petition:field_cmcp_cta:value]\\r\\n[message:value]\\r\\nSincerely,\\r\\n[sign:value]",
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
  $items['rules_cmcp_thank_you_message'] = entity_import('rules_config', '{ "rules_cmcp_thank_you_message" : {
      "LABEL" : "CRM Core Petition Thank You Message",
      "PLUGIN" : "rule set",
      "OWNER" : "rules",
      "TAGS" : [ "CRM Core Petition", "CRM Core Petition Thank You Message" ],
      "REQUIRES" : [ "rules", "mimemail" ],
      "USES VARIABLES" : {
        "email_to" : { "label" : "Email To", "type" : "text" },
        "signatory" : { "label" : "Signatory", "type" : "crm_core_contact" },
        "petition" : { "label" : "Petition", "type" : "node" }
      },
      "RULES" : [
        { "RULE" : {
            "IF" : [
              { "entity_is_of_type" : { "entity" : [ "signatory" ], "type" : "crm_core_contact" } },
              { "entity_has_field" : { "entity" : [ "signatory" ], "field" : "contact_name" } },
              { "NOT data_is_empty" : { "data" : [ "email-to" ] } }
            ],
            "DO" : [
              { "mimemail" : {
                  "key" : "rules_cmcp_thank_you_message",
                  "to" : "[email-to:value]",
                  "subject" : "Thank you for your signature",
                  "body" : "\\u003Cstrong\\u003EDear [signatory:contact-name:given],\\u003C\\/strong\\u003E\\r\\n\\r\\nThank you for signing our petition \\u0022[petition:title].\\u0022 Your support is appreciated!\\r\\n\\r\\nCan you help this petition succeed by asking your friends to sign too? There\\u0027s a sample message contained below that you can forward onto your personal networks.\\r\\n\\r\\n------- FORWARD THIS TO YOUR FRIENDS -------\\r\\nHi,\\r\\n\\r\\nI just signed the petition \\u0022[petition:title].\\u0022\\r\\n\\r\\nIt\\u0027s an important cause, and I hope you could sign too. There\\u0027s no cost, and it will only take a few moments of your time.\\r\\n\\r\\nHere\\u0027s the link:\\r\\n\\r\\n[petition:url]\\r\\n\\r\\nThanks!\\r\\n[signatory:contact-name:given]\\r\\n\\r\\n------------------------------\\r\\n\\r\\nThank you again, \\r\\nYour friends at \\u003Ca href=\\u0022[site:url]\\u0022\\u003E[site:name]\\u003C\\/a\\u003E",
                  "plaintext" : "Dear [signatory:contact-name:given],\\r\\n\\r\\nThank you for signing our petition \\u0022[petition:title].\\u0022 Your support is appreciated!\\r\\n\\r\\nCan you help this petition succeed by asking your friends to sign too? There\\u0027s a sample message contained below that you can forward onto your personal networks.\\r\\n\\r\\n------- FORWARD THIS TO YOUR FRIENDS -------\\r\\nHi,\\r\\n\\r\\nI just signed the petition \\u0022[petition:title].\\u0022\\r\\n\\r\\nIt\\u0027s an important cause, and I hope you could sign too. There\\u0027s no cost, and it will only take a few moments of your time.\\r\\n\\r\\nHere\\u0027s the link:\\r\\n\\r\\n[petition:url]\\r\\n\\r\\nThanks!\\r\\n[signatory:contact-name:given]\\r\\n\\r\\n------------------------------\\r\\n\\r\\nThank you again, \\r\\nYour friends at [site:name] - [site:url]",
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
