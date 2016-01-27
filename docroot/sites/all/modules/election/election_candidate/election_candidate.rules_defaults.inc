<?php
/**
 * @file
 * Default Rules for the Election Candidate module.
 */

/**
 * Implements hook_default_rules_configuration().
 */
function election_candidate_default_rules_configuration() {

  $items = array();

  // E-mail the user who submitted a nomination (the 'nominator').
  $items['election_email_nominator'] = entity_import('rules_config', '{ "election_email_nominator" : {
    "LABEL" : "E-mail the user who submitted a nomination",
    "PLUGIN" : "reaction rule",
    "ACTIVE" : false,
    "TAGS" : [ "election" ],
    "REQUIRES" : [ "rules", "election_candidate" ],
    "ON" : { "election_candidate_nomination_submitted" : [] },
    "DO" : [
      { "mail" : {
          "to" : "[account:name] \u003C[account:mail]\u003E",
          "subject" : "Thank you for submitting a nomination for [post:title]",
          "message" : "This is an automatic e-mail from [site:name] ([site:url-brief]) to confirm your nomination.\r\n\r\n    Election: [election:title]\r\n        [election:url]\r\n\r\n    Position: [post:title]\r\n        [post:url]\r\n\r\n    Candidate name: [candidate:name]\r\n    Candidate e-mail address: [candidate:candidate-mail]\r\n",
          "language" : [ "" ]
        }
      }
    ]
  }
}');

  // E-mail the nominee (if different from the nominator).
  $items['election_email_nominee'] = entity_import('rules_config', '{ "election_email_nominee" : {
    "LABEL" : "E-mail the nominee (if nominee \u0026 nominator e-mail addresses differ)",
    "PLUGIN" : "reaction rule",
    "WEIGHT" : "1",
    "ACTIVE" : false,
    "TAGS" : [ "election" ],
    "REQUIRES" : [ "rules", "election_candidate" ],
    "ON" : { "election_candidate_nomination_submitted" : [] },
    "IF" : [
      { "entity_has_field" : { "entity" : [ "candidate" ], "field" : "candidate_mail" } },
      { "NOT data_is" : { "data" : [ "account:mail" ], "value" : [ "candidate:candidate-mail" ] } }
    ],
    "DO" : [
      { "mail" : {
          "to" : "[candidate:name] \u003C[candidate:candidate-mail]\u003E",
          "subject" : "You have been nominated for [post:title]",
          "message" : "This is an automatic e-mail from [site:name] ([site:url-brief]) to confirm your nomination.\r\n\r\n    Election: [election:title]\r\n        [election:url]\r\n\r\n    Position: [post:title]\r\n        [post:url]\r\n\r\nYou were nominated by the user [account:name] \u003C[account:mail]\u003E.\r\n",
          "language" : [ "" ]
        }
      }
    ]
  }
}');

  // E-mail the nominee (if nominator is anonymous).
  $items['election_email_nominated_by_anonymous'] = entity_import('rules_config', '{ "election_email_nominated_by_anonymous" : {
    "LABEL" : "E-mail the nominee (if nominator is anonymous)",
    "PLUGIN" : "reaction rule",
    "WEIGHT" : "2",
    "ACTIVE" : false,
    "TAGS" : [ "election" ],
    "REQUIRES" : [ "rules", "election_candidate" ],
    "ON" : { "election_candidate_nomination_submitted" : [] },
    "IF" : [
      { "user_has_role" : { "account" : [ "account" ], "roles" : { "value" : { "1" : "1" } } } },
      { "entity_has_field" : { "entity" : [ "candidate" ], "field" : "candidate_mail" } }
    ],
    "DO" : [
      { "mail" : {
          "to" : [ "candidate:candidate-mail" ],
          "subject" : "You have been nominated for [post:title]",
          "message" : "This is an automatic e-mail from [site:name] ([site:url-brief]) to confirm your nomination.\r\n\r\n    Election: [election:title]\r\n        [election:url:absolute]\r\n\r\n    Position: [post:title]\r\n        [post:url:absolute]\r\n",
          "language" : [ "" ]
        }
      }
    ]
  }
}');

  return $items;
}
