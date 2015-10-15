<?php
/**
 * @file
 * crm_core_donation.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function crm_core_donation_default_rules_configuration() {
  $items = array();
  $items['rules_cmcd_thank_you_message'] = entity_import('rules_config', '{ "rules_cmcd_thank_you_message" : {
      "LABEL" : "Donation: Thank You Message",
      "PLUGIN" : "rule set",
      "OWNER" : "rules",
      "TAGS" : [ "CRM Core Donation Thank You Message" ],
      "REQUIRES" : [ "rules", "mimemail" ],
      "USES VARIABLES" : {
        "email_to" : { "label" : "Email to", "type" : "text" },
        "donor" : { "label" : "Donor", "type" : "crm_core_contact" },
        "donation_page" : { "label" : "Donation page", "type" : "node" },
        "amount" : { "label" : "Amount", "type" : "text" }
      },
      "RULES" : [
        { "RULE" : {
            "DO" : [
              { "mimemail" : {
                  "key" : "rules_cmcd_thank_you_message",
                  "to" : "[email-to:value]",
                  "subject" : "Thank you for your support!",
                  "body" : "\\u003Cstrong\\u003EDear [donor:contact-name:given],\\u003C\\/strong\\u003E\\r\\n\\r\\nThank you for your donation in the amount of \\u0022[amount:value]\\u0022. Your support is appreciated!\\r\\n\\r\\nCan you help this campaign succeed by asking your friends to donate too? There\\u0027s a sample message contained below that you can forward onto your personal networks.\\r\\n\\r\\n------- FORWARD THIS TO YOUR FRIENDS -------\\r\\nHi,\\r\\n\\r\\nI just made a donation to \\u0022[donation-page:title]\\u0022.\\r\\n\\r\\nIt\\u0027s an important cause, and I hope you would consider donating too. It will only take a few moments of your time and go to help a good cause.\\r\\n\\r\\nHere\\u0027s the link:\\r\\n\\r\\n[donation-page:url]\\r\\n\\r\\nThanks!\\r\\n[donor:contact-name:given]\\r\\n\\r\\n------------------------------\\r\\n\\r\\nThank you again, \\r\\nYour friends at \\u003Ca href=\\u0022[site:url]\\u0022\\u003E[site:name]\\u003C\\/a\\u003E",
                  "plaintext" : "Dear [donor:contact-name:given],\\r\\n\\r\\nThank you for your donation in the amount of \\u0022[amount:value]\\u0022. Your support is appreciated!\\r\\n\\r\\nCan you help this campaign succeed by asking your friends to donate too? There\\u0027s a sample message contained below that you can forward onto your personal networks.\\r\\n\\r\\n------- FORWARD THIS TO YOUR FRIENDS -------\\r\\nHi,\\r\\n\\r\\nI just made a donation to \\u0022[donation-page:title]\\u0022.\\r\\n\\r\\nIt\\u0027s an important cause, and I hope you would consider donating too. It will only take a few moments of your time and go to help a good cause.\\r\\n\\r\\nHere\\u0027s the link:\\r\\n\\r\\n[donation-page:url]\\r\\n\\r\\nThanks!\\r\\n[donor:contact-name:given]\\r\\n\\r\\n------------------------------\\r\\n\\r\\nThank you again, \\r\\nYour friends at [site:name] [site:url]",
                  "language" : [ "" ]
                }
              }
            ],
            "LABEL" : "Send HTML Email"
          }
        }
      ]
    }
  }');
  return $items;
}
