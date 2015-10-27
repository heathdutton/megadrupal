<?php
/**
 * @file
 * cp_search.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function cp_search_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "facetapi" && $api == "facetapi_defaults") {
    return array("version" => "1");
  }
  list($module, $api) = func_get_args();
  if ($module == "page_manager" && $api == "pages_default") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function cp_search_views_api() {
  return array("version" => "3.0");
}

/**
 * Implements hook_default_search_api_index().
 */
function cp_search_default_search_api_index() {
  $items = array();
  $items['cp_node_index'] = entity_import('search_api_index', '{
    "name" : "Course planner node index",
    "machine_name" : "cp_node_index",
    "description" : "Index of Course planner items of type course offering, course outline, course section and resource (but not lesson).",
    "server" : "cp_database_search",
    "item_type" : "node",
    "options" : {
      "index_directly" : 0,
      "cron_limit" : "50",
      "fields" : {
        "nid" : { "type" : "integer" },
        "type" : { "type" : "string" },
        "title" : { "type" : "text", "boost" : "8.0" },
        "changed" : { "type" : "date" },
        "author" : { "type" : "integer", "entity_type" : "user" },
        "cp_canonical_course" : { "type" : "list\\u003Cinteger\\u003E", "entity_type" : "taxonomy_term" },
        "cp_keywords" : { "type" : "list\\u003Cinteger\\u003E", "entity_type" : "taxonomy_term" },
        "cp_resource_reference" : { "type" : "text", "boost" : "0.5" },
        "cp_resource_source" : { "type" : "integer", "entity_type" : "taxonomy_term" },
        "cp_resource_type" : { "type" : "string" },
        "search_api_language" : { "type" : "string" },
        "body:value" : { "type" : "text" },
        "body:summary" : { "type" : "text", "boost" : "2.0" },
        "cp_keywords:name" : { "type" : "list\\u003Ctext\\u003E", "boost" : "5.0" }
      },
      "data_alter_callbacks" : {
        "search_api_alter_bundle_filter" : {
          "status" : 1,
          "weight" : "-10",
          "settings" : {
            "default" : "0",
            "bundles" : {
              "cp_offering" : "cp_offering",
              "cp_outline" : "cp_outline",
              "cp_section" : "cp_section",
              "cp_resource" : "cp_resource"
            }
          }
        },
        "search_api_alter_node_access" : { "status" : 0, "weight" : "0", "settings" : [] },
        "search_api_alter_node_status" : { "status" : 1, "weight" : "0", "settings" : [] },
        "search_api_alter_add_hierarchy" : { "status" : 0, "weight" : "0", "settings" : { "fields" : [] } },
        "search_api_alter_add_viewed_entity" : { "status" : 0, "weight" : "0", "settings" : { "mode" : "full" } },
        "search_api_alter_add_url" : { "status" : 0, "weight" : "0", "settings" : [] },
        "search_api_alter_add_aggregation" : { "status" : 0, "weight" : "0", "settings" : [] }
      },
      "processors" : {
        "search_api_case_ignore" : {
          "status" : 1,
          "weight" : "0",
          "settings" : { "fields" : { "title" : true, "cp_resource_reference" : true } }
        },
        "search_api_html_filter" : {
          "status" : 1,
          "weight" : "10",
          "settings" : {
            "fields" : { "cp_resource_reference" : true },
            "title" : 0,
            "alt" : 1,
            "tags" : "h1 = 5\\r\\nh2 = 3\\r\\nh3 = 2\\r\\nstrong = 2\\r\\nb = 2\\r\\nem = 1.5\\r\\nu = 1.5"
          }
        },
        "search_api_tokenizer" : {
          "status" : 0,
          "weight" : "20",
          "settings" : {
            "fields" : { "title" : true, "cp_resource_reference" : true },
            "spaces" : "[^[:alnum:]]",
            "ignorable" : "[\\u0027]"
          }
        },
        "search_api_stopwords" : {
          "status" : 0,
          "weight" : "30",
          "settings" : {
            "fields" : { "title" : true, "cp_resource_reference" : true },
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

/**
 * Implements hook_default_search_api_server().
 */
function cp_search_default_search_api_server() {
  $items = array();
  $items['cp_database_search'] = entity_import('search_api_server', '{
    "name" : "Course planner database search",
    "machine_name" : "cp_database_search",
    "description" : "Database search used by the Course planner module.",
    "class" : "search_api_db_service",
    "options" : {
      "database" : "default:default",
      "min_chars" : "1",
      "indexes" : { "cp_node_index" : {
          "nid" : {
            "table" : "search_api_db_cp_node_index",
            "column" : "nid",
            "type" : "integer",
            "boost" : "1.0"
          },
          "type" : {
            "table" : "search_api_db_cp_node_index",
            "column" : "type",
            "type" : "string",
            "boost" : "1.0"
          },
          "title" : {
            "table" : "search_api_db_cp_node_index_title",
            "type" : "text",
            "boost" : "8.0"
          },
          "changed" : {
            "table" : "search_api_db_cp_node_index",
            "column" : "changed",
            "type" : "date",
            "boost" : "1.0"
          },
          "author" : {
            "table" : "search_api_db_cp_node_index",
            "column" : "author",
            "type" : "integer",
            "boost" : "1.0"
          },
          "cp_canonical_course" : {
            "table" : "search_api_db_cp_node_index_cp_canonical_course",
            "column" : "value",
            "type" : "list\\u003Cinteger\\u003E",
            "boost" : "1.0"
          },
          "cp_keywords" : {
            "table" : "search_api_db_cp_node_index_cp_keywords",
            "column" : "value",
            "type" : "list\\u003Cinteger\\u003E",
            "boost" : "1.0"
          },
          "cp_resource_reference" : {
            "table" : "search_api_db_cp_node_index_cp_resource_reference",
            "type" : "text",
            "boost" : "0.5"
          },
          "cp_resource_source" : {
            "table" : "search_api_db_cp_node_index",
            "column" : "cp_resource_source",
            "type" : "integer",
            "boost" : "1.0"
          },
          "cp_resource_type" : {
            "table" : "search_api_db_cp_node_index",
            "column" : "cp_resource_type",
            "type" : "string",
            "boost" : "1.0"
          },
          "search_api_language" : {
            "table" : "search_api_db_cp_node_index",
            "column" : "search_api_language",
            "type" : "string",
            "boost" : "1.0"
          },
          "body:value" : {
            "table" : "search_api_db_cp_node_index_body_value",
            "type" : "text",
            "boost" : "1.0"
          },
          "body:summary" : {
            "table" : "search_api_db_cp_node_index_body_summary",
            "type" : "text",
            "boost" : "2.0"
          },
          "cp_keywords:name" : {
            "table" : "search_api_db_cp_node_index_cp_keywords_name",
            "type" : "list\\u003Ctext\\u003E",
            "boost" : "5.0"
          }
        }
      }
    },
    "enabled" : "1",
    "rdf_mapping" : []
  }');
  return $items;
}
