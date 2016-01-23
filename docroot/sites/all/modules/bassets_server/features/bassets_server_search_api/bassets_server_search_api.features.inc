<?php
/**
 * @file
 * bassets_server_search_api.features.inc
 */

/**
 * Implements hook_views_api().
 */
function bassets_server_search_api_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}

/**
 * Implements hook_default_search_api_index().
 */
function bassets_server_search_api_default_search_api_index() {
  $items = array();
  $items['files_index'] = entity_import('search_api_index', '{
    "name" : "Files Index",
    "machine_name" : "files_index",
    "description" : null,
    "server" : "bassets_database",
    "item_type" : "file",
    "options" : {
      "index_directly" : 1,
      "cron_limit" : "50",
      "fields" : {
        "field_asset_caption" : { "type" : "text" },
        "field_asset_copyright" : { "type" : "text" },
        "field_asset_description" : { "type" : "text" },
        "field_asset_license" : { "type" : "text" },
        "field_asset_photographer" : { "type" : "text" },
        "field_asset_source" : { "type" : "text" },
        "field_asset_tags" : { "type" : "text" },
        "field_image_motive_category:name" : { "type" : "string" },
        "field_image_motive_type:name" : { "type" : "string" },
        "field_image_orientation" : { "type" : "string" },
        "field_image_photometricinterpret" : { "type" : "string" },
        "field_license:name" : { "type" : "string" },
        "field_remarks" : { "type" : "text" },
        "name" : { "type" : "text" },
        "search_api_language" : { "type" : "string" }
      },
      "data_alter_callbacks" : {
        "search_api_alter_bundle_filter" : {
          "status" : 0,
          "weight" : "-10",
          "settings" : { "default" : "1", "bundles" : [] }
        },
        "search_api_alter_add_hierarchy" : { "status" : 0, "weight" : "0", "settings" : { "fields" : [] } },
        "search_api_alter_add_viewed_entity" : { "status" : 0, "weight" : "0", "settings" : { "mode" : "teaser" } },
        "search_api_alter_add_url" : { "status" : 1, "weight" : "0", "settings" : [] },
        "bassets_server_excluded_locked_files" : { "status" : 1, "weight" : "0", "settings" : [] },
        "search_api_alter_add_aggregation" : { "status" : 0, "weight" : "0", "settings" : [] }
      },
      "processors" : {
        "search_api_case_ignore" : {
          "status" : 1,
          "weight" : "0",
          "settings" : { "fields" : {
              "field_asset_caption" : true,
              "field_asset_copyright" : true,
              "field_asset_description" : true,
              "field_asset_photographer" : true,
              "field_asset_source" : true,
              "field_remarks" : true,
              "field_image_motive_category:name" : true,
              "field_image_motive_type:name" : true,
              "field_license:name" : true
            }
          }
        },
        "search_api_html_filter" : {
          "status" : 0,
          "weight" : "10",
          "settings" : {
            "fields" : {
              "field_asset_caption" : true,
              "field_asset_copyright" : true,
              "field_asset_description" : true,
              "field_asset_photographer" : true,
              "field_asset_source" : true,
              "field_remarks" : true
            },
            "title" : 1,
            "alt" : 1,
            "tags" : "h1 = 5\\r\\nh2 = 3\\r\\nh3 = 2\\r\\nstrong = 2\\r\\nb = 2\\r\\nem = 1.5\\r\\nu = 1.5"
          }
        },
        "search_api_transliteration" : {
          "status" : 0,
          "weight" : "15",
          "settings" : { "fields" : {
              "name" : true,
              "field_asset_caption" : true,
              "field_asset_copyright" : true,
              "field_asset_description" : true,
              "field_asset_photographer" : true,
              "field_asset_source" : true,
              "field_remarks" : true
            }
          }
        },
        "search_api_tokenizer" : {
          "status" : 1,
          "weight" : "20",
          "settings" : {
            "fields" : {
              "field_asset_caption" : true,
              "field_asset_copyright" : true,
              "field_asset_description" : true,
              "field_asset_photographer" : true,
              "field_asset_source" : true,
              "field_remarks" : true
            },
            "spaces" : "[^[:alnum:]]",
            "ignorable" : "[\\u0027]"
          }
        },
        "search_api_stopwords" : {
          "status" : 0,
          "weight" : "30",
          "settings" : {
            "fields" : {
              "field_asset_caption" : true,
              "field_asset_copyright" : true,
              "field_asset_description" : true,
              "field_asset_photographer" : true,
              "field_asset_source" : true,
              "field_remarks" : true
            },
            "file" : "",
            "stopwords" : "but\\r\\ndid\\r\\nthe this that those\\r\\netc"
          }
        },
        "search_api_highlighting" : {
          "status" : 0,
          "weight" : "35",
          "settings" : {
            "prefix" : "\\u003Cstrong\\u003E",
            "suffix" : "\\u003C\\/strong\\u003E",
            "excerpt" : 1,
            "excerpt_length" : "256",
            "exclude_fields" : [],
            "highlight" : "always"
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
function bassets_server_search_api_default_search_api_server() {
  $items = array();
  $items['bassets_database'] = entity_import('search_api_server', '{
    "name" : "Bassets Database",
    "machine_name" : "bassets_database",
    "description" : "",
    "class" : "search_api_db_service",
    "options" : {
      "database" : "default:default",
      "min_chars" : "3",
      "partial_matches" : 0,
      "indexes" : { "files_index" : {
          "name" : {
            "table" : "search_api_db_files_index_text",
            "type" : "text",
            "boost" : "1.0"
          },
          "field_asset_caption" : {
            "table" : "search_api_db_files_index_text",
            "type" : "text",
            "boost" : "1.0"
          },
          "field_asset_copyright" : {
            "table" : "search_api_db_files_index_text",
            "type" : "text",
            "boost" : "1.0"
          },
          "field_asset_description" : {
            "table" : "search_api_db_files_index_text",
            "type" : "text",
            "boost" : "1.0"
          },
          "field_asset_license" : {
            "table" : "search_api_db_files_index_text",
            "type" : "text",
            "boost" : "1.0"
          },
          "field_asset_photographer" : {
            "table" : "search_api_db_files_index_text",
            "type" : "text",
            "boost" : "1.0"
          },
          "field_asset_source" : {
            "table" : "search_api_db_files_index_text",
            "type" : "text",
            "boost" : "1.0"
          },
          "field_asset_tags" : {
            "table" : "search_api_db_files_index_text",
            "type" : "text",
            "boost" : "1.0"
          },
          "field_image_orientation" : {
            "table" : "search_api_db_files_index",
            "column" : "field_image_orientation",
            "type" : "string",
            "boost" : "1.0"
          },
          "field_image_photometricinterpret" : {
            "table" : "search_api_db_files_index",
            "column" : "field_image_photometricinterpret",
            "type" : "string",
            "boost" : "1.0"
          },
          "field_remarks" : {
            "table" : "search_api_db_files_index_text",
            "type" : "text",
            "boost" : "1.0"
          },
          "search_api_language" : {
            "table" : "search_api_db_files_index",
            "column" : "search_api_language",
            "type" : "string",
            "boost" : "1.0"
          },
          "field_image_motive_category:name" : {
            "table" : "search_api_db_files_index",
            "column" : "field_image_motive_category_name",
            "type" : "string",
            "boost" : "1.0"
          },
          "field_image_motive_type:name" : {
            "table" : "search_api_db_files_index",
            "column" : "field_image_motive_type_name",
            "type" : "string",
            "boost" : "1.0"
          },
          "field_license:name" : {
            "table" : "search_api_db_files_index",
            "column" : "field_license_name",
            "type" : "string",
            "boost" : "1.0"
          }
        }
      }
    },
    "enabled" : "1",
    "rdf_mapping" : []
  }');
  $items['bassets_solr'] = entity_import('search_api_server', '{
    "name" : "Bassets Solr",
    "machine_name" : "bassets_solr",
    "description" : "",
    "class" : "search_api_solr_service",
    "options" : {
      "host" : "localhost",
      "port" : "8983",
      "path" : "\\/solr",
      "http_user" : "",
      "http_pass" : "",
      "excerpt" : 0,
      "retrieve_data" : 0,
      "highlight_data" : 0,
      "http_method" : "POST"
    },
    "enabled" : "0",
    "rdf_mapping" : []
  }');
  return $items;
}
