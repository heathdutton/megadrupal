<?php
/**
 * @file
 * recruiter_job.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function recruiter_job_default_rules_configuration() {
  $items = array();
  $items['rules_link_condition_job_publish'] = entity_import('rules_config', '{ "rules_link_condition_job_publish" : {
      "LABEL" : "Rules link: Job publish condition",
      "PLUGIN" : "and",
      "OWNER" : "rules",
      "REQUIRES" : [ "rules" ],
      "USES VARIABLES" : { "node" : { "label" : "node", "type" : "node" } },
      "AND" : [
        { "OR" : [
            { "user_has_role" : {
                "account" : [ "site:current-user" ],
                "roles" : { "value" : { "30037204" : "30037204", "139881272" : "139881272" } },
                "operation" : "OR"
              }
            },
            { "AND" : [
                { "data_is" : { "data" : [ "node:author" ], "value" : [ "site:current-user" ] } },
                { "user_has_role" : {
                    "account" : [ "site:current-user" ],
                    "roles" : { "value" : { "100482362" : "100482362" } }
                  }
                }
              ]
            }
          ]
        },
        { "node_is_of_type" : {
            "node" : [ "node" ],
            "type" : { "value" : {
                "job_per_file" : "job_per_file",
                "job_per_link" : "job_per_link",
                "job_per_template" : "job_per_template"
              }
            }
          }
        },
        { "data_is" : { "data" : [ "node:field-job-workflow-state" ], "value" : "unpublished" } }
      ]
    }
  }');
  $items['rules_link_condition_job_unpublish'] = entity_import('rules_config', '{ "rules_link_condition_job_unpublish" : {
      "LABEL" : "Rules link: Job unpublish condition",
      "PLUGIN" : "and",
      "OWNER" : "rules",
      "REQUIRES" : [ "rules" ],
      "USES VARIABLES" : { "node" : { "label" : "node", "type" : "node" } },
      "AND" : [
        { "OR" : [
            { "user_has_role" : {
                "account" : [ "site:current-user" ],
                "roles" : { "value" : { "30037204" : "30037204", "139881272" : "139881272" } },
                "operation" : "OR"
              }
            },
            { "AND" : [
                { "data_is" : { "data" : [ "site:current-user" ], "value" : [ "node:author" ] } },
                { "user_has_role" : {
                    "account" : [ "site:current-user" ],
                    "roles" : { "value" : { "100482362" : "100482362" } }
                  }
                }
              ]
            }
          ]
        },
        { "node_is_of_type" : {
            "node" : [ "node" ],
            "type" : { "value" : {
                "job_per_file" : "job_per_file",
                "job_per_link" : "job_per_link",
                "job_per_template" : "job_per_template"
              }
            }
          }
        },
        { "data_is" : { "data" : [ "node:field-job-workflow-state" ], "value" : "published" } }
      ]
    }
  }');
  $items['rules_link_set_job_publish'] = entity_import('rules_config', '{ "rules_link_set_job_publish" : {
      "LABEL" : "Rules link: Job publish rules set",
      "PLUGIN" : "rule set",
      "OWNER" : "rules",
      "REQUIRES" : [ "rules" ],
      "ACCESS_EXPOSED" : "1",
      "USES VARIABLES" : { "node" : { "label" : "node", "type" : "node" } },
      "RULES" : [
        { "RULE" : {
            "IF" : [
              { "node_is_of_type" : {
                  "node" : [ "node" ],
                  "type" : { "value" : {
                      "job_per_file" : "job_per_file",
                      "job_per_link" : "job_per_link",
                      "job_per_template" : "job_per_template"
                    }
                  }
                }
              }
            ],
            "DO" : [
              { "data_set" : { "data" : [ "node:field-job-workflow-state" ], "value" : "published" } },
              { "drupal_message" : { "message" : "Publish job [node:title]." } }
            ],
            "LABEL" : "Publish job"
          }
        }
      ]
    }
  }');
  $items['rules_link_set_job_unpublish'] = entity_import('rules_config', '{ "rules_link_set_job_unpublish" : {
      "LABEL" : "Rules link: Job unpublish rules set",
      "PLUGIN" : "rule set",
      "OWNER" : "rules",
      "REQUIRES" : [ "rules" ],
      "ACCESS_EXPOSED" : "1",
      "USES VARIABLES" : { "node" : { "label" : "node", "type" : "node" } },
      "RULES" : [
        { "RULE" : {
            "IF" : [
              { "node_is_of_type" : {
                  "node" : [ "node" ],
                  "type" : { "value" : {
                      "job_per_file" : "job_per_file",
                      "job_per_link" : "job_per_link",
                      "job_per_template" : "job_per_template"
                    }
                  }
                }
              }
            ],
            "DO" : [
              { "data_set" : { "data" : [ "node:field-job-workflow-state" ], "value" : "unpublished" } },
              { "drupal_message" : { "message" : "Unpublished job [node:title]." } }
            ],
            "LABEL" : "Unpublish job"
          }
        }
      ]
    }
  }');
  return $items;
}
