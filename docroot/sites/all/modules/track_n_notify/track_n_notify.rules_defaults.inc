<?php
/**
 * @file
 * track_n_notify.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function track_n_notify_default_rules_configuration() {
  $items = array();
  $items['rules_auto_track_from_comment'] = entity_import('rules_config', '{ "rules_auto_track_from_comment" : {
      "LABEL" : "Auto-track from comment",
      "PLUGIN" : "reaction rule",
      "REQUIRES" : [ "rules", "comment" ],
      "ON" : [ "comment_insert" ],
      "DO" : [
        { "entity_create" : {
            "USING" : {
              "type" : "node",
              "param_type" : "track",
              "param_title" : "[comment:author] will now track ___.",
              "param_author" : [ "comment:author" ]
            },
            "PROVIDE" : { "entity_created" : { "track_created" : "Created track" } }
          }
        },
        { "data_set" : {
            "data" : [ "track-created:field-parent" ],
            "value" : [ "comment:parent:node" ]
          }
        }
      ]
    }
  }');
  $items['rules_create_track'] = entity_import('rules_config', '{ "rules_create_track" : {
      "LABEL" : "Create track",
      "PLUGIN" : "rule",
      "REQUIRES" : [ "rules" ],
      "USES VARIABLES" : { "target" : { "label" : "Target", "type" : "node" } },
      "DO" : [
        { "entity_create" : {
            "USING" : {
              "type" : "node",
              "param_type" : "track",
              "param_title" : "[target:author] will now track ___.",
              "param_author" : [ "target:author" ]
            },
            "PROVIDE" : { "entity_created" : { "track_created" : "Created track" } }
          }
        }
      ]
    }
  }');
  $items['rules_email_trackers'] = entity_import('rules_config', '{ "rules_email_trackers" : {
      "LABEL" : "Email trackers",
      "PLUGIN" : "rule",
      "REQUIRES" : [ "rules" ],
      "USES VARIABLES" : { "parentnid" : { "label" : "parentnid", "type" : "integer" } },
      "DO" : [
        { "VIEW LOOP" : {
            "VIEW" : "trackers",
            "DISPLAY" : "views_rules_1",
            "ROW VARIABLES" : { "nid" : { "child" : "Child" }, "mail" : { "mail" : "(author) E-mail" } },
            "USING" : { "field_parent_target_id" : [ "parentnid" ] },
            "VIEW LOOP" : [
              { "mail" : {
                  "to" : [ "mail" ],
                  "subject" : "Another user is now tracking [site:current-page]",
                  "message" : "todo",
                  "language" : [ "" ]
                }
              }
            ]
          }
        }
      ]
    }
  }');
  $items['rules_link_condition_track_button'] = entity_import('rules_config', '{ "rules_link_condition_track_button" : {
      "LABEL" : "Rules link: Track Button condition",
      "PLUGIN" : "and",
      "REQUIRES" : [ "rules" ],
      "USES VARIABLES" : { "node" : { "type" : "node", "label" : "node" } },
      "AND" : [
        { "NOT user_has_role" : {
            "account" : [ "site:current-user" ],
            "roles" : { "value" : { "1" : "1" } }
          }
        }
      ]
    }
  }');
  $items['rules_link_set_track_button'] = entity_import('rules_config', '{ "rules_link_set_track_button" : {
      "LABEL" : "Rules link: Track Button rules set",
      "PLUGIN" : "rule set",
      "REQUIRES" : [ "rules" ],
      "USES VARIABLES" : { "node" : { "type" : "node", "label" : "node" } },
      "RULES" : [
        { "RULE" : {
            "PROVIDE" : { "entity_created" : { "track_created" : "Created track" } },
            "DO" : [
              { "entity_create" : {
                  "USING" : {
                    "type" : "node",
                    "param_type" : "track",
                    "param_title" : "Track for [site:current-user] for ___.",
                    "param_author" : [ "site:current-user" ]
                  },
                  "PROVIDE" : { "entity_created" : { "track_created" : "Created track" } }
                }
              },
              { "data_set" : { "data" : [ "track-created:field-parent" ], "value" : [ "node" ] } }
            ],
            "LABEL" : "Create track"
          }
        }
      ]
    }
  }');
  $items['rules_notify_trackers_of_new_tracker'] = entity_import('rules_config', '{ "rules_notify_trackers_of_new_tracker" : {
      "LABEL" : "Notify trackers of new tracker",
      "PLUGIN" : "reaction rule",
      "REQUIRES" : [ "rules" ],
      "ON" : [ "node_insert" ],
      "IF" : [
        { "entity_has_field" : { "entity" : [ "node" ], "field" : "field_parent" } }
      ],
      "DO" : [
        { "variable_add" : {
            "USING" : { "type" : "integer", "value" : [ "node:field-parent:nid" ] },
            "PROVIDE" : { "variable_added" : { "variable_added" : "Added variable" } }
          }
        },
        { "component_rules_email_trackers" : { "parentnid" : [ "variable-added" ] } }
      ]
    }
  }');
  return $items;
}
