<?php
/**
 * @file
 * storiedmaps_story.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function storiedmaps_story_default_rules_configuration() {
  $items = array();
  $items['rules_story_created'] = entity_import('rules_config', '{ "rules_story_created" : {
      "LABEL" : "Story created",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "REQUIRES" : [ "rules" ],
      "ON" : { "node_insert" : [] },
      "IF" : [
        { "node_is_of_type" : { "node" : [ "node" ], "type" : { "value" : { "story" : "story" } } } },
        { "NOT node_is_published" : { "node" : [ "node" ] } }
      ],
      "DO" : [
        { "drupal_message" : { "message" : "Thank you for contributing a story. It will now be reviewed by an editor before publication." } },
        { "mail" : {
            "to" : "[site:mail]",
            "subject" : "A new story has been posted on [site:name]",
            "message" : "A new story [node:title] has been posted on the [site:name].\\r\\n\\r\\nIt can be reviewed at: [node:url].",
            "language" : [ "" ]
          }
        }
      ]
    }
  }');
  return $items;
}
