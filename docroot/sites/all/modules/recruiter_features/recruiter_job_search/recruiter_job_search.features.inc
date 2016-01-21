<?php
/**
 * @file
 * recruiter_job_search.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function recruiter_job_search_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "context" && $api == "context") {
    return array("version" => "3");
  }
  list($module, $api) = func_get_args();
  if ($module == "facetapi" && $api == "facetapi") {
    return array("version" => "1");
  }
  list($module, $api) = func_get_args();
  if ($module == "facetapi" && $api == "facetapi_defaults") {
    return array("version" => "1");
  }
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function recruiter_job_search_views_api() {
  return array("version" => "3.0");
}

/**
 * Implements hook_flag_default_flags().
 */
function recruiter_job_search_flag_default_flags() {
  $flags = array();
  // Exported flag: "Job bookmarks".
  $flags['job_bookmarks'] = array(
    'entity_type' => 'node',
    'title' => 'Job bookmarks',
    'global' => '0',
    'types' => array(
      0 => 'job_per_file',
      1 => 'job_per_link',
      2 => 'job_per_template',
    ),
    'flag_short' => 'Bookmark job',
    'flag_long' => '',
    'flag_message' => '',
    'unflag_short' => 'Unmark job',
    'unflag_long' => '',
    'unflag_message' => '',
    'unflag_denied_text' => '',
    'link_type' => 'toggle',
    'weight' => 0,
    'show_in_links' => array(
      'full' => 'full',
      'teaser' => 'teaser',
      'rss' => 0,
      'diff_standard' => 0,
      'token' => 0,
    ),
    'show_as_field' => 0,
    'show_on_form' => 0,
    'access_author' => '',
    'show_contextual_link' => 0,
    'i18n' => 0,
    'module' => 'recruiter_job_search',
    'locked' => array(
      0 => 'name',
    ),
    'api_version' => 3,
  );
  return $flags;

}

/**
 * Implements hook_default_search_api_index().
 */
function recruiter_job_search_default_search_api_index() {
  $items = array();
  $items['job_search'] = entity_import('search_api_index', '{
    "name" : "Job search",
    "machine_name" : "job_search",
    "description" : "Index for job nodes used by the recruiter distribution.",
    "server" : "db_server",
    "item_type" : "node",
    "options" : {
      "index_directly" : 1,
      "cron_limit" : "50",
      "fields" : {
        "type" : { "type" : "string" },
        "status" : { "type" : "boolean" },
        "promote" : { "type" : "boolean" },
        "sticky" : { "type" : "boolean" },
        "created" : { "type" : "date" },
        "changed" : { "type" : "date" },
        "author" : { "type" : "integer", "entity_type" : "user" },
        "field_job_employment_type_term" : { "type" : "list\\u003Cinteger\\u003E", "entity_type" : "taxonomy_term" },
        "field_job_experience_term" : { "type" : "integer", "entity_type" : "taxonomy_term" },
        "field_job_languages" : { "type" : "list\\u003Cinteger\\u003E", "entity_type" : "taxonomy_term" },
        "field_job_location" : { "type" : "string" },
        "field_job_occupational_field" : { "type" : "list\\u003Cinteger\\u003E", "entity_type" : "taxonomy_term" },
        "field_job_of_study" : { "type" : "list\\u003Cinteger\\u003E", "entity_type" : "taxonomy_term" },
        "field_job_organization" : { "type" : "string" },
        "field_job_region" : { "type" : "list\\u003Cinteger\\u003E", "entity_type" : "taxonomy_term" },
        "field_job_skills_general" : { "type" : "list\\u003Cinteger\\u003E", "entity_type" : "taxonomy_term" },
        "field_job_skills_it" : { "type" : "list\\u003Cinteger\\u003E", "entity_type" : "taxonomy_term" },
        "field_job_workflow_state" : { "type" : "string" },
        "search_api_language" : { "type" : "string" },
        "search_api_viewed" : { "type" : "text" },
        "field_job_employment_type_term:parents_all" : {
          "type" : "list\\u003Clist\\u003Cinteger\\u003E\\u003E",
          "entity_type" : "taxonomy_term"
        },
        "field_job_languages:parents_all" : {
          "type" : "list\\u003Clist\\u003Cinteger\\u003E\\u003E",
          "entity_type" : "taxonomy_term"
        },
        "field_job_occupational_field:parents_all" : {
          "type" : "list\\u003Clist\\u003Cinteger\\u003E\\u003E",
          "entity_type" : "taxonomy_term"
        },
        "field_job_of_study:parents_all" : {
          "type" : "list\\u003Clist\\u003Cinteger\\u003E\\u003E",
          "entity_type" : "taxonomy_term"
        },
        "field_job_region:parents_all" : {
          "type" : "list\\u003Clist\\u003Cinteger\\u003E\\u003E",
          "entity_type" : "taxonomy_term"
        },
        "field_job_skills_general:parents_all" : {
          "type" : "list\\u003Clist\\u003Cinteger\\u003E\\u003E",
          "entity_type" : "taxonomy_term"
        },
        "field_job_skills_it:parents_all" : {
          "type" : "list\\u003Clist\\u003Cinteger\\u003E\\u003E",
          "entity_type" : "taxonomy_term"
        }
      },
      "data_alter_callbacks" : {
        "search_api_alter_add_hierarchy" : { "status" : 0, "weight" : "0", "settings" : { "fields" : [] } },
        "search_api_alter_node_access" : { "status" : 0, "weight" : "0", "settings" : [] },
        "search_api_alter_add_viewed_entity" : { "status" : 1, "weight" : "0", "settings" : { "mode" : "full" } },
        "search_api_alter_add_aggregation" : { "status" : 1, "weight" : "0", "settings" : [] },
        "search_api_alter_add_url" : { "status" : 0, "weight" : "0", "settings" : [] },
        "search_api_alter_bundle_filter" : {
          "status" : 1,
          "weight" : "0",
          "settings" : {
            "default" : "0",
            "bundles" : {
              "job_per_file" : "job_per_file",
              "job_per_link" : "job_per_link",
              "job_per_template" : "job_per_template"
            }
          }
        }
      },
      "processors" : {
        "search_api_case_ignore" : {
          "status" : 1,
          "weight" : "0",
          "settings" : { "fields" : { "search_api_viewed" : true } }
        },
        "content_taxonomy_autocomplete_moderated_terms" : {
          "status" : 1,
          "weight" : "0",
          "settings" : { "fields" : {
              "field_job_languages:parents_all" : true,
              "field_job_skills_general:parents_all" : true,
              "field_job_skills_it:parents_all" : true
            }
          }
        },
        "search_api_html_filter" : {
          "status" : 1,
          "weight" : "10",
          "settings" : {
            "fields" : { "search_api_viewed" : true },
            "title" : 0,
            "alt" : 0,
            "tags" : "h1 = 5\\r\\n                                                                                                                h2 = 3\\r\\n                                                                                                                h3 = 2\\r\\n                                                                                                                strong = 2\\r\\n                                                                                                                b = 2\\r\\n                                                                                                                em = 1.5\\r\\n                                                                                                                u = 1.5"
          }
        },
        "search_api_tokenizer" : {
          "status" : 1,
          "weight" : "20",
          "settings" : {
            "fields" : { "search_api_viewed" : true },
            "spaces" : "[^\\\\p{L}\\\\p{N}]",
            "ignorable" : ""
          }
        },
        "search_api_stopwords" : {
          "status" : 0,
          "weight" : "30",
          "settings" : {
            "fields" : [],
            "file" : "",
            "stopwords" : "but\\r\\ndid\\r\\nthe this that those\\r\\netc"
          }
        }
      }
    },
    "enabled" : "1",
    "read_only" : "0",
    "rdf_mapping" : []
  }');
  return $items;
}
