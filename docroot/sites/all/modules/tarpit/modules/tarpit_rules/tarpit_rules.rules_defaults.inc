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
  $items['rules_tarpit_pre'] = entity_import('rules_config', '{ "rules_tarpit_pre" : {
    "LABEL" : "Tarpit rules before limit is reached.",
    "PLUGIN" : "reaction rule",
    "ACTIVE" : false,
    "OWNER" : "rules",
    "REQUIRES" : [ "rules", "tarpit_rules" ],
    "ON" : { "tarpit" : [] },
    "IF" : [
      { "data_is" : {
          "data" : [ "tarpit-depth" ],
          "op" : "\u003C",
          "value" : [ "tarpit-depth-config" ]
        }
      }
    ],
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

  $items['rules_tarpit_post'] = entity_import('rules_config', '{ "rules_tarpit_post" : {
    "LABEL" : "Tarpit rules when limit is reached",
    "PLUGIN" : "reaction rule",
    "ACTIVE" : false,
    "OWNER" : "rules",
    "REQUIRES" : [ "rules", "tarpit_rules" ],
    "ON" : { "tarpit" : [] },
    "IF" : [
      { "data_is" : {
          "data" : [ "tarpit-depth" ],
          "op" : "\u003E",
          "value" : { "select" : "tarpit-depth-config", "num_offset" : { "value" : "-1" } }
        }
      }
    ],
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
