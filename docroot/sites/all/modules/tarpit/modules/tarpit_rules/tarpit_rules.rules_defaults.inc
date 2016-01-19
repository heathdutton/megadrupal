<?php
/**
 * @file
 * tarpit_rules.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function tarpit_rules_default_rules_configuration() {
  $items = array();
  $items['tarpit_rules_default'] = entity_import('rules_config', '{ "tarpit_rules_default" : {
    "LABEL" : "Tarpit rules when someone is viewing a tarpit.",
    "PLUGIN" : "reaction rule",
    "ACTIVE" : false,
    "OWNER" : "rules",
    "TAGS" : [ "tarpit" ],
    "REQUIRES" : [ "tarpit_rules" ],
    "ON" : { "tarpit_view" : [] },
    "DO" : [
      { "tarpit_disable_blocks" : { "blocks_to_disable" : "*::*" } },
      { "tarpit_sleep" : { "sleep_min" : "0", "sleep_max" : "2" } },
      { "tarpit_generate_content" : {
          "words" : "400",
          "links" : "100",
          "file" : "sites\/all\/modules\/tarpit\/assets\/words.txt"
        }
      }
    ]
  }
}');

  $items['tarpit_rules_default_block'] = entity_import('rules_config', '{ "tarpit_rules_default_block" : {
    "LABEL" : "When someone crossed the maximum depth of links in a tarpit.",
    "PLUGIN" : "reaction rule",
    "ACTIVE" : false,
    "OWNER" : "rules",
    "TAGS" : [ "tarpit" ],
    "REQUIRES" : [ "rules", "tarpit_rules" ],
    "ON" : { "tarpit_reaction" : [] },
    "DO" : [
      { "tarpit_disable_blocks" : { "blocks_to_disable" : "*::*" } },
      { "tarpit_sleep" : { "sleep_min" : "0", "sleep_max" : "2" } },
      { "tarpit_generate_content" : {
          "words" : "400",
          "links" : "100",
          "file" : "sites\/all\/modules\/tarpit\/assets\/words.txt"
        }
      },
      { "block_ip" : [] }
    ]
  }
}');

  return $items;
}
