<?php
/**
 * @file
 * sharedcontent_template_ui.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function sharedcontent_template_ui_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "page_manager" && $api == "pages_default") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function sharedcontent_template_ui_views_api() {
  return array("version" => "3.0");
}

/**
 * Implements hook_flag_default_flags().
 */
function sharedcontent_template_ui_flag_default_flags() {
  $flags = array();
  // Exported flag: "Shared Content Local Queue".
  $flags['sharedcontent_local_queue'] = array(
    'content_type' => 'sharedcontent_index',
    'title' => 'Shared Content Local Queue',
    'global' => '1',
    'types' => array(
      0 => 'local',
    ),
    'flag_short' => 'Queue',
    'flag_long' => '',
    'flag_message' => '',
    'unflag_short' => 'Dequeue',
    'unflag_long' => '',
    'unflag_message' => '',
    'unflag_denied_text' => '',
    'link_type' => 'normal',
    'roles' => array(
      'flag' => array(
        0 => '3',
      ),
      'unflag' => array(
        0 => '3',
      ),
    ),
    'weight' => 0,
    'show_on_entity' => 0,
    'show_on_form' => 1,
    'access_author' => '',
    'module' => 'sharedcontent_template_ui',
    'locked' => array(
      0 => 'name',
    ),
    'api_version' => 2,
  );
  // Exported flag: "Shared Content Remote Queue".
  $flags['sharedcontent_remote_queue'] = array(
    'content_type' => 'sharedcontent_index',
    'title' => 'Shared Content Remote Queue',
    'global' => '1',
    'types' => array(
      0 => 'local',
      1 => 'remote',
      2 => 'shadow',
    ),
    'flag_short' => 'Queue',
    'flag_long' => '',
    'flag_message' => '',
    'unflag_short' => 'Dequeue',
    'unflag_long' => '',
    'unflag_message' => '',
    'unflag_denied_text' => '',
    'link_type' => 'normal',
    'roles' => array(
      'flag' => array(
        0 => '3',
      ),
      'unflag' => array(
        0 => '3',
      ),
    ),
    'weight' => 0,
    'show_on_entity' => 0,
    'show_on_form' => 1,
    'access_author' => '',
    'module' => 'sharedcontent_template_ui',
    'locked' => array(
      0 => 'name',
    ),
    'api_version' => 2,
  );
  return $flags;

}

/**
 * Implements hook_default_search_api_index().
 */
function sharedcontent_template_ui_default_search_api_index() {
  $items = array();
  $items['sharedcontent'] = entity_import('search_api_index', '{
    "name" : "Shared Content",
    "machine_name" : "sharedcontent",
    "description" : null,
    "server" : null,
    "item_type" : "sharedcontent_index",
    "options" : {
      "index_directly" : 1,
      "cron_limit" : "100",
      "fields" : {
        "id" : { "type" : "integer" },
        "connection_name" : { "type" : "string" },
        "origin" : { "type" : "string" },
        "uuid" : { "type" : "string" },
        "entity_id" : { "type" : "integer" },
        "entity_type" : { "type" : "string", "boost" : "0.5" },
        "entity_bundle" : { "type" : "string", "boost" : "0.5" },
        "title" : { "type" : "text", "boost" : "3.0" },
        "keywords" : { "type" : "text", "boost" : "2.0" },
        "tags" : { "type" : "text" },
        "status" : { "type" : "integer" },
        "created" : { "type" : "date" },
        "changed" : { "type" : "date" },
        "is_linkable" : { "type" : "boolean" },
        "sharedcontent_remote_queue" : { "type" : "boolean" },
        "sharedcontent_local_queue" : { "type" : "boolean" },
        "search_api_language" : { "type" : "string" }
      },
      "data_alter_callbacks" : {
        "search_api_alter_bundle_filter" : {
          "status" : 0,
          "weight" : "-10",
          "settings" : { "default" : "1", "bundles" : [] }
        },
        "search_api_alter_add_hierarchy" : { "status" : 0, "weight" : "0", "settings" : { "fields" : [] } },
        "search_api_alter_add_viewed_entity" : {
          "status" : 0,
          "weight" : "0",
          "settings" : { "mode" : "sharedcontent_teaser" }
        },
        "search_api_alter_add_aggregation" : { "status" : 0, "weight" : "0", "settings" : [] },
        "search_api_alter_add_url" : { "status" : 0, "weight" : "0", "settings" : [] }
      },
      "processors" : {
        "search_api_case_ignore" : {
          "status" : 0,
          "weight" : "0",
          "settings" : { "fields" : {
              "entity_type" : true,
              "entity_bundle" : true,
              "title" : true,
              "keywords" : true,
              "tags" : true
            }
          }
        },
        "search_api_html_filter" : {
          "status" : 0,
          "weight" : "10",
          "settings" : {
            "fields" : { "keywords" : true },
            "title" : 0,
            "alt" : 1,
            "tags" : "h1 = 5\\r\\nh2 = 3\\r\\nh3 = 2\\r\\nstrong = 2\\r\\nb = 2\\r\\nem = 1.5\\r\\nu = 1.5"
          }
        },
        "search_api_tokenizer" : {
          "status" : 0,
          "weight" : "20",
          "settings" : {
            "fields" : { "title" : true, "keywords" : true, "tags" : true },
            "spaces" : "[^[:alnum:]]",
            "ignorable" : "[\\u0027]"
          }
        },
        "search_api_stopwords" : {
          "status" : 0,
          "weight" : "30",
          "settings" : {
            "fields" : { "title" : true, "keywords" : true, "tags" : true },
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
