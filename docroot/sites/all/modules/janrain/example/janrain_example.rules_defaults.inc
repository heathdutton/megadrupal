<?php
/**
 * @file
 * Janrain Example submodule rules integration of Janrain Data.
 */

/**
 * Implements hook_default_rules_configuration().
 */
function janrain_example_default_rules_configuration() {
  $items = array();
  if (_janrain_is_login_only()) {
    $items['janrain_example_rules_login'] = entity_import('rules_config',
      '{
        "janrain_example_rules_login" : {
          "LABEL" : "Janrain Login Example Rules",
          "PLUGIN" : "reaction rule",
          "OWNER" : "rules",
          "REQUIRES" : [ "janrain_data" ],
          "ON" : { "janrain_data_profile_updated" : [] },
          "DO" : [
            {"janrain_data_map":{"jsonpath":"$.profile.birthday", "data":["site:current-user:field-birthday"]}},
            {"janrain_data_map":{"jsonpath":"$.profile.gender", "data":["site:current-user:field-gender"]}}
          ]
        }
      }');
  }
  else {
    $items['rules_janrain_set'] = entity_import('rules_config',
      '{
        "rules_janrain_set" : {
          "LABEL" : "janrain rules set",
          "PLUGIN" : "reaction rule",
          "OWNER" : "rules",
          "REQUIRES" : [ "janrain_data" ],
          "ON" : { "janrain_data_profile_updated" : [] },
          "DO" : [
            {"janrain_data_map":{"jsonpath":"$.birthday", "data":["site:current-user:field-birthday"]}},
            {"janrain_data_map":{"jsonpath" : "$.lastLogin","data" : [ "site:current-user:field-last-logged-in" ]}},
            {"janrain_data_map" : { "jsonpath" : "$.gender", "data" : [ "site:current-user:field-gender" ] } }
          ]
        }
      }');
  }
  return $items;
}
