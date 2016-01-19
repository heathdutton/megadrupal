<?php

/**
 * @file
 * Default rule configurations for Drulenium.
 */
function drulenium_default_rules_configuration() {
  $drulenium_rules_notify_on_drulenium_test_failure = '{ "rules_notify_on_drulenium_test_failure" : {
    "LABEL" : "Email notification on Drulenium test failure",
    "PLUGIN" : "reaction rule",
    "REQUIRES" : [ "rules" ],
    "ON" : [ "node_insert" ],
    "IF" : [
      { "node_is_of_type" : {
          "node" : [ "node" ],
          "type" : { "value" : { "drulenium" : "drulenium" } }
        }
      },
      { "data_is" : {
          "data" : [ "node:drulenium-num-test-failures" ],
          "op" : "\u003E",
          "value" : "0"
        }
      }
    ],
    "DO" : [
      { "mail" : {
          "to" : [ "site:mail" ],
          "subject" : "Drulenium Test Failure Log",
          "message" : "[node:title]\r\n\r\n[node:drulenium-result]\tresult\r\n[node:drulenium-total-time]\ttotal_time\r\n[node:drulenium-num-test-passes]\tnum_test_passes\r\n[node:drulenium-num-test-failures]\tnum_test_failures\r\n[node:drulenium-num-command-passes]\tnum_command_passes\r\n[node:drulenium-num-command-failures]\tnum_command_failures\r\n[node:drulenium-num-command-errors]\tnum_command_errors\r\n[node:drulenium-num-test-total]\tnum_test_total\r\n[node:drulenium-suite] Suite",
          "from" : [ "site:mail" ],
          "language" : [ "" ]
        }
      }
    ]
  }
}';
  $configs['rules_decrement_stock_completing_order'] = rules_import($drulenium_rules_notify_on_drulenium_test_failure);

  return $configs;
}