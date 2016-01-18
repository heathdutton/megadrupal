<?php
/**
 * @file
 * Default Rules for the Election Vote module.
 */

/**
 * Implements hook_default_rules_configuration().
 */
function election_vote_default_rules_configuration() {

  $items = array();

  // Display a message to notify the user after abstaining.
  $items['election_vote_notify_abstainer'] = entity_import('rules_config', '{ "election_vote_notify_abstainer" : {
    "LABEL" : "Notify user after abstaining",
    "PLUGIN" : "reaction rule",
    "TAGS" : [ "election" ],
    "REQUIRES" : [ "election_vote" ],
    "ON" : [ "election_vote_abstain" ],
    "DO" : [ { "election_vote_abstain_message" : { "post" : [ "post" ] } } ]
  }
}');

  // Display a message to notify the user after voting.
  $items['election_vote_notify_voter'] = entity_import('rules_config', '{ "election_vote_notify_voter" : {
    "LABEL" : "Notify user after voting",
    "PLUGIN" : "reaction rule",
    "TAGS" : [ "election" ],
    "REQUIRES" : [ "election_vote" ],
    "ON" : [ "election_vote_vote" ],
    "DO" : [ { "election_vote_vote_message" : { "post" : [ "post" ] } } ]
  }
}');

  // Prompt the user to share the election on Facebook or Twitter after voting.
  $items['election_vote_share'] = entity_import('rules_config', '{ "election_vote_share" : {
    "LABEL" : "Prompt user to share the election on Facebook or Twitter after voting",
    "PLUGIN" : "reaction rule",
    "TAGS" : [ "election" ],
    "WEIGHT" : "1",
    "ACTIVE" : false,
    "REQUIRES" : [ "rules", "election_vote" ],
    "ON" : [ "election_vote_vote" ],
    "DO" : [
      { "drupal_message" : {
          "message" : "You can share this [election:type:name] on \u003Ca href=\u0022http:\/\/www.facebook.com\/sharer\/sharer.php?u=https:\/\/[site:url-brief][election:url]\u0022 target=\u0022_blank\u0022\u003EFacebook\u003C\/a\u003E or \u003Ca href=\u0022http:\/\/twitter.com\/share?text=I%20voted%20in%20[election:title]%20-%20https:\/\/[site:url-brief]%20([site:name])\u0022 target=\u0022_blank\u0022\u003ETwitter\u003C\/a\u003E.",
          "repeat" : 0
        }
      }
    ]
  }
}');

  // Email the user after voting.
  $items['election_vote_email_voter'] = entity_import('rules_config', '{ "election_vote_email_voter" : {
    "LABEL" : "Email user after voting",
    "PLUGIN" : "reaction rule",
    "WEIGHT" : "2",
    "ACTIVE" : false,
    "TAGS" : [ "election" ],
    "REQUIRES" : [ "rules", "election_vote" ],
    "ON" : [ "election_vote_vote" ],
    "DO" : [
      { "mail" : {
          "to" : "[account:name] \u003C[account:mail]\u003E",
          "subject" : "Thank you for voting",
          "message" : "You successfully cast a vote, using the system at: [site:url]\r\n\r\n    [election:type:name-ucfirst]: [election:title]\r\n    [election:type:post-name-ucfirst]: [post:title]\r\n    Time of vote: [site:current-date:long]\r\n\r\n(for security reasons further details are not displayed)\r\n\r\nIf you have any queries relating to this information, please contact us.",
          "language" : [ "" ]
        }
      }
    ]
  }
}');

  return $items;
}
