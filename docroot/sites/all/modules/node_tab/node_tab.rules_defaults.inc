<?php
/**
 * @file
 * node_tab.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function node_tab_default_rules_configuration() {
  $items = array();
  $items['redirect_to_default_node_tab'] = entity_import('rules_config', '{ "redirect_to_default_node_tab" : {
    "LABEL" : "Redirect to default node tab",
    "PLUGIN" : "reaction rule",
    "REQUIRES" : [ "node_tab", "rules" ],
    "ON" : [ "init" ],
    "IF" : [ { "node_tab_rules_condition" : [] } ],
    "DO" : [ { "node_tab_redirect" : [] } ]
  }
}');
  return $items;
}
