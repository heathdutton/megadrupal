<?php
/**
 * @file
 * recruiter_job_search.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function recruiter_job_search_default_rules_configuration() {
  $items = array();
  $items['rules_recruiter_job_index'] = entity_import('rules_config', '{ "rules_recruiter_job_index" : {
      "LABEL" : "Index job",
      "PLUGIN" : "action set",
      "OWNER" : "rules",
      "TAGS" : [ "recruiter_job_search" ],
      "REQUIRES" : [ "search_api" ],
      "USES VARIABLES" : { "job_node" : { "label" : "Job node", "type" : "node" } },
      "ACTION SET" : [
        { "search_api_index" : { "entity" : [ "job-node" ], "index" : "job_search" } }
      ]
    }
  }');
  return $items;
}
