<?php
/**
 * @file
 * comment_score.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function comment_score_default_rules_configuration() {
  $items = array();
  $items['rules_remove_trusted_role_from_users_that_make_bad_comments'] = entity_import('rules_config', '{ "rules_remove_trusted_role_from_users_that_make_bad_comments" : {
      "LABEL" : "Remove trusted role from users that make bad comments",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Moderation" ],
      "REQUIRES" : [ "rules", "comment_score" ],
      "ON" : { "comment_score_bad_comment" : [] },
      "IF" : [
        { "user_has_role" : {
            "account" : [ "comment:author" ],
            "roles" : { "value" : { "4" : "4" } }
          }
        }
      ],
      "DO" : [
        { "user_remove_role" : {
            "account" : [ "comment:author" ],
            "roles" : { "value" : { "4" : "4" } }
          }
        }
      ]
    }
  }');
  $items['rules_tell_user_the_comment_is_published'] = entity_import('rules_config', '{ "rules_tell_user_the_comment_is_published" : {
      "LABEL" : "Tell user the comment is published",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Moderation" ],
      "REQUIRES" : [ "rules", "comment_score" ],
      "ON" : { "comment_score_good_comment" : [] },
      "DO" : [
        { "drupal_message" : {
            "message" : "Thank you for you comment, which has been published.",
            "repeat" : 0
          }
        }
      ]
    }
  }');
  $items['rules_tell_user_the_comment_is_queued'] = entity_import('rules_config', '{ "rules_tell_user_the_comment_is_queued" : {
      "LABEL" : "Tell user the comment is queued",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Moderation" ],
      "REQUIRES" : [ "rules", "comment_score" ],
      "ON" : { "comment_score_bad_comment" : [] },
      "DO" : [
        { "drupal_message" : {
            "message" : "Thank you for your comment. It will be visible after it has been approved by the forum moderators.",
            "type" : "warning",
            "repeat" : 0
          }
        }
      ]
    }
  }');
  return $items;
}
