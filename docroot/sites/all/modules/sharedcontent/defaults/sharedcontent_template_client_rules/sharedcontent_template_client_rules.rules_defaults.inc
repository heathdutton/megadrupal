<?php
/**
 * @file
 * sharedcontent_template_client_rules.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function sharedcontent_template_client_rules_default_rules_configuration() {
  $items = array();
  $items['rules_sharedcontent_back_link_add'] = entity_import('rules_config', '{ "rules_sharedcontent_back_link_add" : {
      "LABEL" : "Shared Content add link back to source",
      "PLUGIN" : "reaction rule",
      "ACTIVE" : false,
      "TAGS" : [ "Shared Content" ],
      "REQUIRES" : [ "rules", "sharedcontent_client", "sharedcontent" ],
      "ON" : [ "sharedcontent_assignment_client_link" ],
      "IF" : [
        { "data_is" : { "data" : [ "source:origin" ], "value" : "remote" } },
        { "data_is" : { "data" : [ "target:origin" ], "value" : "local" } }
      ],
      "DO" : [
        { "sharedcontent_client_rule_content_link" : {
            "local_content" : [ "target" ],
            "remote_content" : [ "source" ],
            "reference_fields" : { "value" : { "sharedcontent_reference" : "sharedcontent_reference" } }
          }
        }
      ]
    }
  }');
  $items['rules_sharedcontent_back_link_remove'] = entity_import('rules_config', '{ "rules_sharedcontent_back_link_remove" : {
      "LABEL" : "Shared Content remove link back to source",
      "PLUGIN" : "reaction rule",
      "ACTIVE" : false,
      "TAGS" : [ "Shared Content" ],
      "REQUIRES" : [ "rules", "sharedcontent_client", "sharedcontent" ],
      "ON" : [ "sharedcontent_assignment_client_unlink" ],
      "IF" : [
        { "data_is" : { "data" : [ "source:origin" ], "value" : "remote" } },
        { "data_is" : { "data" : [ "target:origin" ], "value" : "local" } }
      ],
      "DO" : [
        { "sharedcontent_client_rule_content_unlink" : {
            "local_content" : [ "target" ],
            "remote_content" : [ "source" ],
            "reference_fields" : { "value" : { "sharedcontent_reference" : "sharedcontent_reference" } }
          }
        }
      ]
    }
  }');
  $items['rules_sharedcontent_queue_local'] = entity_import('rules_config', '{ "rules_sharedcontent_queue_local" : {
      "LABEL" : "Add new content to local queue",
      "PLUGIN" : "reaction rule",
      "TAGS" : [ "Shared Content" ],
      "REQUIRES" : [ "rules", "flag", "sharedcontent" ],
      "ON" : [ "sharedcontent_index_insert" ],
      "IF" : [
        { "data_is" : { "data" : [ "sharedcontent-index:origin" ], "value" : "local" } }
      ],
      "DO" : [
        { "flag_flagsharedcontent_index" : {
            "flag" : "sharedcontent_local_queue",
            "sharedcontent_index" : [ "sharedcontent_index" ],
            "flagging_user" : [ "site:current-user" ],
            "permission_check" : 1
          }
        }
      ]
    }
  }');
  $items['rules_sharedcontent_queue_remote'] = entity_import('rules_config', '{ "rules_sharedcontent_queue_remote" : {
      "LABEL" : "Add new remote content to remote queue",
      "PLUGIN" : "reaction rule",
      "TAGS" : [ "Shared Content" ],
      "REQUIRES" : [ "rules", "flag", "sharedcontent" ],
      "ON" : [ "sharedcontent_index_insert" ],
      "IF" : [
        { "data_is" : { "data" : [ "sharedcontent-index:origin" ], "value" : "remote" } },
        { "NOT data_is" : { "data" : [ "sharedcontent-index:status" ], "value" : "0" } }
      ],
      "DO" : [
        { "flag_flagsharedcontent_index" : {
            "flag" : "sharedcontent_remote_queue",
            "sharedcontent_index" : [ "sharedcontent_index" ],
            "flagging_user" : [ "site:current-user" ],
            "permission_check" : 1
          }
        }
      ]
    }
  }');
  $items['rules_sharedcontent_queue_remote_update'] = entity_import('rules_config', '{ "rules_sharedcontent_queue_remote_update" : {
      "LABEL" : "Remove deleted remote content from remote queue",
      "PLUGIN" : "reaction rule",
      "TAGS" : [ "Shared Content" ],
      "REQUIRES" : [ "rules", "flag", "sharedcontent" ],
      "ON" : [ "sharedcontent_index_update" ],
      "IF" : [
        { "data_is" : { "data" : [ "sharedcontent-index:status" ], "value" : "0" } },
        { "flag_flagged_sharedcontent_index" : {
            "flag" : "sharedcontent_remote_queue",
            "sharedcontent_index" : [ "sharedcontent_index" ],
            "flagging_user" : [ "site:current-user" ]
          }
        }
      ],
      "DO" : [
        { "flag_unflagsharedcontent_index" : {
            "flag" : "sharedcontent_remote_queue",
            "sharedcontent_index" : [ "sharedcontent_index" ],
            "flagging_user" : [ "site:current-user" ],
            "permission_check" : 1
          }
        }
      ]
    }
  }');
  $items['rules_sharedcontent_search_enqueue_match'] = entity_import('rules_config', '{ "rules_sharedcontent_search_enqueue_match" : {
      "LABEL" : "Enqueue local content item on search match",
      "PLUGIN" : "reaction rule",
      "ACTIVE" : false,
      "TAGS" : [ "Shared Content" ],
      "REQUIRES" : [ "rules", "flag", "sharedcontent_client" ],
      "ON" : [ "sharedcontent_client_search_result" ],
      "IF" : [
        { "data_is" : { "data" : [ "score" ], "op" : "\\u003E", "value" : "4.9" } },
        { "entity_is_of_type" : { "entity" : [ "result" ], "type" : "sharedcontent_index" } },
        { "data_is" : { "data" : [ "result:entity-type" ], "value" : "node" } }
      ],
      "DO" : [
        { "entity_fetch" : {
            "USING" : { "type" : "node", "id" : [ "result:entity-id" ] },
            "PROVIDE" : { "entity_fetched" : { "node" : "Local node" } }
          }
        },
        { "flag_flagnode" : {
            "flag" : "sharedcontent_local_queue",
            "node" : [ "node" ],
            "flagging_user" : [ "site:current-user" ],
            "permission_check" : 1
          }
        }
      ]
    }
  }');
  $items['rules_sharedcontent_search_local_match_on_new_remote'] = entity_import('rules_config', '{ "rules_sharedcontent_search_local_match_on_new_remote" : {
      "LABEL" : "Search for matches on new remote content",
      "PLUGIN" : "reaction rule",
      "ACTIVE" : false,
      "TAGS" : [ "Shared Content" ],
      "REQUIRES" : [ "rules", "sharedcontent_client", "sharedcontent" ],
      "ON" : [ "sharedcontent_index_insert" ],
      "IF" : [
        { "data_is" : { "data" : [ "sharedcontent-index:origin" ], "value" : "remote" } },
        { "NOT data_is" : { "data" : [ "sharedcontent-index:status" ], "value" : "0" } }
      ],
      "DO" : [
        { "sharedcontent_client_rules_search" : {
            "search_index" : "sharedcontent",
            "keywords" : [ "sharedcontent-index:keywords" ],
            "sharedcontent_index" : [ "sharedcontent_index" ]
          }
        }
      ]
    }
  }');
  return $items;
}
