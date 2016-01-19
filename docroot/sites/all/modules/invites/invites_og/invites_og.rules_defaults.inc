<?php
/**
 * @file
 * invites_og.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function invites_og_default_rules_configuration() {
  $items = array();
   $items['rules_invites_approval_mail_og'] = entity_import('rules_config', '{ "rules_invites_approval_mail_og" : {
      "LABEL" : "Invites Approval Mail OG",
      "PLUGIN" : "reaction rule",
      "TAGS" : [ "Invites" ],
      "REQUIRES" : [ "invites", "rules" ],
      "ON" : [ "invites_approval" ],
      "IF" : [
        { "invites_is_of_type" : { "invites" : [ "invites" ], "types" : { "value" : { "og" : "og" } } } }
      ],
      "DO" : [
        { "mail" : {
            "to" : "[invites:inviter-mail]",
            "subject" : "Invitation to [invites:mail] has been accepted.",
            "message" : "An invitation to [invites:mail] on [site:name] has been accepted.",
            "from" : [ "site:mail" ],
            "language" : [ "" ]
          }
        }
      ]
    }
  }');
  $items['rules_invites_mail_og'] = entity_import('rules_config', '{ "rules_invites_mail_og" : {
      "LABEL" : "Invites Mail OG",
      "PLUGIN" : "reaction rule",
      "TAGS" : [ "Invites" ],
      "REQUIRES" : [ "invites", "rules" ],
      "ON" : [ "invites_insert" ],
      "IF" : [
        { "invites_is_of_type" : { "invites" : [ "invites" ], "types" : { "value" : { "og" : "og" } } } }
      ],
      "DO" : [
        { "mail" : {
            "to" : [ "invites:mail" ],
            "subject" : "Invitation from [site:name] by [invites:inviter-name]",
            "message" : "Hi,\\r\\nYou have an invitation at [site:name] from [invites:inviter-name].\\r\\nYou can approve the invitation by clicking this link or copying and pasting it to your browser:\\r\\n\\r\\n[invites:mail-url]\\r\\n\\r\\nThis link can only be used once and will lead you to your user page.\\r\\nAfter approved the invitation, you will be able to get the information at [invites:user-url].",
            "from" :  "[invites:inviter-mail]",
            "language" : [ "" ]
          }
        }
      ]
    }
  }');
  return $items;
}
